#ifndef SE_INCL_STACKDUMP_H
#define SE_INCL_STACKDUMP_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

// Initialize exception handler
ENGINE_API extern void InitExceptionHandler(const char* FileName);
// Close exception handler
ENGINE_API extern void CloseExceptionHandler(void);
// Convert absolute address from logical address 
ENGINE_API extern BOOL GetLogicalAddress(void *pAddr, char *szModule, ULONG ulLen,
                                         ULONG &ulSection, ULONG &ulOffset);

ENGINE_API extern void DumpCurrentStack(CTFileStream *pstrm=NULL);

#endif // SE_INCL_STACKDUMP_H
