#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>
#include "console/myconsole.h"
#include "circuit/circuit_node.h"
#include "calc/analyze_context.h"
#include "devices/dynamic_device.h"

DynamicDevice::DynamicDevice() {}

void DynamicDevice::appendStampTRAN(AnalyzeContext* context) {
    throw std::runtime_error("Method Not Implemented");
} 

double DynamicDevice::getLastVoltage(AnalyzeContext* context) {
    throw std::runtime_error("Method Not Implemented");
} 

double DynamicDevice::getLastCurrent(AnalyzeContext* context) {
    throw std::runtime_error("Method Not Implemented");
} 

double DynamicDevice::filterLTE(AnalyzeContext* context, double step) {
    throw std::runtime_error("Method Not Implemented");
}

