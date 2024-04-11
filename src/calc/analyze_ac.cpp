#include <vector>
#include <string>
#include <format>
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
#include "chart/plot_manager.h"

AnalyzeAC::AnalyzeAC(Circuit* circuit, std::vector<CircuitNode>& nodes, int nodeCount)
    : circuit(circuit), nodes(nodes), nodeCount(nodeCount) {}

void AnalyzeAC::BuildACMNA(AnalyzeContext* context)
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

void AnalyzeAC::SolveAC()
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
        PlotManager::PlotAC(context->nodeCount - 1, context->nodes, results);
    }
    delete context;
}

Eigen::VectorXcd& AnalyzeAC::SolveACOneTime(AnalyzeContext* context)
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