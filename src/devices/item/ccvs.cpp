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
#include "devices/item/ccvs.h"

D_CCVS::D_CCVS(std::string name, std::string n1, std::string n2, std::string nc1, std::string nc2, double value) 
    : BaseDevice(name), Value(value), N1(n1), N2(n2), NC1(nc1), NC2(nc2) {}

std::string D_CCVS::GetDeviceType() {
    return "CCVS";
}

template<typename MatrixType, typename VectorType>
void D_CCVS::appendStamp(
    int &nodeCount, 
    MatrixType &mat, 
    VectorType &rhs,
    std::vector<CircuitNode> &nodes
) {
    auto it1 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->N1; });
    auto it2 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->N2; });
    auto it3 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->NC1; });
    auto it4 = std::find_if(nodes.begin(), nodes.end(), 
        [&](CircuitNode& n){ return n.nodeName == this->NC2; });
    if (it1 == nodes.end() || it2 == nodes.end() 
        || it3 == nodes.end() || it4 == nodes.end()) {
        throw std::runtime_error("Node not found");
    }
    int index1 = std::distance(nodes.begin(), it1);
    int index2 = std::distance(nodes.begin(), it2);
    int index3 = std::distance(nodes.begin(), it3);
    int index4 = std::distance(nodes.begin(), it4);
    nodeCount+=2;
    int index5 = nodeCount - 2;
    int index6 = nodeCount - 1;
    mat.conservativeResize(nodeCount, nodeCount);
    mat.col(index5).fill(0);
    mat.col(index6).fill(0);
    mat.row(index5).fill(0);
    mat.row(index6).fill(0);
    rhs.conservativeResize(nodeCount);
    rhs(index5) = 0;
    rhs(index6) = 0;
    mat(index1, index5) += 1;
    mat(index2, index5) += -1;
    mat(index3, index6) += 1;
    mat(index4, index6) += -1;
    mat(index5, index1) += 1;
    mat(index5, index2) += -1;
    mat(index5, index6) += -this->Value;
    mat(index6, index3) += 1;
    mat(index6, index4) += -1;
    
    nodes.push_back(CircuitNode(Name, "I_k"));
    nodes.push_back(CircuitNode(Name, "I_c"));
}

void D_CCVS::appendStampDC(AnalyzeContext* context) {
    appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes);
}

void D_CCVS::appendStampAC(AnalyzeContext* context) {
    appendStamp(context->nodeCount, context->matc, context->rhsc, context->nodes);
}

void D_CCVS::checkNode(
    std::ostringstream &oss,
    std::string &node
) {
    if (node == N1) oss<<Name<<":N+"<<"\t";
    if (node == N2) oss<<Name<<":N-"<<"\t";
    if (node == NC1) oss<<Name<<":NC+"<<"\t";
    if (node == NC2) oss<<Name<<":NC-"<<"\t";
}

void D_CCVS::print(MyConsole* console) {
    console->log(std::format("*CCVS(Name:{}, N+:{}, N-:{}, NC+:{}, NC-:{}, Value:{:.3e})", Name, N1, N2, NC1, NC2, Value));
}

std::vector<std::string>& D_CCVS::getInfoList()
{
    std::vector<std::string>* res = new std::vector<std::string>();
    res->push_back(std::format("N+:{}", N1));
    res->push_back(std::format("N-:{}", N2));
    res->push_back(std::format("NC+:{}", NC1));
    res->push_back(std::format("NC-:{}", NC2));
    res->push_back(std::format("Value:{:.3e}", Value));
    return *res;
}
