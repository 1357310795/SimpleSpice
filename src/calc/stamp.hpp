#pragma once
#include <QtCharts>

#include <vector>
#include <string>
#include "global.hpp"
#include <iostream>
#include <sstream>
#include <Eigen/Dense>
#include "myconsole.h"
#include "circuit/circuit.hpp"
#include "circuit/circuit_node.h"
#include "calc/analyze_entry.h"
#include "chart/mychart.h"

extern MyConsole* console;
class Circuit;

class CircuitAnalyze
{
private:
    Circuit* circuit;

    Eigen::MatrixXd mat;
    Eigen::VectorXd rhs;
    std::vector<CircuitNode> nodes;
    std::vector<CircuitNode> actualNodes;
    std::ostringstream oss;

    Eigen::MatrixXcd matc;
    Eigen::VectorXcd rhsc;

    CircuitNode ground;
    int nodeCount;
    int actualNodeCount;

public:
    CircuitAnalyze(Circuit* circuit): circuit(circuit) {};

    void StartAnalyze()
    {
        nodeCount = circuit->nodemap.size();
        if (nodeCount <= 0)
        {
            console->log("[SpParser] no nodes! stop analyze!");
            return;
        }
        for (const auto& pair : circuit->nodemap) {
            nodes.push_back(CircuitNode(pair.first, "V"));
        }
        std::sort(nodes.begin(), nodes.end(), 
            [&](CircuitNode& v1, CircuitNode& v2){ return v1.nodeName < v2.nodeName; });

        if (circuit->command_OPTIONS.node)
        {
            console->log("****** element node table");
            for (auto& node : nodes) {
                oss<<node.nodeName<<'\t';
                for (auto& device : circuit->devices) {
                    device->checkNode(oss, node.nodeName);
                }
                console->log(oss.str()); oss = std::ostringstream();
            }
        }
        if (circuit->command_OPTIONS.list)
        {
            console->log("****** element list");
            for (auto& device : circuit->devices) {
                device->print(console);
            }
        }

        console->log("[SpParser] node name:");
        for (const auto& node : nodes) {
            oss<<node.nodeName<<'\t';
            console->log(oss.str()); oss = std::ostringstream();
        }

        if (circuit->command_DC.enabled || circuit->command_OP.enabled)
        {
            if (circuit->command_DC.sweepOptions.size() > 1)
            {
                console->log("[SpParser] circuit analyzer doesn't support multiple sweeps currently!");
                return;
            }
            SolveDC();
        }
        if (circuit->command_AC.enabled)
        {
            BuildACMNA();
        }
        if (circuit->command_TRAN.enabled)
        {
            SolveTran();
        }
    }

    void BuildDCMNA()
    {
        // console->log(std::format("[SpParser] Start Build DC MNA"));
        actualNodeCount = nodeCount;
        actualNodes = nodes;
        mat.resize(actualNodeCount, actualNodeCount);
        rhs.resize(actualNodeCount, Eigen::NoChange);
        mat.fill(0);
        rhs.fill(0);

        for (auto& device : circuit->devices) {
            device->appendStampDC(actualNodeCount, mat, rhs, actualNodes);
        }

        // console->log("[SpParser] MNA Matrix (including ground)");
        // oss<<mat<<std::endl;
        // console->log(oss.str()); oss = std::ostringstream();

        // console->log("[SpParser] RHS Vector");
        // oss<<rhs<<std::endl;
        // console->log(oss.str()); oss = std::ostringstream();
    }

    void BuildTRANMNA(
        std::unordered_map<std::string, double> &lastValues,
        double ts,
        double interval
    ) {
        // console->log(std::format("[SpParser] Start Build TRAN MNA"));
        actualNodeCount = nodeCount;
        actualNodes = nodes;
        mat.resize(actualNodeCount, actualNodeCount);
        rhs.resize(actualNodeCount, Eigen::NoChange);
        mat.fill(0);
        rhs.fill(0);

        for (auto& device : circuit->devices) {
            DynamicDevice* dd = dynamic_cast<DynamicDevice*>(device);
            if (dd)
            {
                double value = lastValues[dd->Name];
                dd->appendStampTRAN(actualNodeCount, mat, rhs, actualNodes, interval, value);
            }
            else
            {
                SourceDevice* sd = dynamic_cast<SourceDevice*>(device);
                if (sd)
                {
                    sd->appendStampTRAN(actualNodeCount, mat, rhs, actualNodes, ts);
                }
                else
                {
                    device->appendStampDC(actualNodeCount, mat, rhs, actualNodes);
                }
            }
        }

        // console->log("[SpParser] MNA Matrix (including ground)");
        // oss<<mat<<std::endl;
        // console->log(oss.str()); oss = std::ostringstream();

        // console->log("[SpParser] RHS Vector");
        // oss<<rhs<<std::endl;
        // console->log(oss.str()); oss = std::ostringstream();
    }

    void BuildACMNA()
    {
        // console->log(std::format("[SpParser] Start Build AC MNA"));
        actualNodeCount = nodeCount;
        actualNodes = nodes;
        matc.resize(actualNodeCount, actualNodeCount);
        rhsc.resize(actualNodeCount, Eigen::NoChange);
        matc.fill(0);
        rhsc.fill(0);

        for (auto& device : circuit->devices) {
            device->appendStampAC(actualNodeCount, matc, rhsc, actualNodes);
        }

        // console->log("[SpParser] MNA Matrix (including ground)");
        // oss<<matc<<std::endl;
        // console->log(oss.str()); oss = std::ostringstream();

        // console->log("[SpParser] RHS Vector");
        // oss<<rhsc<<std::endl;
        // console->log(oss.str()); oss = std::ostringstream();
    }

    void SolveDC()
    {
        std::vector<AnalyzeEntry*> results;
        AnalyzeEntry* entry = NULL;
        if (circuit->command_DC.sweepOptions.size() == 0)
        {
            auto& res = SolveDCOneTime();
            entry = new AnalyzeEntry();
            entry->result = res;
            results.push_back(entry);
        }
        else
        {
            SweepOption& opt = circuit->command_DC.sweepOptions[0];
            SourceDevice* source = NULL;
            for (auto& device : circuit->devices) {
                if (device->Name == opt.deviceName)
                {
                    source = dynamic_cast<SourceDevice*>(device);
                }
            }
            if (!source)
            {
                console->log("[SpParser] can not find" + opt.deviceName);
                return;
            }
            for(double v = opt.start; std::abs(v-opt.stop) > 1e-5; v += opt.step)
            {
                source->DC_Value = v;
                auto& res = SolveDCOneTime();
                entry = new AnalyzeEntry();
                entry->result = res;
                entry->sweepEntries.push_back(SweepOptionEntry(opt.deviceName, v));
                results.push_back(entry);
            }
        }

        console->log("[SpParser] Result:");
        for(auto& entry: results)
        {
            if (entry->sweepEntries.size() > 0)
            {
                console->log(std::format("For {} = {}:", entry->sweepEntries[0].deviceName, entry->sweepEntries[0].value));
            }
            for(int i = 0; i < actualNodeCount - 1; i++)
            {
                oss<<actualNodes[i].prefix<<'('<<actualNodes[i].nodeName<<')'<<'\t'<<'='<<'\t'<<entry->result(i);
                console->log(oss.str()); oss = std::ostringstream();
            }
        }
        
    }

    Eigen::VectorXd& SolveDCOneTime()
    {
        BuildDCMNA();

        Eigen::MatrixXd mat_ex = mat.block(1, 1, actualNodeCount - 1, actualNodeCount - 1);
        Eigen::VectorXd rhs_ex = rhs.tail(actualNodeCount - 1);
        actualNodes.erase(actualNodes.begin());

        console->log("[SpParser] Start Solve DC");
        console->log("[SpParser] MNA Matrix (excluding ground)");
        oss<<mat_ex<<std::endl;
        console->log(oss.str()); oss = std::ostringstream();
        
        console->log("[SpParser] RHS Vector");
        oss<<rhs_ex<<std::endl;
        console->log(oss.str()); oss = std::ostringstream();

        Eigen::VectorXd* x = new Eigen::VectorXd();
        *x = mat_ex.fullPivLu().solve(rhs_ex);
        return *x;
    }

    Eigen::VectorXd& SolveTRANOneTime(
        std::unordered_map<std::string, double> &lastValues, 
        double ts,
        double interval)
    {
        BuildTRANMNA(lastValues, ts, interval);
        ground = actualNodes[0];
        Eigen::MatrixXd mat_ex = mat.block(1, 1, actualNodeCount - 1, actualNodeCount - 1);
        Eigen::VectorXd rhs_ex = rhs.tail(actualNodeCount - 1);
        actualNodes.erase(actualNodes.begin());

        console->log("[SpParser] Start Solve TRAN");
        console->log("[SpParser] MNA Matrix (excluding ground)");
        oss<<mat_ex<<std::endl;
        console->log(oss.str()); oss = std::ostringstream();
        
        console->log("[SpParser] RHS Vector");
        oss<<rhs_ex<<std::endl;
        console->log(oss.str()); oss = std::ostringstream();

        Eigen::VectorXd* x = new Eigen::VectorXd();
        *x = mat_ex.fullPivLu().solve(rhs_ex);
        return *x;
    }

    void SolveTran()
    {
        std::unordered_map<std::string, double> lastValues;

        std::vector<AnalyzeEntry*> results;
        AnalyzeEntry* entry = NULL;

        double ts = circuit->command_TRAN.tstart;
        while (ts < circuit->command_TRAN.tstop)
        {
            ts += circuit->command_TRAN.tstep;

            auto& res = SolveTRANOneTime(lastValues, ts, circuit->command_TRAN.tstep);
            entry = new AnalyzeEntry();
            entry->time = ts;
            entry->result = res;
            results.push_back(entry);
            
            for (auto& device : circuit->devices) {
                DynamicDevice* dd = dynamic_cast<DynamicDevice*>(device);
                if (dd)
                {
                    double value = dd->getLastValue(actualNodeCount, res, actualNodes, ground);
                    lastValues[dd->Name] = value;
                }
            }
        }

        console->log("[SpParser] Result:");
        for(auto& entry: results)
        {
            console->log(std::format("For time {}:", entry->time));
            for(int i = 0; i < actualNodeCount - 1; i++)
            {
                oss<<actualNodes[i].prefix<<'('<<actualNodes[i].nodeName<<')'<<'\t'<<'='<<'\t'<<entry->result(i);
                console->log(oss.str()); oss = std::ostringstream();
            }
        }

        if (circuit->command_PLOT.enabled)
        {
            PlotTRAN(actualNodeCount - 1, actualNodes, results);
        }
    }

    void PlotTRAN(
        int nodeCount,
        std::vector<CircuitNode>& nodes,
        std::vector<AnalyzeEntry*>& results
    ) {
        std::vector<QAbstractSeries*> *seriesGroups = new std::vector<QAbstractSeries*>();
        for (auto & plotItem : circuit->command_PLOT.nodes)
        {
            auto it = std::find_if(nodes.begin(), nodes.end(), 
                [&](CircuitNode& n){ return n.nodeName == plotItem.nodeName && n.prefix == plotItem.prefix; });
            
            if (it == nodes.end())
                continue;

            int index = std::distance(nodes.begin(), it);

            QSplineSeries* series = new QSplineSeries();   // 创建一个样条曲线对象
            series->setName(QString("%1(%2)").arg(
                QString::fromStdString(plotItem.prefix), 
                QString::fromStdString(plotItem.nodeName)
            ));
            
            for(auto& entry: results)
            {
                series->append(entry->time, entry->result(index));
            }
            seriesGroups->push_back(series);
        }
        ChartViewWindow* chart = new ChartViewWindow();
        chart->initChart(*seriesGroups);
        chart->show();
        console->log("[SpParser] Plotted.");
    }
};
