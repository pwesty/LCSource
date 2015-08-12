#ifndef SE_INCL_CTSTRING_H
#define SE_INCL_CTSTRING_H
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Base/Types.h>
#ifdef KALYDO
#include <Engine/Base/TLVar.h>
#endif

/*
 * Main string class
 */
class ENGINE_API CTString {
public:
	char *str_String;         // pointer to memory holding the character string
public:
	/* Default constructor. */
	inline CTString(void);
	/* Copy constructor. */
	inline CTString(const CTString &strOriginal);
	/* Constructor from character string. */
	inline CTString(const char *strCharString);
	/* Constructor from wide character string. */
	inline CTString(const UWORD *strwCharString);
	/* Constructor with formatting. */
	inline CTString(INDEX iDummy, const char *strFormat, ...);
	/* Destructor. */
	inline ~CTString();
	/* Clear the object. */
	inline void Clear(void);

	/* Conversion into character string. */
	inline operator const char*() const;

	/* Assignment. */
	inline CTString &operator=(const char *strCharString);
	inline CTString &operator=(const UWORD *strwCharString);
	inline CTString &operator=(const CTString &strOther);

	/* Check if string data is valid. */
	BOOL IsValid(void) const;
	BOOL IsEmpty(void) const;

	// return length of the string
	inline INDEX Length(void) const 
	{
		if( str_String == NULL )
			return 0;
		else
			return strlen(str_String); 
	};

	INDEX LengthNaked(void) const;

	// strip decorations from the string
	CTString Undecorated(void) const;

	/* Find index of a substring in a string (returns -1 if not found). */
	INDEX FindSubstr(const CTString &strSub);
	/* Replace a substring in a string. */
	BOOL ReplaceSubstr(const CTString &strSub, const CTString &strNewSub);
	/* Check if has given prefix */
	BOOL HasPrefix( const CTString &strPrefix) const;
	/* Remove given prefix string from this string */
	BOOL RemovePrefix( const CTString &strPrefix);
	/* Trim the string to contain at most given number of characters. */
	INDEX TrimLeft(  INDEX ctCharacters);
	INDEX TrimRight( INDEX ctCharacters);
	
	INDEX TrimLeftChar( char cCharacter );
	INDEX TrimRightChar( char cCharacter );
	/* Trim the string from spaces. */
	INDEX TrimSpacesLeft(void);
	INDEX TrimSpacesRight(void);
	/* Calcuate hashing value for the string. */
	ULONG GetHash(void) const;
	// retain only first line of the string
	void OnlyFirstLine(void);
	// convert all letter characters in string to uppercase
	void ToUpper(void);
	// convert all letter characters in string to lowercase
	void ToLower(void);

	/* Equality comparison. */
	BOOL operator==(const CTString &strOther) const;
	BOOL operator==(const char *strOther) const;
	ENGINE_API friend BOOL operator==(const char *strThis, const CTString &strOther);
	/* Inequality comparison. */
	BOOL operator!=(const CTString &strOther) const;
	BOOL operator!=(const char *strOther) const;
	ENGINE_API friend BOOL operator!=(const char *strThis, const CTString &strOther);
	// wild card comparison (other string may contain wildcards)
	BOOL Matches(const CTString &strOther) const;
	BOOL Matches(const char *strOther) const;
//안태훈 수정 시작	//(Fix Bug)(0.1)
	BOOL IsEqualCaseSensitive(const CTString &strOther) const;
	BOOL IsEqualCaseSensitive(const char *strOther) const;
	ENGINE_API friend BOOL IsEqualCaseSensitive(const char *strThis, const CTString &strOther);
//안태훈 수정 끝	//(Fix Bug)(0.1)

	/* String concatenation. */
	CTString operator+(const CTString &strSecond) const;
	CTString &operator+=(const CTString &strSecond);
	ENGINE_API friend CTString operator+(const char *strFirst, const CTString &strSecond);

	// split string in two strings at specified position (char AT splitting position goes to str2)
	void Split( INDEX iPos, CTString &str1, CTString &str2);
	void InsertChar( INDEX iPos, char cChr); // insert char at position
	void DeleteChar( INDEX iPos); // delete char at position
	void InsertSubString( INDEX iPos, const CTString &strToInsert);
	void DeleteChars( INDEX iPos, INDEX ctChars);

	// is this string of Integer
	BOOL IsInteger(void);

	/* Throw exception */
	void Throw_t(void);

	/* Read from stream. */
	ENGINE_API friend CTStream &operator>>(CTStream &strmStream, CTString &strString);
	void ReadFromText_t(CTStream &strmStream, const CTString &strKeyword); // throw char *
	/* Write to stream. */
	ENGINE_API friend CTStream &operator<<(CTStream &strmStream, const CTString &strString);

	/* Load an entire text file into a string. */
	void Load_t(const class CTFileName &fnmFile);  // throw char *
	void LoadKeepCRLF_t(const class CTFileName &fnmFile);  // throw char *
	void ReadUntilEOF_t(CTStream &strmStream);  // throw char *
	/* Save an entire string into a text file. */
	void Save_t(const class CTFileName &fnmFile);  // throw char *
	void SaveKeepCRLF_t(const class CTFileName &fnmFile);  // throw char *

	// Print formatted to a string
	INDEX PrintF(const char *strFormat, ...);
	INDEX VPrintF(const char *strFormat, va_list arg);
	// Scan formatted from a string
	INDEX ScanF(const char *strFormat, ...);

	// variable management functions
	void LoadVar(const CTFileName &fnmFile);
	void SaveVar(const CTFileName &fnmFile);

	// read/write to stream with uword length
	void WriteToStreamShort(CTStream* ostr);
	void ReadFromStreamShort(CTStream* istr);
};


// general variable functions
ENGINE_API void LoadStringVar( const CTFileName &fnmVar, CTString &strVar);
ENGINE_API void SaveStringVar( const CTFileName &fnmVar, CTString &strVar);
ENGINE_API void LoadIntVar(    const CTFileName &fnmVar, INDEX &iVar);
ENGINE_API void SaveIntVar(    const CTFileName &fnmVar, INDEX &iVar);

ENGINE_API CTString RemoveSpecialCodes( const CTString &str);

// allocate a copy of a string. - fatal error if not enough memory
// NOTE: String alloc functions never allocate empty strings! Only one empty string is shared.
ENGINE_API extern char *StringDuplicate(const char *strOriginal);
// allocate a copy of a wide string. - fatal error if not enough memory or if wide chars can't be duplicated.
ENGINE_API extern char *StringDuplicate(const UWORD *strwOriginal);
// free a string
ENGINE_API extern void StringFree(char *strOld);

// Clears temp memory used by vprintf
extern void FreeVPrintF(void);

// hide ANSI C string allocation
#define strdup ERROR_AlwaysUse_StringDuplicate_InsteadOf_strdup!!!

#include <Engine/Base/CTString.inl>


#endif  /* include-once check. */

