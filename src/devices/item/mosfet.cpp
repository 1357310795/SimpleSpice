#include <iostream>
#include <string>
#include <format>
#include <complex>
#include <Eigen/Dense>
#include "devices/base_device.h"
#include "console/myconsole.h"
#include "circuit/circuit_node.h"
#include "devices/source_device.h"
#include "devices/dynamic_device.h"
#include "devices/nonlinear_device.h"
#include "calc/analyze_context.h"
#include "devices/item/mosfet.h"

D_Mosfet::D_Mosfet(
    std::string name, 
    std::string nd,
    std::string ng, 
    std::string ns, 
    std::string nb, 
    std::string model
) : BaseDevice(name), NonlinearDevice(), ND(nd), NG(ng), NS(ns), NB(nb), Model(model) {
    if (model == "mosn")
        this->Type = NMOS;
    else if (model == "mosp")
        this->Type = PMOS;
    this->Width = 1e-9;
    this->Length = 1e-9;
}

std::string D_Mosfet::getDeviceType() {
    return "Mosfet";
}

template<typename MatrixType, typename VectorType>
void D_Mosfet::appendStamp(
    int &nodeCount, 
    MatrixType &mat, 
    VectorType &rhs,
    std::vector<CircuitNode> &nodes,
    MosfetIterationContext &iterContext
) {
    int indexg = FindNodeIndex(nodes, NG);
    int indexb = FindNodeIndex(nodes, NB);
    int indexd = FindNodeIndex(nodes, ND);
    int indexs = FindNodeIndex(nodes, NS);
    
    double vov = iterContext.vgs - getVth();
    bool sat = this->Type == NMOS ? 
        iterContext.vds >= vov:
        iterContext.vds <= vov;
    double sign = this->Type == NMOS ? 1 : -1;

    double gm = sat ? 
        sign * 2 * getK() * vov
        : sign * 2 * getK() * iterContext.vds;

    double gds = sat ? 
        sign * 0
        : sign * 2 * getK() * vov - 2 * getK() * iterContext.vds;

    double jds = getCurrent(iterContext.vgs, iterContext.vds) - gm * iterContext.vgs - gds * iterContext.vds;
        
    mat(indexd, indexd) += gds;
    mat(indexd, indexs) += -gds-gm;
    mat(indexd, indexg) += gm;
    mat(indexs, indexd) += -gds;
    mat(indexs, indexs) += gds+gm;
    mat(indexs, indexg) += -gm;
    rhs(indexd) += -jds;
    rhs(indexs) += jds;
}

void D_Mosfet::appendStampDC(AnalyzeContext* context) {
    auto baseContext = context->IterContexts[this->Name]->back();
    auto iterContext = dynamic_cast<MosfetIterationContext*>(baseContext);

    if (!checkOpen(iterContext->vgs))
    {
        console->log(std::format("For vgs {}, mos not open.", iterContext->vgs));
        return;
    }

    this->appendStamp(
        context->nodeCount, 
        context->mat, 
        context->rhs, 
        context->nodes,
        *iterContext
    );
}

void D_Mosfet::appendStampAC(AnalyzeContext* context) {
    throw std::runtime_error("Not supported");
}

void D_Mosfet::appendStampTRAN(AnalyzeContext* context) {
    throw std::runtime_error("Not used");
}

BaseIterationContext* D_Mosfet::getLastContext(AnalyzeContext* context) {
    int &nodeCount = context->nodeCount;
    Eigen::VectorXd* result = &context->res;
    std::vector<CircuitNode> &nodes = context->nodes;
    CircuitNode &ground = context->ground;

    int indexg = ground.nodeName == this->NG ? -1 : FindNodeIndex(nodes, NG);
    int indexb = ground.nodeName == this->NB ? -1 : FindNodeIndex(nodes, NB);
    int indexd = ground.nodeName == this->ND ? -1 : FindNodeIndex(nodes, ND);
    int indexs = ground.nodeName == this->NS ? -1 : FindNodeIndex(nodes, NS);

    double vg = 0, vb = 0, vd = 0, vs = 0;
    vg = indexg == -1 ? 0 : (*result)[indexg];
    vb = indexb == -1 ? 0 : (*result)[indexb];
    vd = indexd == -1 ? 0 : (*result)[indexd];
    vs = indexs == -1 ? 0 : (*result)[indexs];
    
    return new MosfetIterationContext(this->Name, vg - vs, vd - vs);
}

BaseIterationContext* D_Mosfet::getDefaultIterationContext() {
    return new MosfetIterationContext(Name, 0, 0);
} 

double D_Mosfet::checkConvergence(AnalyzeContext* context) {
    auto it = context->IterContexts[this->Name]->end();
    auto iterContext1 = dynamic_cast<MosfetIterationContext*>(*(it-1));
    auto iterContext2 = dynamic_cast<MosfetIterationContext*>(*(it-2));

    double delta_vds = std::abs(iterContext1->vds - iterContext2->vds);
    double delta_vgs = std::abs(iterContext1->vgs - iterContext2->vgs);
    return std::max(delta_vds, delta_vgs);
}

double D_Mosfet::getVth() {
    return this->Type == NMOS ? vth_n : vth_p;
}
    
double D_Mosfet::getmuCox() {
    return this->Type == NMOS ? mucox_n : mucox_p;
}

double D_Mosfet::getK() {
    return 0.5 * getmuCox() * Width / Length;
}

double D_Mosfet::getCurrent(double vgs, double vds) {
    if (!checkOpen(vgs))
        return 0;
    double vov = vgs - getVth();
    bool sat = this->Type == NMOS ? 
        vds >= vov:
        vds <= vov;
    double sign = this->Type == NMOS ? 1 : -1;
    if (sat)
        return sign * getK() * vov * vov;
    else
        return sign * getK() * (2 * vov * vds - vds * vds);
}

bool D_Mosfet::checkOpen(double vgs) {
    if (this->Type == NMOS)
        return vgs > getVth();
    else 
        return vgs < getVth();
}

void D_Mosfet::checkNode(
    std::ostringstream &oss,
    std::string &node
) {
    if (node == NG) oss<<Name<<":Gate"<<"\t";
    if (node == NB) oss<<Name<<":Base"<<"\t";
    if (node == ND) oss<<Name<<":Drain"<<"\t";
    if (node == NS) oss<<Name<<":Source"<<"\t";
}

void D_Mosfet::print(MyConsole* console) {
    console->log(std::format("*Mosfet(Name:{}, Type:{}, Gate:{}, Base:{}, Drain:{}, Source:{})", Name, (int)Type, NG, NB, ND, NS));
}

std::vector<std::string>& D_Mosfet::getInfoList()
{
    std::vector<std::string>* res = new std::vector<std::string>();
    res->push_back(std::format("Type:{}", (int)Type));
    res->push_back(std::format("Gate:{}", NG));
    res->push_back(std::format("Base:{}", NB));
    res->push_back(std::format("Drain:{}", ND));
    res->push_back(std::format("Source:{}", NS));
    res->push_back(std::format("Width:{}", Width));
    res->push_back(std::format("Length:{}", Length));
    return *res;
}

int D_Mosfet::FindNodeIndex(std::vector<CircuitNode> &nodes, std::string nodeName) {
    auto it1 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == nodeName; });
    if (it1 == nodes.end()) {
        throw std::runtime_error("Node not found");
    }
    int index1 = std::distance(nodes.begin(), it1);
    return index1;
}
