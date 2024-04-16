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
#include "calc/analyze_tran.h"
#include "chart/plot_manager.h"
#include "calc/iteration_context.hpp"
#include "chart/print_manager.h"

AnalyzeTRAN::AnalyzeTRAN(Circuit* circuit, std::vector<CircuitNode>& nodes, int nodeCount)
    : circuit(circuit), nodes(nodes), nodeCount(nodeCount) {}

void AnalyzeTRAN::BuildTRANMNA(AnalyzeContext* context) {
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

Eigen::VectorXd& AnalyzeTRAN::SolveTRANSingle(AnalyzeContext* context)
{
    if (!circuit->hasNonlinearDevice)
        return SolveTRANOneTime(context);
    for (auto& device : circuit->devices) {
        NonlinearDevice* nd = dynamic_cast<NonlinearDevice*>(device);
        if (nd)
        {
            context->IterContexts[nd->Name] = new std::vector<BaseIterationContext*>();
            context->IterContexts[nd->Name]->push_back(nd->getDefaultIterationContext());
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
                auto lastContext = nd->getLastContext(context);
                context->IterContexts[nd->Name]->push_back(lastContext);
                //console->log(std::format("[SpParser] IterCurrent: {}", value));
                //console->log(std::format("[SpParser] IterVoltage: {}", value));

                eps = std::max(eps, nd->checkConvergence(context));
            }
        }
        console->log(std::format("[SpParser] Newton iteration counter: {}", cnt));
        for(int i = 0; i < context->nodeCount - 1; i++)
        {
            oss<<context->nodes[i].prefix<<'('<<context->nodes[i].nodeName<<')'<<'\t'<<'='<<'\t'<<res(i);
            console->log(oss.str()); oss = std::ostringstream();
        }
        if (cnt > 100)
        {
            console->log(std::format("[SpParser] Newton iteration failed to converge"));
            throw std::runtime_error("convergence error");
        }
        if (eps < 1e-6)
        {
            return res;
        }
    }
}

Eigen::VectorXd& AnalyzeTRAN::SolveTRANOneTime(AnalyzeContext* context)
{
    context->nodes = nodes;
    context->nodeCount = nodeCount;
    BuildTRANMNA(context);
    context->ground = context->nodes[0];
    Eigen::MatrixXd mat_ex = context->mat.block(1, 1, context->nodeCount - 1, context->nodeCount - 1);
    Eigen::VectorXd rhs_ex = context->rhs.tail(context->nodeCount - 1);
    context->nodes.erase(context->nodes.begin());

#ifdef MY_DEBUG
    console->log("[SpParser] Start Solve TRAN");
    console->log("[SpParser] MNA Matrix (excluding ground)");
    oss<<mat_ex<<std::endl;
    console->log(oss.str()); oss = std::ostringstream();
    
    console->log("[SpParser] RHS Vector");
    oss<<rhs_ex<<std::endl;
    console->log(oss.str()); oss = std::ostringstream();
#endif
    Eigen::VectorXd* x = new Eigen::VectorXd();
    *x = mat_ex.fullPivLu().solve(rhs_ex);
    context->res = *x;
    return *x;
}

void AnalyzeTRAN::SolveTRAN()
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
    if (context->ts > circuit->command_TRAN.tstop)
    {
        console->log("[SpParser] time range error!");
        return;
    }
    while (context->ts < circuit->command_TRAN.tstop)
    {
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
        context->ts += context->interval;
    }

    console->log("[SpParser] Result:");
    for(auto& entry: results)
    {
        console->log(std::format("For time {}:", entry->time));
        for(int i = 0; i < context->nodeCount - 1; i++)
        {
            oss<<context->nodes[i].prefix<<'('<<context->nodes[i].nodeName<<')'<<'\t'<<'='<<'\t'<<entry->result(i);
            console->log(oss.str()); oss = std::ostringstream();
        }
    }
    if (circuit->command_PRINTs.size() > 0)
    {
        PrintManager::Print(context->nodeCount - 1, context->nodes, results);
    }
    
    if (circuit->command_PLOTs.size() > 0)
    {
        PlotManager::Plot(context->nodeCount - 1, context->nodes, results);
    }
    delete context;
}