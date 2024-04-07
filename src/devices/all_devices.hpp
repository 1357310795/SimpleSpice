#pragma once
#include <iostream>
#include <string>
#include <format>
#include <complex>
#include "base_device.hpp"
#include "global.hpp"
#include "myconsole.h"
#include "circuit/circuit_node.h"
#include "devices/source_device.hpp"
#include "devices/dynamic_device.hpp"
#include "devices/Nonlinear_device.hpp"
#include "calc/analyze_context.h"
#include <Eigen/Dense>

class D_Resistor : public BaseDevice {

public:
    double Value;
    std::string N1;
    std::string N2;

    D_Resistor(std::string name, std::string n1, std::string n2, double value) 
        : BaseDevice(name), Value(value), N1(n1), N2(n2) {};

    std::string GetDeviceType() {
        return "Resistor";
    }

    template<typename MatrixType, typename VectorType>
    void appendStamp(
        int &nodeCount, 
        MatrixType &mat, 
        VectorType &rhs,
        std::vector<CircuitNode> &nodes
    ) {
        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });

        if (it1 == nodes.end() || it2 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);
        int index2 = std::distance(nodes.begin(), it2);

        mat(index1, index1) += 1/this->Value;
        mat(index1, index2) += -1/this->Value;
        mat(index2, index1) += -1/this->Value;
        mat(index2, index2) += 1/this->Value;
    }

    void appendStampDC(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes);
    }

    void appendStampAC(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->matc, context->rhsc, context->nodes);
    }

    void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) {
        if (node == N1) oss<<Name<<":N+"<<"\t";
        if (node == N2) oss<<Name<<":N-"<<"\t";
    }

    void print(MyConsole* console) {
        console->log(std::format("*Resistor(Name:{}, N+:{}, N-:{}, Value:{:.3e})", Name, N1, N2, Value));
    }

    std::vector<std::string>& getInfoList()
    {
        std::vector<std::string>* res = new std::vector<std::string>();
        res->push_back(std::format("N+:{}", N1));
        res->push_back(std::format("N-:{}", N2));
        res->push_back(std::format("Value:{:.3e}", Value));
        return *res;
    }
};

class D_Capacitor : virtual public BaseDevice, public DynamicDevice {

public:
    double Value;
    std::string N1;
    std::string N2;

    D_Capacitor(std::string name, std::string n1, std::string n2, double value) 
        : BaseDevice(name), DynamicDevice(), Value(value), N1(n1), N2(n2) {};

    std::string GetDeviceType() {
        return "Capacitor";
    }

    void appendStampDC(AnalyzeContext* context) {
        //do nothing
    }

    void appendStampAC(AnalyzeContext* context) {
        int &nodeCount = context->nodeCount; 
        Eigen::MatrixXcd* mat = &context->matc;  
        Eigen::VectorXcd* rhs = &context->rhsc; 
        std::vector<CircuitNode> &nodes = context->nodes; 

        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });

        if (it1 == nodes.end() || it2 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);
        int index2 = std::distance(nodes.begin(), it2);

        (*mat)(index1, index1) += std::complex<double>((double)0, (double)2 * M_PI * context->freq) * std::complex<double>(this->Value, (double)0);
        (*mat)(index1, index2) += std::complex<double>((double)0, (double)-2 * M_PI * context->freq) * std::complex<double>(this->Value, (double)0);
        (*mat)(index2, index1) += std::complex<double>((double)0, (double)-2 * M_PI * context->freq) * std::complex<double>(this->Value, (double)0);
        (*mat)(index2, index2) += std::complex<double>((double)0, (double)2 * M_PI * context->freq) * std::complex<double>(this->Value, (double)0);
    }

    void appendStampTRAN(AnalyzeContext* context) {
        int &nodeCount = context->nodeCount; 
        Eigen::MatrixXd* mat = &context->mat;
        Eigen::VectorXd* rhs = &context->rhs;
        std::vector<CircuitNode> &nodes = context->nodes;
        double interval = context->interval; 
        double lastValue = context->lastVoltages[this->Name]->back(); 

        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });

        if (it1 == nodes.end() || it2 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);
        int index2 = std::distance(nodes.begin(), it2);

        nodeCount++;
        int index3 = nodeCount - 1;
        (*mat).conservativeResize(nodeCount, nodeCount);
        (*mat).col(index3).fill(0);
        (*mat).row(index3).fill(0);

        (*rhs).conservativeResize(nodeCount);
        (*rhs)(index3) = this->Value/interval*lastValue;

        (*mat)(index1, index3) = 1;
        (*mat)(index2, index3) = -1;
        (*mat)(index3, index1) = this->Value/interval;
        (*mat)(index3, index2) = -this->Value/interval;
        (*mat)(index3, index3) = -1;
        
        nodes.push_back(CircuitNode(Name, "I"));
    }

    double getLastVoltage(AnalyzeContext* context) {
        int &nodeCount = context->nodeCount;
        Eigen::VectorXd* result = &context->res;
        std::vector<CircuitNode> &nodes = context->nodes;
        CircuitNode &ground = context->ground;

        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });

        if (it1 == nodes.end() && this->N1 != ground.nodeName) {
            throw std::runtime_error("Node not found");
        }
        if (it2 == nodes.end() && this->N2 != ground.nodeName) {
            throw std::runtime_error("Node not found");
        }
        double value1 = 0, value2 = 0;
        if (it1 != nodes.end())
        {
            int index1 = std::distance(nodes.begin(), it1);
            value1 = (*result)[index1];
        }
        if (it2 != nodes.end())
        {
            int index2 = std::distance(nodes.begin(), it2);
            value2 = (*result)[index2];
        }
        
        return value1 - value2;
    } 

    double getLastCurrent(AnalyzeContext* context) {
        int &nodeCount = context->nodeCount;
        Eigen::VectorXd* result = &context->res;
        std::vector<CircuitNode> &nodes = context->nodes;
        CircuitNode &ground = context->ground;

        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->Name; });

        if (it1 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);

        return (*result)[index1];
    } 

    double filterLTE(AnalyzeContext* context, double step) {
        if (context->lastCurrents[this->Name]->size() < 2)
            return step;
        auto it = context->lastCurrents[this->Name]->end();
        double tmp = 2*this->Value/(std::abs(*(it-1)-*(it-2)))*1e-3;
        if (step > tmp)
            return step / 2;
        else if (step * 2 < tmp)
            return step * 2;
        else
            return step;
    } 

    void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) {
        if (node == N1) oss<<Name<<":N+"<<"\t";
        if (node == N2) oss<<Name<<":N-"<<"\t";
    }

    void print(MyConsole* console) {
        console->log(std::format("*Capacitor(Name:{}, N+:{}, N-:{}, Value:{:.3e})", Name, N1, N2, Value));
    }

    std::vector<std::string>& getInfoList()
    {
        std::vector<std::string>* res = new std::vector<std::string>();
        res->push_back(std::format("N+:{}", N1));
        res->push_back(std::format("N-:{}", N2));
        res->push_back(std::format("Value:{:.3e}", Value));
        return *res;
    }
};

class D_Inductor : virtual public BaseDevice, public DynamicDevice {

public:
    double Value;
    std::string N1;
    std::string N2;

    D_Inductor(std::string name, std::string n1, std::string n2, double value) 
        : BaseDevice(name), DynamicDevice(), Value(value), N1(n1), N2(n2) {};

    std::string GetDeviceType() {
        return "Inductor";
    }

    void appendStampDC(AnalyzeContext* context) {
        int &nodeCount = context->nodeCount; 
        Eigen::MatrixXd &mat = context->mat;
        Eigen::VectorXd &rhs = context->rhs;
        std::vector<CircuitNode> &nodes = context->nodes; 

        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });

        if (it1 == nodes.end() || it2 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);
        int index2 = std::distance(nodes.begin(), it2);

        nodeCount++;
        int index3 = nodeCount - 1;
        mat.conservativeResize(nodeCount, nodeCount);
        mat.col(index3).fill(0);
        mat.row(index3).fill(0);

        rhs.conservativeResize(nodeCount);
        rhs(index3) = 0;

        mat(index1, index3) = 1;
        mat(index2, index3) = -1;
        mat(index3, index1) = 1;
        mat(index3, index2) = -1;
        
        nodes.push_back(CircuitNode(Name, "I"));
    }

    void appendStampAC(AnalyzeContext* context) {
        int &nodeCount = context->nodeCount; 
        Eigen::MatrixXcd* mat = &context->matc;
        Eigen::VectorXcd* rhs = &context->rhsc;
        std::vector<CircuitNode> &nodes = context->nodes; 
        
        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });

        if (it1 == nodes.end() || it2 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);
        int index2 = std::distance(nodes.begin(), it2);

        nodeCount++;
        int index3 = nodeCount - 1;
        (*mat).conservativeResize(nodeCount, nodeCount);
        (*mat).col(index3).fill(0);
        (*mat).row(index3).fill(0);

        (*rhs).conservativeResize(nodeCount);
        (*rhs)(index3) = 0;

        (*mat)(index1, index3) = 1;
        (*mat)(index2, index3) = -1;
        (*mat)(index3, index1) = 1;
        (*mat)(index3, index2) = -1;
        (*mat)(index3, index3) = std::complex<double>((double)0, (double)-2 * M_PI * context->freq) * std::complex<double>(this->Value, (double)0);

        nodes.push_back(CircuitNode(Name, "I"));
    }

    void appendStampTRAN(AnalyzeContext* context) {
        int &nodeCount = context->nodeCount; 
        Eigen::MatrixXd* mat = &context->mat;
        Eigen::VectorXd* rhs = &context->rhs;
        std::vector<CircuitNode> &nodes = context->nodes; 
        double interval = context->interval; 
        double lastValue = context->lastCurrents[this->Name]->back(); 

        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });

        if (it1 == nodes.end() || it2 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);
        int index2 = std::distance(nodes.begin(), it2);

        nodeCount++;
        int index3 = nodeCount - 1;
        (*mat).conservativeResize(nodeCount, nodeCount);
        (*mat).col(index3).fill(0);
        (*mat).row(index3).fill(0);

        (*rhs).conservativeResize(nodeCount);
        (*rhs)(index3) = -this->Value/interval*lastValue;

        (*mat)(index1, index3) = 1;
        (*mat)(index2, index3) = -1;
        (*mat)(index3, index1) = 1;
        (*mat)(index3, index2) = -1;
        (*mat)(index3, index3) = -this->Value/interval;
        
        nodes.push_back(CircuitNode(Name, "I"));
    }

    double getLastVoltage(AnalyzeContext* context) {
        int &nodeCount = context->nodeCount;
        Eigen::VectorXd* result = &context->res;
        std::vector<CircuitNode> &nodes = context->nodes;
        CircuitNode &ground = context->ground;

        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });

        if (it1 == nodes.end() && this->N1 != ground.nodeName) {
            throw std::runtime_error("Node not found");
        }
        if (it2 == nodes.end() && this->N2 != ground.nodeName) {
            throw std::runtime_error("Node not found");
        }
        double value1 = 0, value2 = 0;
        if (it1 != nodes.end())
        {
            int index1 = std::distance(nodes.begin(), it1);
            value1 = (*result)[index1];
        }
        if (it2 != nodes.end())
        {
            int index2 = std::distance(nodes.begin(), it2);
            value2 = (*result)[index2];
        }
        
        return value1 - value2;
    } 

    double getLastCurrent(AnalyzeContext* context) {
        int &nodeCount = context->nodeCount;
        Eigen::VectorXd* result = &context->res;
        std::vector<CircuitNode> &nodes = context->nodes;
        CircuitNode &ground = context->ground;

        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->Name; });

        if (it1 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);

        return (*result)[index1];
    } 

    double filterLTE(AnalyzeContext* context, double step) {
        if (context->lastVoltages[this->Name]->size() < 2)
            return step;
        auto it = context->lastVoltages[this->Name]->end();
        double tmp = 2*this->Value/(std::abs(*(it-1)-*(it-2)))*1e-3;
        if (step > tmp)
            return step / 2;
        else if (step * 2 < tmp)
            return step * 2;
        else
            return step;
    } 

    void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) {
        if (node == N1) oss<<Name<<":N+"<<"\t";
        if (node == N2) oss<<Name<<":N-"<<"\t";
    }

    void print(MyConsole* console) {
        console->log(std::format("*Inductor(Name:{}, N+:{}, N-:{}, Value:{:.3e})", Name, N1, N2, Value));
    }

    std::vector<std::string>& getInfoList()
    {
        std::vector<std::string>* res = new std::vector<std::string>();
        res->push_back(std::format("N+:{}", N1));
        res->push_back(std::format("N-:{}", N2));
        res->push_back(std::format("Value:{:.3e}", Value));
        return *res;
    }
};

class D_VSource : virtual public BaseDevice, public SourceDevice {

public:
    std::string N1;
    std::string N2;

    D_VSource(std::string name, std::string n1, std::string n2, double value) 
        : BaseDevice(name), N1(n1), N2(n2), SourceDevice(value) {};

    D_VSource(std::string name, std::string n1, std::string n2, double value, double ac_mag) 
        : BaseDevice(name), N1(n1), N2(n2), SourceDevice(value, ac_mag) {};

    D_VSource(std::string name, std::string n1, std::string n2, double value, double ac_mag, double ac_phrase) 
        : BaseDevice(name), N1(n1), N2(n2), SourceDevice(value, ac_mag, ac_phrase) {};

    std::string GetDeviceType() {
        return "VSource";
    }

    template<typename MatrixType, typename VectorType, typename ValueType>
    void appendStamp(
        int &nodeCount, 
        MatrixType &mat, 
        VectorType &rhs,
        std::vector<CircuitNode> &nodes,
        ValueType value
    ) {
        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });

        if (it1 == nodes.end() || it2 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);
        int index2 = std::distance(nodes.begin(), it2);

        nodeCount++;
        int index3 = nodeCount - 1;
        mat.conservativeResize(nodeCount, nodeCount);
        mat.col(index3).fill(0);
        mat.row(index3).fill(0);

        rhs.conservativeResize(nodeCount);
        rhs(index3) = 0;

        mat(index1, index3) = 1;
        mat(index2, index3) = -1;
        mat(index3, index1) = 1;
        mat(index3, index2) = -1;

        rhs(index3) = value;

        nodes.push_back(CircuitNode(Name, "I"));
    }

    void appendStampDC(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes, DC_Value);
    }

    void appendStampAC(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->matc, context->rhsc, context->nodes, std::complex(AC_Mag, (double)0));
    }

    void appendStampTRAN(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes, getValueByTime(context->ts));
    }

    void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) {
        if (node == N1) oss<<Name<<":N+"<<"\t";
        if (node == N2) oss<<Name<<":N-"<<"\t";
    }

    void print(MyConsole* console) {
        console->log(std::format("*Voltage Source(Name:{}, N+:{}, N-:{}, Value:{:.3e})", Name, N1, N2, DC_Value));
    }

    std::vector<std::string>& getInfoList()
    {
        std::vector<std::string>* res = new std::vector<std::string>();
        res->push_back(std::format("N+:{}", N1));
        res->push_back(std::format("N-:{}", N2));
        res->push_back(std::format("Value:{:.3e}", DC_Value));
        return *res;
    }
};

class D_ISource : virtual public BaseDevice, public SourceDevice {

public:
    std::string N1;
    std::string N2;

    D_ISource(std::string name, std::string n1, std::string n2, double value) 
        : BaseDevice(name), N1(n1), N2(n2), SourceDevice(value) {};

    D_ISource(std::string name, std::string n1, std::string n2, double value, double ac_mag) 
        : BaseDevice(name), N1(n1), N2(n2), SourceDevice(value, ac_mag) {};

    D_ISource(std::string name, std::string n1, std::string n2, double value, double ac_mag, double ac_phrase) 
        : BaseDevice(name), N1(n1), N2(n2), SourceDevice(value, ac_mag, ac_phrase) {};


    std::string GetDeviceType() {
        return "ISource";
    }

    template<typename MatrixType, typename VectorType, typename ValueType>
    void appendStamp(
        int &nodeCount, 
        MatrixType &mat, 
        VectorType &rhs,
        std::vector<CircuitNode> &nodes,
        ValueType value
    ) {
        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });

        if (it1 == nodes.end() || it2 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);
        int index2 = std::distance(nodes.begin(), it2);

        rhs[index1] += -value;
        rhs[index2] += value;
    }

    void appendStampDC(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes, DC_Value);
    }

    void appendStampAC(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->matc, context->rhsc, context->nodes, std::complex(AC_Mag, (double)0));
    }

    void appendStampTRAN(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes, getValueByTime(context->ts));
    }

    void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) {
        if (node == N1) oss<<Name<<":N+"<<"\t";
        if (node == N2) oss<<Name<<":N-"<<"\t";
    }

    void print(MyConsole* console) {
        console->log(std::format("*Current Source(Name:{}, N+:{}, N-:{}, Value:{:.3e})", Name, N1, N2, DC_Value));
    }

    std::vector<std::string>& getInfoList()
    {
        std::vector<std::string>* res = new std::vector<std::string>();
        res->push_back(std::format("N+:{}", N1));
        res->push_back(std::format("N-:{}", N2));
        res->push_back(std::format("Value:{:.3e}", DC_Value));
        return *res;
    }
};

class D_VCCS : public BaseDevice {

public:
    double Value;
    std::string N1;
    std::string N2;
    std::string NC1;
    std::string NC2;

    D_VCCS(std::string name, std::string n1, std::string n2, std::string nc1, std::string nc2, double value) 
        : BaseDevice(name), Value(value), N1(n1), N2(n2), NC1(nc1), NC2(nc2) {};

    std::string GetDeviceType() {
        return "VCCS";
    }

    template<typename MatrixType, typename VectorType>
    void appendStamp(
        int &nodeCount, 
        MatrixType &mat, 
        VectorType &rhs,
        std::vector<CircuitNode> &nodes
    ) {
        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });
        auto it3 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->NC1; });
        auto it4 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->NC2; });

        if (it1 == nodes.end() || it2 == nodes.end() 
            || it3 == nodes.end() || it4 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);
        int index2 = std::distance(nodes.begin(), it2);
        int index3 = std::distance(nodes.begin(), it3);
        int index4 = std::distance(nodes.begin(), it4);

        mat(index1, index3) += this->Value;
        mat(index1, index4) += -this->Value;
        mat(index2, index3) += -this->Value;
        mat(index2, index4) += this->Value;
    }

    void appendStampDC(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes);
    }

    void appendStampAC(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->matc, context->rhsc, context->nodes);
    }

    void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) {
        if (node == N1) oss<<Name<<":N+"<<"\t";
        if (node == N2) oss<<Name<<":N-"<<"\t";
        if (node == NC1) oss<<Name<<":NC+"<<"\t";
        if (node == NC2) oss<<Name<<":NC-"<<"\t";
    }

    void print(MyConsole* console) {
        console->log(std::format("*VCCS(Name:{}, N+:{}, N-:{}, NC+:{}, NC-:{}, Value:{:.3e})", Name, N1, N2, NC1, NC2, Value));
    }

    std::vector<std::string>& getInfoList()
    {
        std::vector<std::string>* res = new std::vector<std::string>();
        res->push_back(std::format("N+:{}", N1));
        res->push_back(std::format("N-:{}", N2));
        res->push_back(std::format("NC+:{}", NC1));
        res->push_back(std::format("NC-:{}", NC2));
        res->push_back(std::format("Value:{:.3e}", Value));
        return *res;
    }
};

class D_VCVS : public BaseDevice {

public:
    double Value;
    std::string N1;
    std::string N2;
    std::string NC1;
    std::string NC2;

    D_VCVS(std::string name, std::string n1, std::string n2, std::string nc1, std::string nc2, double value) 
        : BaseDevice(name), Value(value), N1(n1), N2(n2), NC1(nc1), NC2(nc2) {};

    std::string GetDeviceType() {
        return "VCVS";
    }

    template<typename MatrixType, typename VectorType>
    void appendStamp(
        int &nodeCount, 
        MatrixType &mat, 
        VectorType &rhs,
        std::vector<CircuitNode> &nodes
    ) {
        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });
        auto it3 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->NC1; });
        auto it4 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->NC2; });

        if (it1 == nodes.end() || it2 == nodes.end() 
            || it3 == nodes.end() || it4 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);
        int index2 = std::distance(nodes.begin(), it2);
        int index3 = std::distance(nodes.begin(), it3);
        int index4 = std::distance(nodes.begin(), it4);

        nodeCount++;
        int index5 = nodeCount - 1;
        mat.conservativeResize(nodeCount, nodeCount);
        mat.col(index5).fill(0);
        mat.row(index5).fill(0);

        rhs.conservativeResize(nodeCount);
        rhs(index5) = 0;

        mat(index1, index5) += 1;
        mat(index2, index5) += -1;
        mat(index5, index1) += 1;
        mat(index5, index2) += -1;
        mat(index5, index3) += -this->Value;
        mat(index5, index4) += this->Value;
        
        nodes.push_back(CircuitNode(Name, "I_k"));
    }

    void appendStampDC(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes);
    }

    void appendStampAC(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->matc, context->rhsc, context->nodes);
    }

    void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) {
        if (node == N1) oss<<Name<<":N+"<<"\t";
        if (node == N2) oss<<Name<<":N-"<<"\t";
        if (node == NC1) oss<<Name<<":NC+"<<"\t";
        if (node == NC2) oss<<Name<<":NC-"<<"\t";
    }

    void print(MyConsole* console) {
        console->log(std::format("*VCVS(Name:{}, N+:{}, N-:{}, NC+:{}, NC-:{}, Value:{:.3e})", Name, N1, N2, NC1, NC2, Value));
    }

    std::vector<std::string>& getInfoList()
    {
        std::vector<std::string>* res = new std::vector<std::string>();
        res->push_back(std::format("N+:{}", N1));
        res->push_back(std::format("N-:{}", N2));
        res->push_back(std::format("NC+:{}", NC1));
        res->push_back(std::format("NC-:{}", NC2));
        res->push_back(std::format("Value:{:.3e}", Value));
        return *res;
    }
};

class D_CCCS : public BaseDevice {

public:
    double Value;
    std::string N1;
    std::string N2;
    std::string NC1;
    std::string NC2;

    D_CCCS(std::string name, std::string n1, std::string n2, std::string nc1, std::string nc2, double value) 
        : BaseDevice(name), Value(value), N1(n1), N2(n2), NC1(nc1), NC2(nc2) {};

    std::string GetDeviceType() {
        return "CCCS";
    }

    template<typename MatrixType, typename VectorType>
    void appendStamp(
        int &nodeCount, 
        MatrixType &mat, 
        VectorType &rhs,
        std::vector<CircuitNode> &nodes
    ) {
        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });
        auto it3 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->NC1; });
        auto it4 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->NC2; });

        if (it1 == nodes.end() || it2 == nodes.end() 
            || it3 == nodes.end() || it4 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);
        int index2 = std::distance(nodes.begin(), it2);
        int index3 = std::distance(nodes.begin(), it3);
        int index4 = std::distance(nodes.begin(), it4);

        nodeCount++;
        int index5 = nodeCount - 1;
        mat.conservativeResize(nodeCount, nodeCount);
        mat.col(index5).fill(0);
        mat.row(index5).fill(0);

        rhs.conservativeResize(nodeCount);
        rhs(index5) = 0;

        mat(index1, index5) += this->Value;
        mat(index2, index5) += -this->Value;
        mat(index3, index5) += 1;
        mat(index4, index5) += -1;
        mat(index5, index3) += 1;
        mat(index5, index4) += -1;

        nodes.push_back(CircuitNode(Name, "I_c"));
    }

    void appendStampDC(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes);
    }

    void appendStampAC(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->matc, context->rhsc, context->nodes);
    }

    void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) {
        if (node == N1) oss<<Name<<":N+"<<"\t";
        if (node == N2) oss<<Name<<":N-"<<"\t";
        if (node == NC1) oss<<Name<<":NC+"<<"\t";
        if (node == NC2) oss<<Name<<":NC-"<<"\t";
    }

    void print(MyConsole* console) {
        console->log(std::format("*CCCS(Name:{}, N+:{}, N-:{}, NC+:{}, NC-:{}, Value:{:.3e})", Name, N1, N2, NC1, NC2, Value));
    }

    std::vector<std::string>& getInfoList()
    {
        std::vector<std::string>* res = new std::vector<std::string>();
        res->push_back(std::format("N+:{}", N1));
        res->push_back(std::format("N-:{}", N2));
        res->push_back(std::format("NC+:{}", NC1));
        res->push_back(std::format("NC-:{}", NC2));
        res->push_back(std::format("Value:{:.3e}", Value));
        return *res;
    }
};

class D_CCVS : public BaseDevice {

public:
    double Value;
    std::string N1;
    std::string N2;
    std::string NC1;
    std::string NC2;

    D_CCVS(std::string name, std::string n1, std::string n2, std::string nc1, std::string nc2, double value) 
        : BaseDevice(name), Value(value), N1(n1), N2(n2), NC1(nc1), NC2(nc2) {};

    std::string GetDeviceType() {
        return "CCVS";
    }

    template<typename MatrixType, typename VectorType>
    void appendStamp(
        int &nodeCount, 
        MatrixType &mat, 
        VectorType &rhs,
        std::vector<CircuitNode> &nodes
    ) {
        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });
        auto it3 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->NC1; });
        auto it4 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->NC2; });

        if (it1 == nodes.end() || it2 == nodes.end() 
            || it3 == nodes.end() || it4 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);
        int index2 = std::distance(nodes.begin(), it2);
        int index3 = std::distance(nodes.begin(), it3);
        int index4 = std::distance(nodes.begin(), it4);

        nodeCount+=2;
        int index5 = nodeCount - 2;
        int index6 = nodeCount - 1;
        mat.conservativeResize(nodeCount, nodeCount);
        mat.col(index5).fill(0);
        mat.col(index6).fill(0);
        mat.row(index5).fill(0);
        mat.row(index6).fill(0);

        rhs.conservativeResize(nodeCount);
        rhs(index5) = 0;
        rhs(index6) = 0;

        mat(index1, index5) += 1;
        mat(index2, index5) += -1;
        mat(index3, index6) += 1;
        mat(index4, index6) += -1;
        mat(index5, index1) += 1;
        mat(index5, index2) += -1;
        mat(index5, index6) += -this->Value;
        mat(index6, index3) += 1;
        mat(index6, index4) += -1;
        
        nodes.push_back(CircuitNode(Name, "I_k"));
        nodes.push_back(CircuitNode(Name, "I_c"));
    }

    void appendStampDC(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->mat, context->rhs, context->nodes);
    }

    void appendStampAC(AnalyzeContext* context) {
        appendStamp(context->nodeCount, context->matc, context->rhsc, context->nodes);
    }

    void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) {
        if (node == N1) oss<<Name<<":N+"<<"\t";
        if (node == N2) oss<<Name<<":N-"<<"\t";
        if (node == NC1) oss<<Name<<":NC+"<<"\t";
        if (node == NC2) oss<<Name<<":NC-"<<"\t";
    }

    void print(MyConsole* console) {
        console->log(std::format("*CCVS(Name:{}, N+:{}, N-:{}, NC+:{}, NC-:{}, Value:{:.3e})", Name, N1, N2, NC1, NC2, Value));
    }

    std::vector<std::string>& getInfoList()
    {
        std::vector<std::string>* res = new std::vector<std::string>();
        res->push_back(std::format("N+:{}", N1));
        res->push_back(std::format("N-:{}", N2));
        res->push_back(std::format("NC+:{}", NC1));
        res->push_back(std::format("NC-:{}", NC2));
        res->push_back(std::format("Value:{:.3e}", Value));
        return *res;
    }
};

class D_Diode : virtual public BaseDevice, public NonlinearDevice {

public:
    std::string N1;
    std::string N2;

    D_Diode(std::string name, std::string n1, std::string n2) 
        : BaseDevice(name), NonlinearDevice(), N1(n1), N2(n2) {};

    std::string GetDeviceType() {
        return "Diode";
    }

    template<typename MatrixType, typename VectorType>
    void appendStamp(
        int &nodeCount, 
        MatrixType &mat, 
        VectorType &rhs,
        std::vector<CircuitNode> &nodes,
        double lastVoltage,
        double lastCurrent
    ) {
        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });

        if (it1 == nodes.end() || it2 == nodes.end()) {
            throw std::runtime_error("Node not found");
        }

        int index1 = std::distance(nodes.begin(), it1);
        int index2 = std::distance(nodes.begin(), it2);

        double g = 40*std::exp(40*lastVoltage);
        double j = lastCurrent-g*lastVoltage;

        mat(index1, index1) += g;
        mat(index1, index2) += -g;
        mat(index2, index1) += -g;
        mat(index2, index2) += g;

        rhs(index1) += -j;
        rhs(index2) += j;
    }

    void appendStampDC(AnalyzeContext* context) {
        if (context->IterVoltages[this->Name]->back() < 0)
            return;
        this->appendStamp(
            context->nodeCount, 
            context->mat, 
            context->rhs, 
            context->nodes,
            context->IterVoltages[this->Name]->back(),
            context->IterCurrents[this->Name]->back()
        );
    }

    void appendStampAC(AnalyzeContext* context) {
        throw std::runtime_error("Not supported");
    }

    void appendStampTRAN(AnalyzeContext* context) {
        throw std::runtime_error("Not used");
    }

    double getLastVoltage(AnalyzeContext* context) {
        int &nodeCount = context->nodeCount;
        Eigen::VectorXd* result = &context->res;
        std::vector<CircuitNode> &nodes = context->nodes;
        CircuitNode &ground = context->ground;

        auto it1 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N1; });
        auto it2 = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == this->N2; });

        if (it1 == nodes.end() && this->N1 != ground.nodeName) {
            throw std::runtime_error("Node not found");
        }
        if (it2 == nodes.end() && this->N2 != ground.nodeName) {
            throw std::runtime_error("Node not found");
        }
        double value1 = 0, value2 = 0;
        if (it1 != nodes.end())
        {
            int index1 = std::distance(nodes.begin(), it1);
            value1 = (*result)[index1];
        }
        if (it2 != nodes.end())
        {
            int index2 = std::distance(nodes.begin(), it2);
            value2 = (*result)[index2];
        }
        
        return value1 - value2;
    } 

    double getLastCurrent(AnalyzeContext* context) {
        double v = getLastVoltage(context);
        return v<=0 ? 0 : (std::exp(40*v) - 1);
    } 

    void checkNode(
        std::ostringstream &oss,
        std::string &node
    ) {
        if (node == N1) oss<<Name<<":N+"<<"\t";
        if (node == N2) oss<<Name<<":N-"<<"\t";
    }

    void print(MyConsole* console) {
        console->log(std::format("*Diode(Name:{}, N+:{}, N-:{})", Name, N1, N2));
    }

    std::vector<std::string>& getInfoList()
    {
        std::vector<std::string>* res = new std::vector<std::string>();
        res->push_back(std::format("N+:{}", N1));
        res->push_back(std::format("N-:{}", N2));
        return *res;
    }
};
