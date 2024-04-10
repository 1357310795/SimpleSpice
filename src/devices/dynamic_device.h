#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>
#include "console/myconsole.h"
#include "circuit/circuit_node.h"
#include "calc/analyze_context.h"
#include "devices/base_device.h"

class DynamicDevice: virtual public BaseDevice {
public:
    DynamicDevice();

    virtual void appendStampTRAN(AnalyzeContext* context);

    virtual double getLastVoltage(AnalyzeContext* context);

    virtual double getLastCurrent(AnalyzeContext* context);

    virtual double filterLTE(AnalyzeContext* context, double step);
};
