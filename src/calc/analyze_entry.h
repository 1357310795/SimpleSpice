#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <Eigen/Dense>
#include "console/myconsole.h"
#include "circuit/circuit.h"
#include "circuit/circuit_node.h"
#include "cmds/ac_dc.hpp"
#include "global/global.h"

class AnalyzeEntry {
public:
    Eigen::VectorXd result; //DC and TRAN
    Eigen::VectorXcd resultc; //AC
    std::vector<SweepOptionEntry> sweepEntries; //DC
    double freq; //AC
    double time; //TRAN
};