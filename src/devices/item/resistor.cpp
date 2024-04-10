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
#include "devices/item/resistor.h"

D_Resistor::D_Resistor(std::string name, std::string n1, std::string n2, double value) 
    : BaseDevice(name), Value(value), N1(n1), N2(n2) {};

std::string D_Resistor::GetDeviceType() {
    return "Resistor";
}

template<typename MatrixType, typename VectorType>
void D_Resistor::appendStamp(
    int &nodeCount, 
    MatrixType &mat, 
    VectorType &rhs,
    std::vector<CircuitNode> &nodes
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
    mat(index1, index1) += 1/this->Value;
    mat(index1, index2) += -1/this->Value;
    mat(index2, index1) += -1/this->Value;
    mat(index2, index2) += 1/this->Value;
}

void D_Resistor::appendStampDC(AnalyzeContext* context) {
    appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes);
}

void D_Resistor::appendStampAC(AnalyzeContext* context) {
    appendStamp(context->nodeCount, context->matc, context->rhsc, context->nodes);
}

void D_Resistor::checkNode(
    std::ostringstream &oss,
    std::string &node
) {
    if (node == N1) oss<<Name<<":N+"<<"\t";
    if (node == N2) oss<<Name<<":N-"<<"\t";
}

void D_Resistor::print(MyConsole* console) {
    console->log(std::format("*Resistor(Name:{}, N+:{}, N-:{}, Value:{:.3e})", Name, N1, N2, Value));
}

std::vector<std::string>& D_Resistor::getInfoList() {
    std::vector<std::string>* res = new std::vector<std::string>();
    res->push_back(std::format("N+:{}", N1));
    res->push_back(std::format("N-:{}", N2));
    res->push_back(std::format("Value:{:.3e}", Value));
    return *res;
}
