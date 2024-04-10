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

class D_Capacitor : virtual public BaseDevice, public DynamicDevice {

public:
    double Value;
    std::string N1;
    std::string N2;

    D_Capacitor(std::string name, std::string n1, std::string n2, double value);

    std::string GetDeviceType() override;

    void appendStampDC(AnalyzeContext* context) override;

    void appendStampAC(AnalyzeContext* context) override;

    void appendStampTRAN(AnalyzeContext* context) override;
    
    double getLastVoltage(AnalyzeContext* context) override;

    double getLastCurrent(AnalyzeContext* context) override;

    double filterLTE(AnalyzeContext* context, double step) override;

    void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) override; 

    void print(MyConsole* console) override;

    std::vector<std::string>& getInfoList() override;
};
