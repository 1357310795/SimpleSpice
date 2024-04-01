#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>
#include "global.hpp"
#include "myconsole.h"
#include "circuit/circuit_node.h"

class BaseDevice {
private:
    int id;

public:
    std::string Name;

    BaseDevice() {};
    BaseDevice(std::string name) : Name(name) {};

    virtual std::string GetDeviceType() {
        return "undefined";
    }
    
    virtual void appendStampDC(
        int &nodeCount, 
        Eigen::MatrixXd &mat, 
        Eigen::VectorXd &rhs,
        std::vector<CircuitNode> &nodes
    ) {
        throw std::runtime_error("Method Not Implemented");
    }

    virtual void appendStampAC(
        int &nodeCount, 
        Eigen::MatrixXcd &mat, 
        Eigen::VectorXcd &rhs,
        std::vector<CircuitNode> &nodes
    ) {
        throw std::runtime_error("Method Not Implemented");
    }

    virtual void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) {
        throw std::runtime_error("Method Not Implemented");
    }

    virtual void print(MyConsole* console) {
        throw std::runtime_error("Method Not Implemented");
    }

    virtual std::vector<std::string>& getInfoList()
    {
        throw std::runtime_error("Method Not Implemented");
    }
};
