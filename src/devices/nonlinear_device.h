#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>
#include "console/myconsole.h"
#include "circuit/circuit_node.h"
#include "calc/analyze_context.h"
#include "devices/base_device.h"
#include "calc/iteration_context.hpp"

class NonlinearDevice: virtual public BaseDevice {
public:
    NonlinearDevice();

    virtual void appendStampTRAN(AnalyzeContext* context);

    virtual BaseIterationContext* getLastContext(AnalyzeContext* context);

    virtual BaseIterationContext* getDefaultIterationContext();

    virtual double checkConvergence(AnalyzeContext* context);
};
