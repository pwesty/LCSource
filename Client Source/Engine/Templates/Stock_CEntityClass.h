#ifndef SE_INCL_STOCK_CENTITYCLASS_H
#define SE_INCL_STOCK_CENTITYCLASS_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Entities/EntityClass.h>

#define TYPE CEntityClass
#define CStock_TYPE CStock_CEntityClass
#define CNameTable_TYPE CNameTable_CEntityClass
#define CNameTableSlot_TYPE CNameTableSlot_CEntityClass

#include <Engine/Templates/NameTable.h>
#include <Engine/Templates/Stock.h>

#undef CStock_TYPE
#undef CNameTableSlot_TYPE
#undef CNameTable_TYPE
#undef TYPE

ENGINE_API extern CStock_CEntityClass *_pEntityClassStock;

#define ENTITY_STOCK_RELEASE(p) { if(p){ _pEntityClassStock->Release(p); p = NULL; } }

#endif  /* include-once check. */

