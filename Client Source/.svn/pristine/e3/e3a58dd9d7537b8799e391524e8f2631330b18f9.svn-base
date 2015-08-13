
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"

#include "StdAfx.h"
#include "ParsingSymbols.h"

#include <Engine/Templates/DynamicStackArray.cpp>
#include <Engine/Ska/StringTable.h>
#include <Engine/Ska/Render.h>

// callback function to update shader params
extern void UpdateShaderParamsOnConvert(CStaticArray<struct SurfaceShader> &_assSurfaceShaders);
extern CTFileName strCurentFileName;
MeshLOD *pMeshLOD;
SkeletonLOD *pSkeletonLOD;


static INDEX _ctVertices  = 0;
static INDEX _ctNormals   = 0;
static INDEX _ctUVMaps    = 0;
static INDEX _ctTexCoords = 0;
static INDEX _ctSurfaces  = 0;
static INDEX _ctTriangles = 0;
static INDEX _ctWeights   = 0;
static INDEX _ctVertexWeights = 0;
static INDEX _ctMorphs    = 0;
static INDEX _ctVertexMorphs = 0;
static INDEX _ctBones     = 0;
static INDEX _ctBoneEnvelopes = 0;
static INDEX _ctMorphEnvelopes = 0;
static INDEX _ctFrames = 0;

static INDEX _iBone       = 0;
static INDEX _iVertex     = 0;
static INDEX _iNormal     = 0;
static INDEX _iUVMap      = 0;
static INDEX _iTexCoord   = 0;
static INDEX _iSurface    = 0;
static INDEX _iTriangle   = 0;
static INDEX _iWeight     = 0;
static INDEX _iVertexWeight  = 0;
static INDEX _iMorph      = 0;
static INDEX _iVertexMorph = 0;
static INDEX _iBoneEnvelope = 0;
static INDEX _iMorphEnvelope = 0;
static INDEX _iFrame = 0;

/*
** Shader
*/
INDEX _ishParamIndex; // current index of shader param
INDEX _ctshParamsMax; // size of array allocated for shader params
CStaticArray<struct SurfaceShader> _assSurfaceShaders;
INDEX _iShaderSurfIndex = 0;
INDEX _ctShaderSurfaces = 0;

/*
** 
*/
float _fCurentMaxDistance;// max distance for next lod
static float _fThreshold = 0;// treshold for next animation
float _fAnimSpeed = -1;
BOOL bCompresion = FALSE;// is animation is using compresions

/* Line 189 of yacc.c  */
#line 64 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"

#define YYERROR_VERBOSE 1                  

// if error occurs in parsing
void yyerror(const char *strFormat, ...)
{
  va_list arg;
  va_start(arg, strFormat);
  CTString strError;
  strError.VPrintF(strFormat, arg);
  // throw the string
  ThrowF_t("File '%s' (line %d)\n%s",(const char*)strCurentFileName, _yy_iLine, (const char*)strError);
};



/* Line 189 of yacc.c  */
#line 154 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     c_float = 258,
     c_int = 259,
     c_string = 260,
     k_SE_MESH = 261,
     k_SE_SKELETON = 262,
     k_PARENT = 263,
     k_BONES = 264,
     k_VERTICES = 265,
     k_NORMALS = 266,
     k_UVMAPS = 267,
     k_NAME = 268,
     k_TEXCOORDS = 269,
     k_SURFACES = 270,
     k_TRIANGLE_SET = 271,
     k_WEIGHTS = 272,
     k_WEIGHT_SET = 273,
     k_MORPHS = 274,
     k_RELATIVE = 275,
     k_TRUE = 276,
     k_FALSE = 277,
     k_MORPH_SET = 278,
     k_SE_MESH_END = 279,
     k_SE_SKELETON_END = 280,
     k_SE_ANIM = 281,
     k_SEC_PER_FRAME = 282,
     k_FRAMES = 283,
     k_BONEENVELOPES = 284,
     k_MORPHENVELOPES = 285,
     k_DEFAULT_POSE = 286,
     k_SE_ANIM_END = 287,
     k_ANIM_SET_LIST = 288,
     k_ANIM_ID = 289,
     k_MAX_DISTANCE = 290,
     k_MESHLODLIST = 291,
     k_SKELETONLODLIST = 292,
     k_TRESHOLD = 293,
     k_COMPRESION = 294,
     k_LENGTH = 295,
     k_ANIMSPEED = 296,
     k_SHADER_PARAMS = 297,
     k_SHADER_PARAMS_END = 298,
     k_SHADER_SURFACES = 299,
     k_SHADER_SURFACE = 300,
     k_SHADER_NAME = 301,
     k_SHADER_TEXTURES = 302,
     k_SHADER_UVMAPS = 303,
     k_SHADER_COLORS = 304,
     k_SHADER_FLOATS = 305,
     k_SHADER_FLAGS = 306,
     k_FULL_FACE_FORWARD = 307,
     k_HALF_FACE_FORWARD = 308
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 82 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"

  INDEX i;
  float f;
  const char *str;
  float v2[2];
  float v3[3];
  int   i3[3];
  float f12[12];



/* Line 214 of yacc.c  */
#line 255 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 267 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  22
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   424

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  58
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  127
/* YYNRULES -- Number of rules.  */
#define YYNRULES  179
/* YYNRULES -- Number of states.  */
#define YYNSTATES  427

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   308

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    57,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    56,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    54,     2,    55,     2,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    13,    18,    19,
      21,    23,    26,    27,    31,    44,    49,    50,    52,    54,
      57,    66,    67,    70,    71,    75,    76,    80,    84,    94,
      95,   102,   103,   105,   107,   110,   111,   123,   125,   128,
     131,   132,   139,   140,   142,   144,   147,   148,   155,   157,
     160,   163,   166,   169,   174,   175,   177,   179,   182,   187,
     191,   192,   199,   200,   202,   204,   207,   208,   220,   225,
     228,   229,   231,   232,   233,   246,   247,   249,   251,   254,
     263,   271,   272,   280,   281,   283,   285,   288,   291,   292,
     300,   301,   303,   305,   308,   311,   312,   320,   321,   323,
     325,   328,   331,   332,   340,   341,   343,   345,   348,   351,
     352,   354,   358,   362,   363,   366,   367,   370,   371,   378,
     380,   383,   386,   387,   394,   396,   399,   402,   403,   410,
     411,   413,   415,   418,   419,   431,   433,   436,   439,   440,
     447,   449,   452,   453,   465,   467,   470,   473,   474,   481,
     482,   484,   486,   489,   490,   502,   504,   507,   514,   515,
     522,   523,   525,   527,   530,   531,   545,   547,   550,   567,
     570,   572,   574,   576,   580,   586,   592,   598,   622,   625
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      59,     0,    -1,    64,    -1,    91,    -1,    67,    -1,    65,
      -1,    60,    -1,    36,    54,    61,    55,    -1,    -1,    62,
      -1,    64,    -1,    62,    64,    -1,    -1,    35,   177,    56,
      -1,    63,   104,   134,   135,   136,   137,   141,   145,   153,
     160,   168,   176,    -1,    33,    54,    66,    55,    -1,    -1,
      67,    -1,    68,    -1,    67,    68,    -1,    70,    69,    71,
      72,    73,    74,    82,    90,    -1,    -1,    39,   184,    -1,
      -1,    38,   177,    56,    -1,    -1,    41,   177,    56,    -1,
      26,     3,    56,    -1,    27,     3,    56,    28,     4,    56,
      34,     5,    56,    -1,    -1,    29,     4,    75,    54,    76,
      55,    -1,    -1,    77,    -1,    78,    -1,    76,    78,    -1,
      -1,    13,     5,    31,    54,   183,    56,    79,    55,    54,
      80,    55,    -1,    81,    -1,    80,    81,    -1,   183,    56,
      -1,    -1,    30,     4,    83,    54,    84,    55,    -1,    -1,
      85,    -1,    86,    -1,    84,    86,    -1,    -1,    13,     5,
      87,    54,    88,    55,    -1,    89,    -1,    88,    89,    -1,
       3,    56,    -1,     4,    56,    -1,    32,    56,    -1,    37,
      54,    92,    55,    -1,    -1,    93,    -1,    94,    -1,    93,
      94,    -1,    63,    95,    96,   103,    -1,     7,     3,    56,
      -1,    -1,     9,     4,    54,    97,    98,    55,    -1,    -1,
      99,    -1,   100,    -1,    98,   100,    -1,    -1,    13,     5,
      56,     8,     5,    56,    40,   177,   101,    56,   102,    -1,
      54,   183,    56,    55,    -1,    25,    56,    -1,    -1,   105,
      -1,    -1,    -1,    42,   177,    56,    44,     4,    54,   106,
     108,    55,    56,   107,    43,    -1,    -1,   109,    -1,   110,
      -1,   109,   110,    -1,   111,   112,   117,   122,   127,   132,
      55,    56,    -1,    45,     5,    56,    54,    46,     5,    56,
      -1,    -1,    47,     4,   113,    54,   114,    55,    56,    -1,
      -1,   115,    -1,   116,    -1,   115,   116,    -1,     5,    56,
      -1,    -1,    48,     4,   118,    54,   119,    55,    56,    -1,
      -1,   120,    -1,   121,    -1,   120,   121,    -1,     4,    56,
      -1,    -1,    49,     4,   123,    54,   124,    55,    56,    -1,
      -1,   125,    -1,   126,    -1,   125,   126,    -1,     4,    56,
      -1,    -1,    50,     4,   128,    54,   129,    55,    56,    -1,
      -1,   130,    -1,   131,    -1,   130,   131,    -1,   177,    56,
      -1,    -1,   133,    -1,    51,     4,    56,    -1,     6,     3,
      56,    -1,    -1,    52,   184,    -1,    -1,    53,   184,    -1,
      -1,    10,     4,    54,   138,   139,    55,    -1,   140,    -1,
     139,   140,    -1,   180,    56,    -1,    -1,    11,     4,    54,
     142,   143,    55,    -1,   144,    -1,   143,   144,    -1,   181,
      56,    -1,    -1,    12,     4,    54,   146,   147,    55,    -1,
      -1,   148,    -1,   149,    -1,   148,   149,    -1,    -1,    54,
      13,     5,    56,    14,     4,    54,   150,   151,    55,    55,
      -1,   152,    -1,   151,   152,    -1,   179,    56,    -1,    -1,
      15,     4,    54,   154,   155,    55,    -1,   156,    -1,   155,
     156,    -1,    -1,    54,    13,     5,    56,    16,     4,    54,
     157,   158,    55,    55,    -1,   159,    -1,   158,   159,    -1,
     182,    56,    -1,    -1,    17,     4,    54,   161,   162,    55,
      -1,    -1,   163,    -1,   164,    -1,   162,   164,    -1,    -1,
      54,    13,     5,    56,    18,     4,    54,   165,   166,    55,
      55,    -1,   167,    -1,   166,   167,    -1,    54,     4,    56,
     177,    56,    55,    -1,    -1,    19,     4,    54,   169,   170,
      55,    -1,    -1,   171,    -1,   172,    -1,   170,   172,    -1,
      -1,    54,    13,     5,    56,    20,   184,    23,     4,   173,
      54,   174,    55,    55,    -1,   175,    -1,   174,   175,    -1,
      54,     4,    56,   177,    57,   177,    57,   177,    56,   177,
      57,   177,    57,   177,    56,    55,    -1,    24,    56,    -1,
       3,    -1,     4,    -1,     4,    -1,   177,    57,   177,    -1,
     177,    57,   177,    57,   177,    -1,   177,    57,   177,    57,
     177,    -1,   178,    57,   178,    57,   178,    -1,   177,    57,
     177,    57,   177,    57,   177,    57,   177,    57,   177,    57,
     177,    57,   177,    57,   177,    57,   177,    57,   177,    57,
     177,    -1,    21,    56,    -1,    22,    56,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   165,   165,   166,   167,   168,   169,   177,   180,   182,
     186,   187,   192,   195,   202,   211,   214,   216,   220,   221,
     225,   230,   231,   236,   237,   243,   246,   253,   269,   300,
     299,   324,   326,   330,   331,   336,   335,   380,   381,   385,
     405,   404,   425,   427,   431,   432,   437,   436,   466,   467,
     471,   484,   500,   509,   512,   514,   518,   519,   523,   527,
     535,   534,   564,   566,   570,   571,   576,   575,   607,   623,
     633,   639,   646,   654,   645,   661,   663,   667,   668,   671,
     678,   689,   688,   704,   706,   709,   710,   714,   735,   734,
     750,   752,   755,   756,   759,   775,   774,   790,   792,   795,
     796,   799,   815,   814,   831,   833,   836,   837,   840,   854,
     856,   859,   867,   882,   885,   896,   899,   910,   910,   928,
     929,   933,   947,   947,   969,   970,   974,   988,   988,  1009,
    1011,  1015,  1016,  1021,  1020,  1049,  1050,  1054,  1071,  1070,
    1096,  1097,  1102,  1101,  1158,  1159,  1163,  1181,  1180,  1198,
    1200,  1204,  1205,  1210,  1209,  1236,  1237,  1241,  1264,  1263,
    1282,  1284,  1287,  1288,  1293,  1292,  1320,  1321,  1325,  1348,
    1353,  1357,  1363,  1369,  1376,  1384,  1392,  1400,  1417,  1421
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "c_float", "c_int", "c_string",
  "k_SE_MESH", "k_SE_SKELETON", "k_PARENT", "k_BONES", "k_VERTICES",
  "k_NORMALS", "k_UVMAPS", "k_NAME", "k_TEXCOORDS", "k_SURFACES",
  "k_TRIANGLE_SET", "k_WEIGHTS", "k_WEIGHT_SET", "k_MORPHS", "k_RELATIVE",
  "k_TRUE", "k_FALSE", "k_MORPH_SET", "k_SE_MESH_END", "k_SE_SKELETON_END",
  "k_SE_ANIM", "k_SEC_PER_FRAME", "k_FRAMES", "k_BONEENVELOPES",
  "k_MORPHENVELOPES", "k_DEFAULT_POSE", "k_SE_ANIM_END", "k_ANIM_SET_LIST",
  "k_ANIM_ID", "k_MAX_DISTANCE", "k_MESHLODLIST", "k_SKELETONLODLIST",
  "k_TRESHOLD", "k_COMPRESION", "k_LENGTH", "k_ANIMSPEED",
  "k_SHADER_PARAMS", "k_SHADER_PARAMS_END", "k_SHADER_SURFACES",
  "k_SHADER_SURFACE", "k_SHADER_NAME", "k_SHADER_TEXTURES",
  "k_SHADER_UVMAPS", "k_SHADER_COLORS", "k_SHADER_FLOATS",
  "k_SHADER_FLAGS", "k_FULL_FACE_FORWARD", "k_HALF_FACE_FORWARD", "'{'",
  "'}'", "';'", "','", "$accept", "program", "meshlod_list",
  "meshlod_array_opt", "meshlod_array", "max_distance", "meshlod",
  "animset_list", "animset_array_opt", "animset_array", "animset",
  "compresion_opt", "treshold_opt", "animspeed_opt", "animset_begin",
  "animation_header", "bone_envelopes", "$@1", "bone_env_header_opt",
  "bone_env_header", "bone_envelope", "$@2", "bone_env_array",
  "bone_env_m", "morph_envelopes", "$@3", "morph_env_header",
  "morph_env_header_notnull", "morph_env", "$@4", "morph_env_array",
  "morph_env_i", "animset_end", "skeletonlod_list",
  "opt_skeletonlod_array", "skeletonlod_array", "skeletonlod",
  "skeleton_begin", "bones", "$@5", "bone_headers", "bone_headers_notnull",
  "bone_header", "$@6", "bone", "skeleton_end", "shader_params_opt",
  "shader_params", "$@7", "$@8", "surface_shader_params_array_opt",
  "surface_shader_params_array", "surface_shader_params", "shader_name",
  "shader_textures", "$@9", "shader_texture_array_opt",
  "shader_texture_array", "shader_texture", "shader_uvmaps", "$@10",
  "shader_uvmaps_array_opt", "shader_uvmaps_array", "shader_uvmap",
  "shader_colors", "$@11", "shader_colors_array_opt",
  "shader_colors_array", "shader_color", "shader_floats", "$@12",
  "shader_floats_array_opt", "shader_floats_array", "shader_float",
  "shader_flag_opt", "shader_flags", "mesh_begin",
  "is_full_faceforward_opt", "is_half_faceforward_opt", "vertices", "$@13",
  "vert3_array", "vert3_array_value", "normals", "$@14", "norm3_array",
  "norm3_array_value", "uvmaps", "$@15", "uvmaps_opt_array",
  "uvmaps_array", "uvmap", "$@16", "uv2_array", "uv2_array_value",
  "surfaces", "$@17", "surfaces_array", "triangleset", "$@18",
  "triangle_array", "triangle_array_value", "weights", "$@19",
  "weights_array", "weights_array_notnull", "weightset", "$@20",
  "weight_map_array", "weight_map_array_value", "morphs", "$@21",
  "morphs_array", "morphs_array_notnull", "morphset", "$@22",
  "morph_set_array", "morph_set_array_value", "mesh_end", "float_const",
  "int_const", "uv2", "vert3", "norm3", "triangle", "matrix", "boolean", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   123,   125,    59,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    58,    59,    59,    59,    59,    59,    60,    61,    61,
      62,    62,    63,    63,    64,    65,    66,    66,    67,    67,
      68,    69,    69,    70,    70,    71,    71,    72,    73,    75,
      74,    76,    76,    77,    77,    79,    78,    80,    80,    81,
      83,    82,    84,    84,    85,    85,    87,    86,    88,    88,
      89,    89,    90,    91,    92,    92,    93,    93,    94,    95,
      97,    96,    98,    98,    99,    99,   101,   100,   102,   103,
     104,   104,   106,   107,   105,   108,   108,   109,   109,   110,
     111,   113,   112,   114,   114,   115,   115,   116,   118,   117,
     119,   119,   120,   120,   121,   123,   122,   124,   124,   125,
     125,   126,   128,   127,   129,   129,   130,   130,   131,   132,
     132,   133,   134,   135,   135,   136,   136,   138,   137,   139,
     139,   140,   142,   141,   143,   143,   144,   146,   145,   147,
     147,   148,   148,   150,   149,   151,   151,   152,   154,   153,
     155,   155,   157,   156,   158,   158,   159,   161,   160,   162,
     162,   163,   163,   165,   164,   166,   166,   167,   169,   168,
     170,   170,   171,   171,   173,   172,   174,   174,   175,   176,
     177,   177,   178,   179,   180,   181,   182,   183,   184,   184
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     4,     0,     1,
       1,     2,     0,     3,    12,     4,     0,     1,     1,     2,
       8,     0,     2,     0,     3,     0,     3,     3,     9,     0,
       6,     0,     1,     1,     2,     0,    11,     1,     2,     2,
       0,     6,     0,     1,     1,     2,     0,     6,     1,     2,
       2,     2,     2,     4,     0,     1,     1,     2,     4,     3,
       0,     6,     0,     1,     1,     2,     0,    11,     4,     2,
       0,     1,     0,     0,    12,     0,     1,     1,     2,     8,
       7,     0,     7,     0,     1,     1,     2,     2,     0,     7,
       0,     1,     1,     2,     2,     0,     7,     0,     1,     1,
       2,     2,     0,     7,     0,     1,     1,     2,     2,     0,
       1,     3,     3,     0,     2,     0,     2,     0,     6,     1,
       2,     2,     0,     6,     1,     2,     2,     0,     6,     0,
       1,     1,     2,     0,    11,     1,     2,     2,     0,     6,
       1,     2,     0,    11,     1,     2,     2,     0,     6,     0,
       1,     1,     2,     0,    11,     1,     2,     6,     0,     6,
       0,     1,     1,     2,     0,    13,     1,     2,    16,     2,
       1,     1,     1,     3,     5,     5,     5,    23,     2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      23,     0,     0,     0,     0,     0,     0,     6,    70,     2,
       5,    23,    18,    21,     3,    23,   170,   171,     0,    12,
      12,     0,     1,     0,     0,    71,    19,     0,    25,     0,
      23,    13,     0,    12,    10,     0,     0,    12,    56,    24,
       0,     0,   113,     0,     0,    22,     0,     0,    15,     7,
      11,     0,     0,    53,    57,     0,     0,     0,   115,   178,
     179,     0,     0,     0,     0,     0,     0,     0,   112,   114,
       0,     0,    26,     0,     0,     0,    59,     0,     0,    58,
       0,   116,     0,     0,    27,     0,     0,     0,    60,    69,
      72,     0,     0,     0,     0,    29,     0,     0,    62,    75,
     117,     0,     0,     0,     0,     0,    40,     0,    20,     0,
       0,    63,    64,     0,     0,    76,    77,     0,     0,   122,
       0,     0,     0,     0,    31,     0,    52,     0,    61,    65,
       0,     0,    78,     0,     0,     0,   119,     0,     0,     0,
     127,     0,     0,     0,     0,     0,     0,    32,    33,    42,
       0,     0,    73,    81,     0,     0,   118,   120,     0,   121,
       0,   124,     0,     0,   129,   138,     0,     0,     0,     0,
       0,    30,    34,     0,     0,    43,    44,     0,     0,     0,
       0,    88,     0,     0,     0,   123,   125,     0,   126,     0,
       0,   130,   131,     0,   147,     0,     0,    14,     0,     0,
      46,    41,    45,     0,     0,    74,    83,     0,    95,     0,
     109,     0,     0,     0,   128,   132,     0,     0,   140,   149,
     158,   169,    28,     0,     0,     0,     0,     0,     0,    84,
      85,    90,     0,   102,     0,     0,   110,   174,     0,     0,
       0,   139,   141,     0,     0,   150,   151,   160,     0,     0,
       0,     0,    80,    87,     0,    86,     0,     0,    91,    92,
      97,     0,     0,     0,   175,     0,     0,     0,   148,   152,
       0,     0,   161,   162,     0,    35,     0,     0,     0,    48,
      66,    82,    94,     0,    93,     0,     0,    98,    99,   104,
     111,    79,     0,     0,     0,     0,   159,   163,     0,     0,
      50,    51,    47,    49,     0,    89,   101,     0,   100,     0,
     105,   106,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,   107,   108,   133,     0,     0,     0,     0,     0,
       0,    67,   103,     0,   142,     0,     0,     0,     0,    37,
       0,     0,     0,   135,     0,     0,     0,   153,     0,     0,
      36,    38,    39,     0,     0,   136,     0,   137,   172,     0,
     144,     0,     0,     0,     0,     0,    68,   134,   173,     0,
     145,     0,   146,     0,     0,   155,   164,     0,   143,     0,
       0,     0,   156,     0,     0,     0,     0,   154,     0,     0,
     176,     0,     0,     0,   166,     0,     0,     0,     0,   167,
       0,   157,     0,   165,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   177,     0,     0,     0,   168
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     6,     7,    32,    33,     8,     9,    10,    29,    11,
      12,    28,    13,    47,    63,    75,    87,   105,   146,   147,
     148,   299,   338,   339,    97,   125,   174,   175,   176,   224,
     278,   279,   108,    14,    36,    37,    38,    52,    66,    98,
     110,   111,   112,   304,   331,    79,    24,    25,    99,   179,
     114,   115,   116,   117,   134,   180,   228,   229,   230,   155,
     207,   257,   258,   259,   183,   232,   286,   287,   288,   210,
     261,   309,   310,   311,   235,   236,    42,    58,    71,    83,
     118,   135,   136,    93,   139,   160,   161,   103,   164,   190,
     191,   192,   333,   342,   343,   122,   193,   217,   218,   346,
     359,   360,   143,   219,   244,   245,   246,   363,   374,   375,
     168,   247,   271,   272,   273,   383,   393,   394,   197,   248,
     361,   345,   138,   163,   362,   340,    45
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -338
static const yytype_int16 yypact[] =
{
      34,   -23,    25,   -21,   -15,    25,    55,  -338,    15,  -338,
    -338,    16,  -338,    24,  -338,   -18,  -338,  -338,    29,   -17,
     -13,    30,  -338,    25,    62,  -338,  -338,    14,    38,    20,
      -8,  -338,    32,   -10,  -338,    77,    33,    -9,  -338,  -338,
      35,    86,    40,    37,    39,  -338,    25,    64,  -338,  -338,
    -338,    91,    87,  -338,  -338,    53,    42,    14,    46,  -338,
    -338,    44,    98,    75,    47,   100,    80,   102,  -338,  -338,
      14,    97,  -338,    52,   107,    82,  -338,    58,    57,  -338,
      60,  -338,   111,   106,  -338,    63,   114,    90,  -338,  -338,
    -338,    67,   118,   112,    95,  -338,   121,    94,   115,    84,
    -338,    73,   126,   116,   128,    81,  -338,    78,  -338,   131,
       4,  -338,  -338,   133,    85,    84,  -338,    92,    25,  -338,
      88,   137,   127,    89,   130,    93,  -338,    96,  -338,  -338,
      99,   101,  -338,   142,   103,     1,  -338,   104,   108,    25,
    -338,   105,   144,   134,   120,   145,     6,  -338,  -338,   136,
     152,   109,  -338,  -338,   158,   117,  -338,  -338,    25,  -338,
       3,  -338,   110,   113,   119,  -338,   122,   161,   146,   163,
     140,  -338,  -338,   167,    10,  -338,  -338,   169,   129,   135,
     123,  -338,   175,   132,   124,  -338,  -338,    25,  -338,   170,
     125,   119,  -338,   138,  -338,   139,   141,  -338,   143,   147,
    -338,  -338,  -338,   148,   179,  -338,   181,   149,  -338,   183,
     151,    25,   150,   184,  -338,  -338,   177,    -4,  -338,   154,
    -338,  -338,  -338,    25,   156,   155,   157,   159,   162,   181,
    -338,   187,   160,  -338,   190,   164,  -338,  -338,    25,   165,
     191,  -338,  -338,   185,    19,  -338,  -338,   166,   168,   171,
      49,    25,  -338,  -338,   172,  -338,   173,   176,   187,  -338,
     196,   178,   174,   182,  -338,   192,   186,   200,  -338,  -338,
     198,    23,  -338,  -338,    25,  -338,   188,   189,     5,  -338,
    -338,  -338,  -338,   194,  -338,   195,   180,   196,  -338,    25,
    -338,  -338,   208,   202,   197,   211,  -338,  -338,   199,   193,
    -338,  -338,  -338,  -338,   201,  -338,  -338,   203,  -338,   205,
      25,  -338,   206,   204,   218,   215,   207,    25,   210,   212,
    -338,   209,  -338,  -338,  -338,   213,   219,   214,   216,    25,
      25,  -338,  -338,    25,  -338,   217,    14,    25,     7,  -338,
     220,   221,     9,  -338,   222,   225,   233,  -338,   223,   226,
    -338,  -338,  -338,   227,   229,  -338,    25,  -338,  -338,    11,
    -338,   228,   230,   224,   235,    25,  -338,  -338,  -338,   234,
    -338,   233,  -338,   236,    26,  -338,  -338,   231,  -338,   237,
     239,   238,  -338,   242,    25,   233,    25,  -338,   243,   241,
    -338,   244,   248,    28,  -338,    25,   246,   247,   249,  -338,
     245,  -338,    25,  -338,    25,   250,   252,    25,    25,   253,
     254,    25,    25,   256,   257,    25,    25,   259,   260,    25,
      25,   261,  -338,    25,   263,   251,  -338
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -338,  -338,  -338,  -338,  -338,    12,     8,  -338,  -338,   232,
      13,  -338,  -338,  -338,  -338,  -338,  -338,  -338,  -338,  -338,
     153,  -338,  -338,  -150,  -338,  -338,  -338,  -338,    50,  -338,
    -338,   -52,  -338,  -338,  -338,  -338,   255,  -338,  -338,  -338,
    -338,  -338,   240,  -338,  -338,  -338,  -338,  -338,  -338,  -338,
    -338,  -338,   258,  -338,  -338,  -338,  -338,  -338,    41,  -338,
    -338,  -338,  -338,    -3,  -338,  -338,  -338,  -338,   -46,  -338,
    -338,  -338,  -338,   -67,  -338,  -338,  -338,  -338,  -338,  -338,
    -338,  -338,   262,  -338,  -338,  -338,   264,  -338,  -338,  -338,
    -338,    70,  -338,  -338,   -88,  -338,  -338,  -338,    51,  -338,
    -338,   -90,  -338,  -338,  -338,  -338,    31,  -338,  -338,  -100,
    -338,  -338,  -338,  -338,    54,  -338,  -338,  -103,  -338,    -2,
    -337,  -338,  -338,  -338,  -338,  -221,   -56
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -56
static const yytype_int16 yytable[] =
{
      18,    69,   249,    21,    16,    17,    16,    17,   276,   277,
      16,    17,    16,    17,    81,   358,    -4,   109,     2,   145,
       5,    40,     2,   173,    26,     2,     2,    34,    16,    17,
       5,    15,    35,    19,   379,    43,    44,   -16,    -8,    20,
     -12,    50,   -54,    26,    61,    -9,   -55,   -17,   390,    35,
     216,   241,   276,   277,     5,    22,   156,    23,   185,   128,
     302,   171,   350,    27,   354,   201,   369,     1,    41,     2,
       3,     4,     5,   243,   268,    48,   -12,   270,   296,    46,
     373,   381,   392,   398,    51,    31,    39,    49,    53,    56,
      62,    55,    57,    59,    64,    60,    65,    67,    68,    70,
      72,    73,    74,    76,    77,    78,    80,    82,    84,   341,
      85,    86,    88,    89,    90,    91,   137,    92,    95,    94,
      96,   100,   101,   104,   102,   106,   107,   119,   109,   113,
     120,   121,   123,   137,   126,   124,   127,   162,   130,   133,
     131,   141,   140,   145,   142,   144,   153,   149,   166,   173,
     170,   154,   150,   167,   169,   151,   184,   152,   162,   165,
     177,   158,   181,   178,   159,   195,   182,   187,   198,   188,
     196,   199,   200,   189,   203,   204,   194,   206,   205,   208,
     214,   211,   209,   213,   226,   212,   227,   233,   351,   239,
     240,   256,   216,   220,   262,   251,   266,   221,   267,   222,
     285,   223,   234,   231,   225,   294,   292,   238,   243,   237,
     250,   295,   313,   252,   260,   253,   316,   254,   314,   263,
     270,   265,   325,   335,   202,   274,   303,   275,   281,   282,
     290,   283,   289,   326,   336,   307,   264,   358,   291,   376,
     380,   308,   293,   322,   300,   301,   364,    30,   318,   280,
     305,   306,   397,   315,   355,   284,   317,   319,   324,   320,
     321,   215,   323,   327,   329,   332,   330,   334,   242,   370,
     255,   347,   298,   337,   382,   269,   352,   353,   373,   356,
     348,   357,   366,   365,   367,   371,   372,   312,   384,   378,
     399,     0,    54,   387,   385,   386,   388,   392,   395,   172,
     396,   401,   404,   402,   403,     0,   426,   407,   312,   408,
     411,   412,   415,     0,   416,   328,   419,   420,   423,   425,
       0,     0,     0,     0,     0,   297,     0,     0,     0,     0,
       0,   344,     0,     0,     0,   349,     0,     0,     0,     0,
     344,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     129,     0,     0,     0,   368,     0,     0,     0,     0,     0,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   132,     0,     0,     0,     0,     0,     0,
       0,     0,   389,     0,   391,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,     0,   157,     0,     0,
     405,     0,   406,     0,     0,   409,   410,     0,     0,   413,
     414,     0,     0,   417,   418,     0,     0,   421,   422,     0,
       0,   424,     0,     0,   186
};

static const yytype_int16 yycheck[] =
{
       2,    57,   223,     5,     3,     4,     3,     4,     3,     4,
       3,     4,     3,     4,    70,     4,     0,    13,    35,    13,
      38,    23,    35,    13,    11,    35,    35,    19,     3,     4,
      38,    54,    20,    54,   371,    21,    22,    55,    55,    54,
       6,    33,    55,    30,    46,    55,    55,    55,   385,    37,
      54,    55,     3,     4,    38,     0,    55,    42,    55,    55,
      55,    55,    55,    39,    55,    55,    55,    33,     6,    35,
      36,    37,    38,    54,    55,    55,    42,    54,    55,    41,
      54,    55,    54,    55,     7,    56,    56,    55,    55,     3,
      26,    56,    52,    56,     3,    56,     9,    44,    56,    53,
      56,     3,    27,    56,     4,    25,     4,    10,    56,   330,
       3,    29,    54,    56,    54,     4,   118,    11,     4,    56,
      30,    54,     4,    28,    12,     4,    32,    54,    13,    45,
       4,    15,     4,   135,    56,    54,     5,   139,     5,    47,
      55,     4,    54,    13,    17,    56,     4,    54,     4,    13,
       5,    48,    56,    19,    34,    56,   158,    56,   160,    54,
       8,    57,     4,    54,    56,     4,    49,    57,     5,    56,
      24,    31,     5,    54,     5,    46,    54,    54,    43,     4,
      55,    57,    50,    13,     5,   187,     5,     4,   338,     5,
      13,     4,    54,    54,     4,    40,     5,    56,    13,    56,
       4,    54,    51,    54,    56,     5,    14,    57,    54,   211,
      54,    13,     4,    56,    54,    56,     5,    55,    16,    55,
      54,    56,     4,     4,   174,    57,   278,    56,    56,    56,
      56,    55,    54,    18,    20,    55,   238,     4,    56,     4,
       4,   287,    56,   310,    56,    56,    23,    15,    55,   251,
      56,    56,     4,    56,   342,   258,    57,    56,    54,    56,
      55,   191,    56,    56,    54,    56,    54,    54,   217,   359,
     229,    54,   274,    57,   374,   244,    56,    56,    54,    57,
     336,    56,    55,    57,    55,    57,    56,   289,    57,    55,
     393,    -1,    37,    55,    57,    56,    54,    54,    57,   146,
      56,    55,    57,    56,    55,    -1,    55,    57,   310,    57,
      57,    57,    56,    -1,    57,   317,    57,    57,    57,    56,
      -1,    -1,    -1,    -1,    -1,   271,    -1,    -1,    -1,    -1,
      -1,   333,    -1,    -1,    -1,   337,    -1,    -1,    -1,    -1,
     342,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,   356,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   365,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   384,    -1,   386,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   395,    -1,    -1,    -1,   135,    -1,    -1,
     402,    -1,   404,    -1,    -1,   407,   408,    -1,    -1,   411,
     412,    -1,    -1,   415,   416,    -1,    -1,   419,   420,    -1,
      -1,   423,    -1,    -1,   160
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    33,    35,    36,    37,    38,    59,    60,    63,    64,
      65,    67,    68,    70,    91,    54,     3,     4,   177,    54,
      54,   177,     0,    42,   104,   105,    68,    39,    69,    66,
      67,    56,    61,    62,    64,    63,    92,    93,    94,    56,
     177,     6,   134,    21,    22,   184,    41,    71,    55,    55,
      64,     7,    95,    55,    94,    56,     3,    52,   135,    56,
      56,   177,    26,    72,     3,     9,    96,    44,    56,   184,
      53,   136,    56,     3,    27,    73,    56,     4,    25,   103,
       4,   184,    10,   137,    56,     3,    29,    74,    54,    56,
      54,     4,    11,   141,    56,     4,    30,    82,    97,   106,
      54,     4,    12,   145,    28,    75,     4,    32,    90,    13,
      98,    99,   100,    45,   108,   109,   110,   111,   138,    54,
       4,    15,   153,     4,    54,    83,    56,     5,    55,   100,
       5,    55,   110,    47,   112,   139,   140,   177,   180,   142,
      54,     4,    17,   160,    56,    13,    76,    77,    78,    54,
      56,    56,    56,     4,    48,   117,    55,   140,    57,    56,
     143,   144,   177,   181,   146,    54,     4,    19,   168,    34,
       5,    55,    78,    13,    84,    85,    86,     8,    54,   107,
     113,     4,    49,   122,   177,    55,   144,    57,    56,    54,
     147,   148,   149,   154,    54,     4,    24,   176,     5,    31,
       5,    55,    86,     5,    46,    43,    54,   118,     4,    50,
     127,    57,   177,    13,    55,   149,    54,   155,   156,   161,
      54,    56,    56,    54,    87,    56,     5,     5,   114,   115,
     116,    54,   123,     4,    51,   132,   133,   177,    57,     5,
      13,    55,   156,    54,   162,   163,   164,   169,   177,   183,
      54,    40,    56,    56,    55,   116,     4,   119,   120,   121,
      54,   128,     4,    55,   177,    56,     5,    13,    55,   164,
      54,   170,   171,   172,    57,    56,     3,     4,    88,    89,
     177,    56,    56,    55,   121,     4,   124,   125,   126,    54,
      56,    56,    14,    56,     5,    13,    55,   172,   177,    79,
      56,    56,    55,    89,   101,    56,    56,    55,   126,   129,
     130,   131,   177,     4,    16,    56,     5,    57,    55,    56,
      56,    55,   131,    56,    54,     4,    18,    56,   177,    54,
      54,   102,    56,   150,    54,     4,    20,    57,    80,    81,
     183,   183,   151,   152,   177,   179,   157,    54,   184,   177,
      55,    81,    56,    56,    55,   152,    57,    56,     4,   158,
     159,   178,   182,   165,    23,    57,    55,    55,   177,    55,
     159,    57,    56,    54,   166,   167,     4,   177,    55,   178,
       4,    55,   167,   173,    57,    57,    56,    55,    54,   177,
     178,   177,    54,   174,   175,    57,    56,     4,    55,   175,
     177,    55,    56,    55,    57,   177,   177,    57,    57,   177,
     177,    57,    57,   177,   177,    56,    57,   177,   177,    57,
      57,   177,   177,    57,   177,    56,    55
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 12:

/* Line 1455 of yacc.c  */
#line 192 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  // fill with default values
;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 196 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _fCurentMaxDistance = (yyvsp[(2) - (3)].f);
;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 230 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    { bCompresion = FALSE; ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 232 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    { bCompresion  = (yyvsp[(2) - (2)].i); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 236 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    { _fThreshold = 0.0f; ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 238 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    { _fThreshold = (yyvsp[(2) - (3)].f); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 243 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _fAnimSpeed = -1;
;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 247 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _fAnimSpeed = (yyvsp[(2) - (3)].f);
;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 254 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  // get curent animation count
  INDEX ctan = _yy_pAnimSet->as_Anims.Count();
  // increase curent animation count
  _yy_pAnimSet->as_Anims.Expand(ctan+1);
  // get ref to new created animation
  Animation &an = _yy_pAnimSet->as_Anims[ctan];
  // set animation allready read treshold
  an.an_fThreshold = _fThreshold;
  // set animation allready read compresion flag
  an.an_bCompresed =  bCompresion;
;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 270 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _ctFrames = (yyvsp[(5) - (9)].i);
  if(_ctFrames<0) {
    yyerror("Negative frame count found %d",_ctFrames);
  }

  // get current animset count
  INDEX ctan = _yy_pAnimSet->as_Anims.Count();
  // get ref to last animation
  Animation &an = _yy_pAnimSet->as_Anims[ctan-1];
  // set animations seconds per frame
  an.an_fSecPerFrame = (yyvsp[(2) - (9)].f);
  // set animation frame count
  an.an_iFrames = _ctFrames;
  // set animation ID
  an.an_iID = ska_GetIDFromStringTable((yyvsp[(8) - (9)].str));
  // set animation source file (current file)
  an.an_fnSourceFile = strCurentFileName;
  // set animation custom speed (if any)
  an.an_bCustomSpeed = FALSE;
  if(_fAnimSpeed>=0)
  {
    an.an_bCustomSpeed = TRUE;
    an.an_fSecPerFrame = _fAnimSpeed;
  }
;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 300 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  // set bone envelopes count
  _ctBoneEnvelopes = (yyvsp[(2) - (2)].i);

  if(_ctBoneEnvelopes<0) {
    yyerror("Negative bone envelope count found %d",_ctBoneEnvelopes);
  }

  // count animations
  INDEX ctan = _yy_pAnimSet->as_Anims.Count();
  Animation &an = _yy_pAnimSet->as_Anims[ctan-1];
  // add new bone envelope array
  an.an_abeBones.New(_ctBoneEnvelopes);
  // set current bone envelope index
  _iBoneEnvelope = 0;
;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 317 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iBoneEnvelope!=_ctBoneEnvelopes) {
    yyerror("Incorect number of bone envelopes.\nExpecting %d but found only %d",_ctBoneEnvelopes,_iBoneEnvelope);
  }
;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 336 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iBoneEnvelope>=_ctBoneEnvelopes) {
    yyerror("Incorect number of bone envelopes - %d",_ctBoneEnvelopes);
  }
  
  // get last animation
  INDEX ctan = _yy_pAnimSet->as_Anims.Count();
  Animation &an = _yy_pAnimSet->as_Anims[ctan-1];
  BoneEnvelope &be = an.an_abeBones[_iBoneEnvelope];

  // create array of bone posiotions and rotations
  be.be_apPos.New(_ctFrames);
  be.be_arRot.New(_ctFrames);
  // for each frame
  for(INDEX ifn=0;ifn<_ctFrames;ifn++)
  {
    // set bone position and rotation frame number
    be.be_apPos[ifn].ap_iFrameNum = ifn;
    be.be_arRot[ifn].ar_iFrameNum = ifn;
  }
  // get id of this bone envelope
  be.be_iBoneID = ska_GetIDFromStringTable((yyvsp[(2) - (6)].str));
  // Calculate offset len on bone envelope
  be.be_OffSetLen = (FLOAT3D((yyvsp[(5) - (6)].f12)[3],(yyvsp[(5) - (6)].f12)[7],(yyvsp[(5) - (6)].f12)[11])).Length();
  
  _iFrame = 0;
;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 364 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _iBoneEnvelope++;
  if(_iFrame!=_ctFrames) {
    INDEX ctan = _yy_pAnimSet->as_Anims.Count();
    Animation &an = _yy_pAnimSet->as_Anims[ctan-1];
    BoneEnvelope &be = an.an_abeBones[_iBoneEnvelope-1];
    CTString strBoneEnvelope = ska_GetStringFromTable(be.be_iBoneID);

     yyerror("Incorect number of bone envelope frames count for bone envelope '%s'.\nExpecting %d but found only %d",
             (const char*)strBoneEnvelope,_ctFrames,_iFrame);
  }
;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 386 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iFrame>=_ctFrames) {
    yyerror("Incorect number of bone envelope frames - %d",_ctFrames);
  }

  INDEX ctan = _yy_pAnimSet->as_Anims.Count();
  QVect qvPlacement;
  Matrix12ToQVect(qvPlacement,(yyvsp[(1) - (2)].f12));
  Animation &an = _yy_pAnimSet->as_Anims[ctan-1];
  BoneEnvelope &be = an.an_abeBones[_iBoneEnvelope];

  be.be_apPos[_iFrame].ap_vPos = qvPlacement.vPos;
  be.be_arRot[_iFrame].ar_qRot = qvPlacement.qRot;
  _iFrame++;
;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 405 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  // set morph envelopes count
  _ctMorphEnvelopes = (yyvsp[(2) - (2)].i);

  if(_ctMorphEnvelopes<0) {
    yyerror("Negative morph envelope count found %d",_ctMorphEnvelopes);
  }

  INDEX ctan = _yy_pAnimSet->as_Anims.Count();
  _yy_pAnimSet->as_Anims[ctan-1].an_ameMorphs.New(_ctMorphEnvelopes);
  _iMorphEnvelope = 0;
;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 418 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iMorphEnvelope!=_ctMorphEnvelopes) {
    yyerror("Incorect number of morph envelopes.\nExpecting %d but found only %d",_ctMorphEnvelopes,_iMorphEnvelope);
  }
;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 437 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iMorphEnvelope>=_ctMorphEnvelopes) {
    yyerror("Incorect number of morph envelopes - %d",_ctMorphEnvelopes);
  }
    INDEX ctan = _yy_pAnimSet->as_Anims.Count();
    // get ref to animation and morph envelope
    Animation &an = _yy_pAnimSet->as_Anims[ctan-1];
    MorphEnvelope &me = an.an_ameMorphs[_iMorphEnvelope];
    // create array for morphs factors
    me.me_aFactors.New(_ctFrames);
    me.me_iMorphMapID = ska_GetIDFromStringTable((yyvsp[(2) - (2)].str));
    _iFrame = 0;
;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 451 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _iMorphEnvelope++;
  if(_iFrame!=_ctFrames) {
    INDEX ctan = _yy_pAnimSet->as_Anims.Count();
    Animation &an = _yy_pAnimSet->as_Anims[ctan-1];
    MorphEnvelope &me = an.an_ameMorphs[_iMorphEnvelope-1];
    CTString strMorphEnvelope = ska_GetStringFromTable(me.me_iMorphMapID);

     yyerror("Incorect number of morph envelope frames count for morph envelope '%s'.\nExpecting %d but found only %d",
             (const char*)strMorphEnvelope,_ctFrames,_iFrame);
  }
;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 472 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iFrame>=_ctFrames) {
    yyerror("Incorect number of morph envelope frames - %d",_ctFrames);
  }

  INDEX ctan = _yy_pAnimSet->as_Anims.Count();
  Animation &an = _yy_pAnimSet->as_Anims[ctan-1];
  MorphEnvelope &me = an.an_ameMorphs[_iMorphEnvelope];
  me.me_aFactors[_iFrame] = (yyvsp[(1) - (2)].f);

  _iFrame++;
;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 485 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iFrame>=_ctFrames) {
    yyerror("Incorect number of morph envelope frames - %d",_ctFrames);
  }

  INDEX ctan = _yy_pAnimSet->as_Anims.Count();
  Animation &an = _yy_pAnimSet->as_Anims[ctan-1];
  MorphEnvelope &me = an.an_ameMorphs[_iMorphEnvelope];
  me.me_aFactors[_iFrame] = (yyvsp[(1) - (2)].i);

  _iFrame++;
;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 528 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  // check for version of skeleton ascii file
;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 535 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _ctBones = (yyvsp[(2) - (3)].i);
  if(_ctBones<0) {
    yyerror("Negative bone count found %d",_ctBones);
  }

  // set bone index to 0
  _iBone = 0;
  // get skeleton lods count
  INDEX ctskllod = _yy_pSkeleton->skl_aSkeletonLODs.Count();
  // increase skeleton lods count
  _yy_pSkeleton->skl_aSkeletonLODs.Expand(ctskllod+1);
  // get ref to new skeleton created
  pSkeletonLOD = &_yy_pSkeleton->skl_aSkeletonLODs[ctskllod];
  // read source file name of skeleton ascii file
  pSkeletonLOD->slod_fnSourceFile = strCurentFileName;
  // get allready remembered max distance
  pSkeletonLOD->slod_fMaxDistance = _fCurentMaxDistance;
  // create new array for bones
  pSkeletonLOD->slod_aBones.New(_ctBones);
;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 557 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iBone!=_ctBones) {
    yyerror("Incorect number of bones.\nExpecting %d but found only %d",_ctBones,_iBone);
  }
;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 576 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iBone>=_ctBones) {
    yyerror("Incorect number of bones - %d",_ctBones);
  }

  SkeletonBone &sb = pSkeletonLOD->slod_aBones[_iBone];

  CTString strBoneID = (yyvsp[(2) - (8)].str);
  CTString strParentBoneID = (yyvsp[(5) - (8)].str);

  // if bone name is invalid
  if(strBoneID.Length()==0) {
    yyerror("Invalid bone name");
  } else {
    sb.sb_iID = ska_StringToID(strBoneID);
  }

  // if parent bone is invalid
  if(strParentBoneID.Length()==0) {
    sb.sb_iParentID = -1;
  // else get parent bone id
  } else {
    sb.sb_iParentID = ska_StringToID(strParentBoneID);
  }
  // get bone length
  sb.sb_fBoneLength = (yyvsp[(8) - (8)].f);
;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 608 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  SkeletonBone &sb = pSkeletonLOD->slod_aBones[_iBone];
  // convert matrix to qvect struct
  Matrix12ToQVect(sb.sb_qvRelPlacement,(yyvsp[(2) - (4)].f12));
  // copy matrix to bone as its absolute placement
  memcpy(&sb.sb_mAbsPlacement,(yyvsp[(2) - (4)].f12),sizeof(float)*12);
  // calculate bone offset length
  FLOAT3D vOffset = FLOAT3D((yyvsp[(2) - (4)].f12)[3],(yyvsp[(2) - (4)].f12)[7],(yyvsp[(2) - (4)].f12)[11]);
  sb.sb_fOffSetLen = vOffset.Length();
  // increase bone count
  _iBone++;
;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 624 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  // end of skeleton parsing
;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 633 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  // no shader for next surface
  _assSurfaceShaders.Clear();
  _ctShaderSurfaces=0;
  _iShaderSurfIndex=0;
;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 639 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  UpdateShaderParamsOnConvert(_assSurfaceShaders);
;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 646 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _assSurfaceShaders.Clear();
  _ctShaderSurfaces = (yyvsp[(5) - (6)].i);
  // create array of shader params for each surface in mesh file
  _assSurfaceShaders.New(_ctShaderSurfaces);
  // reset surface index
  _iShaderSurfIndex=0;
;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 654 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  // check if surfaces count match count of read surfaces
  if(_iShaderSurfIndex!=_ctShaderSurfaces) {
    yyerror("Incorect number of surfaces");
  }
;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 672 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _iShaderSurfIndex++;
;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 679 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iShaderSurfIndex>=_ctShaderSurfaces) yyerror("Incorect number of surfaces");
  SurfaceShader &SurfShader = _assSurfaceShaders[_iShaderSurfIndex];
  SurfShader.ss_iSurfaceID = ska_GetIDFromStringTable((yyvsp[(2) - (7)].str));
  // set shader fn
  SurfShader.fnShaderName = (CTString)(yyvsp[(6) - (7)].str);
;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 689 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _ctshParamsMax = (yyvsp[(2) - (2)].i);
  SurfaceShader &SurfShader = _assSurfaceShaders[_iShaderSurfIndex];
  SurfShader.ss_spShaderParams.sp_aiTextureIDs.New(_ctshParamsMax);
  _ishParamIndex = 0;
;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 695 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  // incorect params count
  if(_ishParamIndex!=_ctshParamsMax)
  {
    CTString strErr = CTString(0,"Incorect texture params count\nExpecting %d but found %d",_ctshParamsMax,_ishParamIndex);
    yyerror((char*)(const char*)strErr);
  }
;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 715 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_ishParamIndex>=_ctshParamsMax)
  {
    CTString strErr = CTString(0,"Incorect texture params count %d",_ctshParamsMax);
    yyerror((char*)(const char*)strErr);
  }
  SurfaceShader &SurfShader = _assSurfaceShaders[_iShaderSurfIndex];
  // set ID of current texture name
  CTString strTextureID = (yyvsp[(1) - (2)].str);
  if(strTextureID.Length()==0) {
    SurfShader.ss_spShaderParams.sp_aiTextureIDs[_ishParamIndex] = -1;
  } else {
    SurfShader.ss_spShaderParams.sp_aiTextureIDs[_ishParamIndex] = ska_StringToID((yyvsp[(1) - (2)].str));
  }
  // increase shading parametar index
  _ishParamIndex++;
;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 735 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _ctshParamsMax = (yyvsp[(2) - (2)].i);
  SurfaceShader &SurfShader = _assSurfaceShaders[_iShaderSurfIndex];
  SurfShader.ss_spShaderParams.sp_aiTexCoordsIndex.New(_ctshParamsMax);
  _ishParamIndex = 0;
;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 741 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  // incorect params count
  if(_ishParamIndex!=_ctshParamsMax)
  {
    CTString strErr = CTString(0,"Incorect uvmap params count\nExpecting %d but found %d",_ctshParamsMax,_ishParamIndex);
    yyerror((char*)(const char*)strErr);
  }
;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 760 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_ishParamIndex>=_ctshParamsMax)
  {
    CTString strErr = CTString(0,"Incorect uvmap params count %d",_ctshParamsMax);
    yyerror((char*)(const char*)strErr);
  }
  SurfaceShader &SurfShader = _assSurfaceShaders[_iShaderSurfIndex];
  // set index of current uvmap name
  SurfShader.ss_spShaderParams.sp_aiTexCoordsIndex[_ishParamIndex] = (yyvsp[(1) - (2)].i);
  // increase shading parametar index
  _ishParamIndex++;
;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 775 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _ctshParamsMax = (yyvsp[(2) - (2)].i);
  SurfaceShader &SurfShader = _assSurfaceShaders[_iShaderSurfIndex];
  SurfShader.ss_spShaderParams.sp_acolColors.New(_ctshParamsMax);
  _ishParamIndex = 0;
;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 781 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  // incorect params count
  if(_ishParamIndex!=_ctshParamsMax)
  {
    CTString strErr = CTString(0,"Incorect color params count\nExpecting %d but found %d",_ctshParamsMax,_ishParamIndex);
    yyerror((char*)(const char*)strErr);
  }
;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 800 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_ishParamIndex>=_ctshParamsMax)
  {
    CTString strErr = CTString(0,"Incorect colors params count %d",_ctshParamsMax);
    yyerror((char*)(const char*)strErr);
  }
  SurfaceShader &SurfShader = _assSurfaceShaders[_iShaderSurfIndex];
  // set color
  SurfShader.ss_spShaderParams.sp_acolColors[_ishParamIndex] = (yyvsp[(1) - (2)].i);
  // increase color parametar index
  _ishParamIndex++;
;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 815 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _ctshParamsMax = (yyvsp[(2) - (2)].i);
  SurfaceShader &SurfShader = _assSurfaceShaders[_iShaderSurfIndex];
  SurfShader.ss_spShaderParams.sp_afFloats.New(_ctshParamsMax);
  _ishParamIndex = 0;

;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 822 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  // incorect params count
  if(_ishParamIndex!=_ctshParamsMax)
  {
    CTString strErr = CTString(0,"Incorect floats params count\nExpecting %d but found %d",_ctshParamsMax,_ishParamIndex);
    yyerror((char*)(const char*)strErr);
  }
;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 841 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_ishParamIndex>=_ctshParamsMax)
  {
    CTString strErr = CTString(0,"Incorect floats params count %d",_ctshParamsMax);
    yyerror((char*)(const char*)strErr);
  }
  // set color
  SurfaceShader &SurfShader = _assSurfaceShaders[_iShaderSurfIndex];
  SurfShader.ss_spShaderParams.sp_afFloats[_ishParamIndex] = (yyvsp[(1) - (2)].f);
  // increase floats parametar index
  _ishParamIndex++;
;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 860 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  SurfaceShader &SurfShader = _assSurfaceShaders[_iShaderSurfIndex];
  SurfShader.ss_spShaderParams.sp_ulFlags = (yyvsp[(2) - (3)].i);
;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 868 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if ((yyvsp[(2) - (3)].f)!=0.1) NOTHING; //parseMesh->error
  INDEX ctmshlod = _yy_pMesh->msh_aMeshLODs.Count();
  _yy_pMesh->msh_aMeshLODs.Expand(ctmshlod+1);
  pMeshLOD = &_yy_pMesh->msh_aMeshLODs[ctmshlod];
  pMeshLOD->mlod_fnSourceFile = strCurentFileName;
  pMeshLOD->mlod_fMaxDistance = _fCurentMaxDistance;
  _yy_iWIndex=0;
  _yy_iMIndex=0;
;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 882 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  pMeshLOD->mlod_ulFlags &= ~ML_FULL_FACE_FORWARD;
;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 886 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if((yyvsp[(2) - (2)].i) == TRUE) {
    pMeshLOD->mlod_ulFlags|=ML_FULL_FACE_FORWARD;
  } else {
    pMeshLOD->mlod_ulFlags &=~ML_FULL_FACE_FORWARD;
  }
;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 896 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  pMeshLOD->mlod_ulFlags &= ~ML_HALF_FACE_FORWARD;
;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 900 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if((yyvsp[(2) - (2)].i) == TRUE) {
    pMeshLOD->mlod_ulFlags|=ML_HALF_FACE_FORWARD;
  } else {
    pMeshLOD->mlod_ulFlags &=~ML_HALF_FACE_FORWARD;
  }
;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 910 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _ctVertices = (yyvsp[(2) - (3)].i);
  if(_ctVertices<0) {
    yyerror("Negative vertex count found %d",_ctVertices);
  }

  pMeshLOD->mlod_aVertices.New(_ctVertices);
  _iVertex = 0;
;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 920 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iVertex!=_ctVertices) {
    yyerror("Incorect number of vertices.\nExpecting %d but found only %d",_ctVertices,_iVertex);
  }
;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 933 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iVertex>=_ctVertices) {
    yyerror("Incorect number of vertices - %d",_ctVertices);
  }

  pMeshLOD->mlod_aVertices[_iVertex].x = (yyvsp[(1) - (2)].v3)[0];
  pMeshLOD->mlod_aVertices[_iVertex].y = (yyvsp[(1) - (2)].v3)[1];
  pMeshLOD->mlod_aVertices[_iVertex].z = (yyvsp[(1) - (2)].v3)[2];
  _iVertex++;
;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 947 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _ctNormals = (yyvsp[(2) - (3)].i);
  if(_ctNormals != _ctVertices) {
    yyerror("Number of normals differs from the number of vertices!");
  }
  if(_ctNormals <0) {
    yyerror("Negative normal count found %d",_ctNormals);
  }

  _iNormal = 0;

  pMeshLOD->mlod_aNormals.New(_ctNormals);
;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 961 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iNormal!=_ctNormals) {
    yyerror("Incorect number of normals.\nExpecting %d but found only %d",_ctNormals,_iNormal);
  }
;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 975 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iNormal>=_ctNormals) {
    yyerror("Incorect number of normals - %d",_ctNormals);
  }

  pMeshLOD->mlod_aNormals[_iNormal].nx = (yyvsp[(1) - (2)].v3)[0];
  pMeshLOD->mlod_aNormals[_iNormal].ny = (yyvsp[(1) - (2)].v3)[1];
  pMeshLOD->mlod_aNormals[_iNormal].nz = (yyvsp[(1) - (2)].v3)[2];
  _iNormal++;
;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 988 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {

  _ctUVMaps = (yyvsp[(2) - (3)].i);
  if(_ctUVMaps<0) {
    yyerror("Negative UVMaps count found %d",_ctUVMaps);
  }
  if(_ctUVMaps>MAX_UVMAPS_PER_MESH) {
    yyerror("Mesh has %d uvmaps and %d uvmaps is maximum",_ctUVMaps,MAX_UVMAPS_PER_MESH);
  }
  _iUVMap = 0;

  pMeshLOD->mlod_aUVMaps.New(_ctUVMaps);
;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 1002 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iUVMap!=_ctUVMaps) {
    yyerror("Incorect number of UVMaps.\nExpecting %d but found only %d",_ctUVMaps,_iUVMap);
  }
;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 1021 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iUVMap>=_ctUVMaps) {
    yyerror("Incorect number of UVMaps - %d",_ctUVMaps);
  }

  _ctTexCoords = (yyvsp[(6) - (7)].i);
  if(_ctTexCoords != _ctVertices) {
    yyerror("Number of texcords differs from the number of vertices!");
  }
  if(_ctTexCoords <0) {
    yyerror("Negative texcoords count found %d",_ctTexCoords);
  }
  _iTexCoord = 0;

  MeshUVMap &uvmap = pMeshLOD->mlod_aUVMaps[_iUVMap];
  uvmap.muv_aTexCoords.New(_ctTexCoords);
  uvmap.muv_iID = ska_GetIDFromStringTable((yyvsp[(3) - (7)].str));
;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 1040 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _iUVMap++;
  if(_iTexCoord!=_ctTexCoords) {
    yyerror("Incorect number of TexCoords for UVMap %d.\nExpecting %d but found only %d",_iUVMap,_ctTexCoords,_iTexCoord);
  }
;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1055 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iTexCoord>=_ctTexCoords) {
    yyerror("Incorect number of TexCoords - %d",_ctTexCoords);
  }

  MeshUVMap &uvmap = pMeshLOD->mlod_aUVMaps[_iUVMap];
  MeshTexCoord &mtc = uvmap.muv_aTexCoords[_iTexCoord];

  mtc.u = (yyvsp[(1) - (2)].v2)[0];
  mtc.v = (yyvsp[(1) - (2)].v2)[1];
  _iTexCoord++;
;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1071 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _ctSurfaces = (yyvsp[(2) - (3)].i);
  if(_ctSurfaces <0) {
    yyerror("Negative surface count found %d",_ctSurfaces);
  }

  _iSurface = 0;

  pMeshLOD->mlod_aSurfaces.New(_ctSurfaces);

  // reset shader for each surface
  for(INDEX isrf=0;isrf<_ctSurfaces;isrf++) {
    MeshSurface &msrf = pMeshLOD->mlod_aSurfaces[isrf];
    msrf.msrf_pShader = NULL;
  }
;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 1088 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iSurface!=_ctSurfaces) {
    yyerror("Incorect number of surfaces.\nExpecting %d but found only %d",_ctSurfaces,_iSurface);
  }
;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 1102 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _ctTriangles = (yyvsp[(6) - (7)].i);
  if(_ctTriangles <0) {
    yyerror("Negative triangle count found %d",_ctTriangles);
  }

  if(_iSurface>=_ctSurfaces) {
    yyerror("Incorect number of surfaces - %d",_ctSurfaces);
  }

  MeshSurface &msrf = pMeshLOD->mlod_aSurfaces[_iSurface];
  msrf.msrf_auwTriangles.New(_ctTriangles);
  msrf.msrf_iSurfaceID = ska_GetIDFromStringTable((yyvsp[(3) - (7)].str));
  msrf.msrf_pShader = NULL;

  _iTriangle = 0;

  INDEX ctSrfSha = _assSurfaceShaders.Count();
  // find this surface in array of shader surfaces that was parsed before mesh
  for(INDEX iSrfSha=0;iSrfSha<ctSrfSha;iSrfSha++)
  {
    // reset shader
    SurfaceShader &ssSurfShader = _assSurfaceShaders[iSrfSha];
    if(ssSurfShader.ss_iSurfaceID == msrf.msrf_iSurfaceID)
    {
      // copy shader texture ID's to mesh
      msrf.msrf_ShadingParams.sp_aiTextureIDs = ssSurfShader.ss_spShaderParams.sp_aiTextureIDs;
      msrf.msrf_ShadingParams.sp_aiTexCoordsIndex = ssSurfShader.ss_spShaderParams.sp_aiTexCoordsIndex;
      msrf.msrf_ShadingParams.sp_acolColors = ssSurfShader.ss_spShaderParams.sp_acolColors;
      msrf.msrf_ShadingParams.sp_afFloats = ssSurfShader.ss_spShaderParams.sp_afFloats;
      msrf.msrf_ShadingParams.sp_ulFlags  = ssSurfShader.ss_spShaderParams.sp_ulFlags;
      // set shader for this surface
      if(ssSurfShader.fnShaderName.Length()>0) {
        try {
          ChangeSurfaceShader_t(msrf,ssSurfShader.fnShaderName);
        } catch(char *strErr) {
          CPrintF(strErr);
          msrf.msrf_pShader=NULL;
        }
      } else {
        msrf.msrf_pShader=NULL;
      }
      break;
    }
  }
;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1149 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _iSurface++;
  if(_iTriangle!=_ctTriangles) {
    yyerror("Incorect number of triangles for surface %d.\nExpecting %d but found only %d",_iSurface,_ctTriangles,_iTriangle);
  }
;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 1164 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iTriangle>=_ctTriangles) {
    yyerror("Incorect number of triangles - %d",_ctTriangles);
  }

  MeshSurface &msrf = pMeshLOD->mlod_aSurfaces[_iSurface];
  MeshTriangle &mt = msrf.msrf_auwTriangles[_iTriangle];
  mt.mt_uwVertexIndex[0]=(yyvsp[(1) - (2)].i3)[0];
  mt.mt_uwVertexIndex[1]=(yyvsp[(1) - (2)].i3)[1];
  mt.mt_uwVertexIndex[2]=(yyvsp[(1) - (2)].i3)[2];
  _iTriangle++;
;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 1181 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _ctWeights = (yyvsp[(2) - (3)].i);
  if(_ctWeights <0) {
    yyerror("Negative weights count found %d",_ctWeights);
  }

  _iWeight = 0;
  pMeshLOD->mlod_aWeightMaps.New(_ctWeights);
;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 1191 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iWeight!=_ctWeights) {
    yyerror("Incorect number of weights.\nExpecting %d but found only %d",_ctWeights,_iWeight);
  }
;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1210 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _ctVertexWeights = (yyvsp[(6) - (7)].i);
  if(_ctVertexWeights <0) {
    yyerror("Negative vertex weights count found %d",_ctVertexWeights);
  }

  if(_iWeight>=_ctWeights) {
    yyerror("Incorect number of weights - %d",_ctWeights);
  }

  MeshWeightMap &mwm = pMeshLOD->mlod_aWeightMaps[_iWeight];

  mwm.mwm_iID = ska_GetIDFromStringTable((yyvsp[(3) - (7)].str));
  mwm.mwm_aVertexWeight.New(_ctVertexWeights);
  _iVertexWeight = 0;
;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1227 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _iWeight++;
  if(_iVertexWeight!=_ctVertexWeights) {
    yyerror("Incorect number of vertex weights.\nExpecting %d but found only %d",_ctVertexWeights,_iVertexWeight);
  }
;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1242 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iVertexWeight>=_ctVertexWeights) {
    yyerror("Incorect number of vertex weights - %d",_ctVertexWeights);
  }

  MeshWeightMap &mwm = pMeshLOD->mlod_aWeightMaps[_iWeight];
  MeshVertexWeight &mvw = mwm.mwm_aVertexWeight[_iVertexWeight];
  
  FLOAT fWeight = (yyvsp[(4) - (6)].f);
  if(fWeight<0) {
    yyerror("Weight value is negative");
  }

  mvw.mww_iVertex = (yyvsp[(2) - (6)].i);
  mvw.mww_fWeight = fWeight;
  _iVertexWeight++;
;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1264 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _ctMorphs = (yyvsp[(2) - (3)].i);

  if(_ctMorphs<0) {
    yyerror("Negative morph count found %d",_ctMorphs);
  }

  pMeshLOD->mlod_aMorphMaps.New((yyvsp[(2) - (3)].i));
  _iMorph = 0;
;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1275 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iMorph!=_ctMorphs) {
    yyerror("Incorect number of morphs.\nExpecting %d but found only %d",_ctMorphs,_iMorph);
  }
;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1293 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _ctVertexMorphs = (yyvsp[(8) - (8)].i);

  if(_ctVertexMorphs <0) {
    yyerror("Negative vertex morphs count found %d",_ctVertexMorphs);
  }

  if(_iMorph>=_ctMorphs) {
    yyerror("Incorect number of morphs - %d",_ctMorphs);
  }

  MeshMorphMap &mmm = pMeshLOD->mlod_aMorphMaps[_iMorph];
  mmm.mmp_aMorphMap.New((yyvsp[(8) - (8)].i));
  mmm.mmp_iID = ska_GetIDFromStringTable((yyvsp[(3) - (8)].str));
  mmm.mmp_bRelative = (yyvsp[(6) - (8)].i);
  _iVertexMorph = 0;
;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1311 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  _iMorph++;
  if(_iVertexMorph!=_ctVertexMorphs) {
    yyerror("Incorect number of vertex morphs.\nExpecting %d but found only %d",_ctVertexMorphs,_iVertexMorph);
  }
;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1327 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  if(_iVertexMorph>=_ctVertexMorphs) {
    yyerror("Incorect number of vertex morphs - %d",_ctVertexMorphs);
  }

  // add mesh vertex morph to morph array
  MeshMorphMap &mmm = pMeshLOD->mlod_aMorphMaps[_iMorph];
  MeshVertexMorph &mwm = mmm.mmp_aMorphMap[_iVertexMorph];
  mwm.mwm_iVxIndex = (yyvsp[(2) - (16)].i);
  mwm.mwm_x = (yyvsp[(4) - (16)].f);
  mwm.mwm_y = (yyvsp[(6) - (16)].f);
  mwm.mwm_z = (yyvsp[(8) - (16)].f);
  mwm.mwm_nx = (yyvsp[(10) - (16)].f);
  mwm.mwm_ny = (yyvsp[(12) - (16)].f);
  mwm.mwm_nz = (yyvsp[(14) - (16)].f);
   _iVertexMorph++;
;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1354 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  (yyval.f) = (yyvsp[(1) - (1)].f);
;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1357 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  (yyval.f) = (float)(yyvsp[(1) - (1)].i);
;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1363 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  (yyval.i) = (yyvsp[(1) - (1)].i);
;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1369 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  (yyval.v2)[0] = (yyvsp[(1) - (3)].f);
  (yyval.v2)[1] = (yyvsp[(3) - (3)].f);
;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1376 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  (yyval.v3)[0] = (yyvsp[(1) - (5)].f);
  (yyval.v3)[1] = (yyvsp[(3) - (5)].f);
  (yyval.v3)[2] = (yyvsp[(5) - (5)].f);
;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1384 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  (yyval.v3)[0] = (yyvsp[(1) - (5)].f);
  (yyval.v3)[1] = (yyvsp[(3) - (5)].f);
  (yyval.v3)[2] = (yyvsp[(5) - (5)].f);
;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1392 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  (yyval.i3)[0] = (yyvsp[(1) - (5)].i);
  (yyval.i3)[1] = (yyvsp[(3) - (5)].i);
  (yyval.i3)[2] = (yyvsp[(5) - (5)].i);
;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1400 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  (yyval.f12)[0] = (yyvsp[(1) - (23)].f);
  (yyval.f12)[1] = (yyvsp[(3) - (23)].f);
  (yyval.f12)[2] = (yyvsp[(5) - (23)].f);
  (yyval.f12)[3] = (yyvsp[(7) - (23)].f);
  (yyval.f12)[4] = (yyvsp[(9) - (23)].f);
  (yyval.f12)[5] = (yyvsp[(11) - (23)].f);
  (yyval.f12)[6] = (yyvsp[(13) - (23)].f);
  (yyval.f12)[7] = (yyvsp[(15) - (23)].f);
  (yyval.f12)[8] = (yyvsp[(17) - (23)].f);
  (yyval.f12)[9] = (yyvsp[(19) - (23)].f);
  (yyval.f12)[10] = (yyvsp[(21) - (23)].f);
  (yyval.f12)[11] = (yyvsp[(23) - (23)].f);
;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1418 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  (yyval.i) = TRUE;
;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1422 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"
    {
  (yyval.i) = FALSE;
;}
    break;



/* Line 1455 of yacc.c  */
#line 3173 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



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
		      yytoken, &yylval);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1426 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"


