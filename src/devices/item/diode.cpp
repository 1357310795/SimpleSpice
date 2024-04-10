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
#include "devices/item/diode.h"

D_Diode::D_Diode(std::string name, std::string n1, std::string n2) 
    : BaseDevice(name), NonlinearDevice(), N1(n1), N2(n2) {}

std::string D_Diode::GetDeviceType() {
    return "Diode";
}

template<typename MatrixType, typename VectorType>
void D_Diode::appendStamp(
    int &nodeCount, 
    MatrixType &mat, 
    VectorType &rhs,
    std::vector<CircuitNode> &nodes,
    double lastVoltage,
    double lastCurrent
) {
    auto it1 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->N1; });
    auto it2 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->N2; });
    if (it1 == nodes.end() || it2 == nodes.end()) {
        throw std::runtime_error("Node not found");
    }
    int index1 = std::distance(nodes.begin(), it1);
    int index2 = std::distance(nodes.begin(), it2);
    double g = 40*std::exp(40*lastVoltage);
    double j = lastCurrent-g*lastVoltage;
    mat(index1, index1) += g;
    mat(index1, index2) += -g;
    mat(index2, index1) += -g;
    mat(index2, index2) += g;
    rhs(index1) += -j;
    rhs(index2) += j;
}

void D_Diode::appendStampDC(AnalyzeContext* context) {
    if (context->IterVoltages[this->Name]->back() < 0)
        return;
    this->appendStamp(
        context->nodeCount, 
        context->mat, 
        context->rhs, 
        context->nodes,
        context->IterVoltages[this->Name]->back(),
        context->IterCurrents[this->Name]->back()
    );
}

void D_Diode::appendStampAC(AnalyzeContext* context) {
    throw std::runtime_error("Not supported");
}

void D_Diode::appendStampTRAN(AnalyzeContext* context) {
    throw std::runtime_error("Not used");
}

double D_Diode::getLastVoltage(AnalyzeContext* context) {
    int &nodeCount = context->nodeCount;
    Eigen::VectorXd* result = &context->res;
    std::vector<CircuitNode> &nodes = context->nodes;
    CircuitNode &ground = context->ground;
    auto it1 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->N1; });
    auto it2 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->N2; });
    if (it1 == nodes.end() && this->N1 != ground.nodeName) {
        throw std::runtime_error("Node not found");
    }
    if (it2 == nodes.end() && this->N2 != ground.nodeName) {
        throw std::runtime_error("Node not found");
    }
    double value1 = 0, value2 = 0;
    if (it1 != nodes.end())
    {
        int index1 = std::distance(nodes.begin(), it1);
        value1 = (*result)[index1];
    }
    if (it2 != nodes.end())
    {
        int index2 = std::distance(nodes.begin(), it2);
        value2 = (*result)[index2];
    }
    
    return value1 - value2;
} 

double D_Diode::getLastCurrent(AnalyzeContext* context) {
    double v = getLastVoltage(context);
    return v<=0 ? 0 : (std::exp(40*v) - 1);
} 

void D_Diode::checkNode(
    std::ostringstream &oss,
    std::string &node
) {
    if (node == N1) oss<<Name<<":N+"<<"\t";
    if (node == N2) oss<<Name<<":N-"<<"\t";
}

void D_Diode::print(MyConsole* console) {
    console->log(std::format("*Diode(Name:{}, N+:{}, N-:{})", Name, N1, N2));
}

std::vector<std::string>& D_Diode::getInfoList()
{
    std::vector<std::string>* res = new std::vector<std::string>();
    res->push_back(std::format("N+:{}", N1));
    res->push_back(std::format("N-:{}", N2));
    return *res;
}
