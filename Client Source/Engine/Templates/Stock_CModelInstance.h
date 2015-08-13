#ifndef SE_INCL_STOCK_CMODELINSTANCE_H
#define SE_INCL_STOCK_CMODELINSTANCE_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Ska/ModelInstance.h>

#define TYPE CModelInstanceSerial
#define CStock_TYPE CStock_CModelInstance
#define CNameTable_TYPE CNameTable_CModelInstanceSerial
#define CNameTableSlot_TYPE CNameTableSlot_CModelInstanceSerial

#include <Engine/Templates/NameTable.h>
#include <Engine/Templates/Stock.h>

#undef CStock_TYPE
#undef CNameTableSlot_TYPE
#undef CNameTable_TYPE
#undef TYPE

ENGINE_API extern CStock_CModelInstance *_pModelInstanceStock;


#endif  /* include-once check. */

