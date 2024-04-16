/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 162 "sp.y"

    #include <cstdio>
    #include <cstdlib>
    #include <cstring>

#line 55 "parser.hpp"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    DOCEOF = 258,                  /* DOCEOF  */
    P_RESISTOR = 259,              /* P_RESISTOR  */
    P_CAPACITOR = 260,             /* P_CAPACITOR  */
    P_INDUCTOR = 261,              /* P_INDUCTOR  */
    P_ISOURCE = 262,               /* P_ISOURCE  */
    P_VSOURCE = 263,               /* P_VSOURCE  */
    P_VCCS = 264,                  /* P_VCCS  */
    P_CCCS = 265,                  /* P_CCCS  */
    P_CCVS = 266,                  /* P_CCVS  */
    P_VCVS = 267,                  /* P_VCVS  */
    P_OPAMP = 268,                 /* P_OPAMP  */
    P_SWITCH = 269,                /* P_SWITCH  */
    P_DIODE = 270,                 /* P_DIODE  */
    P_MOSFET = 271,                /* P_MOSFET  */
    RK_AC = 272,                   /* RK_AC  */
    RK_ACI = 273,                  /* RK_ACI  */
    RK_AM = 274,                   /* RK_AM  */
    RK_DC = 275,                   /* RK_DC  */
    RK_DEC = 276,                  /* RK_DEC  */
    RK_EXP = 277,                  /* RK_EXP  */
    RK_LIN = 278,                  /* RK_LIN  */
    RK_OCT = 279,                  /* RK_OCT  */
    RK_PAT = 280,                  /* RK_PAT  */
    RK_PE = 281,                   /* RK_PE  */
    RK_PL = 282,                   /* RK_PL  */
    RK_POI = 283,                  /* RK_POI  */
    RK_PU = 284,                   /* RK_PU  */
    RK_PULSE = 285,                /* RK_PULSE  */
    RK_PWL = 286,                  /* RK_PWL  */
    RK_R = 287,                    /* RK_R  */
    RK_RD = 288,                   /* RK_RD  */
    RK_SFFM = 289,                 /* RK_SFFM  */
    RK_SIN = 290,                  /* RK_SIN  */
    RK_OP = 291,                   /* RK_OP  */
    RK_TRAN = 292,                 /* RK_TRAN  */
    CMD_OP = 293,                  /* CMD_OP  */
    CMD_END = 294,                 /* CMD_END  */
    CMD_PRINT = 295,               /* CMD_PRINT  */
    CMD_DC = 296,                  /* CMD_DC  */
    CMD_AC = 297,                  /* CMD_AC  */
    CMD_OPTIONS = 298,             /* CMD_OPTIONS  */
    CMD_PLOT = 299,                /* CMD_PLOT  */
    CMD_TRAN = 300,                /* CMD_TRAN  */
    CMD_TITLE = 301,               /* CMD_TITLE  */
    OPTIONS_ITEM = 302,            /* OPTIONS_ITEM  */
    VAR_V = 303,                   /* VAR_V  */
    VAR_I = 304,                   /* VAR_I  */
    LBRACKET = 305,                /* LBRACKET  */
    RBRACKET = 306,                /* RBRACKET  */
    EOL = 307,                     /* EOL  */
    STRING = 308,                  /* STRING  */
    COMMENTLINE = 309,             /* COMMENTLINE  */
    EQUAL = 310,                   /* EQUAL  */
    INTEGER = 311,                 /* INTEGER  */
    REAL = 312,                    /* REAL  */
    VALUE = 313                    /* VALUE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 169 "sp.y"

    int    num;
    double value;
    char*  str;

#line 136 "parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
