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
#include "devices/item/isource.h"

D_ISource::D_ISource(std::string name, std::string n1, std::string n2, double value) 
    : BaseDevice(name), N1(n1), N2(n2), SourceDevice(value) {}

D_ISource::D_ISource(std::string name, std::string n1, std::string n2, double value, double ac_mag) 
    : BaseDevice(name), N1(n1), N2(n2), SourceDevice(value, ac_mag) {}

D_ISource::D_ISource(std::string name, std::string n1, std::string n2, double value, double ac_mag, double ac_phrase) 
    : BaseDevice(name), N1(n1), N2(n2), SourceDevice(value, ac_mag, ac_phrase) {}

std::string D_ISource::getDeviceType() {
    return "ISource";
}

template<typename MatrixType, typename VectorType, typename ValueType>
void D_ISource::appendStamp(
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
    rhs[index1] += -value;
    rhs[index2] += value;
}

void D_ISource::appendStampDC(AnalyzeContext* context) {
    appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes, DC_Value);
}

void D_ISource::appendStampAC(AnalyzeContext* context) {
    appendStamp(context->nodeCount, context->matc, context->rhsc, context->nodes, std::complex(AC_Mag, (double)0));
}

void D_ISource::appendStampTRAN(AnalyzeContext* context) {
    appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes, getValueByTime(context->ts));
}

void D_ISource::checkNode(
    std::ostringstream &oss,
    std::string &node
) {
    if (node == N1) oss<<Name<<":N+"<<"\t";
    if (node == N2) oss<<Name<<":N-"<<"\t";
}

void D_ISource::print(MyConsole* console) {
    console->log(std::format("*Current Source(Name:{}, N+:{}, N-:{}, Value:{:.3e})", Name, N1, N2, DC_Value));
}

std::vector<std::string>& D_ISource::getInfoList()
{
    std::vector<std::string>* res = new std::vector<std::string>();
    res->push_back(std::format("N+:{}", N1));
    res->push_back(std::format("N-:{}", N2));
    res->push_back(std::format("Value:{:.3e}", DC_Value));
    return *res;
}
