#pragma once
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
#include "global/global.h"
#include "devices/base_device.h"
#include "devices/dynamic_device.h"
#include "devices/nonlinear_device.h"
#include "devices/source_device.h"
#include "calc/analyze_ac.h"
#include "calc/analyze_dc.h"
#include "calc/analyze_tran.h"

class AnalyzeManager
{
private:
    Circuit* circuit;
    std::vector<CircuitNode> nodes;
    int nodeCount;

    std::ostringstream oss;

    AnalyzeAC* analyze_ac;
    AnalyzeDC* analyze_dc;
    AnalyzeTRAN* analyze_tran;

public:
    AnalyzeManager(Circuit* circuit): circuit(circuit) {};

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

            analyze_dc = new AnalyzeDC(circuit, nodes, nodeCount);
            analyze_dc->SolveDC();
        }
        if (circuit->command_AC.enabled)
        {
            analyze_ac = new AnalyzeAC(circuit, nodes, nodeCount);
            analyze_ac->SolveAC();
        }
        if (circuit->command_TRAN.enabled)
        {
            analyze_tran = new AnalyzeTRAN(circuit, nodes, nodeCount);
            analyze_tran->SolveTRAN();
        }
    }
};
