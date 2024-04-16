#pragma once
#include <vector>
#include <string>
#include "circuit/circuit_node.h"

enum PrintType{
    PRINT_DC,
    PRINT_AC,
    PRINT_TRAN,
    PRINT_OP
};

class Command_PRINT {
public:
    bool enabled;
    PrintType type;
    std::vector<CircuitNode> nodes;
};
