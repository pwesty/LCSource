#include "stdh.h"

#include <Engine/Base/CTString.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/Memory.h>
#include <Engine/Base/Stream.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/TLVar.h>


// string statistics
INDEX _ctStrings = 0;
INDEX _ctStringBytes = 0;
INDEX _ctStringReqBytes = 0;
INDEX _ctEmptyStrings = 0;

static char _pEmptyString[5] = {
	0, 0x12, 0x34, 0x56, 0x78,
};
#define STRING_CHECKMEMORY() \
	ASSERT(                     \
	_pEmptyString[0] == 0 &&    \
	_pEmptyString[1] == 0x12 && \
	_pEmptyString[2] == 0x34 && \
	_pEmptyString[3] == 0x56 && \
	_pEmptyString[4] == 0x78 );
// && _CrtCheckMemory());

char *StringAlloc(INDEX ctLen)
{
	STRING_CHECKMEMORY();
	ASSERT(ctLen>=0);
	// if the string is empty
	if (ctLen==0) {
		_ctEmptyStrings++;
		// just return the global empty string
		return _pEmptyString;
	}

	MEMTRACK_SETFLAGS(mem,MTF_STRING);
#if MEMORY_TRACKING
	INDEX imdOld = GetMemoryDescription();
	if (imdOld==-1) {
		SetMemoryDescription("Misc strings");
	}
#endif
	// allocate that much memory
	char *str = (char *)AllocMemory(ctLen+1);
	_ctStrings++;
	_ctStringBytes += MEM_Size(str);

#if MEMORY_TRACKING
	_ctStringReqBytes += TrackMemoryReqSize(str);
#endif

#if MEMORY_TRACKING
	SetMemoryDescription(imdOld);
#endif
	return str;
}
void StringFree(char *strOld)
{
	STRING_CHECKMEMORY();
	if (strOld==_pEmptyString) {
		// never free the global empty string
		_ctEmptyStrings--;
		return;
	}

	_ctStrings--;
	_ctStringBytes -= MEM_Size(strOld);
#if MEMORY_TRACKING
	_ctStringReqBytes -= TrackMemoryReqSize(strOld);
#endif
	FreeMemory(strOld);
}

void StringResize(char **pstrOriginal, INDEX ctOldLen, INDEX ctNewLen)
{
	STRING_CHECKMEMORY();
	ASSERT(ctNewLen>=0);

	if (*pstrOriginal==_pEmptyString) {
		ASSERT(ctOldLen==0);
		// never free the global empty string
		_ctEmptyStrings--;
		*pstrOriginal = StringAlloc(ctNewLen);
		**pstrOriginal = 0;
		STRING_CHECKMEMORY();
		return;
	}

	_ctStringBytes -= MEM_Size(*pstrOriginal);
#if MEMORY_TRACKING
	_ctStringReqBytes += TrackMemoryReqSize(*pstrOriginal);
#endif
	ResizeMemory((void**)pstrOriginal, ctNewLen+1);
	_ctStringBytes += MEM_Size(*pstrOriginal);
#if MEMORY_TRACKING
	_ctStringReqBytes -= TrackMemoryReqSize(*pstrOriginal);
#endif
	STRING_CHECKMEMORY();
}
/*
 * Allocate a copy of a string. - fatal error if not enough memory.
 */
char *StringDuplicate(const char *strOriginal)
{
	STRING_CHECKMEMORY();
	// get the len
	INDEX ctLen = strlen(strOriginal);
	// allocate that much memory
	char *strCopy = StringAlloc(ctLen);
	// copy it there
	if(strCopy != strOriginal) memcpy(strCopy, strOriginal, ctLen);
	strCopy[ctLen] = '\0';
	STRING_CHECKMEMORY();
	// result is the pointer to the copied string
	return strCopy;
}

/*
 * Allocate a copy of a wide string. - fatal error if not enough memory or if wide chars can't be duplicated.
 */
char *StringDuplicate(const UWORD *strwOriginal)
{
	STRING_CHECKMEMORY();
	// get the len
	INDEX ctLen = wcslen((const wchar_t *)strwOriginal);
	// allocate that much memory
	char *strCopy = StringAlloc(ctLen);
	// copy it there
	for(INDEX ich=0;ich<ctLen;ich++) {
		// do remappings for special unicode chars (USE TABLE FOR THIS!!!!)
		if (strwOriginal[ich]==0x0178) {
			strCopy[ich] = (UBYTE)0x9F;
		} else if (strwOriginal[ich]==0x20AC) {
			strCopy[ich] = (UBYTE)0x80;
		} else if((strwOriginal[ich]&0xFF00)!=0) {
			strCopy[ich] = 31;
		} else {
			strCopy[ich] = (char)strwOriginal[ich]&0x00FF;
		}
	}
	strCopy[ctLen] = 0;
	STRING_CHECKMEMORY();
	// result is the pointer to the copied string
	return strCopy;
}

/*
 * Equality comparison.
 */
BOOL CTString::operator==(const CTString &strOther) const
{
	ASSERT(IsValid() && strOther.IsValid());

	return stricmp( str_String, strOther.str_String) == 0;
}
BOOL CTString::operator==(const char *strOther) const
{
	ASSERT(IsValid() && strOther!=NULL);

	return stricmp( str_String, strOther) == 0;
}
 BOOL operator==(const char *strThis, const CTString &strOther)
{
	ASSERT(strOther.IsValid() && strThis!=NULL);

	return strOther == strThis;
}

/*
 * Inequality comparison.
 */
BOOL CTString::operator!=(const CTString &strOther) const
{
	ASSERT(IsValid() && strOther.IsValid());

	return !( *this == strOther );
}
BOOL CTString::operator!=(const char *strOther) const
{
	ASSERT(IsValid() && strOther!=NULL);

	return !( *this == strOther );
}
 BOOL operator!=(const char *strThis, const CTString &strOther)
{
	ASSERT(strOther.IsValid() && strThis!=NULL);

	return !( strOther == strThis);
}


//안태훈 수정 시작	//(Fix Bug)(0.1)
BOOL CTString::IsEqualCaseSensitive(const CTString &strOther) const
{
	ASSERT(IsValid() && strOther.IsValid());

	return strcmp(str_String, strOther.str_String) == 0;
}

BOOL CTString::IsEqualCaseSensitive(const char *strOther) const
{
	ASSERT(IsValid() && strOther!=NULL);

	return strcmp(str_String, strOther) == 0;
}

ENGINE_API BOOL IsEqualCaseSensitive(const char *strThis, const CTString &strOther)
{
	ASSERT(strOther.IsValid() && strThis!=NULL);

	return strcmp(strThis, strOther.str_String) == 0;
}
//안태훈 수정 끝	//(Fix Bug)(0.1)

/*
 * String concatenation.
 */
CTString CTString::operator+(const CTString &strSecond) const
{
	ASSERT(IsValid() && strSecond.IsValid());

	return(CTString(*this)+=strSecond);
}
CTString operator+(const char *strFirst, const CTString &strSecond)
{
	ASSERT(strFirst!=NULL && strSecond.IsValid());

	return(CTString(strFirst)+=strSecond);
}
CTString &CTString::operator+=(const CTString &strSecond)
{
	ASSERT(IsValid() && strSecond.IsValid());

	INDEX ctLen = strlen( str_String);
	StringResize( &str_String,  ctLen, ctLen + strlen( strSecond) );
	strcat(str_String, strSecond.str_String);
	STRING_CHECKMEMORY();
	return *this;
}

/*
 * Remove given prefix string from this string
 */
BOOL CTString::RemovePrefix( const CTString &strPrefix)
{
	INDEX lenOld = strlen( str_String);
	INDEX lenPrefix = strlen( strPrefix);
	INDEX lenDest = lenOld - lenPrefix;

	if( strnicmp( str_String, strPrefix, lenPrefix) != 0)
		return FALSE;
	CTString strTemp = CTString( &str_String[ lenPrefix]);
	StringResize( &str_String, lenOld, lenDest);
	strcpy( str_String, strTemp.str_String);
	STRING_CHECKMEMORY();
	return TRUE;
}
/* Check if has given prefix */
BOOL CTString::HasPrefix( const CTString &strPrefix) const
{
	INDEX lenPrefix = strlen( strPrefix);
	if( strnicmp( str_String, strPrefix, lenPrefix) != 0)
		return FALSE;
	return TRUE;
}

/* Find index of a substring in a string (returns -1 if not found). */
INDEX CTString::FindSubstr(const CTString &strSub)
{
	INDEX ct = Length();
	INDEX ctSub = strSub.Length();
	for (INDEX i=0; i<ct-ctSub+1; i++) {
		for (INDEX iSub=0; iSub<ctSub; iSub++) {
			if ((*this)[i+iSub]!=strSub[iSub]) {
				goto wrong;
			}
		}
		return i;
wrong:;
	}
	return -1;
}

/* Replace a substring in a string. */
BOOL CTString::ReplaceSubstr(const CTString &strSub, const CTString &strNewSub)
{
	INDEX iPos = FindSubstr(strSub);
	if (iPos<0) {
		return FALSE;
	}

	CTString strPart1, strPart2;
	Split(iPos, strPart1, strPart2);
	strPart2.RemovePrefix(strSub);

	*this = strPart1+strNewSub+strPart2;

	STRING_CHECKMEMORY();
	return TRUE;
}

/* Trim the string from left to contain at most given number of characters. */
INDEX CTString::TrimLeft( INDEX ctCharacters)
{
	// clamp negative values
	if( ctCharacters<0) ctCharacters = 0;
	// find how much characters to remove
	INDEX lenOriginal = strlen(str_String);
	INDEX lenPrefix = lenOriginal-ctCharacters;
	// if nothing needs to be removed
	if( lenPrefix<=0) return 0;
	// crop
	memmove( str_String, &str_String[ lenPrefix], ctCharacters+1);
	StringResize( &str_String, lenOriginal, ctCharacters);
	STRING_CHECKMEMORY();
	return lenPrefix;
}


/* Trim the string from right to contain at most given number of characters. */
INDEX CTString::TrimRight( INDEX ctCharacters)
{
	// clamp negative values
	if( ctCharacters<0) ctCharacters = 0;
	// find how much characters to remove
	INDEX lenOriginal = strlen(str_String);
	INDEX lenPrefix = lenOriginal-ctCharacters;
	// if nothing needs to be removed
	if( lenPrefix<=0) return 0;
	// crop
	str_String[ctCharacters] = '\0';
	StringResize( &str_String, lenOriginal, ctCharacters);
	STRING_CHECKMEMORY();
	return lenPrefix;
}


/* Trim the string from left to contain at most given number of characters. */
//Not Test ...
INDEX CTString::TrimLeftChar( char cCharacter )
{
	// clamp negative values
	if( cCharacter == ' ' ) return 0;
			
	for( int i = 0; i < this->Length(); i++ )
	{
		if( str_String[i] == '(' )
		{
			return this->TrimLeft( i );
		}
	}
	return 0;
}


/* Trim the string from right to contain at most given number of characters. */
INDEX CTString::TrimRightChar( char cCharacter )
{
	// clamp negative values
	if( cCharacter == ' ' ) return 0;
			
	for( int i = 0; i < this->Length(); i++ )
	{
		if( str_String[i] == '(' )
		{
			return this->TrimRight( i );
		}
	}

	return 0;
}


// return naked length of the string (ignoring all decorate codes)
INDEX CTString::LengthNaked(void) const
{
	return Undecorated().Length();
}

// strip decorations from the string
CTString CTString::Undecorated(void) const
{
	// make a copy of the string to hold the result - we will rewrite it without the codes
	CTString strResult = *this;

	// start at the beginning of both strings
	const char *pchSrc = str_String;
	char *pchDst = strResult.str_String;

	// while the source is not finished
	while(pchSrc[0]!=0) {
		// if the source char is not escape char
		if (pchSrc[0]!='^') {
			// copy it over
			*pchDst++ = *pchSrc++;
			// go to next char
			continue;
		}
		// check the next char
		switch(pchSrc[1]) {
			// if one of the control codes, skip corresponding number of characters
			case 'c':  pchSrc += 2+FindByte( 0, (UBYTE*)pchSrc+2, 6);  break;
			case 'a':  pchSrc += 2+FindByte( 0, (UBYTE*)pchSrc+2, 2);  break;
			case 'f':  pchSrc += 2+FindByte( 0, (UBYTE*)pchSrc+2, 2);  break;
			case 'b':  case 'i':  case 'r':  case 'o':
			case 'C':  case 'A':  case 'F':  case 'B':  case 'I':  pchSrc+=2;  break;
			// if it is the escape char again, skip the first escape and copy the char
			case '^':  pchSrc++; *pchDst++ = *pchSrc++; break;
			// if it is something else
			default:
				// just copy over the control char
				*pchDst++ = *pchSrc++;
				break;
		}
	}
	*pchDst++ = 0;
	ASSERT(strResult.Length()<=Length());
	return strResult;
}

BOOL IsSpace(char c)
{
	return c==' ' || c=='\t' || c=='\n' || c=='\r';
}

/* Trim the string from from spaces from left. */
INDEX CTString::TrimSpacesLeft(void)
{
	// for each character in string
	const char *chr;
	for(chr = str_String; *chr!=0; chr++) {
		// if the character is not space 
		if (!IsSpace(*chr)) {
			// stop searching
			break;
		}
	}
	// trim to that character
	return TrimLeft(str_String+strlen(str_String) - chr);
}

/* Trim the string from from spaces from right. */
INDEX CTString::TrimSpacesRight(void)
{
	// for each character in string reversed
	const char *chr;
	for(chr = str_String+strlen(str_String)-1; chr>str_String; chr--) {
		// if the character is not space 
		if (!IsSpace(*chr)) {
			// stop searching
			break;
		}
	}
	// trim to that character
	return TrimRight(chr-str_String+1);
}


// retain only first line of the string
void CTString::OnlyFirstLine(void)
{
	// get position of first line end
	const char *pchNL = strchr(str_String, '\n');
	// if none
	if (pchNL==NULL) {
		// do nothing
		return;
	}
	// trim everything after that char
	TrimRight(pchNL-str_String);
}

// convert all letter characters in string to uppercase
void CTString::ToUpper(void)
{
	for (INDEX i=0; i<Length(); i++) {
		str_String[i] = toupper(str_String[i]);
	}
}

// convert all letter characters in string to lowercase
void CTString::ToLower(void)
{
	for (INDEX i=0; i<Length(); i++) {
		str_String[i] = tolower(str_String[i]);
	}
}


/* Calculate hashing value for the string. */
ULONG CTString::GetHash(void) const
{
	ULONG ulKey = 0;
	INDEX len = strlen(str_String);

	for(INDEX i=0; i<len; i++) {
//안태훈 수정 시작	//(For Performance)(0.3)
		ulKey = _rotl(ulKey,4)+toupper(str_String[i]);	//원본
		//ulKey = _rotl(ulKey,4) + str_String[i];	//대소문자 구분함.
//안태훈 수정 끝	//(For Performance)(0.3)
	}
	return ulKey;
}

/*
 * Throw exception
 */
void CTString::Throw_t(void)
{
	throw(str_String);
}


/*
 * Read from stream.
 */
CTStream &operator>>(CTStream &strmStream, CTString &strString)
{
	ASSERT(strString.IsValid());

	// read length
	INDEX iLength;
	strmStream>>iLength;
	ASSERT(iLength>=0);
	// allocate that much memory
	StringFree(strString.str_String);
	strString.str_String = StringAlloc(iLength);
	// if the string is not empty
	if (iLength>0) {
		// read string
		strmStream.Read_t( strString.str_String, iLength);  // without end-marker
	}
	// set end-marker
	strString.str_String[iLength] = 0;

	STRING_CHECKMEMORY();
	return strmStream;
}


void CTString::ReadFromText_t(CTStream &strmStream,
															const CTString &strKeyword="") // throw char *
{
	ASSERT(IsValid());

	// keyword must be present
	strmStream.ExpectKeyword_t(strKeyword);

	// read the string from the file
	char str[1024];
	strmStream.GetLine_t(str, sizeof(str));

	// copy it here
	(*this) = str;
	STRING_CHECKMEMORY();
}

/*
 * Write to stream.
 */
CTStream &operator<<(CTStream &strmStream, const CTString &strString)
{
	ASSERT(strString.IsValid());

	// calculate size
	INDEX iStringLen = strlen( strString);
	// write size
	strmStream<<iStringLen;
	// if the string is not empty
	if (iStringLen>0) {
		// write string
		strmStream.Write_t(strString.str_String, iStringLen); // without end-marker
	}

	return strmStream;
}


#ifndef NDEBUG
/*
 * Check if string data is valid.
 */
BOOL CTString::IsValid(void) const
{
	ASSERT(this!=NULL && str_String!=NULL);
	return TRUE;
}
#endif // NDEBUG

// return to empty string data
BOOL CTString::IsEmpty(void) const
{
	return (str_String == NULL || str_String[0] == 0);
}

/* Load an entire text file into a string. */
void CTString::ReadUntilEOF_t(CTStream &strmFile)  // throw char *
{
	// get the file size
	SLONG slFileSize = strmFile.GetStreamSize()-strmFile.GetPos_t();

	// allocate that much memory
	StringFree(str_String);
	str_String = StringAlloc(slFileSize);
	// read the entire file there
	if (slFileSize>0) {
		strmFile.Read_t( str_String, slFileSize);
	}
	// add end marker
	str_String[slFileSize] = 0;

	// rewrite entire string
	char *pchRead=str_String;
	char *pchWrite=str_String;
	while(*pchRead!=0) {
		// skip the '\r' characters
		if (*pchRead!='\r') {
			*pchWrite++ = *pchRead++;
		} else {
			pchRead++;
		}
	}
	*pchWrite = 0;
	STRING_CHECKMEMORY();
}


void CTString::Load_t(const class CTFileName &fnmFile)  // throw char *
{
	ASSERT(IsValid());

	// open the file for reading
	CTFileStream strmFile;
	strmFile.Open_t(fnmFile);

	// read string until end of file
	ReadUntilEOF_t(strmFile);
}


void CTString::LoadKeepCRLF_t(const class CTFileName &fnmFile)  // throw char *
{
	ASSERT(IsValid());

	// open the file for reading
	CTFileStream strmFile;
	strmFile.Open_t(fnmFile);
	// get the file size
	SLONG slFileSize = strmFile.GetStreamSize();

	// allocate that much memory
	StringFree(str_String);
	str_String = StringAlloc(slFileSize);
	// read the entire file there
	if (slFileSize>0) {
		strmFile.Read_t( str_String, slFileSize);
	}
	// add end marker
	str_String[slFileSize] = 0;
	STRING_CHECKMEMORY();
}

/* Save an entire string into a text file. */
void CTString::Save_t(const class CTFileName &fnmFile)  // throw char *
{
	// open the file for writing
	CTFileStream strmFile;
	strmFile.Create_t(fnmFile);
	// save the string to the file
	strmFile.PutString_t(*this);
}
void CTString::SaveKeepCRLF_t(const class CTFileName &fnmFile)  // throw char *
{
	// open the file for writing
	CTFileStream strmFile;
	strmFile.Create_t(fnmFile);
	// save the string to the file
	if (strlen(str_String)>0) {
		strmFile.Write_t(str_String, strlen(str_String));
	}
}


// Print formatted to a string
INDEX CTString::PrintF(const char *strFormat, ...)
{
	va_list arg;
	va_start(arg, strFormat);
	return VPrintF(strFormat, arg);
}


//static _declspec(thread) INDEX _ctBufferSize = 0;
//static _declspec(thread) char *_pchBuffer = NULL;
static TLVar<INDEX> _ctBufferSize = 0;
//static TLVar<char*> _pchBuffer = NULL;

INDEX CTString::VPrintF(const char *strFormat, va_list arg)
{
	INDEX ctBufferSize = 0;
	char *pchBuffer = NULL;
	// if buffer was not allocated yet
	if (ctBufferSize==0) {
		// allocate it
		ctBufferSize = 256;
		pchBuffer = (char*)AllocMemory(ctBufferSize);
	}

	// repeat
	INDEX iLen;
	FOREVER {
		// print to the buffer
		iLen = _vsnprintf(pchBuffer, ctBufferSize, strFormat, arg);
		// if printed ok
		if (iLen!=-1) {
			// stop
			break;
		}
		// increase the buffer size
		ctBufferSize += 256;
		GrowMemory((void**)&pchBuffer, ctBufferSize);
	}

	(*this) = pchBuffer;
	FreeMemory(pchBuffer);
	STRING_CHECKMEMORY();
	return iLen;
}

// Clears temp memory used by vprintf
extern void FreeVPrintF(void)
{
	// if vprintf allocated some memory
//	if(_pchBuffer!=NULL) {
//		// free temp memory
//		FreeMemory(_pchBuffer);
//		_pchBuffer = NULL;
//	}
}

static void *psscanf = &sscanf;
// Scan formatted from a string
__declspec(naked) INDEX CTString::ScanF(const char *strFormat, ...)
{
	__asm {
		push    eax
		mov     eax,dword ptr [esp+8]
		mov     eax,dword ptr [eax]
		mov     dword ptr [esp+8], eax
		pop     eax
		jmp     dword ptr [psscanf]
	}
}


	// split string in two strings at specified position (char AT splitting position goes to str2)
void CTString::Split( INDEX iPos, CTString &str1, CTString &str2)
{
	str1 = str_String;
	str2 = str_String;
	str1.TrimRight(iPos);
	str2.TrimLeft(strlen(str2)-iPos);
}


// insert one character in string at specified pos
void CTString::InsertChar( INDEX iPos, char cChr)
{
	// clamp position
	INDEX ctChars = strlen(str_String);
	if( iPos>ctChars) iPos=ctChars;
	else if( iPos<0)  iPos=0;
	// grow memory used by string
	StringResize( &str_String, ctChars, ctChars+1);
	// copy part of string to make room for char to insert
	memmove( &str_String[iPos+1], &str_String[iPos], ctChars+1-iPos);
	str_String[iPos] = cChr;
	STRING_CHECKMEMORY();
}


// delete one character from string at specified pos
void CTString::DeleteChar( INDEX iPos)
{
	// clamp position
	INDEX ctChars = strlen(str_String);
	if (ctChars==0) {
		return;
	}
	if( iPos>=ctChars) iPos=ctChars-1;
	else if( iPos<0)  iPos=0;
	// copy part of string
	memmove( &str_String[iPos], &str_String[iPos+1], ctChars-iPos);
	// shrink memory used by string over deleted char
	StringResize( &str_String, ctChars, ctChars-1);
	STRING_CHECKMEMORY();
}


// insert substring into a string at specified pos
void CTString::InsertSubString( INDEX iPos, const CTString &strToInsert)
{
	// clamp position
	INDEX ctChars = strlen(str_String);
	if( iPos>ctChars) iPos=ctChars;
	else if( iPos<0)  iPos=0;
	INDEX ctCharsToInsert = strlen(strToInsert);
	// grow memory used by string
	StringResize( &str_String, ctChars, ctChars+ctCharsToInsert);
	// copy part of string to make room for char to insert
	memmove( &str_String[iPos+ctCharsToInsert], &str_String[iPos], ctChars+1-iPos);
	memcpy( &str_String[iPos], (const char *)strToInsert, ctCharsToInsert);
	STRING_CHECKMEMORY();
}

// delete characters from string at specified pos
void CTString::DeleteChars( INDEX iPos, INDEX ctCharsToDelete)
{
	// clamp position
	INDEX ctChars = strlen(str_String);
	if( iPos>ctChars) iPos=ctChars;
	else if( iPos<0)  iPos=0;
	if (iPos+ctCharsToDelete>ctChars) {
		return;
	}
	// copy part of string
	memmove( &str_String[iPos], &str_String[iPos+ctCharsToDelete], ctChars-iPos-ctCharsToDelete+1);
	// shrink memory used by string over deleted char
	StringResize( &str_String, ctChars, ctChars-ctCharsToDelete);
	STRING_CHECKMEMORY();
}

// Is string of integer
BOOL CTString::IsInteger( void )
{
	INDEX ctChars = strlen( str_String );

	if( ctChars == 0 ) return FALSE;

	for( INDEX i=0; i<ctChars; i++ )
	{
		// WSS_NEW_GUILD_SYSTEM 070703 ------------------>><<
		// && -> ||
		if( str_String[i] < '0' || str_String[i] > '9' )
			return FALSE;
	}

	return TRUE;
}

// wild card comparison
BOOL CTString::Matches(const CTString &strOther) const
{
	return Matches(strOther.str_String);
}


BOOL CTString::Matches(const char *strOther) const
{
// pattern matching code from sourceforge.net codesnippet archive
// adjusted a bit to match in/out parameters
#define        MAX_CALLS        200
	int        calls=0, wild=0, q=0;
	const char *mask=strOther, *name=str_String;
	const char *m=mask, *n=name, *ma=mask, *na=name;
	
	for(;;) {
		if (++calls > MAX_CALLS) {
			return FALSE;
		}
		if (*m == '*') {
			while (*m == '*') ++m;
			wild = 1;
			ma = m;
			na = n;
		}
		
		if (!*m) {
			if (!*n) {
				return TRUE;
			}
			
			for (--m; (m > mask) && (*m == '?'); --m) ;
			
			if ((*m == '*') && (m > mask) &&
				(m[-1] != '\\')) {
				return TRUE;
			}
			if (!wild) {
				return FALSE;
			}
			m = ma;
		} else if (!*n) {
			while(*m == '*') ++m;
			if (*m != 0) {
				return FALSE;
			} else {
				return TRUE;
			}
		}
		if ((*m == '\\') && ((m[1] == '*') || (m[1] == '?'))) {
			++m;
			q = 1;
		} else {
			q = 0;
		}
		
		if ((tolower(*m) != tolower(*n)) && ((*m != '?') || q)) {
			if (!wild) {
				return FALSE;
			}
			m = ma;
			n = ++na;
		} else {
			if (*m) ++m;
				if (*n) ++n;
		}
	}
}


// variable management functions
void CTString::LoadVar(const class CTFileName &fnmFile)
{
	try {
		CTString str;
		str.Load_t(fnmFile);
		*this = str;
	} catch (char *strError) {
		CPrintF(TRANS("Cannot load variable from '%s':\n%s\n"), (CTString&)fnmFile, strError);
	}
}

void CTString::SaveVar(const class CTFileName &fnmFile)
{
	try {
		Save_t(fnmFile);
	} catch (char *strError) {
		CPrintF(TRANS("Cannot save variable to '%s':\n%s\n"), (CTString&)fnmFile, strError);
	}
}


// general variable functions
void LoadStringVar(const CTFileName &fnmVar, CTString &strVar)
{
	strVar.LoadVar(fnmVar);
}

void SaveStringVar(const CTFileName &fnmVar, CTString &strVar)
{
	strVar.SaveVar(fnmVar);
}


void LoadIntVar(const CTFileName &fnmVar, INDEX &iVar)
{
	CTString strVar;
	strVar.LoadVar(fnmVar);
	if (strVar!="") {
		CTString strHex = strVar;
		if (strHex.RemovePrefix("0x")) {
			strHex.ScanF("%x", &iVar);
		} else {
			strVar.ScanF("%d", &iVar);
		}
	}
}

void SaveIntVar(const CTFileName &fnmVar, INDEX &iVar)
{
	CTString strVar;
	strVar.PrintF("%d", iVar);
	strVar.SaveVar(fnmVar);
}



// remove special codes from string
CTString RemoveSpecialCodes( const CTString &str)
{
	CTString strRet=str;
	char *pcSrc = (char*)(const char*)strRet;
	char *pcDst = (char*)(const char*)strRet;
	// copy char inside string skipping special codes
	while( *pcSrc != 0)
	{
		if( *pcSrc != '^')
		{ // advance to next char
			*pcDst = *pcSrc;
			pcSrc++;
			pcDst++;
		}
		else
		{ // skip some characters 
			pcSrc++;
			switch( *pcSrc) {
			case 'c':  pcSrc+=FindByte( 0, (UBYTE*)pcSrc, 7);  continue;
			case 'a':  pcSrc+=FindByte( 0, (UBYTE*)pcSrc, 3);  continue;
			case 'f':  pcSrc+=FindByte( 0, (UBYTE*)pcSrc, 2);  continue;
			case 'b':  case 'i':  case 'r':  case 'o':
			case 'C':  case 'A':  case 'F':  case 'B':  case 'I':  pcSrc+=1;  continue;
			// if we get here this means that ^ or an unrecognized special code was specified
			default: continue;
			}
		}
	}
	// terminate string
	*pcDst = 0;
	STRING_CHECKMEMORY();
	return strRet;
}



// read/write to stream with uword length
void CTString::WriteToStreamShort(CTStream* ostr) 
{
	ASSERT(IsValid());
	ASSERT(ostr!=NULL);

	// calculate size
	UWORD iStringLen = strlen(str_String);
	// write size
	(*ostr)<<iStringLen;
	// if the string is not empty
	if (iStringLen>0) {
		// write string
		ostr->Write_t(str_String, iStringLen); // without end-marker
	}

};

void CTString::ReadFromStreamShort(CTStream* istr)
{
	ASSERT(IsValid());
	ASSERT(istr!=NULL);

	// read length
	UWORD uwLength;
	(*istr)>>uwLength;
	ASSERT(uwLength>=0);
	// allocate that much memory
	StringFree(str_String);
	str_String = StringAlloc(uwLength);
	// if the string is not empty
	if (uwLength>0) {
		// read string
		istr->Read_t( str_String, uwLength);  // without end-marker
	}
	// set end-marker
	str_String[uwLength] = 0;

	STRING_CHECKMEMORY();
};
