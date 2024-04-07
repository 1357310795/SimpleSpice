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
#include "calc/analyze_context.h"
#include "chart/mychart.h"

extern MyConsole* console;
class Circuit;

class CircuitAnalyze
{
private:
    Circuit* circuit;
    std::ostringstream oss;
    
    std::vector<CircuitNode> nodes;
    int nodeCount;

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
            SolveAC();
        }
        if (circuit->command_TRAN.enabled)
        {
            SolveTran();
        }
    }

    void BuildDCMNA(AnalyzeContext* context)
    {
        // console->log(std::format("[SpParser] Start Build DC MNA"));
        context->mat.resize(context->nodeCount, context->nodeCount);
        context->rhs.resize(context->nodeCount, Eigen::NoChange);
        context->mat.fill(0);
        context->rhs.fill(0);

        for (auto& device : circuit->devices) {
            device->appendStampDC(context);
        }

        // console->log("[SpParser] MNA Matrix (including ground)");
        // oss<<mat<<std::endl;
        // console->log(oss.str()); oss = std::ostringstream();

        // console->log("[SpParser] RHS Vector");
        // oss<<rhs<<std::endl;
        // console->log(oss.str()); oss = std::ostringstream();
    }

    void BuildTRANMNA(AnalyzeContext* context) {
        // console->log(std::format("[SpParser] Start Build TRAN MNA"));
        context->mat.resize(context->nodeCount, context->nodeCount);
        context->rhs.resize(context->nodeCount, Eigen::NoChange);
        context->mat.fill(0);
        context->rhs.fill(0);

        for (auto& device : circuit->devices) {
            DynamicDevice* dd = dynamic_cast<DynamicDevice*>(device);
            if (dd)
            {
                dd->appendStampTRAN(context);
            }
            else
            {
                SourceDevice* sd = dynamic_cast<SourceDevice*>(device);
                if (sd)
                {
                    sd->appendStampTRAN(context);
                }
                else
                {
                    device->appendStampDC(context);
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

    void BuildACMNA(AnalyzeContext* context)
    {
        // console->log(std::format("[SpParser] Start Build AC MNA"));
        context->matc.resize(context->nodeCount, context->nodeCount);
        context->rhsc.resize(context->nodeCount, Eigen::NoChange);
        context->matc.fill(0);
        context->rhsc.fill(0);

        for (auto& device : circuit->devices) {
            device->appendStampAC(context);
        }

        // console->log("[SpParser] MNA Matrix (including ground)");
        // oss<<matc<<std::endl;
        // console->log(oss.str()); oss = std::ostringstream();

        // console->log("[SpParser] RHS Vector");
        // oss<<rhsc<<std::endl;
        // console->log(oss.str()); oss = std::ostringstream();
    }

    Eigen::VectorXd& SolveDCSingle(AnalyzeContext* context)
    {
        if (!circuit->hasNonlinearDevice)
            return SolveDCOneTime(context);
        for (auto& device : circuit->devices) {
            NonlinearDevice* nd = dynamic_cast<NonlinearDevice*>(device);
            if (nd)
            {
                context->IterCurrents[nd->Name] = new std::vector<double>();
                context->IterCurrents[nd->Name]->push_back(0);
                context->IterVoltages[nd->Name] = new std::vector<double>();
                context->IterVoltages[nd->Name]->push_back(0);
            }
        }
        int cnt = 0;
        while (1)
        {
            cnt++;
            auto& res = SolveDCOneTime(context);
            double eps = 0;
            for (auto& device : circuit->devices) {
                NonlinearDevice* nd = dynamic_cast<NonlinearDevice*>(device);
                if (nd)
                {
                    double value = nd->getLastCurrent(context);
                    console->log(std::format("[SpParser] IterCurrent: {}", value));
                    context->IterCurrents[nd->Name]->push_back(value);
                    value = nd->getLastVoltage(context);
                    console->log(std::format("[SpParser] IterVoltage: {}", value));
                    context->IterVoltages[nd->Name]->push_back(value);

                    auto it = context->IterVoltages[nd->Name]->end();
                    eps = std::max(eps, (std::abs(*(it-1)-*(it-2))));
                }
            }
            console->log(std::format("[SpParser] Newton iteration counter: {}", cnt));
            for(int i = 0; i < context->nodeCount - 1; i++)
            {
                oss<<context->nodes[i].prefix<<'('<<context->nodes[i].nodeName<<')'<<'\t'<<'='<<'\t'<<res(i);
                console->log(oss.str()); oss = std::ostringstream();
            }
            if (eps < 1e-4)
            {
                return res;
            }
        }
    }

    void SolveDC()
    {
        AnalyzeContext* context = new AnalyzeContext(nodes, nodeCount);
        std::vector<AnalyzeEntry*> results;
        AnalyzeEntry* entry = NULL;
        if (circuit->command_DC.sweepOptions.size() == 0)
        {
            auto& res = SolveDCSingle(context);
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
                auto& res = SolveDCSingle(context);
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
            for(int i = 0; i < nodeCount - 1; i++)
            {
                oss<<nodes[i+1].prefix<<'('<<nodes[i+1].nodeName<<')'<<'\t'<<'='<<'\t'<<entry->result(i);
                console->log(oss.str()); oss = std::ostringstream();
            }
        }
        if (circuit->command_PLOT.enabled)
        {
            PlotDC(context->nodeCount - 1, context->nodes, results);
        }
    }

    Eigen::VectorXd& SolveDCOneTime(AnalyzeContext* context)
    {
        context->nodes = nodes;
        context->nodeCount = nodeCount;
        BuildDCMNA(context);
        context->ground = context->nodes[0];
        Eigen::MatrixXd mat_ex = context->mat.block(1, 1, context->nodeCount - 1, context->nodeCount - 1);
        Eigen::VectorXd rhs_ex = context->rhs.tail(context->nodeCount - 1);
        context->nodes.erase(context->nodes.begin());

        console->log("[SpParser] Start Solve DC");
        console->log("[SpParser] MNA Matrix (excluding ground)");
        oss<<mat_ex<<std::endl;
        console->log(oss.str()); oss = std::ostringstream();
        
        console->log("[SpParser] RHS Vector");
        oss<<rhs_ex<<std::endl;
        console->log(oss.str()); oss = std::ostringstream();

        Eigen::VectorXd* x = new Eigen::VectorXd();
        *x = mat_ex.fullPivLu().solve(rhs_ex);
        context->res = *x;
        return *x;
    }

    Eigen::VectorXd& SolveTRANSingle(AnalyzeContext* context)
    {
        if (!circuit->hasNonlinearDevice)
            return SolveTRANOneTime(context);
        for (auto& device : circuit->devices) {
            NonlinearDevice* nd = dynamic_cast<NonlinearDevice*>(device);
            if (nd)
            {
                context->IterCurrents[nd->Name] = new std::vector<double>();
                context->IterCurrents[nd->Name]->push_back(0);
                context->IterVoltages[nd->Name] = new std::vector<double>();
                context->IterVoltages[nd->Name]->push_back(0);
            }
        }
        int cnt = 0;
        while (1)
        {
            cnt++;
            auto& res = SolveTRANOneTime(context);
            double eps = 0;
            for (auto& device : circuit->devices) {
                NonlinearDevice* nd = dynamic_cast<NonlinearDevice*>(device);
                if (nd)
                {
                    double value = nd->getLastCurrent(context);
                    console->log(std::format("[SpParser] IterCurrent: {}", value));
                    context->IterCurrents[nd->Name]->push_back(value);
                    value = nd->getLastVoltage(context);
                    console->log(std::format("[SpParser] IterVoltage: {}", value));
                    context->IterVoltages[nd->Name]->push_back(value);

                    auto it = context->IterVoltages[nd->Name]->end();
                    eps = std::max(eps, (std::abs(*(it-1)-*(it-2))));
                }
            }
            console->log(std::format("[SpParser] Newton iteration counter: {}", cnt));
            for(int i = 0; i < context->nodeCount - 1; i++)
            {
                oss<<context->nodes[i].prefix<<'('<<context->nodes[i].nodeName<<')'<<'\t'<<'='<<'\t'<<res(i);
                console->log(oss.str()); oss = std::ostringstream();
            }
            if (eps < 1e-4)
            {
                return res;
            }
        }
    }

    Eigen::VectorXd& SolveTRANOneTime(AnalyzeContext* context)
    {
        context->nodes = nodes;
        context->nodeCount = nodeCount;
        BuildTRANMNA(context);
        context->ground = context->nodes[0];
        Eigen::MatrixXd mat_ex = context->mat.block(1, 1, context->nodeCount - 1, context->nodeCount - 1);
        Eigen::VectorXd rhs_ex = context->rhs.tail(context->nodeCount - 1);
        context->nodes.erase(context->nodes.begin());

        console->log("[SpParser] Start Solve TRAN");
        console->log("[SpParser] MNA Matrix (excluding ground)");
        oss<<mat_ex<<std::endl;
        console->log(oss.str()); oss = std::ostringstream();
        
        console->log("[SpParser] RHS Vector");
        oss<<rhs_ex<<std::endl;
        console->log(oss.str()); oss = std::ostringstream();

        Eigen::VectorXd* x = new Eigen::VectorXd();
        *x = mat_ex.fullPivLu().solve(rhs_ex);
        context->res = *x;
        return *x;
    }

    void SolveTran()
    {
        AnalyzeContext* context = new AnalyzeContext(nodes, nodeCount);
        std::vector<AnalyzeEntry*> results;
        AnalyzeEntry* entry = NULL;
        
        context->ts = circuit->command_TRAN.tstart;
        context->interval = circuit->command_TRAN.tstep;

        // auto& res = SolveDCOneTime(context);
        // entry = new AnalyzeEntry();
        // entry->time = context->ts;
        // entry->result = context->res;
        // results.push_back(entry);
        for (auto& device : circuit->devices) {
            DynamicDevice* dd = dynamic_cast<DynamicDevice*>(device);
            if (dd)
            {
                context->lastCurrents[dd->Name] = new std::vector<double>();
                context->lastCurrents[dd->Name]->push_back(0);
                context->lastVoltages[dd->Name] = new std::vector<double>();
                context->lastVoltages[dd->Name]->push_back(0);
            }
        }

        while (context->ts < circuit->command_TRAN.tstop)
        {
            context->ts += context->interval;

            auto& res = SolveTRANSingle(context);
            entry = new AnalyzeEntry();
            entry->time = context->ts;
            entry->result = context->res;
            results.push_back(entry);
            
            for (auto& device : circuit->devices) {
                DynamicDevice* dd = dynamic_cast<DynamicDevice*>(device);
                if (dd)
                {
                    double value = dd->getLastCurrent(context);
                    context->lastCurrents[dd->Name]->push_back(value);
                    value = dd->getLastVoltage(context);
                    context->lastVoltages[dd->Name]->push_back(value);
                }
            }

            // double min_step = INT_MAX;
            // for (auto& device : circuit->devices) {
            //     DynamicDevice* dd = dynamic_cast<DynamicDevice*>(device);
            //     if (dd)
            //     {
            //         double value = dd->filterLTE(context, context->interval);
            //         min_step = std::min(min_step, value);
            //     }
            // }
            // if (min_step != INT_MAX)
            //     context->interval = min_step;
            // console->log(std::format("[SpParser] step: {}",context->interval));
        }

        // console->log("[SpParser] Result:");
        // for(auto& entry: results)
        // {
        //     console->log(std::format("For time {}:", entry->time));
        //     for(int i = 0; i < context->nodeCount - 1; i++)
        //     {
        //         oss<<context->nodes[i].prefix<<'('<<context->nodes[i].nodeName<<')'<<'\t'<<'='<<'\t'<<entry->result(i);
        //         console->log(oss.str()); oss = std::ostringstream();
        //     }
        // }

        if (circuit->command_PLOT.enabled)
        {
            PlotTRAN(context->nodeCount - 1, context->nodes, results);
        }
    }

    void SolveAC()
    {
        AnalyzeContext* context = new AnalyzeContext(nodes, nodeCount);
        std::vector<AnalyzeEntry*> results;
        AnalyzeEntry* entry = NULL;

        double step = std::pow((double)10, 1/(double)circuit->command_AC.np);

        for(double v = circuit->command_AC.fstart; std::abs(v-circuit->command_AC.fstop) > 1e-5; v *= step)
        {
            context->freq = v;
            auto& res = SolveACOneTime(context);
            entry = new AnalyzeEntry();
            entry->resultc = res;
            entry->freq = v;
            results.push_back(entry);
        }

        console->log("[SpParser] Result:");
        for(auto& entry: results)
        {
            for(int i = 0; i < nodeCount - 1; i++)
            {
                oss<<nodes[i+1].prefix<<'('<<nodes[i+1].nodeName<<')'<<'\t'<<'='<<'\t'<<entry->resultc(i);
                console->log(oss.str()); oss = std::ostringstream();
            }
        }
        if (circuit->command_PLOT.enabled)
        {
            PlotAC(context->nodeCount - 1, context->nodes, results);
        }
    }

    Eigen::VectorXcd& SolveACOneTime(AnalyzeContext* context)
    {
        context->nodes = nodes;
        context->nodeCount = nodeCount;
        BuildACMNA(context);
        context->ground = context->nodes[0];
        Eigen::MatrixXcd mat_ex = context->matc.block(1, 1, context->nodeCount - 1, context->nodeCount - 1);
        Eigen::VectorXcd rhs_ex = context->rhsc.tail(context->nodeCount - 1);
        context->nodes.erase(context->nodes.begin());

        console->log("[SpParser] Start Solve AC");
        console->log("[SpParser] MNA Matrix (excluding ground)");
        oss<<mat_ex<<std::endl;
        console->log(oss.str()); oss = std::ostringstream();
        
        console->log("[SpParser] RHS Vector");
        oss<<rhs_ex<<std::endl;
        console->log(oss.str()); oss = std::ostringstream();

        Eigen::VectorXcd* x = new Eigen::VectorXcd();
        *x = mat_ex.fullPivLu().solve(rhs_ex);
        context->resc = *x;
        return *x;
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

            // QSplineSeries* series = new QSplineSeries();
            QLineSeries* series = new QLineSeries();
            //QScatterSeries* series = new QScatterSeries();
            series->setName(QString("%1(%2)").arg(
                QString::fromStdString(plotItem.prefix), 
                QString::fromStdString(plotItem.nodeName)
            ));
            //series->setMarkerShape(QScatterSeries::MarkerShapeCircle);  // 设置绘制的散点的样式为圆
            //series->setMarkerSize(10);                                  // 设置绘制的点的大小
            
            for(auto& entry: results)
            {
                series->append(entry->time, entry->result(index));
            }
            seriesGroups->push_back(series);
        }
        ChartViewWindow* chart = new ChartViewWindow();
        chart->initChart(*seriesGroups, false);
        chart->show();
        console->log("[SpParser] Plotted.");
    }

    void PlotDC(
        int nodeCount,
        std::vector<CircuitNode>& nodes,
        std::vector<AnalyzeEntry*>& results
    ) {
        std::vector<QAbstractSeries*> *seriesGroups = new std::vector<QAbstractSeries*>();
        
        for (auto & plotItem : circuit->command_PLOT.nodes)
        {
            int tmp = 0;
            auto it = std::find_if(nodes.begin(), nodes.end(), 
                [&](CircuitNode& n){ return n.nodeName == plotItem.nodeName && n.prefix == plotItem.prefix; });
            
            if (it == nodes.end())
                continue;

            int index = std::distance(nodes.begin(), it);

            QLineSeries* series = new QLineSeries();
            // QSplineSeries* series = new QSplineSeries();   // 创建一个样条曲线对象
            series->setName(QString("%1(%2)").arg(
                QString::fromStdString(plotItem.prefix), 
                QString::fromStdString(plotItem.nodeName)
            ));
            
            for(auto& entry: results)
            {
                series->append(entry->sweepEntries[0].value, entry->result(index));
                tmp++;
            }
            seriesGroups->push_back(series);
        }
        ChartViewWindow* chart = new ChartViewWindow();
        chart->initChart(*seriesGroups, false);
        chart->show();
        console->log("[SpParser] Plotted.");
    }

    void PlotAC(
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

            QLineSeries* series = new QLineSeries();
            // QSplineSeries* series = new QSplineSeries();   // 创建一个样条曲线对象
            series->setName(QString("%1(%2)").arg(
                QString::fromStdString(plotItem.prefix), 
                QString::fromStdString(plotItem.nodeName)
            ));
            
            for(auto& entry: results)
            {
                double value = std::abs(entry->resultc(index));
                assert(value > 0);
                value = 20 * std::log10(value);
                console->log(std::format("[SpParser] debug : {}, {}", entry->freq, value));
                series->append(entry->freq, value);
            }
            seriesGroups->push_back(series);
        }
        ChartViewWindow* chart = new ChartViewWindow();
        chart->initChart(*seriesGroups, true);
        chart->show();
        console->log("[SpParser] Plotted.");
    }
};
