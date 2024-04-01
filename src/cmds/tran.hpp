#pragma once
#include <vector>
#include <string>

class Command_TRAN {
public:
    bool enabled;
    double tstep;
    double tstop;
    double tstart;

    Command_TRAN(): tstep(0), tstop(0), tstart(0) {}
};