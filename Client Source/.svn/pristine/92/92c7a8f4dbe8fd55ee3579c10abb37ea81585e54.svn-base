#ifndef SE_INCL_SYMBOLLOCATOR_H
#define SE_INCL_SYMBOLLOCATOR_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/CTString.h>

class ENGINE_API CSymbolLocator {
public:
  CTString sl_strName;
  void *sl_pvSymbol;

  CSymbolLocator(const CTString &strName, void *pvSymbol);
};

ENGINE_API void *LocateSymbol(const CTString &strName);

#endif  /* include-once check. */
