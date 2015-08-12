#ifndef SE_INCL_CTWSTRING_H
#define SE_INCL_CTWSTRING_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/Types.h>

/*
 * Main string class
 */
class ENGINE_API CTWString {
public:
  /* Default constructor. */
  CTWString(void);
  /* Copy constructor. */
  CTWString(const CTWString &strwOriginal);
  /* Constructor from wide character string. */
  CTWString(const UWORD *strwCharString);
  /* Constructor from character string. */
  CTWString(const char *strCharString);
  /* Destructor. */
  ~CTWString();
  /* Clear the object. */
  void Clear(void);

  /* Conversion into character string. */
  inline operator const UWORD*() const;

  /* Assignment. */
  CTWString &operator=(const CTWString &strwOther);
  CTWString &operator=(const UWORD *strwCharString);
  CTWString &operator=(const char *strCharString);

  /* Check if string data is valid. */
  BOOL IsValid(void) const;

  // return length of the string
  INDEX Length(void) const { return wcslen((const wchar_t *)strw_String); };

  /* Equality comparison. */
  BOOL operator==(const CTWString &strOther) const;
  BOOL operator==(const UWORD *strOther) const;
  /* Inequality comparison. */
  BOOL operator!=(const CTWString &strOther) const;
  BOOL operator!=(const UWORD *strOther) const;
  /* String concatenation. */
  CTWString operator+(const CTWString &strSecond) const;
  CTWString &operator+=(const CTWString &strSecond);
public:
  UWORD *strw_String;       // pointer to memory holding the wide character string
};

inline CTWString::operator const UWORD*() const {
  ASSERT(IsValid());
  return strw_String;
}

#endif  /* include-once check. */
