#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <Eigen/Dense>
#include "circuit/circuit.h"
#include "circuit/circuit_node.h"
#include "cmds/ac_dc.hpp"
#include "calc/analyze_context.h"

AnalyzeContext::AnalyzeContext() {}

AnalyzeContext::AnalyzeContext(std::vector<CircuitNode> &nodes, int& nodeCount) {
    this->nodes = nodes;
    this->nodeCount = nodeCount;
}

AnalyzeContext::~AnalyzeContext() {
    for (auto& pair : this->IterContexts) {
        for (auto& item : *(pair.second)) {
            delete item;
        }
    }
    for (auto& pair : this->lastCurrents) {
        delete pair.second;
    }
    for (auto& pair : this->lastVoltages) {
        delete pair.second;
    }
}