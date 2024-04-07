#pragma once
#include <vector>
#include <string>

class Sinusoid {
public:
    double v0;
    double va;
    double freq;
    double tdelay;
    double theta;

    Sinusoid(
        double v0,
        double va,
        double freq,
        double tdelay,
        double theta
    ): v0(v0), va(va), freq(freq), tdelay(tdelay), theta(theta) {} 
};