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

enum MosfetType
{
    NMOS,
    PMOS
};

class D_Mosfet : virtual public BaseDevice, public NonlinearDevice {
private:
    const double vth_n = 0.47965;
    const double mucox_n = 0.03634841053;

    const double vth_p = -0.43121;
    const double mucox_p = 0.03453099;

public:
    std::string ND;
    std::string NG;
    std::string NS;
    std::string NB;
    std::string Model;
    MosfetType Type;
    double Width;
    double Length;

    D_Mosfet(
        std::string name, 
        std::string nd, 
        std::string ng, 
        std::string ns, 
        std::string nb, 
        std::string model
    );

    ~D_Mosfet() {}

    std::string getDeviceType() override;

    template<typename MatrixType, typename VectorType>
    void appendStamp(
        int &nodeCount, 
        MatrixType &mat, 
        VectorType &rhs,
        std::vector<CircuitNode> &nodes,
        MosfetIterationContext &iterContext
    );

    void appendStampDC(AnalyzeContext* context)override;

    void appendStampAC(AnalyzeContext* context)override;

    void appendStampTRAN(AnalyzeContext* context)override;

    BaseIterationContext* getLastContext(AnalyzeContext* context) override;

    BaseIterationContext* getDefaultIterationContext() override;

    double checkConvergence(AnalyzeContext* context) override;

    double getVth();

    double getmuCox();
    
    double getK();

    double getCurrent(double vgs, double vds);

    bool checkOpen(double vgs);

    void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) override;

    void print(MyConsole* console) override;

    std::vector<std::string>& getInfoList() override;
private:
    int FindNodeIndex(std::vector<CircuitNode> &nodes, std::string nodeName);
};
