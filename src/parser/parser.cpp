/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "sp.y"

#define YYDEBUG 1
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "parser/scanner.hpp"
#include "global.hpp"
#include "myconsole.h"
#include "devices/all_devices.hpp"
#include "cmds/plot.hpp"
#include "circuit/circuit_node.h"

void yyerror (char const *s) {
   fprintf (stderr, "[SpParser] error at line %d: %s\n", yylineno, s);
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


#line 199 "parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_DOCEOF = 3,                     /* DOCEOF  */
  YYSYMBOL_P_RESISTOR = 4,                 /* P_RESISTOR  */
  YYSYMBOL_P_CAPACITOR = 5,                /* P_CAPACITOR  */
  YYSYMBOL_P_INDUCTOR = 6,                 /* P_INDUCTOR  */
  YYSYMBOL_P_ISOURCE = 7,                  /* P_ISOURCE  */
  YYSYMBOL_P_VSOURCE = 8,                  /* P_VSOURCE  */
  YYSYMBOL_P_VCCS = 9,                     /* P_VCCS  */
  YYSYMBOL_P_CCCS = 10,                    /* P_CCCS  */
  YYSYMBOL_P_CCVS = 11,                    /* P_CCVS  */
  YYSYMBOL_P_VCVS = 12,                    /* P_VCVS  */
  YYSYMBOL_P_OPAMP = 13,                   /* P_OPAMP  */
  YYSYMBOL_P_SWITCH = 14,                  /* P_SWITCH  */
  YYSYMBOL_P_DIODE = 15,                   /* P_DIODE  */
  YYSYMBOL_P_MOSFET = 16,                  /* P_MOSFET  */
  YYSYMBOL_RK_AC = 17,                     /* RK_AC  */
  YYSYMBOL_RK_ACI = 18,                    /* RK_ACI  */
  YYSYMBOL_RK_AM = 19,                     /* RK_AM  */
  YYSYMBOL_RK_DC = 20,                     /* RK_DC  */
  YYSYMBOL_RK_DEC = 21,                    /* RK_DEC  */
  YYSYMBOL_RK_EXP = 22,                    /* RK_EXP  */
  YYSYMBOL_RK_LIN = 23,                    /* RK_LIN  */
  YYSYMBOL_RK_OCT = 24,                    /* RK_OCT  */
  YYSYMBOL_RK_PAT = 25,                    /* RK_PAT  */
  YYSYMBOL_RK_PE = 26,                     /* RK_PE  */
  YYSYMBOL_RK_PL = 27,                     /* RK_PL  */
  YYSYMBOL_RK_POI = 28,                    /* RK_POI  */
  YYSYMBOL_RK_PU = 29,                     /* RK_PU  */
  YYSYMBOL_RK_PULSE = 30,                  /* RK_PULSE  */
  YYSYMBOL_RK_PWL = 31,                    /* RK_PWL  */
  YYSYMBOL_RK_R = 32,                      /* RK_R  */
  YYSYMBOL_RK_RD = 33,                     /* RK_RD  */
  YYSYMBOL_RK_SFFM = 34,                   /* RK_SFFM  */
  YYSYMBOL_RK_SIN = 35,                    /* RK_SIN  */
  YYSYMBOL_RK_OP = 36,                     /* RK_OP  */
  YYSYMBOL_RK_TRAN = 37,                   /* RK_TRAN  */
  YYSYMBOL_CMD_OP = 38,                    /* CMD_OP  */
  YYSYMBOL_CMD_END = 39,                   /* CMD_END  */
  YYSYMBOL_CMD_PRINT = 40,                 /* CMD_PRINT  */
  YYSYMBOL_CMD_DC = 41,                    /* CMD_DC  */
  YYSYMBOL_CMD_AC = 42,                    /* CMD_AC  */
  YYSYMBOL_CMD_OPTIONS = 43,               /* CMD_OPTIONS  */
  YYSYMBOL_CMD_PLOT = 44,                  /* CMD_PLOT  */
  YYSYMBOL_CMD_TRAN = 45,                  /* CMD_TRAN  */
  YYSYMBOL_OPTIONS_ITEM = 46,              /* OPTIONS_ITEM  */
  YYSYMBOL_VAR_V = 47,                     /* VAR_V  */
  YYSYMBOL_VAR_I = 48,                     /* VAR_I  */
  YYSYMBOL_LBRACKET = 49,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 50,                  /* RBRACKET  */
  YYSYMBOL_EOL = 51,                       /* EOL  */
  YYSYMBOL_STRING = 52,                    /* STRING  */
  YYSYMBOL_COMMENTLINE = 53,               /* COMMENTLINE  */
  YYSYMBOL_INTEGER = 54,                   /* INTEGER  */
  YYSYMBOL_REAL = 55,                      /* REAL  */
  YYSYMBOL_VALUE = 56,                     /* VALUE  */
  YYSYMBOL_YYACCEPT = 57,                  /* $accept  */
  YYSYMBOL_line = 58,                      /* line  */
  YYSYMBOL_component = 59,                 /* component  */
  YYSYMBOL_command = 60,                   /* command  */
  YYSYMBOL_resistor = 61,                  /* resistor  */
  YYSYMBOL_capacitor = 62,                 /* capacitor  */
  YYSYMBOL_inductor = 63,                  /* inductor  */
  YYSYMBOL_vsource = 64,                   /* vsource  */
  YYSYMBOL_isource = 65,                   /* isource  */
  YYSYMBOL_vccs = 66,                      /* vccs  */
  YYSYMBOL_vcvs = 67,                      /* vcvs  */
  YYSYMBOL_cccs = 68,                      /* cccs  */
  YYSYMBOL_ccvs = 69,                      /* ccvs  */
  YYSYMBOL_op = 70,                        /* op  */
  YYSYMBOL_end = 71,                       /* end  */
  YYSYMBOL_print = 72,                     /* print  */
  YYSYMBOL_dc = 73,                        /* dc  */
  YYSYMBOL_ac = 74,                        /* ac  */
  YYSYMBOL_options = 75,                   /* options  */
  YYSYMBOL_plot = 76,                      /* plot  */
  YYSYMBOL_tran = 77,                      /* tran  */
  YYSYMBOL_value = 78,                     /* value  */
  YYSYMBOL_node = 79                       /* node  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  76
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   169

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  79
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  149

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   311


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   229,   229,   230,   231,   232,   233,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   249,   250,   251,   252,
     253,   254,   255,   256,   260,   265,   270,   275,   280,   285,
     290,   295,   303,   308,   313,   318,   325,   332,   339,   346,
     354,   362,   370,   371,   372,   373,   374,   375,   379,   383,
     387,   394,   402,   406,   413,   417,   424,   429,   434,   439,
     445,   454,   461,   471,   476,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   497
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "DOCEOF", "P_RESISTOR",
  "P_CAPACITOR", "P_INDUCTOR", "P_ISOURCE", "P_VSOURCE", "P_VCCS",
  "P_CCCS", "P_CCVS", "P_VCVS", "P_OPAMP", "P_SWITCH", "P_DIODE",
  "P_MOSFET", "RK_AC", "RK_ACI", "RK_AM", "RK_DC", "RK_DEC", "RK_EXP",
  "RK_LIN", "RK_OCT", "RK_PAT", "RK_PE", "RK_PL", "RK_POI", "RK_PU",
  "RK_PULSE", "RK_PWL", "RK_R", "RK_RD", "RK_SFFM", "RK_SIN", "RK_OP",
  "RK_TRAN", "CMD_OP", "CMD_END", "CMD_PRINT", "CMD_DC", "CMD_AC",
  "CMD_OPTIONS", "CMD_PLOT", "CMD_TRAN", "OPTIONS_ITEM", "VAR_V", "VAR_I",
  "LBRACKET", "RBRACKET", "EOL", "STRING", "COMMENTLINE", "INTEGER",
  "REAL", "VALUE", "$accept", "line", "component", "command", "resistor",
  "capacitor", "inductor", "vsource", "isource", "vccs", "vcvs", "cccs",
  "ccvs", "op", "end", "print", "dc", "ac", "options", "plot", "tran",
  "value", "node", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-61)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      96,   -61,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   -61,   -61,    -6,   -61,   -13,   -61,     1,   -38,   -61,
     -61,    12,   -19,   -10,   -61,   -61,   -61,    58,    -7,   -61,
     -61,   -61,   -61,   -61,   -61,   -23,    32,    37,    17,     0,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   -61,   -61,   -61,   -61,   -38,
     -61,   -61,   -61,   -61,   -61,   -38,   -61,   -61,   -61,   -38,
     -38,    24,   -38,   -38,   -61,   -61,   -38,   -38,   -38,   -38,
     -61,   -61,   -61,   -38,   -38,   -38,   -38,   -38,   115,   115,
     115,   115,   -38,   -38,   -61,   -61,   -38,   -61,   -61,   -38,
     -38,   -38,   -38,   -61,   -61,   -61,   -61,   -61,   115,   115,
     115,   115,   -38,   -61,   -38,   -38,   -38,   -38,   -38,   -38,
     -38,   -38,   -38,   -61,   -38,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -38,   -38,   -38,   -38,    26,   -61
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     6,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    40,    41,     0,    48,     0,    54,     0,     0,     5,
       4,     0,     0,     0,     7,     8,     9,    10,    11,    13,
      12,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    65,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,    42,    45,    44,     0,
      57,    56,    58,    64,    63,     0,     1,     2,     3,     0,
       0,     0,     0,     0,    47,    46,     0,     0,     0,     0,
      55,    60,    59,     0,     0,     0,    32,    27,     0,     0,
       0,     0,     0,    61,    30,    29,     0,    35,    34,     0,
       0,     0,     0,    24,    25,    26,    33,    28,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,    49,    50,    52,    53,    36,
      38,    39,    37,     0,     0,     0,     0,     0,    31
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -60,    -3
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    75,    56
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      57,    58,    59,    60,    61,    62,    63,    64,    69,   102,
      82,    65,    76,    83,    66,   103,    73,    74,    70,   104,
     105,    71,   107,   108,    84,    85,   109,   110,   111,   112,
      67,    68,    77,   113,   114,   115,   116,   117,    72,    86,
      87,    78,   122,   123,    88,    89,   124,    91,    92,   125,
     126,   127,   128,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   133,    90,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   106,   143,    79,   148,     0,    80,     0,
       0,     0,     0,   144,   145,   146,   147,     0,    81,     0,
       0,     0,     0,     0,     0,   118,   119,   120,   121,     1,
       2,     3,     4,     5,     6,     7,     8,     9,    10,     0,
       0,     0,     0,     0,     0,   129,   130,   131,   132,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,     0,     0,    11,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,    19,     0,    20,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    54,     0,    55
};

static const yytype_int16 yycheck[] =
{
       3,     4,     5,     6,     7,     8,     9,    10,    21,    69,
      17,    17,     0,    20,    20,    75,    54,    55,    17,    79,
      80,    20,    82,    83,    47,    48,    86,    87,    88,    89,
      36,    37,    51,    93,    94,    95,    96,    97,    37,     7,
       8,    51,   102,   103,     7,     8,   106,    47,    48,   109,
     110,   111,   112,    56,    57,    58,    59,    60,    61,    62,
      63,    64,   122,    46,   124,   125,   126,   127,   128,   129,
     130,   131,   132,    49,   134,    17,    50,    -1,    20,    -1,
      -1,    -1,    -1,   143,   144,   145,   146,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,   120,   121,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    -1,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    -1,    -1,    -1,    -1,    51,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    38,    39,    40,    41,    42,    43,    44,    45,    51,
      53,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    52,    54,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    17,    20,    36,    37,    21,
      17,    20,    37,    54,    55,    78,     0,    51,    51,    17,
      20,    30,    17,    20,    47,    48,     7,     8,     7,     8,
      46,    47,    48,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    78,    78,    78,    78,    49,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    79,    79,
      79,    79,    78,    78,    78,    78,    78,    78,    78,    79,
      79,    79,    79,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    50
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    57,    58,    58,    58,    58,    58,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    60,    60,    60,    60,
      60,    60,    60,    60,    61,    62,    63,    64,    64,    64,
      64,    64,    65,    65,    65,    65,    66,    67,    68,    69,
      70,    71,    72,    72,    72,    72,    72,    72,    73,    73,
      73,    74,    74,    74,    75,    75,    76,    76,    76,    76,
      76,    77,    77,    78,    78,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     4,     3,     4,     3,
       3,    11,     3,     4,     3,     3,     6,     6,     6,     6,
       1,     1,     2,     2,     2,     2,     2,     2,     1,     5,
       5,     5,     5,     5,     1,     2,     2,     2,     2,     2,
       2,     3,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* line: component EOL  */
#line 229 "sp.y"
                  { yylineno++; return 0; }
#line 1754 "parser.cpp"
    break;

  case 3: /* line: command EOL  */
#line 230 "sp.y"
                  { yylineno++; return 0; }
#line 1760 "parser.cpp"
    break;

  case 4: /* line: COMMENTLINE  */
#line 231 "sp.y"
                  { yylineno++; console->log(std::format("[SpParser] commentline: {}", (yyvsp[0].str))); return 0; }
#line 1766 "parser.cpp"
    break;

  case 5: /* line: EOL  */
#line 232 "sp.y"
          { yylineno++; return 0; }
#line 1772 "parser.cpp"
    break;

  case 6: /* line: DOCEOF  */
#line 233 "sp.y"
             { return -255; }
#line 1778 "parser.cpp"
    break;

  case 24: /* resistor: P_RESISTOR node node value  */
#line 261 "sp.y"
    { ParseResistor((yyvsp[-3].str), (yyvsp[-2].str), (yyvsp[-1].str), (yyvsp[0].value)); }
#line 1784 "parser.cpp"
    break;

  case 25: /* capacitor: P_CAPACITOR node node value  */
#line 266 "sp.y"
    { ParseCapacitor((yyvsp[-3].str), (yyvsp[-2].str), (yyvsp[-1].str), (yyvsp[0].value)); }
#line 1790 "parser.cpp"
    break;

  case 26: /* inductor: P_INDUCTOR node node value  */
#line 271 "sp.y"
    { ParseInductor((yyvsp[-3].str), (yyvsp[-2].str), (yyvsp[-1].str), (yyvsp[0].value)); }
#line 1796 "parser.cpp"
    break;

  case 27: /* vsource: P_VSOURCE node node  */
#line 276 "sp.y"
    {
        ParseVSource((yyvsp[-2].str), (yyvsp[-1].str), (yyvsp[0].str), 0);
    }
#line 1804 "parser.cpp"
    break;

  case 28: /* vsource: P_VSOURCE node node value  */
#line 281 "sp.y"
    {
        ParseVSource((yyvsp[-3].str), (yyvsp[-2].str), (yyvsp[-1].str), (yyvsp[0].value));
    }
#line 1812 "parser.cpp"
    break;

  case 29: /* vsource: vsource RK_DC value  */
#line 286 "sp.y"
    {
        d_VSource->DC_Value = (yyvsp[0].value);
    }
#line 1820 "parser.cpp"
    break;

  case 30: /* vsource: vsource RK_AC value  */
#line 291 "sp.y"
    {
        d_VSource->AC_Mag = (yyvsp[0].value);
    }
#line 1828 "parser.cpp"
    break;

  case 31: /* vsource: vsource RK_PULSE LBRACKET value value value value value value value RBRACKET  */
#line 296 "sp.y"
    {
        d_VSource->pulse = new Pulse((yyvsp[-7].value), (yyvsp[-6].value), (yyvsp[-5].value), (yyvsp[-4].value), (yyvsp[-3].value), (yyvsp[-2].value), (yyvsp[-1].value));
        console->log(std::format("[SpParser] [Command] PULSE detected!"));
    }
#line 1837 "parser.cpp"
    break;

  case 32: /* isource: P_ISOURCE node node  */
#line 304 "sp.y"
    {
        ParseISource((yyvsp[-2].str), (yyvsp[-1].str), (yyvsp[0].str), 0);
    }
#line 1845 "parser.cpp"
    break;

  case 33: /* isource: P_ISOURCE node node value  */
#line 309 "sp.y"
    {
        ParseISource((yyvsp[-3].str), (yyvsp[-2].str), (yyvsp[-1].str), (yyvsp[0].value));
    }
#line 1853 "parser.cpp"
    break;

  case 34: /* isource: isource RK_DC value  */
#line 314 "sp.y"
    {
        d_ISource->DC_Value = (yyvsp[0].value);
    }
#line 1861 "parser.cpp"
    break;

  case 35: /* isource: isource RK_AC value  */
#line 319 "sp.y"
    {
        d_ISource->AC_Mag = (yyvsp[0].value);
    }
#line 1869 "parser.cpp"
    break;

  case 36: /* vccs: P_VCCS node node node node value  */
#line 326 "sp.y"
    {
        ParseVCCS((yyvsp[-5].str), (yyvsp[-4].str), (yyvsp[-3].str), (yyvsp[-2].str), (yyvsp[-1].str), (yyvsp[0].value));
    }
#line 1877 "parser.cpp"
    break;

  case 37: /* vcvs: P_VCVS node node node node value  */
#line 333 "sp.y"
    {
        ParseVCVS((yyvsp[-5].str), (yyvsp[-4].str), (yyvsp[-3].str), (yyvsp[-2].str), (yyvsp[-1].str), (yyvsp[0].value));
    }
#line 1885 "parser.cpp"
    break;

  case 38: /* cccs: P_CCCS node node node node value  */
#line 340 "sp.y"
    {
        ParseCCCS((yyvsp[-5].str), (yyvsp[-4].str), (yyvsp[-3].str), (yyvsp[-2].str), (yyvsp[-1].str), (yyvsp[0].value));
    }
#line 1893 "parser.cpp"
    break;

  case 39: /* ccvs: P_CCVS node node node node value  */
#line 347 "sp.y"
    {
        ParseCCVS((yyvsp[-5].str), (yyvsp[-4].str), (yyvsp[-3].str), (yyvsp[-2].str), (yyvsp[-1].str), (yyvsp[0].value));
    }
#line 1901 "parser.cpp"
    break;

  case 40: /* op: CMD_OP  */
#line 355 "sp.y"
    {
        circuit->command_OP.enabled = true;
        console->log(std::format("[SpParser] [Command] .OP detected!"));
    }
#line 1910 "parser.cpp"
    break;

  case 41: /* end: CMD_END  */
#line 363 "sp.y"
    {
        console->log(std::format("[SpParser] [Command] .end detected!"));
        return -255;
    }
#line 1919 "parser.cpp"
    break;

  case 42: /* print: CMD_PRINT RK_DC  */
#line 370 "sp.y"
                    { console->log("[SpParser] [Command] .print detected!"); console->log("[SpParser] type: DC"); }
#line 1925 "parser.cpp"
    break;

  case 43: /* print: CMD_PRINT RK_AC  */
#line 371 "sp.y"
                      { console->log("[SpParser] [Command] .print detected!"); console->log("[SpParser] type: AC"); }
#line 1931 "parser.cpp"
    break;

  case 44: /* print: CMD_PRINT RK_TRAN  */
#line 372 "sp.y"
                        { console->log("[SpParser] [Command] .print detected!"); console->log("[SpParser] type: TRAN"); }
#line 1937 "parser.cpp"
    break;

  case 45: /* print: CMD_PRINT RK_OP  */
#line 373 "sp.y"
                      { console->log("[SpParser] [Command] .print detected!"); console->log("[SpParser] type: OP"); }
#line 1943 "parser.cpp"
    break;

  case 46: /* print: print VAR_I  */
#line 374 "sp.y"
                  { console->log(std::format("[SpParser] print current: {}\n", (yyvsp[0].str))); }
#line 1949 "parser.cpp"
    break;

  case 47: /* print: print VAR_V  */
#line 375 "sp.y"
                  { console->log(std::format("[SpParser] print voltage: {}\n", (yyvsp[0].str))); }
#line 1955 "parser.cpp"
    break;

  case 48: /* dc: CMD_DC  */
#line 379 "sp.y"
           { 
        circuit->command_DC.enabled = true;
        console->log("[SpParser] [Command] .dc detected!");
    }
#line 1964 "parser.cpp"
    break;

  case 49: /* dc: dc P_ISOURCE value value value  */
#line 383 "sp.y"
                                     { 
        circuit->command_DC.sweepOptions.push_back(SweepOption((yyvsp[-3].str), (yyvsp[-2].value), (yyvsp[-1].value), (yyvsp[0].value)));
        console->log(std::format("[SpParser] dc current scan: from {:.3e} to {:.3e} step {:.3e}\n", (yyvsp[-2].value), (yyvsp[-1].value), (yyvsp[0].value)));
    }
#line 1973 "parser.cpp"
    break;

  case 50: /* dc: dc P_VSOURCE value value value  */
#line 387 "sp.y"
                                     { 
        circuit->command_DC.sweepOptions.push_back(SweepOption((yyvsp[-3].str), (yyvsp[-2].value), (yyvsp[-1].value), (yyvsp[0].value)));
        console->log(std::format("[SpParser] dc voltage scan: from {:.3e} to {:.3e} step {:.3e}\n", (yyvsp[-2].value), (yyvsp[-1].value), (yyvsp[0].value)));
    }
#line 1982 "parser.cpp"
    break;

  case 51: /* ac: CMD_AC RK_DEC value value value  */
#line 394 "sp.y"
                                    { 
        circuit->command_AC.enabled = true;
        circuit->command_AC.type = DEC;
        circuit->command_AC.np = (yyvsp[-2].value);
        circuit->command_AC.fstart = (yyvsp[-1].value);
        circuit->command_AC.fstop = (yyvsp[0].value);
        console->log("[SpParser] [Command] .ac detected!");
    }
#line 1995 "parser.cpp"
    break;

  case 52: /* ac: ac P_ISOURCE value value value  */
#line 402 "sp.y"
                                     { 
        circuit->command_AC.sweepOptions.push_back(SweepOption((yyvsp[-3].str), (yyvsp[-2].value), (yyvsp[-1].value), (yyvsp[0].value)));
        console->log(std::format("[SpParser] ac current scan: from {:.3e} to {:.3e} step {:.3e}\n", (yyvsp[-2].value), (yyvsp[-1].value), (yyvsp[0].value)));
    }
#line 2004 "parser.cpp"
    break;

  case 53: /* ac: ac P_VSOURCE value value value  */
#line 406 "sp.y"
                                     { 
        circuit->command_AC.sweepOptions.push_back(SweepOption((yyvsp[-3].str), (yyvsp[-2].value), (yyvsp[-1].value), (yyvsp[0].value)));
        console->log(std::format("[SpParser] ac voltage scan: from {:.3e} to {:.3e} step {:.3e}\n", (yyvsp[-2].value), (yyvsp[-1].value), (yyvsp[0].value)));
    }
#line 2013 "parser.cpp"
    break;

  case 54: /* options: CMD_OPTIONS  */
#line 414 "sp.y"
    {
        console->log("[SpParser] [Command] .options detected!");
    }
#line 2021 "parser.cpp"
    break;

  case 55: /* options: options OPTIONS_ITEM  */
#line 418 "sp.y"
    {
        parseOption((yyvsp[0].str));
    }
#line 2029 "parser.cpp"
    break;

  case 56: /* plot: CMD_PLOT RK_DC  */
#line 424 "sp.y"
                   { 
        console->log("[SpParser] [Command] .plot detected!"); console->log("[SpParser] type: DC"); 
        circuit->command_PLOT.enabled = true;
        circuit->command_PLOT.type = PLOT_DC;
    }
#line 2039 "parser.cpp"
    break;

  case 57: /* plot: CMD_PLOT RK_AC  */
#line 429 "sp.y"
                     { 
        console->log("[SpParser] [Command] .plot detected!"); console->log("[SpParser] type: AC"); 
        circuit->command_PLOT.enabled = true;
        circuit->command_PLOT.type = PLOT_AC;
    }
#line 2049 "parser.cpp"
    break;

  case 58: /* plot: CMD_PLOT RK_TRAN  */
#line 434 "sp.y"
                       { 
        console->log("[SpParser] [Command] .plot detected!"); console->log("[SpParser] type: TRAN"); 
        circuit->command_PLOT.enabled = true;
        circuit->command_PLOT.type = PLOT_TRAN;
    }
#line 2059 "parser.cpp"
    break;

  case 59: /* plot: plot VAR_I  */
#line 439 "sp.y"
                 { 
        console->log(std::format("[SpParser] plot current: {}\n", (yyvsp[0].str))); 
        std::string node = (yyvsp[0].str);

        circuit->command_PLOT.nodes.push_back(CircuitNode(node.substr(2, node.length() - 3), "I"));
    }
#line 2070 "parser.cpp"
    break;

  case 60: /* plot: plot VAR_V  */
#line 445 "sp.y"
                 { 
        console->log(std::format("[SpParser] plot voltage: {}\n", (yyvsp[0].str))); 
        std::string node = (yyvsp[0].str);

        circuit->command_PLOT.nodes.push_back(CircuitNode(node.substr(2, node.length() - 3), "V"));
    }
#line 2081 "parser.cpp"
    break;

  case 61: /* tran: CMD_TRAN value value  */
#line 454 "sp.y"
                         { 
        circuit->command_TRAN.enabled = true;
        circuit->command_TRAN.tstep = (yyvsp[-1].value);
        circuit->command_TRAN.tstop = (yyvsp[0].value);
        console->log("[SpParser] [Command] .tran detected!");
    }
#line 2092 "parser.cpp"
    break;

  case 62: /* tran: CMD_TRAN value value value  */
#line 461 "sp.y"
                               { 
        circuit->command_TRAN.enabled = true;
        circuit->command_TRAN.tstep = (yyvsp[-2].value);
        circuit->command_TRAN.tstop = (yyvsp[-1].value);
        circuit->command_TRAN.tstart = (yyvsp[0].value);
        console->log("[SpParser] [Command] .tran detected!");
    }
#line 2104 "parser.cpp"
    break;

  case 63: /* value: REAL  */
#line 472 "sp.y"
    {
        (yyval.value) = (yyvsp[0].value);
    }
#line 2112 "parser.cpp"
    break;

  case 64: /* value: INTEGER  */
#line 477 "sp.y"
    {
        (yyval.value) = (yyvsp[0].num);
    }
#line 2120 "parser.cpp"
    break;

  case 65: /* node: STRING  */
#line 483 "sp.y"
                    { (yyval.str) = strdup((yyvsp[0].str)); }
#line 2126 "parser.cpp"
    break;

  case 66: /* node: P_RESISTOR  */
#line 484 "sp.y"
                    { (yyval.str) = strdup((yyvsp[0].str)); }
#line 2132 "parser.cpp"
    break;

  case 67: /* node: P_CAPACITOR  */
#line 485 "sp.y"
                    { (yyval.str) = strdup((yyvsp[0].str)); }
#line 2138 "parser.cpp"
    break;

  case 68: /* node: P_INDUCTOR  */
#line 486 "sp.y"
                    { (yyval.str) = strdup((yyvsp[0].str)); }
#line 2144 "parser.cpp"
    break;

  case 69: /* node: P_ISOURCE  */
#line 487 "sp.y"
                    { (yyval.str) = strdup((yyvsp[0].str)); }
#line 2150 "parser.cpp"
    break;

  case 70: /* node: P_VSOURCE  */
#line 488 "sp.y"
                    { (yyval.str) = strdup((yyvsp[0].str)); }
#line 2156 "parser.cpp"
    break;

  case 71: /* node: P_VCCS  */
#line 489 "sp.y"
                    { (yyval.str) = strdup((yyvsp[0].str)); }
#line 2162 "parser.cpp"
    break;

  case 72: /* node: P_CCCS  */
#line 490 "sp.y"
                    { (yyval.str) = strdup((yyvsp[0].str)); }
#line 2168 "parser.cpp"
    break;

  case 73: /* node: P_CCVS  */
#line 491 "sp.y"
                    { (yyval.str) = strdup((yyvsp[0].str)); }
#line 2174 "parser.cpp"
    break;

  case 74: /* node: P_VCVS  */
#line 492 "sp.y"
                    { (yyval.str) = strdup((yyvsp[0].str)); }
#line 2180 "parser.cpp"
    break;

  case 75: /* node: P_OPAMP  */
#line 493 "sp.y"
                    { (yyval.str) = strdup((yyvsp[0].str)); }
#line 2186 "parser.cpp"
    break;

  case 76: /* node: P_SWITCH  */
#line 494 "sp.y"
                    { (yyval.str) = strdup((yyvsp[0].str)); }
#line 2192 "parser.cpp"
    break;

  case 77: /* node: P_DIODE  */
#line 495 "sp.y"
                    { (yyval.str) = strdup((yyvsp[0].str)); }
#line 2198 "parser.cpp"
    break;

  case 78: /* node: P_MOSFET  */
#line 496 "sp.y"
                    { (yyval.str) = strdup((yyvsp[0].str)); }
#line 2204 "parser.cpp"
    break;

  case 79: /* node: INTEGER  */
#line 498 "sp.y"
    {
        (yyval.str) = new char[32];
        sprintf((yyval.str), "%d", (yyvsp[0].num));
    }
#line 2213 "parser.cpp"
    break;


#line 2217 "parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 504 "sp.y"

