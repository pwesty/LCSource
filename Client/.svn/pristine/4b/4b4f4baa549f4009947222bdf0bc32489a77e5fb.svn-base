/*
 *  Game library
 *  Copyright (c) 1997-1999, CroTeam. All rights reserved.
 */

#include "stdafx.h"

extern CGame *_pGame;

/*
 * Default constructor
 */
CControls::CControls(void)
{
}

/*
 * Default destructor
 */
CControls::~CControls(void)
{
  // for each action in the original list
  FORDELETELIST(CButtonAction, ba_lnNode, ctrl_lhButtonActions, itButtonAction)
  {
    // delete button action
    delete( &*itButtonAction);
  }
}

// Assignment operator.
CControls &CControls::operator=(CControls &ctrlOriginal)
{
  // remove old button actions
  {FORDELETELIST(CButtonAction, ba_lnNode, ctrl_lhButtonActions, itButtonAction) {
    delete &*itButtonAction;
  }}
  // for each action in the original list
  {FOREACHINLIST(CButtonAction, ba_lnNode, ctrlOriginal.ctrl_lhButtonActions, itButtonAction) {
    // create and copy button action
    AddButtonAction() = *itButtonAction;
  }}

  // copy global settings
  ctrl_fSensitivity  = ctrlOriginal.ctrl_fSensitivity;
  ctrl_bInvertLook   = ctrlOriginal.ctrl_bInvertLook;
  ctrl_bSmoothAxes   = ctrlOriginal.ctrl_bSmoothAxes;

  return *this;
}

void CControls::SwitchToDefaults(void)
{
  // copy controls from initial player
  try
  {
    CControls ctrlDefaultControls;
//    ctrlDefaultControls.Load_t( CTString("Data\\Defaults\\InitialControls.ctl"));
    *this = ctrlDefaultControls;
  }
  catch( char *strError)
  {
    (void) strError;
  }
}

BOOL OptReadBoolFromLine(CTStream &strm, const CTString &strKeyword)
{
  // Remember position in stream 
  SLONG slStreamPos = strm.GetPos_t();

  CTString strLine;
  strm.GetLine_t(strLine);
  strLine.TrimSpacesLeft();
  // if line doesn't contain searched keyword
  if (!strLine.RemovePrefix(strKeyword)) {
    // Seek line back
    strm.SetPos_t(slStreamPos);
    return FALSE;
  }
  strLine.TrimSpacesLeft();
  strLine.TrimSpacesRight();
  if(strLine == "TRUE") {
    return TRUE;
  }
  return FALSE;
}

CTString OptReadTextLine(CTStream &strm, const CTString &strKeyword, BOOL bTranslate)
{
  // Remember position in stream 
  SLONG slStreamPos = strm.GetPos_t();

  CTString strLine;
  strm.GetLine_t(strLine);
  strLine.TrimSpacesLeft();
  if (!strLine.RemovePrefix(strKeyword)) {
    // Seek line back
    strm.SetPos_t(slStreamPos);
    return "";
  }
  strLine.TrimSpacesLeft();
  if (bTranslate) {
    strLine.RemovePrefix("TTRS");
  }
  strLine.TrimSpacesLeft();
  strLine.TrimSpacesRight();

  return strLine;
}

CTString ReadTextLine(CTStream &strm, const CTString &strKeyword, BOOL bTranslate)
{
  CTString strLine;
  strm.GetLine_t(strLine);
  strLine.TrimSpacesLeft();
  if (!strLine.RemovePrefix(strKeyword)) {
    return "???";
  }
  strLine.TrimSpacesLeft();
  if (bTranslate) {
    strLine.RemovePrefix("TTRS");
  }
  strLine.TrimSpacesLeft();
  strLine.TrimSpacesRight();

  return strLine;
}

BOOL GetStringFromLine(CTString strLine, CTString &strResult, const CTString &strKeyword, BOOL bTranslate)
{
  if(strLine.FindSubstr(strKeyword)>=0) {
    strLine.RemovePrefix(strKeyword);
    strLine.TrimSpacesLeft();
    if(bTranslate) {
      strLine.RemovePrefix("TTRS");
    }
    strLine.TrimSpacesLeft();
    strResult = strLine;
    return TRUE;
  }
  return FALSE;
}

void CControls::Load_t( CTFileName fnFile)
{
	char achrLine[1024] = {0,};
	char achrID[1024] = {0,};
  // open script file for reading
  CTFileStream strmFile;
  strmFile.Open_t( fnFile);				

  // if file can be opened for reading remove old button actions
  {FORDELETELIST(CButtonAction, ba_lnNode, ctrl_lhButtonActions, itButtonAction) {
    delete &*itButtonAction;
  }}

	do
  {
    achrLine[0] = 0;
    achrID[0] = 0;
    strmFile.GetLine_t( achrLine, 1024);
    sscanf( achrLine, "%s", achrID);
    // if name
    // if this is button action
    if(CTString(achrID) == "Control") {
      // create and read button action
      CButtonAction &baNew = AddButtonAction();

      CTString strName = "???";
      CTString strControl1;
      CTString strControl2;
      CTString strDevice1;
      CTString strDevice2;
      CTString strHidden;
      CTString strPressed;
      CTString strReleased;
      CTString strHeld;

      // read each line of one control
      for(INDEX iln=0;iln<32;iln++) {
        if(strmFile.AtEOF()) {
          break;
        }

        CTString strLine;
        strmFile.GetLine_t(strLine);
        strLine.TrimSpacesLeft();
        strLine.TrimSpacesRight();

        BOOL bRet = FALSE;
        bRet |= GetStringFromLine(strLine,strName,"Name:",TRUE);
        bRet |= GetStringFromLine(strLine,strControl1,"Control1:",FALSE);
        bRet |= GetStringFromLine(strLine,strControl2,"Control2:",FALSE);
        bRet |= GetStringFromLine(strLine,strDevice1,"Device1:"  ,FALSE);
        bRet |= GetStringFromLine(strLine,strDevice2,"Device2:"  ,FALSE);
        bRet |= GetStringFromLine(strLine,strHidden,"Hidden:"    ,FALSE);
        bRet |= GetStringFromLine(strLine,strPressed,"Pressed:"  ,FALSE);
        bRet |= GetStringFromLine(strLine,strReleased,"Released:",FALSE);
        bRet |= GetStringFromLine(strLine,strHeld,"Held:"        ,FALSE);

        // if none of strings were found
        if(!bRet) {
          break;
        }
      }

      if(strPressed.Length() == 0) {
        strPressed = strHeld;
      }

      baNew.ba_strName = strName;
      baNew.ba_ulFirstControl  = _pInput->GetControlGIDFromString(strDevice1,strControl1);
      baNew.ba_ulSecondControl = _pInput->GetControlGIDFromString(strDevice2,strControl2);
      baNew.ba_strCommandLineWhenPressed  = strPressed;
      baNew.ba_strCommandLineWhenReleased = strReleased;
      baNew.ba_strCommandLineWhenHeld     = strHeld;
      if(strHidden==CTString("TRUE")) {
        baNew.ba_bHidden = TRUE;
      } else {
        baNew.ba_bHidden = FALSE;
      }
    // if this is axis action
    } else if( CTString( achrID) == "GlobalInvertLook") {
      ctrl_bInvertLook = TRUE;
    } else if( CTString( achrID) == "GlobalDontInvertLook") {
      ctrl_bInvertLook = FALSE;
    } else if( CTString( achrID) == "GlobalSmoothAxes") {
      ctrl_bSmoothAxes = TRUE;
    } else if( CTString( achrID) == "GlobalDontSmoothAxes") {
      ctrl_bSmoothAxes = FALSE;
    } else if( CTString( achrID) == "GlobalSensitivity") {
      sscanf( achrLine, "GlobalSensitivity %g", &ctrl_fSensitivity);
    }
  }
	while( !strmFile.AtEOF());
}

void CControls::Save_t( CTFileName fnFile)
{
  CTString strLine;
  // create file
  CTFileStream strmFile;
  strmFile.Create_t( fnFile, CTStream::CM_TEXT);

  // write button actions
  FOREACHINLIST( CButtonAction, ba_lnNode, ctrl_lhButtonActions, itba)
  {

    strLine.PrintF("Control\n Name: TTRS %s",(const char*)itba->ba_strName);
    strmFile.PutLine_t( strLine);

    if(itba->ba_ulFirstControl!=CONTROL_NONE) {
      strLine.PrintF(" Control1: %s\n Device1: %s",
        (const char*)_pInput->GetControlName(itba->ba_ulFirstControl),
        (const char*)_pInput->GetInputDeviceName(GetInputDeviceID(itba->ba_ulFirstControl)));
      strmFile.PutLine_t( strLine);
    }

    if(itba->ba_ulSecondControl!=CONTROL_NONE) {
      strLine.PrintF(" Control2: %s\n Device2: %s",
        (const char*)_pInput->GetControlName(itba->ba_ulSecondControl),
        (const char*)_pInput->GetInputDeviceName(GetInputDeviceID(itba->ba_ulSecondControl)));
      strmFile.PutLine_t( strLine);
    }

    // if control is hidden
    if(itba->ba_bHidden) {
      strLine.PrintF(" Hidden: TRUE");
      strmFile.PutLine_t( strLine);
    }

    CButtonAction *pba = (CButtonAction*)itba;
    if(itba->ba_strCommandLineWhenPressed.Length()>0 && itba->ba_strCommandLineWhenPressed!=itba->ba_strCommandLineWhenHeld) {
      // export pressed command
      strLine.PrintF(" Pressed:  %s", itba->ba_strCommandLineWhenPressed);
      for( INDEX iLetter = 0; strLine[ iLetter] != 0; iLetter++)
      {
        // delete EOL-s
        if( (strLine[ iLetter] == 0x0d) || (strLine[ iLetter] == 0x0a) )
        {
          ((char*)(const char*)strLine)[ iLetter] = ' ';
        }
      }
      strmFile.PutLine_t( strLine);
    }

    // export released command
    if(itba->ba_strCommandLineWhenReleased.Length()>0) {
      strLine.PrintF(" Released: %s", itba->ba_strCommandLineWhenReleased);
      for( INDEX iLetter = 0; strLine[ iLetter] != 0; iLetter++)
      {
        // delete EOL-s
        if( (strLine[ iLetter] == 0x0d) || (strLine[ iLetter] == 0x0a) )
        {
          ((char*)(const char*)strLine)[ iLetter] = ' ';
        }
      }
      strmFile.PutLine_t( strLine);
    }

    if(itba->ba_strCommandLineWhenHeld.Length()>0) {
      // export held command
      strLine.PrintF(" Held: %s", itba->ba_strCommandLineWhenHeld);
      for( INDEX iLetter = 0; strLine[ iLetter] != 0; iLetter++)
      {
        // delete EOL-s
        if( (strLine[ iLetter] == 0x0d) || (strLine[ iLetter] == 0x0a) )
        {
          ((char*)(const char*)strLine)[ iLetter] = ' ';
        }
      }
      strmFile.PutLine_t( strLine);
    }
    strmFile.PutLine_t("\n");
  }

  // write global parameters
  if(ctrl_bInvertLook) {
    strmFile.PutLine_t( "GlobalInvertLook");
  } else {
    strmFile.PutLine_t( "GlobalDontInvertLook");
  }
  if(ctrl_bSmoothAxes) {
    strmFile.PutLine_t( "GlobalSmoothAxes");
  } else {
    strmFile.PutLine_t( "GlobalDontSmoothAxes");
  }
  strmFile.FPrintF_t("GlobalSensitivity %g\n", ctrl_fSensitivity);
}

// check if these controls use any joystick
BOOL CControls::UsesJoystick(void)
{
  // for each control
  FOREACHINLIST( CButtonAction, ba_lnNode, ctrl_lhButtonActions, itba) {
    CButtonAction &ba = *itba;
    CInputDevice *pidDevice1 = _pInput->GetInputDevice(GetInputDeviceID(ba.ba_ulFirstControl));
    CInputDevice *pidDevice2 = _pInput->GetInputDevice(GetInputDeviceID(ba.ba_ulSecondControl));
    if(pidDevice1!=NULL && pidDevice1->GetDeviceType()==DT_JOYSTICK) {
      return TRUE;
    }
    if(pidDevice2!=NULL && pidDevice2->GetDeviceType()==DT_JOYSTICK) {
      return TRUE;
    }
  }
  return FALSE;
}

