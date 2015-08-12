#include "stdh.h"

#include <Engine/Templates/Stock_CAnimSet.h>

#define TYPE CAnimSet
#define CStock_TYPE CStock_CAnimSet
#define DESCRIPTION "CStock_CAnimSet"
#define CNameTable_TYPE CNameTable_CAnimSet
#define CNameTableSlot_TYPE CNameTableSlot_CAnimSet

#include <Engine/Templates/NameTable.cpp>
#include <Engine/Templates/Stock.cpp>

#undef CStock_TYPE
#undef DESCRIPTION
#undef CNameTableSlot_TYPE
#undef CNameTable_TYPE
#undef TYPE

CStock_CAnimSet *_pAnimSetStock = NULL;
