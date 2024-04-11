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

class D_ISource : virtual public BaseDevice, public SourceDevice {

public:
    std::string N1;
    std::string N2;

    D_ISource(std::string name, std::string n1, std::string n2, double value);

    D_ISource(std::string name, std::string n1, std::string n2, double value, double ac_mag);

    D_ISource(std::string name, std::string n1, std::string n2, double value, double ac_mag, double ac_phrase);

    ~D_ISource() {}

    std::string getDeviceType() override;

    template<typename MatrixType, typename VectorType, typename ValueType>
    void appendStamp(
        int &nodeCount, 
        MatrixType &mat, 
        VectorType &rhs,
        std::vector<CircuitNode> &nodes,
        ValueType value
    );

    void appendStampDC(AnalyzeContext* context) override;

    void appendStampAC(AnalyzeContext* context) override;

    void appendStampTRAN(AnalyzeContext* context) override;

    void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) override; 

    void print(MyConsole* console) override;

    std::vector<std::string>& getInfoList() override;
};