%{
#define YYDEBUG 1
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "parser/scanner.hpp"
#include "console/myconsole.h"
#include "cmds/plot.hpp"
#include "cmds/pulse.hpp"
#include "cmds/sinusoid.hpp"
#include "circuit/circuit_node.h"
#include "global/global.h"

#include "devices/item/resistor.h"
#include "devices/item/capacitor.h"
#include "devices/item/inductor.h"
#include "devices/item/isource.h"
#include "devices/item/vsource.h"
#include "devices/item/vcvs.h"
#include "devices/item/vccs.h"
#include "devices/item/ccvs.h"
#include "devices/item/cccs.h"
#include "devices/item/diode.h"

void yyerror (char const *s) {
   fprintf (stderr, "[SpParser] error at line %d: %s\n", yylineno, s);
   console->log(std::format("[SpParser] error at line {}: {}", yylineno, s));
}

void addNode(std::string input)
{
    if (circuit->nodemap.find(input) != circuit->nodemap.end()) {
        circuit->nodemap[input]++;
    } else {
        circuit->nodemap[input] = 1;
    }
}

bool checkName(char const *name)
{
    if (circuit->namemap.find(name) != circuit->namemap.end()) {
        console->log(std::format("[SpParser] duplicate component {}\n", name));
        return false;
    } else {
        circuit->namemap[name] = 1;
        return true;
    }
}

void ParseResistor(char const *name, char const *node1, char const *node2, double value)
{
    if (!checkName(name)) return;
    addNode(node1); addNode(node2);
    D_Resistor* d = new D_Resistor(name, node1, node2, value);
    circuit->devices.push_back(d);
}

void ParseInductor(char const *name, char const *node1, char const *node2, double value)
{
    if (!checkName(name)) return;
    addNode(node1); addNode(node2);
    D_Inductor* d = new D_Inductor(name, node1, node2, value);
    circuit->devices.push_back(d);
}

void ParseCapacitor(char const *name, char const *node1, char const *node2, double value)
{
    if (!checkName(name)) return;
    addNode(node1); addNode(node2);
    D_Capacitor* d = new D_Capacitor(name, node1, node2, value);
    circuit->devices.push_back(d);
}

D_VSource* d_VSource;
D_ISource* d_ISource;

void ParseVSource(char const *name, char const *node1, char const *node2, double value)
{
    if (!checkName(name)) return;
    addNode(node1); addNode(node2);
    d_VSource = new D_VSource(name, node1, node2, value);
    circuit->devices.push_back(d_VSource);
}

void ParseISource(char const *name, char const *node1, char const *node2, double value)
{
    if (!checkName(name)) return;
    addNode(node1); addNode(node2);
    d_ISource = new D_ISource(name, node1, node2, value);
    circuit->devices.push_back(d_ISource);
}

void ParseVCCS(char const *name, char const *node1, char const *node2, char const *nc1, char const *nc2, double value)
{
    if (!checkName(name)) return;
    addNode(node1); addNode(node2); addNode(nc1); addNode(nc2); 
    D_VCCS* d = new D_VCCS(name, node1, node2, nc1, nc2, value);
    circuit->devices.push_back(d);
}

void ParseVCVS(char const *name, char const *node1, char const *node2, char const *nc1, char const *nc2, double value)
{
    if (!checkName(name)) return;
    addNode(node1); addNode(node2); addNode(nc1); addNode(nc2); 
    D_VCVS* d = new D_VCVS(name, node1, node2, nc1, nc2, value);
    circuit->devices.push_back(d);
}

void ParseCCCS(char const *name, char const *node1, char const *node2, char const *nc1, char const *nc2, double value)
{
    if (!checkName(name)) return;
    addNode(node1); addNode(node2); addNode(nc1); addNode(nc2); 
    D_CCCS* d = new D_CCCS(name, node1, node2, nc1, nc2, value);
    circuit->devices.push_back(d);
}

void ParseCCVS(char const *name, char const *node1, char const *node2, char const *nc1, char const *nc2, double value)
{
    if (!checkName(name)) return;
    addNode(node1); addNode(node2); addNode(nc1); addNode(nc2); 
    D_CCVS* d = new D_CCVS(name, node1, node2, nc1, nc2, value);
    circuit->devices.push_back(d);
}

void ParseDiode(char const *name, char const *node1, char const *node2)
{
    if (!checkName(name)) return;
    addNode(node1); addNode(node2);
    D_Diode* d = new D_Diode(name, node1, node2);
    circuit->devices.push_back(d);
    circuit->hasNonlinearDevice = true;
}

void parseOption(char const *op)
{
    std::string str = op;
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); });
    //std::cout << str << std::endl;
    if (str == "node")
        circuit->command_OPTIONS.node = true;
    if (str == "list")
        circuit->command_OPTIONS.list = true;
}

%}

%code requires {
    #include <cstdio>
    #include <cstdlib>
    #include <cstring>
}


%union {
    int    num;
    double value;
    char*  str;
}

/* it will generate a location class which can be used in your lexer */
%locations

%define parse.trace
%define parse.error verbose
%output "parser.cpp"
%verbose

%token DOCEOF

%token<str> P_RESISTOR  
%token<str> P_CAPACITOR 
%token<str> P_INDUCTOR  
%token<str> P_ISOURCE
%token<str> P_VSOURCE
%token<str> P_VCCS      
%token<str> P_CCCS      
%token<str> P_CCVS      
%token<str> P_VCVS      
%token<str> P_OPAMP     
%token<str> P_SWITCH    
%token<str> P_DIODE     
%token<str> P_MOSFET  

%token<str> RK_AC   
%token<str> RK_ACI  
%token<str> RK_AM   
%token<str> RK_DC   
%token<str> RK_DEC  
%token<str> RK_EXP  
%token<str> RK_LIN  
%token<str> RK_OCT  
%token<str> RK_PAT  
%token<str> RK_PE   
%token<str> RK_PL   
%token<str> RK_POI   
%token<str> RK_PU   
%token<str> RK_PULSE
%token<str> RK_PWL  
%token<str> RK_R    
%token<str> RK_RD   
%token<str> RK_SFFM 
%token<str> RK_SIN  
%token<str> RK_OP  
%token<str> RK_TRAN

%token<str> CMD_OP
%token<str> CMD_END
%token<str> CMD_PRINT
%token<str> CMD_DC
%token<str> CMD_AC
%token<str> CMD_OPTIONS
%token<str> CMD_PLOT
%token<str> CMD_TRAN
%token<str> CMD_TITLE

%token<str> OPTIONS_ITEM

%token<str> VAR_V
%token<str> VAR_I

%token<str> LBRACKET
%token<str> RBRACKET

%token<str> EOL STRING COMMENTLINE
%token<num> INTEGER 
%token<value> REAL VALUE 

%type<str> resistor  
%type<str> capacitor 
%type<str> inductor  
%type<str> isource
%type<str> vsource
%type<str> vccs      
%type<str> cccs      
%type<str> ccvs      
%type<str> vcvs      
%type<str> opamp     
%type<str> switch    
%type<str> diode     
%type<str> mosfet

%type<str> node
%type<value> value

%%

line:
    component EOL { yylineno++; return 0; }
    | command EOL { yylineno++; return 0; }
    | COMMENTLINE { yylineno++; console->log(std::format("[SpParser] commentline: {}", $1)); return 0; }
    | EOL { yylineno++; return 0; }
    | DOCEOF { return -255; }
    ;

component: 
    resistor
    | capacitor
    | inductor
    | vsource
    | isource
    | vcvs
    | vccs
    | cccs
    | ccvs
    | diode
    ;

command:
    title
    | op
    | end
    | print
    | dc
    | ac
    | options
    | plot
    | tran
    ;

resistor: 
    P_RESISTOR node node value
    { ParseResistor($1, $2, $3, $4); }
    ;

capacitor: 
    P_CAPACITOR node node value
    { ParseCapacitor($1, $2, $3, $4); }
    ;

inductor:
    P_INDUCTOR node node value
    { ParseInductor($1, $2, $3, $4); }
    ;

diode: 
    P_DIODE node node
    { ParseDiode($1, $2, $3); }
    ;

vsource:
    P_VSOURCE node node
    {
        ParseVSource($1, $2, $3, 0);
    }
    |
    P_VSOURCE node node value
    {
        ParseVSource($1, $2, $3, $4);
    }
    |
    vsource RK_DC value
    {
        d_VSource->DC_Value = $3;
    }
    |
    vsource RK_AC value
    {
        d_VSource->AC_Mag = $3;
    }
    |
    vsource RK_PULSE LBRACKET value value value value value value value RBRACKET
    {
        d_VSource->pulse = new Pulse($4, $5, $6, $7, $8, $9, $10);
        console->log(std::format("[SpParser] [Command] PULSE detected!"));
    }
    |
    vsource RK_SIN LBRACKET value value value value value RBRACKET
    {
        d_VSource->sin = new Sinusoid($4, $5, $6, $7, $8);
        console->log(std::format("[SpParser] [Command] Sinusoid detected!"));
    }
    ;

isource:
    P_ISOURCE node node
    {
        ParseISource($1, $2, $3, 0);
    }
    |
    P_ISOURCE node node value
    {
        ParseISource($1, $2, $3, $4);
    }
    |
    isource RK_DC value
    {
        d_ISource->DC_Value = $3;
    }
    |
    isource RK_AC value
    {
        d_ISource->AC_Mag = $3;
    }
    |
    isource RK_PULSE LBRACKET value value value value value value value RBRACKET
    {
        d_ISource->pulse = new Pulse($4, $5, $6, $7, $8, $9, $10);
        console->log(std::format("[SpParser] [Command] PULSE detected!"));
    }
    |
    isource RK_SIN LBRACKET value value value value value RBRACKET
    {
        d_ISource->sin = new Sinusoid($4, $5, $6, $7, $8);
        console->log(std::format("[SpParser] [Command] Sinusoid detected!"));
    }
    ;

vccs:
    P_VCCS node node node node value
    {
        ParseVCCS($1, $2, $3, $4, $5, $6);
    }
    ;

vcvs:
    P_VCVS node node node node value
    {
        ParseVCVS($1, $2, $3, $4, $5, $6);
    }
    ;

cccs:
    P_CCCS node node node node value
    {
        ParseCCCS($1, $2, $3, $4, $5, $6);
    }
    ;

ccvs:
    P_CCVS node node node node value
    {
        ParseCCVS($1, $2, $3, $4, $5, $6);
    }
    ;

title:
    CMD_TITLE STRING
    {
        circuit->title = $2;
    }
    ;

op:
    CMD_OP
    {
        circuit->command_OP.enabled = true;
        console->log(std::format("[SpParser] [Command] .OP detected!"));
    }
    ;

end:
    CMD_END
    {
        console->log(std::format("[SpParser] [Command] .end detected!"));
        return -255;
    }
    ;

print:
    CMD_PRINT RK_DC { console->log("[SpParser] [Command] .print detected!"); console->log("[SpParser] type: DC"); }
    | CMD_PRINT RK_AC { console->log("[SpParser] [Command] .print detected!"); console->log("[SpParser] type: AC"); }
    | CMD_PRINT RK_TRAN { console->log("[SpParser] [Command] .print detected!"); console->log("[SpParser] type: TRAN"); }
    | CMD_PRINT RK_OP { console->log("[SpParser] [Command] .print detected!"); console->log("[SpParser] type: OP"); }
    | print VAR_I { console->log(std::format("[SpParser] print current: {}\n", $2)); }
    | print VAR_V { console->log(std::format("[SpParser] print voltage: {}\n", $2)); }
    ;

dc:
    CMD_DC { 
        circuit->command_DC.enabled = true;
        console->log("[SpParser] [Command] .dc detected!");
    }
    | dc P_ISOURCE value value value { 
        circuit->command_DC.sweepOptions.push_back(SweepOption($2, $3, $4, $5));
        console->log(std::format("[SpParser] dc current scan: from {:.3e} to {:.3e} step {:.3e}\n", $3, $4, $5));
    } 
    | dc P_VSOURCE value value value { 
        circuit->command_DC.sweepOptions.push_back(SweepOption($2, $3, $4, $5));
        console->log(std::format("[SpParser] dc voltage scan: from {:.3e} to {:.3e} step {:.3e}\n", $3, $4, $5));
    }
    ; 

ac:
    CMD_AC RK_DEC value value value { 
        circuit->command_AC.enabled = true;
        circuit->command_AC.type = DEC;
        circuit->command_AC.np = $3;
        circuit->command_AC.fstart = $4;
        circuit->command_AC.fstop = $5;
        console->log("[SpParser] [Command] .ac detected!");
    }
    | ac P_ISOURCE value value value { 
        circuit->command_AC.sweepOptions.push_back(SweepOption($2, $3, $4, $5));
        console->log(std::format("[SpParser] ac current scan: from {:.3e} to {:.3e} step {:.3e}\n", $3, $4, $5));
    } 
    | ac P_VSOURCE value value value { 
        circuit->command_AC.sweepOptions.push_back(SweepOption($2, $3, $4, $5));
        console->log(std::format("[SpParser] ac voltage scan: from {:.3e} to {:.3e} step {:.3e}\n", $3, $4, $5));
    } 
    ;

options:
    CMD_OPTIONS 
    {
        console->log("[SpParser] [Command] .options detected!");
    }
    | options OPTIONS_ITEM
    {
        parseOption($2);
    }
    ;

plot:
    CMD_PLOT RK_DC { 
        console->log("[SpParser] [Command] .plot detected!"); console->log("[SpParser] type: DC"); 
        circuit->command_PLOT.enabled = true;
        circuit->command_PLOT.type = PLOT_DC;
    }
    | CMD_PLOT RK_AC { 
        console->log("[SpParser] [Command] .plot detected!"); console->log("[SpParser] type: AC"); 
        circuit->command_PLOT.enabled = true;
        circuit->command_PLOT.type = PLOT_AC;
    }
    | CMD_PLOT RK_TRAN { 
        console->log("[SpParser] [Command] .plot detected!"); console->log("[SpParser] type: TRAN"); 
        circuit->command_PLOT.enabled = true;
        circuit->command_PLOT.type = PLOT_TRAN;
    }
    | plot VAR_I { 
        console->log(std::format("[SpParser] plot current: {}\n", $2)); 
        std::string node = $2;

        circuit->command_PLOT.nodes.push_back(CircuitNode(node.substr(2, node.length() - 3), "I"));
    }
    | plot VAR_V { 
        console->log(std::format("[SpParser] plot voltage: {}\n", $2)); 
        std::string node = $2;

        circuit->command_PLOT.nodes.push_back(CircuitNode(node.substr(2, node.length() - 3), "V"));
    }
    ;

tran:
    CMD_TRAN value value { 
        circuit->command_TRAN.enabled = true;
        circuit->command_TRAN.tstep = $2;
        circuit->command_TRAN.tstop = $3;
        console->log("[SpParser] [Command] .tran detected!");
    }
    |
    CMD_TRAN value value value { 
        circuit->command_TRAN.enabled = true;
        circuit->command_TRAN.tstep = $2;
        circuit->command_TRAN.tstop = $3;
        circuit->command_TRAN.tstart = $4;
        console->log("[SpParser] [Command] .tran detected!");
    }
    ;

value: 
    REAL
    {
        $$ = $1;
    }
    |
    INTEGER
    {
        $$ = $1;
    }
    ;

node: 
    STRING          { $$ = strdup($1); }
    | P_RESISTOR    { $$ = strdup($1); }
    | P_CAPACITOR   { $$ = strdup($1); }
    | P_INDUCTOR    { $$ = strdup($1); }
    | P_ISOURCE     { $$ = strdup($1); }
    | P_VSOURCE     { $$ = strdup($1); }
    | P_VCCS        { $$ = strdup($1); }
    | P_CCCS        { $$ = strdup($1); }
    | P_CCVS        { $$ = strdup($1); }
    | P_VCVS        { $$ = strdup($1); }
    | P_OPAMP       { $$ = strdup($1); }
    | P_SWITCH      { $$ = strdup($1); }
    | P_DIODE       { $$ = strdup($1); }
    | P_MOSFET      { $$ = strdup($1); }
    | INTEGER
    {
        $$ = new char[32];
        sprintf($$, "%d", $1);
    }
    ;
    
%%
