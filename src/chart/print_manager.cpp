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
#include "chart/print_manager.h"
#include "cmds/print.hpp"

void PrintManager::Print(
    int nodeCount,
    std::vector<CircuitNode>& nodes,
    std::vector<AnalyzeEntry*>& results
) {
    for (auto & cmd : circuit->command_PRINTs)
    {
        if (cmd->type == PRINT_DC)
            PrintDC(nodeCount, nodes, results, *cmd);
        else if (cmd->type == PRINT_AC)
            PrintAC(nodeCount, nodes, results, *cmd);
        else if (cmd->type == PRINT_TRAN)
            PrintTRAN(nodeCount, nodes, results, *cmd);
        else if (cmd->type == PRINT_OP)
            PrintOP(nodeCount, nodes, results, *cmd);
    }
}

void PrintManager::PrintTRAN(
    int nodeCount,
    std::vector<CircuitNode>& nodes,
    std::vector<AnalyzeEntry*>& results,
    Command_PRINT& command_PRINT
) {
    std::ostringstream oss;
    console->log("[SpParser] Print Tran Result:");
    oss<<"time"<<'\t';
    std::vector<int> printIndexs;
    for (auto & printItem : command_PRINT.nodes)
    {
        auto it = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == printItem.nodeName && n.prefix == printItem.prefix; });
        
        if (it == nodes.end())
            continue;

        oss<<std::format("{}({})", it->prefix, it->nodeName)<<'\t';

        int index = std::distance(nodes.begin(), it);
        printIndexs.push_back(index);
    }
    console->log(oss.str()); oss = std::ostringstream();

    for(auto& entry: results)
    {
        oss<<entry->time<<'\t';
        for (auto index : printIndexs)
            oss<<entry->result(index)<<'\t';
        console->log(oss.str()); oss = std::ostringstream();
    }
}

void PrintManager::PrintDC(
    int nodeCount,
    std::vector<CircuitNode>& nodes,
    std::vector<AnalyzeEntry*>& results,
    Command_PRINT& command_PRINT
) {
    std::ostringstream oss;
    console->log("[SpParser] Print DC Result:");
    if (circuit->command_DC.sweepOptions.size() > 0)
    {
        oss<<std::format("{}", circuit->command_DC.sweepOptions[0].deviceName)<<'\t';
    }
    std::vector<int> printIndexs;
    for (auto & printItem : command_PRINT.nodes)
    {
        auto it = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == printItem.nodeName && n.prefix == printItem.prefix; });
        
        if (it == nodes.end())
            continue;

        oss<<std::format("{}({})", it->prefix, it->nodeName)<<'\t';

        int index = std::distance(nodes.begin(), it);
        printIndexs.push_back(index);
    }
    console->log(oss.str()); oss = std::ostringstream();

    for(auto& entry: results)
    {
        if (circuit->command_DC.sweepOptions.size() > 0)
            oss<<entry->sweepEntries[0].value<<'\t';
        for (auto index : printIndexs)
            oss<<entry->result(index)<<'\t';
        console->log(oss.str()); oss = std::ostringstream();
    }
}

void PrintManager::PrintAC(
    int nodeCount,
    std::vector<CircuitNode>& nodes,
    std::vector<AnalyzeEntry*>& results,
    Command_PRINT& command_PRINT
) {
    std::ostringstream oss;
    console->log("[SpParser] Print AC Result:");
    oss<<"frequency"<<'\t';
    std::vector<int> printIndexs;
    for (auto & printItem : command_PRINT.nodes)
    {
        auto it = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == printItem.nodeName && n.prefix == printItem.prefix; });
        
        if (it == nodes.end())
            continue;

        oss<<std::format("{}({})", it->prefix, it->nodeName)<<'\t';

        int index = std::distance(nodes.begin(), it);
        printIndexs.push_back(index);
    }
    console->log(oss.str()); oss = std::ostringstream();

    for(auto& entry: results)
    {
        oss<<entry->freq<<'\t';
        for (auto index : printIndexs)
            oss<<entry->result(index)<<'\t';
        console->log(oss.str()); oss = std::ostringstream();
    }
}

void PrintManager::PrintOP(
    int nodeCount,
    std::vector<CircuitNode>& nodes,
    std::vector<AnalyzeEntry*>& results,
    Command_PRINT& command_PRINT
) {
    std::ostringstream oss;
    console->log("[SpParser] Print OP Result:");
    std::vector<int> printIndexs;
    for (auto & printItem : command_PRINT.nodes)
    {
        auto it = std::find_if(nodes.begin(), nodes.end(), 
            [&](CircuitNode& n){ return n.nodeName == printItem.nodeName && n.prefix == printItem.prefix; });
        
        if (it == nodes.end())
            continue;

        oss<<std::format("{}({})", it->prefix, it->nodeName)<<'\t';

        int index = std::distance(nodes.begin(), it);
        printIndexs.push_back(index);
    }
    console->log(oss.str()); oss = std::ostringstream();

    for(auto& entry: results)
    {
        for (auto index : printIndexs)
            oss<<entry->result(index)<<'\t';
        console->log(oss.str()); oss = std::ostringstream();
    }
}