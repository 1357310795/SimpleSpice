#pragma once
#include <vector>
#include <string>
#include "circuit/circuit_node.h"

enum PlotType{
    PLOT_DC,
    PLOT_AC,
    PLOT_TRAN
};

class Command_PLOT {
public:
    bool enabled;
    PlotType type;
    std::vector<CircuitNode> nodes;
};
