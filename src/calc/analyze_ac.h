#pragma once
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
#include "devices/base_device.h"
#include "devices/dynamic_device.h"
#include "devices/nonlinear_device.h"
#include "devices/source_device.h"

class AnalyzeAC
{
private:
    Circuit* circuit;
    std::ostringstream oss;
    
    std::vector<CircuitNode> nodes;
    int nodeCount;

public:
    AnalyzeAC(Circuit* circuit, std::vector<CircuitNode>& nodes, int nodeCount);

    void SolveAC();

    void BuildACMNA(AnalyzeContext* context);

    // Eigen::VectorXcd& SolveACSingle(AnalyzeContext* context);

    Eigen::VectorXcd& SolveACOneTime(AnalyzeContext* context);
};