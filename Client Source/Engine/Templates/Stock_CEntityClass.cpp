#include "stdh.h"

#include <Engine/Templates/Stock_CEntityClass.h>

#define TYPE CEntityClass
#define CStock_TYPE CStock_CEntityClass
#define DESCRIPTION "CStock_CEntityClass"
#define CNameTable_TYPE CNameTable_CEntityClass
#define CNameTableSlot_TYPE CNameTableSlot_CEntityClass

#include <Engine/Templates/NameTable.cpp>
#include <Engine/Templates/Stock.cpp>

#undef CStock_TYPE
#undef DESCRIPTION
#undef CNameTableSlot_TYPE
#undef CNameTable_TYPE
#undef TYPE

CStock_CEntityClass *_pEntityClassStock = NULL;
