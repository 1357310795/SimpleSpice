#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>
#include "console/myconsole.h"
#include "circuit/circuit_node.h"
#include "calc/analyze_context.h"

class BaseDevice {
private:
    int id;

public:
    std::string Name;

    BaseDevice() {}
    BaseDevice(std::string name) : Name(name) {}
    virtual ~BaseDevice() {}

    virtual std::string getDeviceType();
    
    virtual void appendStampDC(AnalyzeContext* context);

    virtual void appendStampAC(AnalyzeContext* context);

    virtual void checkNode(
        std::ostringstream &oss,
        std::string &node
    );

    virtual void print(MyConsole* console);

    virtual std::vector<std::string>& getInfoList();
};
