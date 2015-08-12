#include "stdh.h"

#include <Engine/Base/CTWString.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/Memory.h>

// string statistics
static INDEX _ctStringsW = 0;
static INDEX _ctStringBytesW = 0;
static INDEX _ctEmptyStringsW = 0;

static UWORD _pEmptyStringW[4] = {
  0, 0x1234, 0x5678, 0x9012,
};
#define STRING_CHECKMEMORYW()   \
  ASSERT(                       \
  _pEmptyStringW[0] == 0 &&      \
  _pEmptyStringW[1] == 0x1234 && \
  _pEmptyStringW[2] == 0x5678 && \
  _pEmptyStringW[3] == 0x9012 );
// && _CrtCheckMemory());

static UWORD *StringAllocW(INDEX ctLen)
{
  STRING_CHECKMEMORYW();
  ASSERT(ctLen>=0);
  // if the string is empty
  if(ctLen==0) {
    _ctEmptyStringsW++;
    // just return the global empty string
    return _pEmptyStringW;
  }

#if MEMORY_TRACKING
  INDEX imdOld = GetMemoryDescription();
  if (imdOld==-1) {
    SetMemoryDescription("Misc wide strings");
  }
#endif
  // allocate that much memory
  UWORD *str = (UWORD*)AllocMemory((ctLen+1)*2);
  _ctStringsW++;
  _ctStringBytesW += (ctLen+1)*2;
#if MEMORY_TRACKING
  SetMemoryDescription(imdOld);
#endif
  return str;
}

static void StringFreeW(UWORD *strwOld)
{
  STRING_CHECKMEMORYW();
  if(strwOld==_pEmptyStringW) {
    // never free the global empty string
    _ctEmptyStringsW--;
    return;
  }

  _ctStringsW--;
  _ctStringBytesW -= MEM_Size(strwOld);
  FreeMemory(strwOld);
}

static void StringResizeW(UWORD **pstrwOriginal, INDEX ctOldLen, INDEX ctNewLen)
{
  STRING_CHECKMEMORYW();
  ASSERT(ctNewLen>=0);

  if(*pstrwOriginal==_pEmptyStringW) {
    ASSERT(ctOldLen==0);
    // never free the global empty string
    _ctEmptyStringsW--;
    *pstrwOriginal = StringAllocW(ctNewLen);
    **pstrwOriginal = 0;
    STRING_CHECKMEMORYW();
    return;
  }

  _ctStringBytesW -= MEM_Size(*pstrwOriginal);
  ResizeMemory((void**)pstrwOriginal, (ctNewLen+1)*2);
  _ctStringBytesW += (ctNewLen+1)*2;
  STRING_CHECKMEMORYW();
}
/*
 * Allocate a copy of a string. - fatal error if not enough memory.
 */
static UWORD *StringDuplicateW(const UWORD *strwOriginal)
{
  STRING_CHECKMEMORYW();
  // get the len
  INDEX ctLen = wcslen((const wchar_t *)strwOriginal);
  // allocate that much memory
  UWORD *strwCopy = StringAllocW(ctLen);
  // copy it there
  memcpy(strwCopy, strwOriginal, (ctLen+1)*2);
  STRING_CHECKMEMORYW();
  // result is the pointer to the copied string
  return strwCopy;
}

static UWORD *StringDuplicateW(const char *strOriginal)
{
  STRING_CHECKMEMORYW();
  // get the len
  INDEX ctLen = strlen(strOriginal);
  // allocate that much memory
  UWORD *strwCopy = StringAllocW(ctLen);
  // copy it there
  for(INDEX ich=0;ich<ctLen;ich++) {
    strwCopy[ich] = (unsigned char)strOriginal[ich];
    // do remappings for special unicode chars (USE TABLE FOR THIS!!!!)
    if (strwCopy[ich]==0x009F) {
      strwCopy[ich] = 0x0178;
    } else if (strwCopy[ich]==0x0080) {
      strwCopy[ich] = 0x20AC;
    }
  }
  strwCopy[ctLen] = 0;

  STRING_CHECKMEMORYW();
  // result is the pointer to the copied string
  return strwCopy;
}

/* Default constructor. */
CTWString::CTWString(void)
{
  strw_String = StringDuplicateW("");
}

/* Copy constructor. */
CTWString::CTWString(const CTWString &strwOriginal)
{
  ASSERT(strwOriginal.IsValid());
  // make string duplicate
  strw_String = StringDuplicateW(strwOriginal.strw_String);
}

/* Constructor from wide character string. */
CTWString::CTWString(const UWORD *strwCharString)
{
  ASSERT(strwCharString!=NULL);

  // make string duplicate
  strw_String = StringDuplicateW(strwCharString);
}

/* Constructor from character string. */
CTWString::CTWString(const char *strCharString)
{
  ASSERT(strCharString!=NULL);
  // make string duplicate
  strw_String = StringDuplicateW(strCharString);
}

/* Destructor. */
CTWString::~CTWString()
{
  // check that it is valid
  ASSERT(IsValid());
  // free memory
  StringFreeW(strw_String);
}

/* Clear the object. */
void CTWString::Clear(void)
{
  operator=("");
}

/* Assignment. */
CTWString &CTWString::operator=(const CTWString &strwOther)
{
  ASSERT(IsValid());
  ASSERT(strwOther.IsValid());

  /* The other string must be copied _before_ this memory is freed, since it could be same
     pointer!
   */
  // make a copy of character string
  UWORD *strwCopy = StringDuplicateW(strwOther.strw_String);
  // empty this string
  StringFreeW(strw_String);
  // assign it the copy of the character string
  strw_String = strwCopy;

  return *this;
}
CTWString &CTWString::operator=(const UWORD *strwCharString)
{
  ASSERT(IsValid());
  ASSERT(strwCharString!=NULL);

  /* The other string must be copied _before_ this memory is freed, since it could be same
     pointer!
   */
  // make a copy of character string
  UWORD *strwCopy = StringDuplicateW(strwCharString);
  // empty this string
  StringFreeW(strw_String);
  // assign it the copy of the character string
  strw_String = strwCopy;

  return *this;
}
CTWString &CTWString::operator=(const char *strCharString)
{
  ASSERT(IsValid());
  ASSERT(strCharString!=NULL);

  /* The other string must be copied _before_ this memory is freed, since it could be same
     pointer!
   */
  // make a copy of character string
  UWORD *strwCopy = StringDuplicateW(strCharString);
  // empty this string
  StringFreeW(strw_String);
  // assign it the copy of the character string
  strw_String = strwCopy;

  return *this;
}



/* Equality comparison. */
BOOL CTWString::operator==(const CTWString &strwOther) const
{
  ASSERT(IsValid() && strwOther.IsValid());
  return wcsicmp( (const wchar_t *)strw_String, (const wchar_t *)strwOther.strw_String) == 0;
}
BOOL CTWString::operator==(const UWORD *strwOther) const
{
  ASSERT(IsValid() && strwOther!=NULL);
  return wcsicmp( (const wchar_t *)strw_String, (const wchar_t *)strwOther) == 0;
}

/*
 * Inequality comparison.
 */
BOOL CTWString::operator!=(const CTWString &strwOther) const
{
  ASSERT(IsValid() && strwOther.IsValid());
  return !( *this == strwOther );
}
BOOL CTWString::operator!=(const UWORD *strwOther) const
{
  ASSERT(IsValid() && strwOther!=NULL);
  return !( *this == strwOther );
}


/*
 * String concatenation.
 */
CTWString CTWString::operator+(const CTWString &strwSecond) const
{
  ASSERT(IsValid() && strwSecond.IsValid());
  return(CTWString(*this)+=strwSecond);
}
CTWString operator+(const UWORD *strwFirst, const CTWString &strwSecond)
{
  ASSERT(strwFirst!=NULL && strwSecond.IsValid());
  return(CTWString(strwFirst)+=strwSecond);
}
CTWString &CTWString::operator+=(const CTWString &strwSecond)
{
  ASSERT(IsValid() && strwSecond.IsValid());

  INDEX ctLen = wcslen((const wchar_t *)strw_String);
#pragma message(">> >> >> ???")
  /* ocarina */
  // StringResizeW( &strw_String,  ctLen, ctLen + wcslen((const wchar_t *)strwSecond) );
  StringResizeW( &strw_String,  ctLen, ctLen + strwSecond.Length() );
  wcscat((wchar_t *)strw_String, (const wchar_t *)strwSecond.strw_String);
  STRING_CHECKMEMORYW();
  return *this;
}

#ifndef NDEBUG
/* Check if string data is valid.*/
BOOL CTWString::IsValid(void) const
{
  ASSERT(this!=NULL && strw_String!=NULL);
  return TRUE;
}
#endif // NDEBUG


