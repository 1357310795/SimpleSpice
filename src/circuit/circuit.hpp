#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "devices/all_devices.hpp"
#include "cmds/ac_dc.hpp"
#include "cmds/option.hpp"
#include "cmds/tran.hpp"
#include "cmds/plot.hpp"

class Circuit
{
public:
    std::unordered_map<std::string, int> nodemap;
    std::unordered_map<std::string, int> namemap;

    std::vector<BaseDevice*> devices;
    Command_AC command_AC;
    Command_DC command_DC;
    Command_OP command_OP;
    Command_OPTIONS command_OPTIONS;
    Command_TRAN command_TRAN;
    Command_PLOT command_PLOT;
};