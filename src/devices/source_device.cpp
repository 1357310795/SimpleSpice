#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <Eigen/Dense>
#include "console/myconsole.h"
#include "circuit/circuit_node.h"
#include "cmds/pulse.hpp"
#include "cmds/sinusoid.hpp"
#include "calc/analyze_context.h"
#include "devices/source_device.h"

SourceDevice::SourceDevice(double value) 
    : DC_Value(value), pulse(nullptr), sin(nullptr) {};

SourceDevice::SourceDevice(double value, double ac_mag) 
    : DC_Value(value), AC_Mag(ac_mag), pulse(nullptr), sin(nullptr) {};

SourceDevice::SourceDevice(double value, double ac_mag, double ac_phrase) 
    : DC_Value(value), AC_Mag(ac_mag), AC_Phrase(ac_phrase), pulse(nullptr), sin(nullptr) {};

void SourceDevice::appendStampTRAN(AnalyzeContext* context) {
    throw std::runtime_error("Method Not Implemented");
} 

double SourceDevice::getValueByTime(double t)
{
    if (pulse)
        return getPulseValue(t);
    if (sin)
        return getSinusoidValue(t);
    
    return DC_Value;
}

double SourceDevice::getSinusoidValue(double t)
{
    if (t <= sin->tdelay)
        return sin->v0;
    t -= sin->tdelay;
    double period = 1/sin->freq;
    int p = floor(t / period);
    double t0 = t - p * period;
    return std::exp(-t*sin->theta) * sin->va * std::sin(2*M_PI*(t0/period));
}

double SourceDevice::getPulseValue(double t)
{
    int p = floor(t / pulse->period);
    double t0 = t - p * pulse->period;
    if (t0 <= pulse->tdelay)
        return pulse->v1;
    t0 -= pulse->tdelay;
    if (t0 <= pulse->trise)
        return pulse->v1 + t0 / pulse->trise * (pulse->v2 - pulse->v1);
    t0 -= pulse->trise;
    if (t0 <= pulse->pulseWidth)
        return pulse->v2;
    t0 -= pulse->pulseWidth;
    if (t0 <= pulse->tfall)
        return pulse->v2 - t0 / pulse->tfall * (pulse->v2 - pulse->v1);
    return pulse->v1;
}
