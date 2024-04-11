#pragma once
#include <iostream>
#include <string>
#include <format>
#include <complex>
#include <Eigen/Dense>
#include "devices/base_device.h"
#include "console/myconsole.h"
#include "circuit/circuit_node.h"
#include "devices/source_device.h"
#include "devices/dynamic_device.h"
#include "devices/nonlinear_device.h"
#include "calc/analyze_context.h"

class D_VCVS : public BaseDevice {

public:
    double Value;
    std::string N1;
    std::string N2;
    std::string NC1;
    std::string NC2;

    D_VCVS(std::string name, std::string n1, std::string n2, std::string nc1, std::string nc2, double value);

    ~D_VCVS() {}

    std::string getDeviceType() override;

    template<typename MatrixType, typename VectorType>
    void appendStamp(
        int &nodeCount, 
        MatrixType &mat, 
        VectorType &rhs,
        std::vector<CircuitNode> &nodes
    );

    void appendStampDC(AnalyzeContext* context) override;

    void appendStampAC(AnalyzeContext* context) override;

    void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) override; 

    void print(MyConsole* console) override;

    std::vector<std::string>& getInfoList() override;
};