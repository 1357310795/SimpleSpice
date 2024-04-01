#pragma once
#include <vector>
#include <string>

class Pulse {
public:
    double v1;
    double v2;
    double tdelay;
    double trise;
    double tfall;
    double pulseWidth;
    double period;

    Pulse(
        double v1,
        double v2,
        double tdelay,
        double trise,
        double tfall,
        double pulseWidth,
        double period
    ): v1(v1), v2(v2), tdelay(tdelay), trise(trise), tfall(tfall), pulseWidth(pulseWidth), period(period) {} 
};