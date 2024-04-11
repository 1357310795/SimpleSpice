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
#include "devices/item/inductor.h"

D_Inductor::D_Inductor(std::string name, std::string n1, std::string n2, double value) 
    : BaseDevice(name), DynamicDevice(), Value(value), N1(n1), N2(n2) {};

std::string D_Inductor::getDeviceType() {
    return "Inductor";
}

void D_Inductor::appendStampDC(AnalyzeContext* context) {
    int &nodeCount = context->nodeCount; 
    Eigen::MatrixXd &mat = context->mat;
    Eigen::VectorXd &rhs = context->rhs;
    std::vector<CircuitNode> &nodes = context->nodes; 
    auto it1 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->N1; });
    auto it2 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->N2; });
    if (it1 == nodes.end() || it2 == nodes.end()) {
        throw std::runtime_error("Node not found");
    }
    int index1 = std::distance(nodes.begin(), it1);
    int index2 = std::distance(nodes.begin(), it2);
    nodeCount++;
    int index3 = nodeCount - 1;
    mat.conservativeResize(nodeCount, nodeCount);
    mat.col(index3).fill(0);
    mat.row(index3).fill(0);
    rhs.conservativeResize(nodeCount);
    rhs(index3) = 0;
    mat(index1, index3) = 1;
    mat(index2, index3) = -1;
    mat(index3, index1) = 1;
    mat(index3, index2) = -1;
    
    nodes.push_back(CircuitNode(Name, "I"));
}

void D_Inductor::appendStampAC(AnalyzeContext* context) {
    int &nodeCount = context->nodeCount; 
    Eigen::MatrixXcd* mat = &context->matc;
    Eigen::VectorXcd* rhs = &context->rhsc;
    std::vector<CircuitNode> &nodes = context->nodes; 
    
    auto it1 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->N1; });
    auto it2 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->N2; });
    if (it1 == nodes.end() || it2 == nodes.end()) {
        throw std::runtime_error("Node not found");
    }
    int index1 = std::distance(nodes.begin(), it1);
    int index2 = std::distance(nodes.begin(), it2);
    nodeCount++;
    int index3 = nodeCount - 1;
    (*mat).conservativeResize(nodeCount, nodeCount);
    (*mat).col(index3).fill(0);
    (*mat).row(index3).fill(0);
    (*rhs).conservativeResize(nodeCount);
    (*rhs)(index3) = 0;
    (*mat)(index1, index3) = 1;
    (*mat)(index2, index3) = -1;
    (*mat)(index3, index1) = 1;
    (*mat)(index3, index2) = -1;
    (*mat)(index3, index3) = std::complex<double>((double)0, (double)-2 * M_PI * context->freq) * std::complex<double>(this->Value, (double)0);
    nodes.push_back(CircuitNode(Name, "I"));
}

void D_Inductor::appendStampTRAN(AnalyzeContext* context) {
    int &nodeCount = context->nodeCount; 
    Eigen::MatrixXd* mat = &context->mat;
    Eigen::VectorXd* rhs = &context->rhs;
    std::vector<CircuitNode> &nodes = context->nodes; 
    double interval = context->interval; 
    double lastValue = context->lastCurrents[this->Name]->back(); 
    auto it1 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->N1; });
    auto it2 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->N2; });
    if (it1 == nodes.end() || it2 == nodes.end()) {
        throw std::runtime_error("Node not found");
    }
    int index1 = std::distance(nodes.begin(), it1);
    int index2 = std::distance(nodes.begin(), it2);
    nodeCount++;
    int index3 = nodeCount - 1;
    (*mat).conservativeResize(nodeCount, nodeCount);
    (*mat).col(index3).fill(0);
    (*mat).row(index3).fill(0);
    (*rhs).conservativeResize(nodeCount);
    (*rhs)(index3) = -this->Value/interval*lastValue;
    (*mat)(index1, index3) = 1;
    (*mat)(index2, index3) = -1;
    (*mat)(index3, index1) = 1;
    (*mat)(index3, index2) = -1;
    (*mat)(index3, index3) = -this->Value/interval;
    
    nodes.push_back(CircuitNode(Name, "I"));
}

double D_Inductor::getLastVoltage(AnalyzeContext* context) {
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

double D_Inductor::getLastCurrent(AnalyzeContext* context) {
    int &nodeCount = context->nodeCount;
    Eigen::VectorXd* result = &context->res;
    std::vector<CircuitNode> &nodes = context->nodes;
    CircuitNode &ground = context->ground;
    auto it1 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->Name; });
    if (it1 == nodes.end()) {
        throw std::runtime_error("Node not found");
    }
    int index1 = std::distance(nodes.begin(), it1);
    return (*result)[index1];
} 

double D_Inductor::filterLTE(AnalyzeContext* context, double step) {
    if (context->lastVoltages[this->Name]->size() < 2)
        return step;
    auto it = context->lastVoltages[this->Name]->end();
    double tmp = 2*this->Value/(std::abs(*(it-1)-*(it-2)))*1e-3;
    if (step > tmp)
        return step / 2;
    else if (step * 2 < tmp)
        return step * 2;
    else
        return step;
} 

void D_Inductor::checkNode(
    std::ostringstream &oss,
    std::string &node
) {
    if (node == N1) oss<<Name<<":N+"<<"\t";
    if (node == N2) oss<<Name<<":N-"<<"\t";
}

void D_Inductor::print(MyConsole* console) {
    console->log(std::format("*Inductor(Name:{}, N+:{}, N-:{}, Value:{:.3e})", Name, N1, N2, Value));
}

std::vector<std::string>& D_Inductor::getInfoList()
{
    std::vector<std::string>* res = new std::vector<std::string>();
    res->push_back(std::format("N+:{}", N1));
    res->push_back(std::format("N-:{}", N2));
    res->push_back(std::format("Value:{:.3e}", Value));
    return *res;
}
