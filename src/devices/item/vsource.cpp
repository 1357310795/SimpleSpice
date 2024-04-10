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
#include "devices/item/vsource.h"

D_VSource::D_VSource(std::string name, std::string n1, std::string n2, double value) 
    : BaseDevice(name), N1(n1), N2(n2), SourceDevice(value) {}

D_VSource::D_VSource(std::string name, std::string n1, std::string n2, double value, double ac_mag) 
    : BaseDevice(name), N1(n1), N2(n2), SourceDevice(value, ac_mag) {}

D_VSource::D_VSource(std::string name, std::string n1, std::string n2, double value, double ac_mag, double ac_phrase) 
    : BaseDevice(name), N1(n1), N2(n2), SourceDevice(value, ac_mag, ac_phrase) {}

std::string D_VSource::GetDeviceType() {
    return "VSource";
}

template<typename MatrixType, typename VectorType, typename ValueType>
void D_VSource::appendStamp(
    int &nodeCount, 
    MatrixType &mat, 
    VectorType &rhs,
    std::vector<CircuitNode> &nodes,
    ValueType value
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
    rhs(index3) = value;
    nodes.push_back(CircuitNode(Name, "I"));
}

void D_VSource::appendStampDC(AnalyzeContext* context) {
    appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes, DC_Value);
}

void D_VSource::appendStampAC(AnalyzeContext* context) {
    appendStamp(context->nodeCount, context->matc, context->rhsc, context->nodes, std::complex(AC_Mag, (double)0));
}

void D_VSource::appendStampTRAN(AnalyzeContext* context) {
    appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes, getValueByTime(context->ts));
}

void D_VSource::checkNode(
    std::ostringstream &oss,
    std::string &node
) {
    if (node == N1) oss<<Name<<":N+"<<"\t";
    if (node == N2) oss<<Name<<":N-"<<"\t";
}

void D_VSource::print(MyConsole* console) {
    console->log(std::format("*Voltage Source(Name:{}, N+:{}, N-:{}, Value:{:.3e})", Name, N1, N2, DC_Value));
}

std::vector<std::string>& D_VSource::getInfoList()
{
    std::vector<std::string>* res = new std::vector<std::string>();
    res->push_back(std::format("N+:{}", N1));
    res->push_back(std::format("N-:{}", N2));
    res->push_back(std::format("Value:{:.3e}", DC_Value));
    return *res;
}
