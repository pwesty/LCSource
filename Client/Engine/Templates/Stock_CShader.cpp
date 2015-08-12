#include "stdh.h"

#include <Engine/Templates/Stock_CShader.h>

#define TYPE CShader
#define CStock_TYPE CStock_CShader
#define DESCRIPTION "CStock_CShader"
#define CNameTable_TYPE CNameTable_CShader
#define CNameTableSlot_TYPE CNameTableSlot_CShader

#include <Engine/Templates/NameTable.cpp>
#include <Engine/Templates/Stock.cpp>

#undef CStock_TYPE
#undef DESCRIPTION
#undef CNameTableSlot_TYPE
#undef CNameTable_TYPE
#undef TYPE

CStock_CShader *_pShaderStock = NULL;
