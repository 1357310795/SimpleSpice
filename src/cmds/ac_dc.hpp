#pragma once
#include <vector>
#include <string>

class SweepOptionEntry {
public:
    std::string deviceName;
    double value;

    SweepOptionEntry(std::string devicename, double value)
        : deviceName(devicename), value(value) {};
};

class SweepOption {
public:
    std::string deviceName;
    double start;
    double stop;
    double step;

    SweepOption(std::string devicename, double start, double stop, double step)
        : deviceName(devicename), start(start), stop(stop), step(step) {};
};

enum ACSweepType {
    DEC,
    OCT,
    LIN,
    POI
};

class Command_AC {
public:
    bool enabled;
    ACSweepType type;
    int np;
    double fstart;
    double fstop;
    std::vector<SweepOption> sweepOptions;
};

class Command_DC {
public:
    bool enabled;
    std::vector<SweepOption> sweepOptions;
};

class Command_OP {
public:
    bool enabled;
};