%option outfile="scanner.cpp"
%option header-file="scanner.hpp"

%{
#include <string>
#include <vector>
#include <cctype>
#include "parser.hpp"
#include "console/myconsole.h"

double parseValue(const char *str);
%}

%s state_options state_sourcetype state_onlystring

ALPHA       [_A-Za-z]
DIGIT       [0-9]
ALPHANUM    [_A-Za-z0-9]
STRING      {ALPHANUM}+
INTEGER     {DIGIT}+
REAL        [\-\+]?([0-9]*\.?[0-9]+|[0-9]+\.)([Ee](\+|\-)?[0-9]+)?

P_RESISTOR    [Rr]{ALPHANUM}+
P_CAPACITOR   [Cc]{ALPHANUM}+
P_INDUCTOR    [Ll]{ALPHANUM}+
P_CURRENTSRC  [Ii]{ALPHANUM}+
P_VOLTAGESRC  [Vv]{ALPHANUM}+
P_VCCS        [Gg]{ALPHANUM}+
P_CCCS        [Ff]{ALPHANUM}+
P_CCVS        [Hh]{ALPHANUM}+
P_VCVS        [Ee]{ALPHANUM}+
P_OPAMP       [Oo]{ALPHANUM}+
P_SWITCH      [Ss]{ALPHANUM}+
P_DIODE       [Dd]{ALPHANUM}+
P_MOSFET      [Mm]{ALPHANUM}+

EOL         [\r\n]*
BLANK       [ \t]+
S_UNIT      [Ff]|[Pp]|[Nn]|[Uu]|[Mm]|[Kk]|[Mm]|[Xx]|[Gg]|[Tt]|[Mm][Ee][Gg]
VALUE       ({REAL}|[\-]?{INTEGER}){S_UNIT}
COMMENTLINE \*.*\n
LBRACKET    \(
RBRACKET    \)
EQUAL       =

RK_AC          [Aa][Cc]
RK_ACI         [Aa][Cc][Ii]
RK_AM          [Aa][Mm]
RK_DC          [Dd][Cc]
RK_DEC         [Dd][Ee][Cc]
RK_EXP         [Ee][Xx][Pp]
RK_LIN         [Ll][Ii][Nn]
RK_OCT         [Oo][Cc][Tt]
RK_PAT         [Pp][Aa][Tt]
RK_PE          [Pp][Ee]
RK_PL          [Pp][Ll]
RK_POI         [Pp][Oo][Ii]
RK_PU          [Pp][Uu]
RK_PULSE       [Pp][Uu][Ll][Ss][Ee]
RK_PWL         [Pp][Ww][Ll]
RK_RD          [Rr][Dd]
RK_SFFM        [Ss][Ff][Ff][Mm]
RK_SIN         [Ss][Ii][Nn]
RK_OP          [Oo][Pp]
RK_TRAN        [Tt][Rr][Aa][Nn]

CMD_OP          \.[Oo][Pp]
CMD_END         \.[Ee][Nn][Dd]
CMD_PRINT       \.[Pp][Rr][Ii][Nn][Tt]
CMD_DC          \.[Dd][Cc]
CMD_AC          \.[Aa][Cc]
CMD_OPTIONS     \.[Oo][Pp][Tt][Ii][Oo][Nn][Ss]
CMD_PLOT        \.[Pp][Ll][Oo][Tt]
CMD_TRAN        \.[Tt][Rr][Aa][Nn]
CMD_TITLE        \.[Tt][Ii][Tt][Ll][Ee]

OP_NODE     \.[Nn][Oo][Dd][Ee]
OP_LIST     \.[Ll][Ii][Ss][Tt]

VAR_V           [Vv]\({ALPHANUM}+\)
VAR_I           [Ii]\({ALPHANUM}+\)
%%

{INTEGER}   { yylval.num   = atoi(yytext);      return INTEGER; }
{REAL}      { yylval.value = atof(yytext);       return REAL;    }
{VALUE}     { yylval.value = parseValue(yytext); return REAL;   }

<state_options>{STRING}  { yylval.str = strdup(yytext); return OPTIONS_ITEM;   }
<state_sourcetype>{LBRACKET}     { return LBRACKET; }
<state_sourcetype>{RBRACKET}     { return RBRACKET; }
<state_onlystring>{STRING}  { yylval.str = strdup(yytext); return STRING;   }

{P_RESISTOR}    { yylval.str = strdup(yytext);  return P_RESISTOR;   }
{P_CAPACITOR}   { yylval.str = strdup(yytext);  return P_CAPACITOR;  }
{P_INDUCTOR}    { yylval.str = strdup(yytext);  return P_INDUCTOR;   }
{P_CURRENTSRC}  { yylval.str = strdup(yytext);  return P_ISOURCE;    }
{P_VOLTAGESRC}  { yylval.str = strdup(yytext);  return P_VSOURCE;    }
{P_VCCS}        { yylval.str = strdup(yytext);  return P_VCCS;       }
{P_CCCS}        { yylval.str = strdup(yytext);  return P_CCCS;       }
{P_CCVS}        { yylval.str = strdup(yytext);  return P_CCVS;       }
{P_VCVS}        { yylval.str = strdup(yytext);  return P_VCVS;       }
{P_OPAMP}       { yylval.str = strdup(yytext);  return P_OPAMP;      }
{P_DIODE}       { yylval.str = strdup(yytext);  return P_DIODE;      }
{P_MOSFET}      { yylval.str = strdup(yytext);  return P_MOSFET;     }

{EOL}           { BEGIN(INITIAL); return EOL;           }
<<EOF>>         { return DOCEOF;           }
{COMMENTLINE}   { yylval.str = strdup(yytext); return COMMENTLINE;   }

{RK_AC}{BLANK}      { return RK_AC   ; }
{RK_ACI}{BLANK}     { return RK_ACI  ; }
{RK_AM}{BLANK}      { return RK_AM   ; }
{RK_DC}{BLANK}      { return RK_DC   ; }
{RK_DEC}{BLANK}     { return RK_DEC  ; }
{RK_EXP}{BLANK}     { return RK_EXP  ; }
{RK_LIN}{BLANK}     { return RK_LIN  ; }
{RK_OCT}{BLANK}     { return RK_OCT  ; }
{RK_PAT}{BLANK}     { return RK_PAT  ; }
{RK_PE}{BLANK}      { return RK_PE   ; }
{RK_PL}{BLANK}      { return RK_PL   ; }
{RK_POI}{BLANK}     { return RK_POI  ; }
{RK_PU}{BLANK}      { return RK_PU   ; }
{RK_PULSE}          { BEGIN(state_sourcetype); return RK_PULSE; }
{RK_PWL}{BLANK}     { return RK_PWL  ; }
{RK_RD}{BLANK}      { return RK_RD   ; }
{RK_SFFM}{BLANK}    { return RK_SFFM ; }
{RK_SIN}            { BEGIN(state_sourcetype); return RK_SIN  ; }
{RK_OP}{BLANK}      { return RK_OP   ; }
{RK_TRAN}{BLANK}    { return RK_TRAN ; }

{CMD_OP}            { return CMD_OP; }
{CMD_END}           { return CMD_END; }
{CMD_PRINT}         { return CMD_PRINT; }
{CMD_DC}            { return CMD_DC; }
{CMD_AC}            { return CMD_AC; }
{CMD_OPTIONS}       { BEGIN(state_options); return CMD_OPTIONS; }
{CMD_PLOT}          { return CMD_PLOT; }
{CMD_TRAN}          { return CMD_TRAN; }
{CMD_TITLE}         { BEGIN(state_onlystring); return CMD_TITLE; }

{VAR_V}             { yylval.str = strdup(yytext); return VAR_V; }
{VAR_I}             { yylval.str = strdup(yytext); return VAR_I; }

{STRING}    { yylval.str = strdup(yytext);  return STRING; }
{BLANK}     { }
{EQUAL}     { return EQUAL; }
.           { return *yytext; }
%%

/* {P_SWITCH}      { yylval.str = strdup(yytext);  return P_SWITCH;     } */

int yywrap() { return 1; }

double parseValue(const char *str)
{
    double value = atof(str);
    int    len = strlen(str);

    if (std::tolower(str[len - 1]) == 'g')
        if (std::tolower(str[len - 2]) == 'e')
            if (std::tolower(str[len - 3]) == 'm')
                return value * 1e6;

    char u = str[len - 1];
    if (u == 'F' || u == 'f')
        value *= 1e-15;
    else if (u == 'P' || u == 'p')
        value *= 1e-12;
    else if (u == 'N' || u == 'n')
        value *= 1e-9;
    else if (u == 'U' || u == 'u')
        value *= 1e-6;
    else if (u == 'M' || u == 'm')
        value *= 1e-3;
    else if (u == 'K' || u == 'k')
        value *= 1e3;
    else if (u == 'X' || u == 'x')
        value *= 1e6;
    else if (u == 'G' || u == 'g')
        value *= 1e9;
    else if (u == 'T' || u == 't')
        value *= 1e12;

    return value; 
}
