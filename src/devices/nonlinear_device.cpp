#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>
#include "console/myconsole.h"
#include "circuit/circuit_node.h"
#include "calc/analyze_context.h"
#include "calc/iteration_context.hpp"
#include "devices/nonlinear_device.h"

NonlinearDevice::NonlinearDevice() {};

void NonlinearDevice::appendStampTRAN(AnalyzeContext* context) {
    throw std::runtime_error("Method Not Implemented");
} 

BaseIterationContext* NonlinearDevice::getLastContext(AnalyzeContext* context) {
    throw std::runtime_error("Method Not Implemented");
} 

BaseIterationContext* NonlinearDevice::getDefaultIterationContext() {
    throw std::runtime_error("Method Not Implemented");
} 

double NonlinearDevice::checkConvergence(AnalyzeContext* context) {
    throw std::runtime_error("Method Not Implemented");
} 

