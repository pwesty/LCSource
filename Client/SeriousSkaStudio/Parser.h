
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
#line 82 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.y"

  INDEX i;
  float f;
  const char *str;
  float v2[2];
  float v3[3];
  int   i3[3];
  float f12[12];



/* Line 1676 of yacc.c  */
#line 117 "C:\\_work\\_Ranma_work\\SeriousSkaStudio\\Parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


