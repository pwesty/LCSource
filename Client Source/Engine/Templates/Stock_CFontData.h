#ifndef SE_INCL_STOCK_CFONTDATA_H
#define SE_INCL_STOCK_CFONTDATA_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Graphics/Font.h>

#define TYPE CFontData
#define CStock_TYPE CStock_CFontData
#define CNameTable_TYPE CNameTable_CFontData
#define CNameTableSlot_TYPE CNameTableSlot_CFontData

#include <Engine/Templates/NameTable.h>
#include <Engine/Templates/Stock.h>

#undef CStock_TYPE
#undef CNameTableSlot_TYPE
#undef CNameTable_TYPE
#undef TYPE

ENGINE_API extern CStock_CFontData *_pFontStock;

#define FONT_STOCK_RELEASE(p) { if(p){ _pFontStock->Release(p); p = NULL; } }


#endif  /* include-once check. */

