#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "devices/base_device.h"
#include "cmds/ac_dc.hpp"
#include "cmds/option.hpp"
#include "cmds/tran.hpp"
#include "cmds/plot.hpp"
#include "cmds/print.hpp"

class BaseDevice;

class Circuit
{
public:
    std::string title;
    
    std::unordered_map<std::string, int> nodemap;
    std::unordered_map<std::string, int> namemap;

    std::vector<BaseDevice*> devices;
    bool hasNonlinearDevice;
    
    Command_AC command_AC;
    Command_DC command_DC;
    Command_OP command_OP;
    Command_OPTIONS command_OPTIONS;
    Command_TRAN command_TRAN;
    std::vector<Command_PLOT*> command_PLOTs;
    std::vector<Command_PRINT*> command_PRINTs;

    ~Circuit();
};
