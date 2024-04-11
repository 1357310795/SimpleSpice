#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>
#include "console/myconsole.h"
#include "circuit/circuit_node.h"
#include "calc/analyze_context.h"
#include "devices/base_device.h"

std::string BaseDevice::getDeviceType() {
    return "undefined";
}

void BaseDevice::appendStampDC(AnalyzeContext* context) {
    throw std::runtime_error("Method Not Implemented");
}

void BaseDevice::appendStampAC(AnalyzeContext* context) {
    throw std::runtime_error("Method Not Implemented");
}

void BaseDevice::checkNode(
    std::ostringstream &oss,
    std::string &node
) {
    throw std::runtime_error("Method Not Implemented");
}

void BaseDevice::print(MyConsole* console) {
    throw std::runtime_error("Method Not Implemented");
}

std::vector<std::string>& BaseDevice::getInfoList()
{
    throw std::runtime_error("Method Not Implemented");
}
