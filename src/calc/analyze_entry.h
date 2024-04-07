#pragma once
#include <vector>
#include <string>
#include "global.hpp"
#include <iostream>
#include <sstream>
#include <Eigen/Dense>
#include "myconsole.h"
#include "circuit/circuit.hpp"
#include "circuit/circuit_node.h"
#include "cmds/ac_dc.hpp"

class AnalyzeEntry {
public:
    Eigen::VectorXd result; //DC and TRAN
    Eigen::VectorXcd resultc; //AC
    std::vector<SweepOptionEntry> sweepEntries; //DC
    double freq; //AC
    double time; //TRAN
};