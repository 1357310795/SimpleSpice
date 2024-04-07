#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>
#include "global.hpp"
#include "myconsole.h"
#include "circuit/circuit_node.h"
#include "calc/analyze_context.h"

class NonlinearDevice: virtual public BaseDevice {
public:

    NonlinearDevice() {};

    virtual void appendStampTRAN(AnalyzeContext* context) {
        throw std::runtime_error("Method Not Implemented");
    } 

    virtual double getLastVoltage(AnalyzeContext* context) {
        throw std::runtime_error("Method Not Implemented");
    } 

    virtual double getLastCurrent(AnalyzeContext* context) {
        throw std::runtime_error("Method Not Implemented");
    } 

    virtual bool checkConvergence(AnalyzeContext* context) {
        throw std::runtime_error("Method Not Implemented");
    } 
};
