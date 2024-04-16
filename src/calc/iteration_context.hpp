#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

class BaseIterationContext {
public:
    std::string name;
    BaseIterationContext(std::string name) : name(name) {}

    virtual void workaround() {}
};

class DiodeIterationContext : public BaseIterationContext {
public:
    double iterVoltage;
    double iterCurrent;
    DiodeIterationContext(std::string name, double v, double i)
        : BaseIterationContext(name), iterVoltage(v), iterCurrent(i) {}
};

class MosfetIterationContext : public BaseIterationContext {
public:
    double vgs;
    double vds;
    MosfetIterationContext(std::string name, double vgs, double vds)
        : BaseIterationContext(name), vgs(vgs), vds(vds) {}
};
