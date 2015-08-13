#include "stdh.h"

#include <Engine/Base/FileName.h>

#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/Stream.h>
#include <Engine/Templates/NameTable_CTFileName.h>
#include <Engine/Templates/DynamicStackArray.cpp>

template CDynamicArray<CTFileName>;
template CDynamicStackArray<CTFileName>;
#include <Engine/Templates/StaticStackArray.cpp>
template CStaticStackArray<long>;

/*
 * Get directory part of a filename.
 */
CTFileName CTFileName::FileDir() const
{
	ASSERT(IsValid());

	// make a temporary copy of string
	CTFileName strPath(*this);
	// find last backlash in it
	char *pPathBackSlash = strrchr( strPath.str_String, '\\');
	// if there is no backslash
	if( pPathBackSlash == NULL) {
		// return emptystring as directory
		return( CTFileName(""));
	}
	// set end of string after where the backslash was
	pPathBackSlash[1] = 0;
	// return a copy of temporary string
	return( CTFileName( strPath));
}

CTFileName &CTFileName::operator=(const char *strCharString)
{
	ASSERTALWAYS( "Use CTFILENAME for conversion from char *!");
	return *this;
}

/*
 * Get name part of a filename.
 */
CTFileName CTFileName::FileName() const
{
	ASSERT(IsValid());

	// make a temporary copy of string
	CTFileName strPath(*this);
	// find last dot in it
	char *pDot = strrchr( strPath.str_String, '.');
	// if there is a dot
	if( pDot != NULL) {
		// set end of string there
		pDot[0] = 0;
	}

	// find last backlash in what's left
	char *pBackSlash = strrchr( strPath.str_String, '\\');
	// if there is no backslash
	if( pBackSlash == NULL) {
		// return it all as filename
		return( CTFileName(strPath));
	}
	// return a copy of temporary string, starting after the backslash
	return( CTFileName( pBackSlash+1));
}

/*
 * Get extension part of a filename.
 */
CTFileName CTFileName::FileExt() const
{
	ASSERT(IsValid());

	// find last dot in the string
	char *pExtension = strrchr( str_String, '.');
	// if there is no dot
	if( pExtension == NULL) {
		// return no extension
		return( CTFileName(""));
	}
	// return a copy of the extension part, together with the dot
	return( CTFileName( pExtension));
}

CTFileName CTFileName::NoExt() const
{
	return FileDir()+FileName();
}

/*
 * Remove application path from a file name.
 */
void CTFileName::RemoveApplicationPath_t(void) // throws char *
{
	// remove the path string from beginning of the string
	BOOL bHadRightPath = RemovePrefix(_fnmApplicationPath);
	if (_fnmMod!="") {
		RemovePrefix(_fnmApplicationPath+_fnmMod);
	}
	// if it had wrong path
	if (!bHadRightPath) {
		// throw error
		ThrowF_t(TRANS("File '%s' has got wrong path!\nAll files must reside in directory '%s'."),
			str_String, (CTString&)_fnmApplicationPath);
	}
}

/*
 * Read from stream.
 */
 CTStream &operator>>(CTStream &strmStream, CTFileName &fnmFileName)
{
	// if dictionary is enabled
	if (strmStream.strm_dmDictionaryMode == CTStream::DM_ENABLED) {
		// read the index in dictionary
		INDEX iFileName;
		strmStream>>iFileName;
		// get that file from the dictionary
		fnmFileName = strmStream.strm_afnmDictionary[iFileName];

	// if dictionary is processing or not active
	} else {
		char strTag[] = "_FNM"; strTag[0] = 'D';  // must create tag at run-time!
		// skip dependency catcher header
		strmStream.ExpectID_t(strTag);    // data filename
		// read the string
		strmStream>>(CTString &)fnmFileName;
		fnmFileName.fnm_pserPreloaded = NULL;
	}

	return strmStream;
}

DECLARE_CTFILENAME(_fnmDefaultModel, "Data\\Models\\Editor\\Axis.mdl");
DECLARE_CTFILENAME(_fnmDefaultTexture, "Data\\Textures\\Editor\\Default.tex");
DECLARE_CTFILENAME(_fnmDefaultModelInstance, "Data\\Models\\Editor\\Ska\\Axis.smc");
static CStaticStackArray<CTString> _astrExcludedEditorComponents;
static BOOL bLoadExcludedComponents = TRUE;
const CTFileName &GetReplacementFileName(const CTFileName &fnFileName)
{
	extern INDEX wld_bExcludeEditorModels;
	if(!wld_bExcludeEditorModels) {
		return fnFileName;
	}

	// if need to load components to exclude
	if(bLoadExcludedComponents) {
		_astrExcludedEditorComponents.SetAllocationStep(16);
		try {
			// open exclude list
			CTFileStream istrm;
			istrm.Open_t((CTString)"Data\\ExcludedEditorModels.lst",CTStream::OM_READ);
			CTString strExcludeText;

		  char achrLine[ 256];
		  char achrSource[ 256];
			while(!istrm.AtEOF())
			{
				istrm.GetLine_t( achrLine, 256);
				sscanf( achrLine, "\"%[^\"]\"", achrSource);
				// if string is valid
				if(strlen(achrSource)>0) {
					// add it to list of editor components that need to be excluded
					CTString &strEditorComponent = _astrExcludedEditorComponents.Push();
					strEditorComponent = achrSource;
				}
			}
			istrm.Close();
		} catch(char *) {
		}
		bLoadExcludedComponents = FALSE;
	}

	// search for file name replacement
	const INDEX ctcmp = _astrExcludedEditorComponents.Count();
	for(INDEX icmp=0;icmp<ctcmp;icmp++) {
		const CTFileName &fnComponent = _astrExcludedEditorComponents[icmp];
		if(fnComponent == fnFileName) {
			if(fnFileName.FileExt() == ".mdl") {
				return _fnmDefaultModel;
			} else if(fnFileName.FileExt() == ".tex") {
				return _fnmDefaultTexture;
			} else if(fnFileName.FileExt() == ".smc") {
				return _fnmDefaultModelInstance;
			}
		}
	}
	return fnFileName;
}

/*
 * Write to stream.
 */
 CTStream &operator<<(CTStream &strmStream, const CTFileName &fnmFileName)
{
	// if dictionary is enabled
	if (strmStream.strm_dmDictionaryMode == CTStream::DM_ENABLED) {
		const CTFileName &fnmReplacedName = GetReplacementFileName(fnmFileName);
		// try to find the filename in dictionary
		CTFileName *pfnmExisting = strmStream.strm_ntDictionary.Find(fnmReplacedName);
		// if not existing
		if (pfnmExisting==NULL) {
			// add it
			pfnmExisting = &strmStream.strm_afnmDictionary.Push();
			*pfnmExisting = fnmReplacedName;
			strmStream.strm_ntDictionary.Add(pfnmExisting);
		}
		// write its index
		strmStream<<strmStream.strm_afnmDictionary.Index(pfnmExisting);

	// if dictionary is processing or not active
	} else {
		char strTag[] = "_FNM"; strTag[0] = 'D';  // must create tag at run-time!
		// write dependency catcher header
		strmStream.WriteID_t(strTag);     // data filename
		// write the string
		strmStream<<(CTString &)fnmFileName;
	}

	return strmStream;
}

void CTFileName::ReadFromText_t(CTStream &strmStream,
																const CTString &strKeyword) // throw char *
{
	ASSERT(IsValid());

	char strTag[] = "_FNM "; strTag[0] = 'T';  // must create tag at run-time!
	// keyword must be present
	strmStream.ExpectKeyword_t(strKeyword);
	// after the user keyword, dependency keyword must be present
	strmStream.ExpectKeyword_t(strTag);

	// read the string from the file
	char str[1024];
	strmStream.GetLine_t(str, sizeof(str));
	fnm_pserPreloaded = NULL;

	// copy it here
	(*this) = CTString( (const char *)str);
}
