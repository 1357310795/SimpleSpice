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
    std::string title;//标题
    
    std::unordered_map<std::string, int> nodemap;//节点名称字典
    std::unordered_map<std::string, int> namemap;//器件名称字典

    std::vector<BaseDevice*> devices;//器件列表
    bool hasNonlinearDevice;//是否有非线性元件
    
    Command_AC command_AC;//AC分析命令
    Command_DC command_DC;//DC分析命令
    Command_OP command_OP;//OP分析命令
    Command_OPTIONS command_OPTIONS;//OPTIONS命令
    Command_TRAN command_TRAN;//TRAN分析命令
    std::vector<Command_PLOT*> command_PLOTs;//PLOT命令
    std::vector<Command_PRINT*> command_PRINTs;//PRINT命令

    ~Circuit();
};
