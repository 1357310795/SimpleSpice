#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>
#include <cmath>
#include "global.hpp"
#include "myconsole.h"
#include "circuit/circuit_node.h"
#include "cmds/pulse.hpp"

class SourceDevice: virtual public BaseDevice {
public:
    double DC_Value;
    double AC_Mag;
    double AC_Phrase;

    Pulse* pulse;

    SourceDevice(double value) 
        : DC_Value(value), pulse(nullptr) {};

    SourceDevice(double value, double ac_mag) 
        : DC_Value(value), AC_Mag(ac_mag), pulse(nullptr) {};

    SourceDevice(double value, double ac_mag, double ac_phrase) 
        : DC_Value(value), AC_Mag(ac_mag), AC_Phrase(ac_phrase), pulse(nullptr) {};

    virtual void appendStampTRAN(
        int &nodeCount, 
        Eigen::MatrixXd &mat, 
        Eigen::VectorXd &rhs,
        std::vector<CircuitNode> &nodes,
        double t
    ) {
        throw std::runtime_error("Method Not Implemented");
    } 

    double getValueByTime(double t)
    {
        if (!pulse)
            return DC_Value;
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
};
