#include "stdh.h"

#include <Engine/Entities/PlayerCharacter.h>
#include <Engine/Base/Timer.h>
#include <Engine/Base/Stream.h>
#include <Engine/Network/NetworkMessage.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/function/News.h>

typedef HRESULT __stdcall CoCreateGuid_t(UBYTE *pguid);

// get a GUID from system
static void GetGUID(UBYTE aub[16])
{
  HINSTANCE hOle32Lib = NULL;
  CoCreateGuid_t *pCoCreateGuid = NULL;

  try {
    // load ole32
    hOle32Lib = ::LoadLibrary( "ole32.dll");
    if( hOle32Lib == NULL) {
      ThrowF_t(TRANS("Cannot load ole32.dll."));
    }

    // find GUID function
    pCoCreateGuid = (CoCreateGuid_t*)GetProcAddress(hOle32Lib, "CoCreateGuid");
    if (pCoCreateGuid==NULL) {
      ThrowF_t(TRANS("Cannot find CoCreateGuid()."));
    }

    // create the guid
    HRESULT hres = pCoCreateGuid(&aub[0]);

    // check for success
    if (hres!=S_OK) {
      ThrowF_t(TRANS("CoCreateGuid(): Error 0x%08x"), hres);
    }

    // free the ole32 library
    FreeLibrary(hOle32Lib);

  } catch(char *strError) {
    FatalError(TRANS("Cannot make GUID for a player:\n%s"), strError);
  }
}

/*
 * Default constructor -- no character.
 */
CPlayerCharacter::CPlayerCharacter(void)
  : pc_strName("<invalid player>")
{
  memset(pc_aubGUID, 0, PLAYERGUIDSIZE);
  memset(pc_aubAppearance, 0, MAX_PLAYERAPPEARANCE);
  pc_iPlayerIndex = -1;
  pc_iNoticeRevision = 0;
}

/*
 * Create a new character with its name.
 */
CPlayerCharacter::CPlayerCharacter(const CTString &strName)
  : pc_strName(strName)
{
  // if the name passed to constructor is empty string
  if (strName=="") {
    // make this an unnamed player
    pc_strName = "<unnamed player>";
  }
  // create the guid
  GetGUID(pc_aubGUID);
  memset(pc_aubAppearance, 0, MAX_PLAYERAPPEARANCE);
  pc_iPlayerIndex = -1;
//0217 캐릭터 종류
  pc_iPlayerType = -1;
  pc_iNoticeRevision = 0;
}

void CPlayerCharacter::Load_t( const CTFileName &fnFile) // throw char *
{
  CTFileStream strm;
  strm.Open_t(fnFile);
  Read_t(&strm);
  strm.Close();
}

void CPlayerCharacter::Save_t( const CTFileName &fnFile) // throw char *
{
  CTFileStream strm;
  strm.Create_t(fnFile);
  Write_t(&strm);
  strm.Close();
}

/*
 * Read character from a stream.
 */
void CPlayerCharacter::Read_t(CTStream *pstr) // throw char *
{
  pstr->ExpectID_t("PLC4");
  CTString strTemp;
  (*pstr)>>pc_strName>>strTemp;
  pstr->Read_t(pc_aubGUID, sizeof(pc_aubGUID));
  pstr->Read_t(pc_aubAppearance, sizeof(pc_aubAppearance));
  (*pstr)>>pc_iPlayerIndex;
  //0217
  (*pstr)>>pc_iPlayerType;

  if (GAMEDATAMGR() != NULL && GAMEDATAMGR()->GetNews() != NULL)
  {
	  (*pstr)>>pc_iNoticeRevision;
	  GAMEDATAMGR()->GetNews()->LoadRevision(pc_iNoticeRevision);
  }

}

/*
 * Write character into a stream.
 */
void CPlayerCharacter::Write_t(CTStream *pstr) // throw char *
{
  pstr->WriteID_t("PLC4");
  CTString strTemp;
  (*pstr)<<pc_strName<<strTemp;
  pstr->Write_t(pc_aubGUID, sizeof(pc_aubGUID));
  pstr->Write_t(pc_aubAppearance, sizeof(pc_aubAppearance));
  (*pstr)<<pc_iPlayerIndex;
  //0217
  (*pstr)<<pc_iPlayerType;
  if (GAMEDATAMGR() != NULL && GAMEDATAMGR()->GetNews() != NULL)
  {
	  pc_iNoticeRevision = GAMEDATAMGR()->GetNews()->GetRevision();
	  (*pstr)<<pc_iNoticeRevision;
  }
  
}

/* Get character name. */
const CTString &CPlayerCharacter::GetName(void) const 
{
  return pc_strName; 
};
const CTString CPlayerCharacter::GetNameForPrinting(void) const
{
  return "^o"+pc_strName+"^r"; 
}
/* Set character name. */
void CPlayerCharacter::SetName(CTString strName) 
{ 
  // limit string length to 20 characters not including decorated text control codes
  // strName.TrimRightNaked(20); !!!! needs checking
  pc_strName = strName; 
};

/* Assignment operator. */
CPlayerCharacter &CPlayerCharacter::operator=(const CPlayerCharacter &pcOther)
{
  ASSERT(this!=NULL && &pcOther!=NULL);
  pc_strName = pcOther.pc_strName;  
  memcpy(pc_aubGUID, pcOther.pc_aubGUID, PLAYERGUIDSIZE);
  memcpy(pc_aubAppearance, pcOther.pc_aubAppearance, MAX_PLAYERAPPEARANCE);
  pc_iPlayerIndex = pcOther.pc_iPlayerIndex;
//0217
  pc_iPlayerType = pcOther.pc_iPlayerType;
  pc_iNoticeRevision = pcOther.pc_iNoticeRevision;
  return *this;
};

/* Comparison operator. */
BOOL CPlayerCharacter::operator==(const CPlayerCharacter &pcOther) const
{
  for(INDEX i=0;i<PLAYERGUIDSIZE; i++) {
    if (pc_aubGUID[i] != pcOther.pc_aubGUID[i]) {
      return FALSE;
    }
  }
  return TRUE;
};

// stream operations
CTStream &operator<<(CTStream &strm, CPlayerCharacter &pc)
{
  pc.Write_t(&strm);
  return strm;
};
CTStream &operator>>(CTStream &strm, CPlayerCharacter &pc)
{
  pc.Read_t(&strm);
  return strm;
};
// message operations
CNetworkMessage &operator<<(CNetworkMessage &nm, CPlayerCharacter &pc)
{
  nm<<pc.pc_strName;
  nm.Write(pc.pc_aubGUID, PLAYERGUIDSIZE);
  nm.Write(pc.pc_aubAppearance, MAX_PLAYERAPPEARANCE);
  nm<<pc.pc_iPlayerIndex;
//0217
  nm<<pc.pc_iPlayerType;
  nm<<pc.pc_iNoticeRevision;
  return nm;
};
CNetworkMessage &operator>>(CNetworkMessage &nm, CPlayerCharacter &pc)
{
  nm>>pc.pc_strName;
  nm.Read(pc.pc_aubGUID, PLAYERGUIDSIZE);
  nm.Read(pc.pc_aubAppearance, MAX_PLAYERAPPEARANCE);
  nm>>pc.pc_iPlayerIndex;
//0217
  nm>>pc.pc_iPlayerType;
  nm>>pc.pc_iNoticeRevision;
  return nm;
};
