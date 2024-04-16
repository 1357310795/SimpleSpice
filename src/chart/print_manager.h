#pragma once
#include <QtCharts>

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <Eigen/Dense>
#include "console/myconsole.h"
#include "circuit/circuit.h"
#include "circuit/circuit_node.h"
#include "calc/analyze_entry.h"
#include "calc/analyze_context.h"
#include "chart/mychart.h"
#include "global/global.h"
#include "cmds/plot.hpp"
#include "devices/base_device.h"
#include "devices/dynamic_device.h"
#include "devices/nonlinear_device.h"
#include "devices/source_device.h"

class PrintManager {
public:
    static void PrintTRAN(
        int nodeCount,
        std::vector<CircuitNode>& nodes,
        std::vector<AnalyzeEntry*>& results,
        Command_PRINT& command_PRINT
    );

    static void PrintDC(
        int nodeCount,
        std::vector<CircuitNode>& nodes,
        std::vector<AnalyzeEntry*>& results,
        Command_PRINT& command_PRINT
    );

    static void PrintAC(
        int nodeCount,
        std::vector<CircuitNode>& nodes,
        std::vector<AnalyzeEntry*>& results,
        Command_PRINT& command_PRINT
    );

    static void PrintOP(
        int nodeCount,
        std::vector<CircuitNode>& nodes,
        std::vector<AnalyzeEntry*>& results,
        Command_PRINT& command_PRINT
    );

    static void Print(
        int nodeCount,
        std::vector<CircuitNode>& nodes,
        std::vector<AnalyzeEntry*>& results
    );
};
