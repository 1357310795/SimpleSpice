#include <QtCharts>

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <Eigen/Dense>
#include "console/myconsole.h"
#include "circuit/circuit.h"
#include "circuit/circuit_node.h"
#include "calc/analyze_entry.h"
#include "calc/analyze_context.h"
#include "chart/mychart.h"
#include "global/global.h"
#include "devices/base_device.h"
#include "devices/dynamic_device.h"
#include "devices/nonlinear_device.h"
#include "devices/source_device.h"
#include "chart/plot_manager.h"
#include "chart/plot_context.h"
#include "cmds/plot.hpp"

void PlotManager::Plot(
    int nodeCount,
    std::vector<CircuitNode>& nodes,
    std::vector<AnalyzeEntry*>& results
) {
    for (auto & cmd : circuit->command_PLOTs)
    {
        if (cmd->type == PLOT_DC)
            PlotDC(nodeCount, nodes, results, *cmd);
        else if (cmd->type == PLOT_AC)
            PlotAC(nodeCount, nodes, results, *cmd);
        else if (cmd->type == PLOT_TRAN)
            PlotTRAN(nodeCount, nodes, results, *cmd);
    }
}

void PlotManager::PlotTRAN(
    int nodeCount,
    std::vector<CircuitNode>& nodes,
    std::vector<AnalyzeEntry*>& results,
    Command_PLOT& command_PLOT
) {
    PlotContext* context = new PlotContext();
    for (auto & plotItem : command_PLOT.nodes)
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
            double value = entry->result(index);
            series->append(entry->time, value);
            context->max = std::max(context->max, value);
            context->min = std::min(context->min, value);
        }
        context->seriesGroups.push_back(series);
    }
    context->title = circuit->title;
    context->axisXTitle = "Time (s)";
    context->axisYTitle = "Voltage (V) / Current (A)";
    context->useLogAxis = false;
    context->useExtraAxis = false;

    ChartViewWindow* chart = new ChartViewWindow();
    chart->initChart(*context);
    chart->show();
    console->log("[SpParser] Plotted.");
}

void PlotManager::PlotDC(
    int nodeCount,
    std::vector<CircuitNode>& nodes,
    std::vector<AnalyzeEntry*>& results,
    Command_PLOT& command_PLOT
) {
    PlotContext* context = new PlotContext();
    
    for (auto & plotItem : command_PLOT.nodes)
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
            double value = entry->result(index);
            series->append(entry->sweepEntries[0].value, value);
            context->max = std::max(context->max, value);
            context->min = std::min(context->min, value);
            tmp++;
        }
        context->seriesGroups.push_back(series);
    }
    context->title = circuit->title;
    context->axisXTitle = std::format("Scan Variable ({})", results[0]->sweepEntries[0].deviceName);
    context->axisYTitle = "Voltage (V) / Current (A)";
    context->useLogAxis = false;
    context->useExtraAxis = false;
    
    ChartViewWindow* chart = new ChartViewWindow();
    chart->initChart(*context);
    chart->show();
    console->log("[SpParser] Plotted.");
}

void PlotManager::PlotAC(
    int nodeCount,
    std::vector<CircuitNode>& nodes,
    std::vector<AnalyzeEntry*>& results,
    Command_PLOT& command_PLOT
) {
    PlotContext* context = new PlotContext();
    
    for (auto & plotItem : command_PLOT.nodes)
    {
        auto it = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == plotItem.nodeName && n.prefix == plotItem.prefix; });
        
        if (it == nodes.end())
            continue;

        int index = std::distance(nodes.begin(), it);

        QLineSeries* mag = new QLineSeries();
        QLineSeries* arg = new QLineSeries();
        // QSplineSeries* series = new QSplineSeries();   // 创建一个样条曲线对象
        mag->setName(QString("%1(%2)").arg(
            QString::fromStdString(plotItem.prefix), 
            QString::fromStdString(plotItem.nodeName)
        ));

        arg->setName(QString("%1(%2)").arg(
            QString::fromStdString(plotItem.prefix), 
            QString::fromStdString(plotItem.nodeName)
        ));
        
        double arg_offset = 0;
        double last_arg = 0;
        for(auto& entry: results)
        {
            double value = std::abs(entry->resultc(index));
            assert(value > 0);
            value = 20 * std::log10(value);
            console->log(std::format("[SpParser] debug : {}, {}", entry->freq, value));
            mag->append(entry->freq, value);

            double arg_value = std::arg(entry->resultc(index));
            if (arg_value - last_arg > M_PI)
            {
                arg_offset += -2*M_PI;
            }
            if (last_arg - arg_value > M_PI)
            {
                arg_offset += 2*M_PI;
            }
            arg->append(entry->freq, arg_value + arg_offset);
            last_arg = arg_value;
        }
        context->seriesGroups.push_back(mag);
        context->seriesGroups.push_back(arg);
    }
    context->title = circuit->title;
    context->axisXTitle = "Frequency (Hz)";
    context->axisYTitle = "Magnitude (dB)";
    context->axisZTitle = "Phrase (rad)";
    context->useLogAxis = true;
    context->useExtraAxis = true;

    ChartViewWindow* chart = new ChartViewWindow();
    chart->initChart(*context);
    chart->show();
    console->log("[SpParser] Plotted.");
}
