#include "stdh.h"

#include <Engine/Base/Stream.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Entities/Precaching.h>
#include <Engine/Base/Translation.h>
#include <Engine/Base/CRCTable.h>
#include <Engine/Base/Console.h>


#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/Stock_CAnimData.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Templates/Stock_CModelData.h>
#include <Engine/Templates/Stock_CSoundData.h>
#include <Engine/Templates/Stock_CEntityClass.h>

#include <Engine/Templates/Stock_CEntityClass.h>

// this container will hold entries for all entity classes used in the world
CDynamicContainer<EntityClassEntry> _cEntityClassContainer;


// empty the class container - release memory allocated for entries
void ClearEntityClassContainer() 
{
	INDEX ctEntries = _cEntityClassContainer.Count();
	EntityClassEntry* peceEntityClassEntry;
	while (ctEntries>0) {
		peceEntityClassEntry = _cEntityClassContainer.Pop();
		delete peceEntityClassEntry;
		ctEntries--;
	}

	_cEntityClassContainer.Clear();

};


// lookup a class in the container by it's id
EntityClassEntry* FindClassInContainer(INDEX iID) {
	ASSERT (iID>=0);

	INDEX ctEntries = _cEntityClassContainer.Count();
	for (INDEX iEntry=0;iEntry<ctEntries;iEntry++) {
		EntityClassEntry &eceEntry = _cEntityClassContainer[iEntry];
		if (eceEntry.ece_iClassID == iID) {
			return &(_cEntityClassContainer[iEntry]);
		}
	}
	return NULL;
}


/////////////////////////////////////////////////////////////////////
// CEntityClass

/*
 * Default constructor.
 */
CEntityClass::CEntityClass(void)
{
	ec_fnmClassDLL.Clear();
	ec_hiClassDLL = NULL;
	ec_pdecDLLClass = NULL;
}
/*
 * Constructor for a fixed class.
 */
CEntityClass::CEntityClass(class CDLLEntityClass *pdecDLLClass)
{
	ec_pdecDLLClass = pdecDLLClass;
	ec_hiClassDLL = NULL;
	ec_fnmClassDLL.Clear();
}

/*
 * Destructor.
 */
CEntityClass::~CEntityClass(void)
{
	Clear();
}

/////////////////////////////////////////////////////////////////////
// Reference counting functions
void CEntityClass::AddReference(void) {
	if (this!=NULL) {
		MarkUsed();
	}
};
void CEntityClass::RemReference(void) {
	if (this!=NULL) {
		_pEntityClassStock->Release(this);
	}
};

/*
 * Clear the object.
 */
void CEntityClass::Clear(void)
{
	// if the DLL is loaded
	if (ec_hiClassDLL != NULL) {
		// detach the DLL
		ec_pdecDLLClass->dec_OnEndClass();

		/* The dll is never released from memory, because declared shell symbols
		 * must stay avaliable, since they cannot be undeclared.
		 */
		// free it
		//BOOL bSuccess = FreeLibrary(ec_hiClassDLL);
		//ASSERT(bSuccess);
	}
	// release all components needed by the DLL
	ReleaseComponents();

	ec_pdecDLLClass = NULL;
	ec_hiClassDLL = NULL;
	ec_fnmClassDLL.Clear();
}

/* Check that all properties have been properly declared. */
void CEntityClass::CheckClassProperties(void)
{
// do nothing in release version
#ifndef NDEBUG
	// for all classes in hierarchy of this entity
	{for(CDLLEntityClass *pdecDLLClass1 = ec_pdecDLLClass;
			pdecDLLClass1!=NULL;
			pdecDLLClass1 = pdecDLLClass1->dec_pdecBase) {
		// for all properties
		for(INDEX iProperty1=0; iProperty1<pdecDLLClass1->dec_ctProperties; iProperty1++) {
			CEntityProperty &epProperty1 = pdecDLLClass1->dec_aepProperties[iProperty1];

			// for all classes in hierarchy of this entity
			for(CDLLEntityClass *pdecDLLClass2 = ec_pdecDLLClass;
					pdecDLLClass2!=NULL;
					pdecDLLClass2 = pdecDLLClass2->dec_pdecBase) {
				// for all properties
				for(INDEX iProperty2=0; iProperty2<pdecDLLClass2->dec_ctProperties; iProperty2++) {
					CEntityProperty &epProperty2 = pdecDLLClass2->dec_aepProperties[iProperty2];
					// the two properties must not have same id unless they are same property
					ASSERTMSG(&epProperty1==&epProperty2 || epProperty1.ep_ulID!=epProperty2.ep_ulID,
						"No two properties may have same id!");
				}
			}
		}
	}}

	// for all classes in hierarchy of this entity
	{for(CDLLEntityClass *pdecDLLClass1 = ec_pdecDLLClass;
			pdecDLLClass1!=NULL;
			pdecDLLClass1 = pdecDLLClass1->dec_pdecBase) {
		// for all components
		for(INDEX iComponent1=0; iComponent1<pdecDLLClass1->dec_ctComponents; iComponent1++) {
			CEntityComponent &ecComponent1 = pdecDLLClass1->dec_aecComponents[iComponent1];

			// for all classes in hierarchy of this entity
			for(CDLLEntityClass *pdecDLLClass2 = ec_pdecDLLClass;
					pdecDLLClass2!=NULL;
					pdecDLLClass2 = pdecDLLClass2->dec_pdecBase) {
				// for all components
				for(INDEX iComponent2=0; iComponent2<pdecDLLClass2->dec_ctComponents; iComponent2++) {
					CEntityComponent &ecComponent2 = pdecDLLClass2->dec_aecComponents[iComponent2];
					// the two components must not have same id unless they are same component
					ASSERTMSG(&ecComponent1==&ecComponent2 || ecComponent1.ec_slID!=ecComponent2.ec_slID,
						"No two components may have same id!");
				}
			}
		}
	}}
#endif
}

/*
 * Construct a new member of the class.
 */
CEntity *CEntityClass::New(void)
{
	// the DLL must be loaded
	ASSERT(ec_pdecDLLClass!= NULL);
	// ask the DLL class to call the 'operator new' in the scope where the class is declared
	CEntity *penNew = ec_pdecDLLClass->dec_New();
	// remember this class as class of the entity
	AddReference();
	penNew->en_pecClass = this;
	// set all properties to default
	penNew->SetDefaultProperties();

	// return it
	return penNew;
}

/*
 * Obtain all components from component table.
 */
void CEntityClass::ObtainComponents_t(void)
{
	// for each component
	for (INDEX iComponent=0; iComponent<ec_pdecDLLClass->dec_ctComponents; iComponent++) {
		// if not precaching all
		if( gam_iPrecachePolicy<PRECACHE_ALL) {
			// if component is not class
			CEntityComponent &ec = ec_pdecDLLClass->dec_aecComponents[iComponent];
			if (ec.ec_ectType!=ECT_CLASS) {
				// skip it
				continue;
			}
		}

		// try to
		try {
			// obtain the component
			ec_pdecDLLClass->dec_aecComponents[iComponent].Obtain_t();
		// if failed
		} catch (char *) {
			// if in paranoia mode
			if( gam_iPrecachePolicy==PRECACHE_PARANOIA) {
				// fail
				throw;
			// if not in paranoia mode
			} else {
				// ignore all errors
				NOTHING;
			}
		}
	}
}

/*
 * Release all components from component table.
 */
void CEntityClass::ReleaseComponents(void)
{
	// for each component
	INDEX ctComponents = 0;
	if(ec_pdecDLLClass!=NULL) {
		ctComponents = ec_pdecDLLClass->dec_ctComponents;
	}
	for (INDEX iComponent=0; iComponent<ctComponents; iComponent++) {
		// release the component
		ec_pdecDLLClass->dec_aecComponents[iComponent].Release();
	}
}

// overrides from CSerial /////////////////////////////////////////////////////

/*
 * Load a Dynamic Link Library.
 */
#include <string>
#include <map>
class cDllManager
{
public:
	typedef std::map<std::string, HINSTANCE> my_map;
	cDllManager()
	{
	}
	~cDllManager()
	{
		my_map::iterator iterBegin = GetMap().begin();
		my_map::iterator iterEnd = GetMap().end();
		my_map::iterator iter;
		for(iter = iterBegin; iter != iterEnd; ++iter)
		{
			FreeLibrary((*iter).second);
		}
	}
	std::map<std::string, HMODULE> &GetMap()
	{
		return m_mapDll;
	}
protected:
	std::map<std::string, HINSTANCE> m_mapDll;
};
ENGINE_API cDllManager g_dlls;
HINSTANCE LoadDLL_t(const char *strFileName) // throw char *
{
	HMODULE hiDLL = NULL;
	std::string strfn(strFileName);
	if(g_dlls.GetMap().find(strfn) == g_dlls.GetMap().end())
	{
		hiDLL = ::LoadLibrary(strFileName);
		g_dlls.GetMap()[strfn] = hiDLL;
	}
	else
	{
		hiDLL = g_dlls.GetMap()[strfn];
	}

	// if the DLL can not be loaded
	if (hiDLL==NULL) {
		// get the error code
		DWORD dwMessageId = GetLastError();
		// format the windows error message
		LPVOID lpMsgBuf;
		DWORD dwSuccess = FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				dwMessageId,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL
		);
		CTString strWinError;
		// if formatting succeeds
		if (dwSuccess!=0) {
			// copy the result
			strWinError = ((char *)lpMsgBuf);
			// free the windows message buffer
			LocalFree( lpMsgBuf );
		} else {
			// set our message about the failure
			CTString strError;
			strError.PrintF(
				TRANS("Cannot format error message!\n"
				"Original error code: %d,\n"
				"Formatting error code: %d.\n"),
				dwMessageId, GetLastError());
			strWinError = strError;
		}

		// report error
		ThrowF_t(TRANS("Cannot load DLL file '%s':\n%s"), strFileName, strWinError);
	}	
	return hiDLL;
}

/*
 * Read from stream.
 */
void CEntityClass::Read_t( CTStream *istr) // throw char *
{
	// read the dll filename and class name from the stream
	CTFileName fnmDLL;
	fnmDLL.ReadFromText_t(*istr, "Package: ");
	CTString strClassName;
	strClassName.ReadFromText_t(*istr, "Class: ");

	// create name of dll
#ifndef NDEBUG
		//fnmDLL = fnmDLL.FileDir()+"Debug\\"+fnmDLL.FileName()+_strModExt+"D"+fnmDLL.FileExt();
#	if		defined(_MSC_VER) && (_MSC_VER >= 1600)
#		ifdef	WORLD_EDITOR
			fnmDLL = fnmDLL.FileName() + _strModExt + "D" + fnmDLL.FileExt();
#		else
			fnmDLL = fnmDLL.FileDir()+"Debug2010\\"+fnmDLL.FileName()+_strModExt+"D"+fnmDLL.FileExt();
#		endif
#	else
		fnmDLL = fnmDLL.FileDir()+"Debug\\"+fnmDLL.FileName()+_strModExt+"D"+fnmDLL.FileExt();
#	endif
#else	// NDEBUG
#	ifdef	WORLD_EDITOR
		fnmDLL =  fnmDLL.FileName() + _strModExt+fnmDLL.FileExt();
#	else	// WORLD_EDITOR
		fnmDLL = fnmDLL.FileDir()+fnmDLL.FileName()+_strModExt+fnmDLL.FileExt();
#	endif	// WORLD_EDITOR
#endif

	// load the DLL
	CTFileName fnmExpanded;
#ifndef	WORLD_EDITOR
	ExpandFilePath(EFP_READ, fnmDLL, fnmExpanded);
#else	// WORLD_EDITOR
	fnmExpanded = _fnmApplicationPath + _fnmApplicationExe.FileDir() + fnmDLL;
#endif // WORLD_EDITOR

	ec_hiClassDLL = LoadDLL_t(fnmExpanded);
	ec_fnmClassDLL = fnmDLL;

	// get the pointer to the DLL class structure
	ec_pdecDLLClass = (CDLLEntityClass *) GetProcAddress(ec_hiClassDLL, strClassName+"_DLLClass");
	// if class structure is not found
	if (ec_pdecDLLClass == NULL) {
		// free the library
		//BOOL bSuccess = FreeLibrary(ec_hiClassDLL);
		//ASSERT(bSuccess);
		ec_hiClassDLL = NULL;
		ec_fnmClassDLL.Clear();
		// report error
		ThrowF_t(TRANS("Class '%s' not found in entity class package file '%s'"), strClassName, fnmDLL);
	}

	// obtain all components needed by the DLL
	{
		CTmpPrecachingNow tpn;
		ObtainComponents_t();
	}

	// attach the DLL
	ec_pdecDLLClass->dec_OnInitClass();

	// check that the class properties have been properly declared
	CheckClassProperties();
}

/*
 * Write to stream.
 */
void CEntityClass::Write_t( CTStream *ostr) // throw char *
{
	ASSERTALWAYS("Do not write CEntityClass objects!");
}
// get amount of memory used by this object
SLONG CEntityClass::GetUsedMemory(void)
{
	// we don't know exact memory used, but we want to enumerate them
	return 0;
}
// check if this kind of objects is auto-freed
BOOL CEntityClass::IsAutoFreed(void) 
{ 
	return FALSE;
};
// gather the CRC of the file
void CEntityClass::AddToCRCTable(void)
{
 
	const CTFileName &fnm = GetName();

	// add the file itself
	CRCT_AddFile_t(fnm);

	// add its DLL
	CRCT_AddFile_t(ec_fnmClassDLL);

	// add to container

	// try to find it in the container
	EntityClassEntry* peceEntry = FindClassInContainer(ec_pdecDLLClass->dec_iID);
	// if it's not there, add it
	if (peceEntry == NULL) {
		peceEntry = new EntityClassEntry;
		peceEntry->ece_fnmEntityClass = fnm;
		peceEntry->ece_iClassID = ec_pdecDLLClass->dec_iID;
		_cEntityClassContainer.Add(peceEntry);
	// if it is, print out a warning
	} else {
			static UBYTE aubClassIDs[1000] = {0};
			if (peceEntry->ece_fnmEntityClass != fnm && !aubClassIDs[ec_pdecDLLClass->dec_iID]) {
				CPrintF(TRANS("WARNING: two entity classes share the same class ID! ID is: %ld\n"),ec_pdecDLLClass->dec_iID);
				CPrintF(TRANS("    Class 1: %s\n"),(const char*)peceEntry->ece_fnmEntityClass);
				CPrintF(TRANS("    Class 2: %s\n"),(const char*)fnm);
				aubClassIDs[ec_pdecDLLClass->dec_iID] = 1;
			}
	}

	
}

/* Get pointer to entity property from its name. */
class CEntityProperty *CEntityClass::PropertyForName(const CTString &strPropertyName) {
	return ec_pdecDLLClass->PropertyForName(strPropertyName);
};
/* Get pointer to entity property from its packed identifier. */
class CEntityProperty *CEntityClass::PropertyForTypeAndID(
	ULONG ulType, ULONG ulID) {
	return ec_pdecDLLClass->PropertyForTypeAndID((CEntityProperty::PropertyType)ulType, ulID);
};

/* Get event handler for given state and event code. */
CEntity::pEventHandler CEntityClass::HandlerForStateAndEvent(SLONG slState, SLONG slEvent) {
	return ec_pdecDLLClass->HandlerForStateAndEvent(slState, slEvent);
}

/* Get pointer to component from its identifier. */
class CEntityComponent *CEntityClass::ComponentForTypeAndID(
	EntityComponentType ectType, SLONG slID) {
	return ec_pdecDLLClass->ComponentForTypeAndID(ectType, slID);
}
/* Get pointer to component from the component. */
class CEntityComponent *CEntityClass::ComponentForPointer(void *pv) {
	return ec_pdecDLLClass->ComponentForPointer(pv);
}

// convert value of an enum to its name
const char *CEntityPropertyEnumType::NameForValue(INDEX iValue)
{
	for(INDEX i=0; i<epet_ctValues; i++) {
		if (epet_aepevValues[i].epev_iValue==iValue) {
			return epet_aepevValues[i].epev_strName;
		}
	}
	return "";
}

/*
 * Get pointer to entity property from its name.
 */
class CEntityProperty *CDLLEntityClass::PropertyForName(const CTString &strPropertyName)
{
	// for all classes in hierarchy of this entity
	{for(CDLLEntityClass *pdecDLLClass = this;
			pdecDLLClass!=NULL;
			pdecDLLClass = pdecDLLClass->dec_pdecBase) {
		// for each property
		for (INDEX iProperty=0; iProperty<pdecDLLClass->dec_ctProperties; iProperty++) {
			// if it has that name
			if (pdecDLLClass->dec_aepProperties[iProperty].ep_strName==strPropertyName) {
				// return it
				return &pdecDLLClass->dec_aepProperties[iProperty];
			}
		}
	}}
	// none found
	return NULL;
}

/*
 * Get pointer to entity property from its packed identifier.
 */
class CEntityProperty *CDLLEntityClass::PropertyForTypeAndID(
	CEntityProperty::PropertyType eptType, ULONG ulID)
{
	// for all classes in hierarchy of this entity
	{for(CDLLEntityClass *pdecDLLClass = this;
			pdecDLLClass!=NULL;
			pdecDLLClass = pdecDLLClass->dec_pdecBase) {
		// for each property
		for (INDEX iProperty=0; iProperty<pdecDLLClass->dec_ctProperties; iProperty++) {
			// if it has that same identifier
			if (pdecDLLClass->dec_aepProperties[iProperty].ep_ulID==ulID) {

				// if it also has same type
				if (pdecDLLClass->dec_aepProperties[iProperty].ep_eptType==eptType) {
					// return it
					return &pdecDLLClass->dec_aepProperties[iProperty];

				// if it has different type
				} else {
					// return that it was not found, this makes the whole thing much safer
					return NULL;
				}
			}
		}
	}}

	// if not found, maybe the class id has changed, so try to get this property from this class just by index
	INDEX iPropertyIndex = ulID & 0xFF;
	// for each property
	for (INDEX iProperty=0; iProperty<dec_ctProperties; iProperty++) {
		INDEX iFoundIndex = dec_aepProperties[iProperty].ep_ulID & 0xFF;
		// if it has that same identifier
		if (iPropertyIndex==iFoundIndex) {

			// if it also has same type
			if (dec_aepProperties[iProperty].ep_eptType==eptType) {
//강동민 수정 시작 테스트 클라이언트 작업	06.18
		  // 프로퍼티를 얻는 부분...
		  //CPrintF("Class ID fixup\n");
//강동민 수정 끝 테스트 클라이언트 작업		06.18
				// return it
				return &dec_aepProperties[iProperty];

			// if it has different type
			} else {
				// return that it was not found, this makes the whole thing much safer
				return NULL;
			}
		}
	}
	
	// none found
	return NULL;
};

/*
 * Get pointer to component from its identifier.
 */
class CEntityComponent *CDLLEntityClass::ComponentForTypeAndID(
	EntityComponentType ectType, SLONG slID)
{
	// for all classes in hierarchy of this entity
	{for(CDLLEntityClass *pdecDLLClass = this;
			pdecDLLClass!=NULL;
			pdecDLLClass = pdecDLLClass->dec_pdecBase) {
		// for each component
		for (INDEX iComponent=0; iComponent<pdecDLLClass->dec_ctComponents; iComponent++) {
			// if it has that same identifier
			if (pdecDLLClass->dec_aecComponents[iComponent].ec_slID==slID) {

				// if it also has same type
				if (pdecDLLClass->dec_aecComponents[iComponent].ec_ectType==ectType) {
					// obtain it
					pdecDLLClass->dec_aecComponents[iComponent].ObtainWithCheck();
					// return it
					return &pdecDLLClass->dec_aecComponents[iComponent];

				// if it has different type
				} else {
					// return that it was not found, this makes the whole thing much safer
					return NULL;
				}
			}
		}
	}}
	// none found
	return NULL;
}
/*
 * Get pointer to component from the component.
 */
class CEntityComponent *CDLLEntityClass::ComponentForPointer(void *pv)
{
	// for all classes in hierarchy of this entity
	{for(CDLLEntityClass *pdecDLLClass = this;
			pdecDLLClass!=NULL;
			pdecDLLClass = pdecDLLClass->dec_pdecBase) {
		// for each component
		for (INDEX iComponent=0; iComponent<pdecDLLClass->dec_ctComponents; iComponent++) {
			// if it has that same pointer
			if (pdecDLLClass->dec_aecComponents[iComponent].ec_pvPointer==pv) {
				// obtain it
				pdecDLLClass->dec_aecComponents[iComponent].ObtainWithCheck();
				// return it
				return &pdecDLLClass->dec_aecComponents[iComponent];
			}
		}
	}}
	// none found
	return NULL;
}

// precache given component
void CDLLEntityClass::PrecacheModel(SLONG slID)
{
	CTmpPrecachingNow tpn;

	CEntityComponent *pecModel = ComponentForTypeAndID(ECT_MODEL, slID);
	ASSERT(pecModel!=NULL);
	pecModel->ObtainWithCheck();
}

void CDLLEntityClass::PrecacheTexture(SLONG slID)
{
	CTmpPrecachingNow tpn;

	CEntityComponent *pecTexture = ComponentForTypeAndID(ECT_TEXTURE, slID);
	ASSERT(pecTexture!=NULL);
	pecTexture->ObtainWithCheck();
}

void CDLLEntityClass::PrecacheSound(SLONG slID)
{
	CTmpPrecachingNow tpn;

	CEntityComponent *pecSound = ComponentForTypeAndID(ECT_SOUND, slID);
	ASSERT(pecSound!=NULL);
	pecSound->ObtainWithCheck();
}

void CDLLEntityClass::PrecacheClass(SLONG slID, INDEX iUser /* = -1 */)
{
	CTmpPrecachingNow tpn;

	CEntityComponent *pecClass = ComponentForTypeAndID(ECT_CLASS, slID);
	ASSERT(pecClass!=NULL);
	pecClass->ObtainWithCheck();
	pecClass->ec_pecEntityClass->ec_pdecDLLClass->dec_OnPrecache(
		pecClass->ec_pecEntityClass->ec_pdecDLLClass, iUser);
}

void CDLLEntityClass::PrecacheSkaModel(SLONG slID)
{
	CTmpPrecachingNow tpn;

	CEntityComponent *pecSkaModel = ComponentForTypeAndID(ECT_SKAMODEL, slID);
	ASSERT(pecSkaModel!=NULL);
	pecSkaModel->ObtainWithCheck();
}

/*
 * Get event handler given state and event code.
 */
CEntity::pEventHandler CDLLEntityClass::HandlerForStateAndEvent(SLONG slState, SLONG slEvent)
{
	// we ignore the event code here
	(void) slEvent;

	// for all classes in hierarchy of this entity
	{for(CDLLEntityClass *pdecDLLClass = this;
			pdecDLLClass!=NULL;
			pdecDLLClass = pdecDLLClass->dec_pdecBase) {
		// for each handler
		for (INDEX iHandler=0; iHandler<pdecDLLClass->dec_ctHandlers; iHandler++) {
			// if it has that same state
			if (pdecDLLClass->dec_aeheHandlers[iHandler].ehe_slState==slState) {
				// return it
				return pdecDLLClass->dec_aeheHandlers[iHandler].ehe_pEventHandler;
			}
		}
	}}
	// none found
	return NULL;
}

/* Get event handler name for given state. */
const char *CDLLEntityClass::HandlerNameForState(SLONG slState)
{
	// for all classes in hierarchy of this entity
	{for(CDLLEntityClass *pdecDLLClass = this;
			pdecDLLClass!=NULL;
			pdecDLLClass = pdecDLLClass->dec_pdecBase) {
		// for each handler
		for (INDEX iHandler=0; iHandler<pdecDLLClass->dec_ctHandlers; iHandler++) {
			// if it has that same state
			if (pdecDLLClass->dec_aeheHandlers[iHandler].ehe_slState==slState) {
				// return its name
				return pdecDLLClass->dec_aeheHandlers[iHandler].ehe_strName;
			}
		}
	}}
	// none found
	return "no handler!?";
}

/* Get derived class override for given state. */
SLONG CDLLEntityClass::GetOverridenState(SLONG slState)
{
	// for all classes in hierarchy of this entity
	{for(CDLLEntityClass *pdecDLLClass = this;
			pdecDLLClass!=NULL;
			pdecDLLClass = pdecDLLClass->dec_pdecBase) {
		// for each handler
		for (INDEX iHandler=0; iHandler<pdecDLLClass->dec_ctHandlers; iHandler++) {
			// if it has that same base state
			if (pdecDLLClass->dec_aeheHandlers[iHandler].ehe_slBaseState>=0 &&
					pdecDLLClass->dec_aeheHandlers[iHandler].ehe_slBaseState==slState) {
				// return overriden state with possible recursive overriding
				return GetOverridenState(pdecDLLClass->dec_aeheHandlers[iHandler].ehe_slState);
			}
		}
	}}
	// none found
	return slState;
}
