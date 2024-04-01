#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>
#include "global.hpp"
#include "myconsole.h"
#include "circuit/circuit_node.h"

class DynamicDevice: virtual public BaseDevice {
public:

    DynamicDevice() {};

    virtual void appendStampTRAN(
        int &nodeCount, 
        Eigen::MatrixXd &mat, 
        Eigen::VectorXd &rhs,
        std::vector<CircuitNode> &nodes,
        double interval,
        double lastValue
    ) {
        throw std::runtime_error("Method Not Implemented");
    } 

    virtual double getLastValue(
        int &nodeCount, 
        Eigen::VectorXd &result,
        std::vector<CircuitNode> &nodes,
        CircuitNode &ground
    ) {
        throw std::runtime_error("Method Not Implemented");
    } 
};
