#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>
#include <cmath>
#include "cmds/pulse.hpp"
#include "cmds/sinusoid.hpp"
#include "calc/analyze_context.h"
#include "devices/base_device.h"

class SourceDevice: virtual public BaseDevice {
public:
    double DC_Value;
    double AC_Mag;
    double AC_Phrase;

    Pulse* pulse;
    Sinusoid* sin;

    SourceDevice(double value);

    SourceDevice(double value, double ac_mag);

    SourceDevice(double value, double ac_mag, double ac_phrase);

    virtual void appendStampTRAN(AnalyzeContext* context);

    double getValueByTime(double t);

    double getSinusoidValue(double t);

    double getPulseValue(double t);
};
