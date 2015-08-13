#include "stdh.h"

#include <Engine/Network/EMsgBuffer.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Compression.h>
#include <Engine/Math/Functions.h>
#include <Engine/Base/Memory.h>
#include <Engine/Base/Console.h>
#include <Engine/Math/Quaternion.h>
#include <Engine/Entities/PlayerCharacter.h>


extern INDEX net_bReportServerTraffic;
CEntityMessage _emEntityMessage;

// write a placement message - record position, orientation, translation and rotation
void CEntityMessage::WritePlacement(ULONG &ulEntityID,const CPlacement3D &plPlacement,const CPlacement3D &plSpeed)
{
  // initialize the message
  em_nmMessage.Reinit();
  // set message type
	em_ulType = EMT_SETPLACEMENT;
  // size is: two position vectors (FLOAT3D) and two angles (compressed to ULONGs)
	//em_uwSize = 3*sizeof(UWORD) + sizeof(ULONG) + sizeof(ULONG) + sizeof(ULONG);
  em_uwSize = sizeof(FLOAT3D) + sizeof(ULONG) + sizeof(FLOAT3D) + sizeof(ULONG);
	em_ulEntityID = ulEntityID;	

  // write position and orientation
/*  UWORD uwPositionX,uwPositionY,uwPositionZ;
  uwPositionX = PackFloat(4,11,1,plPlacement.pl_PositionVector(1));
  uwPositionY = PackFloat(4,11,1,plPlacement.pl_PositionVector(2));
  uwPositionZ = PackFloat(4,11,1,plPlacement.pl_PositionVector(3));
  em_nmMessage << uwPositionX << uwPositionY << uwPositionZ;
  //em_nmMessage << plPlacement.pl_PositionVector(1) << plPlacement.pl_PositionVector(2) << plPlacement.pl_PositionVector(3); 
  em_nmMessage << PackAngle(plPlacement.pl_OrientationAngle);

  // write translation and rotation
  ULONG ulPackedSpeed = PackVectorToULONG((FLOAT3D)plSpeed.pl_PositionVector); 
  em_nmMessage << ulPackedSpeed;
  //em_nmMessage << plSpeed.pl_PositionVector(1) << plSpeed.pl_PositionVector(2) << plSpeed.pl_PositionVector(3); 
  em_nmMessage << PackAngle(plSpeed.pl_OrientationAngle);*/
  
  // write position and orientation
  em_nmMessage << plPlacement.pl_PositionVector(1) << plPlacement.pl_PositionVector(2) << plPlacement.pl_PositionVector(3); 
  em_nmMessage << PackAngle(plPlacement.pl_OrientationAngle);

  // write translation and rotation
  em_nmMessage << plSpeed.pl_PositionVector(1) << plSpeed.pl_PositionVector(2) << plSpeed.pl_PositionVector(3); 
  em_nmMessage << PackAngle(plSpeed.pl_OrientationAngle);

  // report to console
  if (net_bReportServerTraffic) {
		CPrintF("Server: SETPLACEMENT:     EntityID: 0x%X     Position: %f, %f, %f     ",ulEntityID,plPlacement.pl_PositionVector(1),plPlacement.pl_PositionVector(2),plPlacement.pl_PositionVector(3));
		CPrintF("Angle: %f, %f, %f\n",plPlacement.pl_OrientationAngle(1),plPlacement.pl_OrientationAngle(2),plPlacement.pl_OrientationAngle(3));
  }


};


// read a placement message - position, orientation, translation and rotation
void CEntityMessage::ReadPlacement(ULONG &ulEntityID,CPlacement3D &plPlacement,CPlacement3D &plSpeed)
{
	ASSERT (em_ulType == EMT_SETPLACEMENT);
  ULONG ulPackedAngle;

  // go to the start of the message
  em_nmMessage.Rewind();
  ulEntityID = em_ulEntityID;

  /*// extract posotion and compressed orientation
  UWORD uwPositionX,uwPositionY,uwPositionZ;
  em_nmMessage >> uwPositionX >> uwPositionY >> uwPositionZ;
  plPlacement.pl_PositionVector(1) = UnpackFloat(4,11,1,uwPositionX);
  plPlacement.pl_PositionVector(2) = UnpackFloat(4,11,1,uwPositionY);
  plPlacement.pl_PositionVector(3) = UnpackFloat(4,11,1,uwPositionZ);
  //em_nmMessage >> plPlacement.pl_PositionVector(1) >> plPlacement.pl_PositionVector(2) >> plPlacement.pl_PositionVector(3); 
  em_nmMessage >> ulPackedAngle; UnpackAngle(ulPackedAngle,plPlacement.pl_OrientationAngle);

  // extract translation and rotation
  ULONG ulPackedSpeed;
  em_nmMessage >> ulPackedSpeed;
  UnpackVectorFromULONG(ulPackedSpeed,plSpeed.pl_PositionVector); 
  //em_nmMessage >> plSpeed.pl_PositionVector(1) >> plSpeed.pl_PositionVector(2) >> plSpeed.pl_PositionVector(3); 
  em_nmMessage >> ulPackedAngle; UnpackAngle(ulPackedAngle,plSpeed.pl_OrientationAngle);*/

  // extract posotion and compressed orientation
  em_nmMessage >> plPlacement.pl_PositionVector(1) >> plPlacement.pl_PositionVector(2) >> plPlacement.pl_PositionVector(3); 
  em_nmMessage >> ulPackedAngle; UnpackAngle(ulPackedAngle,plPlacement.pl_OrientationAngle);

  // extracto translation and rotation
  em_nmMessage >> plSpeed.pl_PositionVector(1) >> plSpeed.pl_PositionVector(2) >> plSpeed.pl_PositionVector(3); 
  em_nmMessage >> ulPackedAngle; UnpackAngle(ulPackedAngle,plSpeed.pl_OrientationAngle);


};


// write an entity create message - id, placement and class
void CEntityMessage::WriteEntityCreate(ULONG &ulEntityID,const CPlacement3D &plPlacement,UWORD &uwEntityClassID)
{
 
  // initialize the message
  em_nmMessage.Reinit();
  // set message type
	em_ulType = EMT_CREATE;
  // size is: one position vector (FLOAT3D), one compressed orientation (ULONG), and class id (UWORD)
	em_uwSize = sizeof(FLOAT3D) + sizeof(ULONG) + sizeof(UWORD);
	em_ulEntityID = ulEntityID;

  // write data to message
  em_nmMessage << plPlacement.pl_PositionVector(1) << plPlacement.pl_PositionVector(2) << plPlacement.pl_PositionVector(3); 
  em_nmMessage << PackAngle(plPlacement.pl_OrientationAngle);
  em_nmMessage << uwEntityClassID;

  // report to console
  if (net_bReportServerTraffic) {
		CPrintF("Server: CREATE:           EntityID: 0x%X     EntityClass: %ld",ulEntityID,uwEntityClassID);
		CPrintF("     Position: %f, %f, %f     ",plPlacement.pl_PositionVector(1),plPlacement.pl_PositionVector(2),plPlacement.pl_PositionVector(3));
		CPrintF("Angle: %f, %f, %f\n",plPlacement.pl_OrientationAngle(1),plPlacement.pl_OrientationAngle(2),plPlacement.pl_OrientationAngle(3));
  }


};

// read an entity create message - id, placement and class
void CEntityMessage::ReadEntityCreate(ULONG &ulEntityID,CPlacement3D &plPlacement,UWORD &uwEntityClassID)
{
	ASSERT (em_ulType == EMT_CREATE);
  ULONG ulPackedAngle;

  // go to the start of the message
  em_nmMessage.Rewind();
  ulEntityID = em_ulEntityID;

  // extract data from the message
  em_nmMessage >> plPlacement.pl_PositionVector(1) >> plPlacement.pl_PositionVector(2) >> plPlacement.pl_PositionVector(3); 
  em_nmMessage >> ulPackedAngle; UnpackAngle(ulPackedAngle,plPlacement.pl_OrientationAngle);
  em_nmMessage >> uwEntityClassID;
  
};


// write a copy entity message - ids of source and target entities, etc...
void CEntityMessage::WriteEntityCopy(ULONG &ulSourceEntityID,ULONG &ulTargetEntityID,const CPlacement3D &plPlacement,UBYTE ubWithDescentants)
{
  // initialize the message
  em_nmMessage.Reinit();
  // set message type
	em_ulType = EMT_COPY;
  // size is: one extra entity id (ULONG), position vector (FLOAT3D), compressed orientation (ULONG)
  //           
	em_uwSize = sizeof(ULONG) + sizeof(FLOAT3D) + sizeof(ULONG);
	em_ulEntityID = ulSourceEntityID;


  em_nmMessage << ulTargetEntityID;
  em_nmMessage << plPlacement.pl_PositionVector(1) << plPlacement.pl_PositionVector(2) << plPlacement.pl_PositionVector(3); 
  em_nmMessage << PackAngle(plPlacement.pl_OrientationAngle);

  if (net_bReportServerTraffic) {
		CPrintF("Server: COPY:           SourceEntityID: 0x%X     TargetEntityID: 0x%X     ",em_ulEntityID,ulTargetEntityID);
		CPrintF("     Position: %f, %f, %f     ",plPlacement.pl_PositionVector(1),plPlacement.pl_PositionVector(2),plPlacement.pl_PositionVector(3));
		CPrintF("Angle: %f, %f, %f\n",plPlacement.pl_OrientationAngle(1),plPlacement.pl_OrientationAngle(2),plPlacement.pl_OrientationAngle(3));
  }

};

// read a copy entity message 
void CEntityMessage::ReadEntityCopy(ULONG &ulSourceEntityID,ULONG &ulTargetEntityID,CPlacement3D &plPlacement,UBYTE ubWithDescentants)
{
	ASSERT (em_ulType == EMT_COPY);
  ULONG ulPackedAngle;

  em_nmMessage.Rewind();
	ulSourceEntityID = em_ulEntityID;


  em_nmMessage >> ulTargetEntityID;
  em_nmMessage >> plPlacement.pl_PositionVector(1) >> plPlacement.pl_PositionVector(2) >> plPlacement.pl_PositionVector(3); 
  em_nmMessage >> ulPackedAngle; UnpackAngle(ulPackedAngle,plPlacement.pl_OrientationAngle);


};


// write a destroy entity message - just the id of the entity to be destroyed
void CEntityMessage::WriteEntityDestroy(ULONG &ulEntityID)                  
{
  // initialize the message	
  em_nmMessage.Reinit();
  // set message type
	em_ulType = EMT_DESTROY;
  // no data carried
	em_uwSize = 0;
  em_ulEntityID = ulEntityID;

  // report to console
  if (net_bReportServerTraffic) {
	  CPrintF("Server: DESTROY:        EntityID: 0x%X\n",ulEntityID);
  }

};

// read a destroy entity message - entitie's id
void CEntityMessage::ReadEntityDestroy(ULONG &ulEntityID)
{
	ASSERT (em_ulType == EMT_DESTROY);
  
  // just set the id
  em_nmMessage.Rewind();
	ulEntityID = em_ulEntityID;

};

// write an entity event message
void CEntityMessage::WriteEntityEvent(ULONG &ulEntityID,SLONG &slEventCode,void* pvEventData,SLONG &slDataSize)
{
  // initialize the message
  em_nmMessage.Reinit();
  // set the message type
	em_ulType = EMT_EVENT;
  // size is: one event code (SLONG), slDataSize bytes of event data
	em_uwSize = sizeof(SLONG) + slDataSize;
	em_ulEntityID = ulEntityID;

  // write data to the message
  em_nmMessage << slEventCode;
  em_nmMessage.Write(pvEventData,slDataSize);

  // report to console
  if (net_bReportServerTraffic) {
	  CPrintF("Server: EVENT:            EntityID: 0x%X     EventCode: %ld     DataSize: %d\n",ulEntityID,slEventCode,slDataSize);
  }

};


// read an entity event message
void CEntityMessage::ReadEntityEvent(ULONG &ulEntityID,SLONG &slEventCode,void* pvEventData,SLONG &slDataSize)
{
	ASSERT (em_ulType == EMT_EVENT);

  // go to the start of the message
  em_nmMessage.Rewind();
  // how many bytes of event data is there
  slDataSize =  em_uwSize - sizeof(SLONG);

  // extract data from message
  em_nmMessage >> slEventCode;
  em_nmMessage.Read(pvEventData,slDataSize);

  ulEntityID = em_ulEntityID;

};

// write an initialization message
void CEntityMessage::WriteEntityInitialize(ULONG &ulEntityID,SLONG &slEventCode,void* pvEventData,SLONG &slDataSize)
{
  // initialize the message
  em_nmMessage.Reinit();
  // set message type
	em_ulType = EMT_INITIALIZE;
  // size is: one event code (ULONG), slDataSize bytes of event data
	em_uwSize = sizeof(SLONG) + slDataSize;
	em_ulEntityID = ulEntityID;

  // write data to the message
  em_nmMessage << slEventCode;
  em_nmMessage.Write(pvEventData,slDataSize);

  // report to console
  if (net_bReportServerTraffic) {
	  CPrintF("Server: INITIALIZE:            EntityID: 0x%X     EventCode: %ld     DataSize: %d\n",ulEntityID,slEventCode,slDataSize);
  }

};

// read an initialization message
void CEntityMessage::ReadEntityInitialize(ULONG &ulEntityID,SLONG &slEventCode,void* pvEventData,SLONG &slDataSize)
{
	ASSERT (em_ulType == EMT_INITIALIZE);

  // go to the start of the message
  em_nmMessage.Rewind();
  // how many bytes of event data is there
	slDataSize =  em_uwSize - sizeof(SLONG);

  // extract data from message
  em_nmMessage >> slEventCode;
  em_nmMessage.Read(pvEventData,slDataSize);

  ulEntityID = em_ulEntityID;
};



// write an add player message - player idex, character info and specified weapons and animator ids
void CEntityMessage::WriteAddPlayer(ULONG &ulEntityID,INDEX &iPlayerIndex,CPlayerCharacter &pcCharacter,ULONG &ulWeaponsID,ULONG &ulAnimatorID,INDEX iClient)
{
  UWORD uwStrLen1;
  
  // initialize the message
  em_nmMessage.Reinit();
  // get the lengths of player name and player's team name
  uwStrLen1 = pcCharacter.pc_strName.Length() + 1;

  // set the message type
  em_ulType = EMT_ADDPLAYER;
  // size is: player index (INDEX), PLAYERGUIDSIZE bytes of gui data, length of player's name (UBYTE), ubStrLen1 bytes to hold the name string
  //          length of team's name (UBYTE), ubStrLen2 bytes to hold the team name string, MAX_PLAYERAPPEARANCE of appearance data,
  //          two entity ids (ULONGS) - one for PlayerWeapons and one for PlayerAnimator, and one index (iClient)
	em_uwSize = sizeof(INDEX) + PLAYERGUIDSIZE + sizeof(UWORD) + uwStrLen1 + MAX_PLAYERAPPEARANCE + sizeof(ULONG) + sizeof(ULONG) + sizeof(INDEX);
	em_ulEntityID = ulEntityID;

  // write data to the message
  em_nmMessage << iPlayerIndex;

  em_nmMessage.Write(pcCharacter.pc_aubGUID,PLAYERGUIDSIZE);

  em_nmMessage << uwStrLen1;
  em_nmMessage.Write(pcCharacter.pc_strName.str_String,uwStrLen1);
  
  em_nmMessage.Write(pcCharacter.pc_aubAppearance,MAX_PLAYERAPPEARANCE);
  
  em_nmMessage << ulWeaponsID;
  em_nmMessage << ulAnimatorID;
  em_nmMessage << iClient;


  // report to console
  if (net_bReportServerTraffic) {
    CPrintF("Server: ADDPLAYER:     EntityID: 0x%X, Name: %s\n",em_ulEntityID,RemoveSpecialCodes(pcCharacter.GetNameForPrinting()));
  }
}


// read an add player message - player idex, character info and specified weapons and animator ids
void CEntityMessage::ReadAddPlayer(ULONG &ulEntityID,INDEX &iPlayerIndex,CPlayerCharacter &pcCharacter,ULONG &ulWeaponsID,ULONG &ulAnimatorID,INDEX &iClient)
{
  ASSERT (em_ulType == EMT_ADDPLAYER);
  UWORD uwStrLen;
  char* strString;

  // go to the start of the message 
  em_nmMessage.Rewind();
  // rad the player's index and gui data
  em_nmMessage >> iPlayerIndex;

  em_nmMessage.Read(pcCharacter.pc_aubGUID,PLAYERGUIDSIZE);

  // get the player's name
  em_nmMessage >> uwStrLen;
  ASSERT(uwStrLen >= 0);
  strString = new char[uwStrLen];
  em_nmMessage.Read(strString,uwStrLen);
  pcCharacter.pc_strName = strString;
  delete []strString;

  // read appearance
  em_nmMessage.Read(pcCharacter.pc_aubAppearance,MAX_PLAYERAPPEARANCE);

  // read weapons and animator id's
  em_nmMessage >> ulWeaponsID;
  em_nmMessage >> ulAnimatorID;
  em_nmMessage >> iClient;
  
  ulEntityID = em_ulEntityID;

}


// write the start animation message
void CEntityMessage::WriteStartAnim(ULONG &ulEntityID,INDEX &iNewModelAnim, ULONG &ulFlags)
{
  // initialize the message
  em_nmMessage.Reinit();
  // set the message type
	em_ulType = EMT_STARTANIM;
  // size is: one animation index(INDEX) and one set of animation flags (ULONG)
	em_uwSize = sizeof(INDEX) + sizeof(ULONG);
	em_ulEntityID = ulEntityID;

  // write date to message
  em_nmMessage << iNewModelAnim;
  em_nmMessage << ulFlags;  

  // report to console
  if (net_bReportServerTraffic) {
	  CPrintF("Server: STARTANIM:            EntityID: 0x%X     NewAnim: %ld     Flags: 0x%X\n",ulEntityID,iNewModelAnim,ulFlags);
  }

};


// read the start animation message
void CEntityMessage::ReadStartAnim(ULONG &ulEntityID,INDEX &iNewModelAnim, ULONG &ulFlags)
{
	ASSERT (em_ulType == EMT_STARTANIM);

  // go to the start of the message 
  em_nmMessage.Rewind();

  // read animation index and flags
  em_nmMessage >> iNewModelAnim;
  em_nmMessage >> ulFlags;

  ulEntityID = em_ulEntityID;

};

//! 플레이어의 정보를 업데이트 한다.->스트림에서 em_nmMessage로 옮긴다.서버 전용함수.
// write an update of player's data - data is prepared in advance by the server and is stored in 'ostr'
void CEntityMessage::WritePlayerUpdate(ULONG &ulEntityID,CNetworkMessage &ostr)
{
  // initialize the message
  em_nmMessage.Reinit();
  // set message type
	em_ulType = EMT_PLAYERUPDATE;
  em_ulEntityID = ulEntityID;

  // size of the message is one byte less than the size of 'ostr', because the first byte in a network message
  // is the identifier of that message's type, and it's not used here
  em_uwSize = ostr.nm_slSize-1;

  // copy data from ostr to this message
  em_nmMessage = ostr;  

  // report to console
  if (net_bReportServerTraffic) {
	  CPrintF("Server: PLAYERUPDATE:            EntityID: 0x%X    \n",ulEntityID);
  }

};


// read an update of player's data - data is not extracted from 'istr', player entity will do that for itself
void CEntityMessage::ReadPlayerUpdate(ULONG &ulEntityID,CNetworkMessage &istr)
{  
	ASSERT (em_ulType == EMT_PLAYERUPDATE);

  // go to the start of the message
  em_nmMessage.Rewind();

  // copy message data to 'istr'
  istr = em_nmMessage;

  ulEntityID = em_ulEntityID;

};



// write a notification of changed placement - this type of message is never sent to the client,
// it is inserted into the buffer by the entities when they execute a SetPlacement with a significant change
// in position or orientation (as defined by ser_fPositionTreshold and ser_fOrientationTreshold)
// this is so the server can query for needed dead reckoning updates only those entities 
// whose placement has changed, and does not have to go through all entities
// an update can be forced, using bForceUpdate
void CEntityMessage::WritePlacementNotify(ULONG &ulEntityID,UBYTE ubForceUpdate /*=FALSE*/)
{

  // initialize the message
  em_nmMessage.Reinit();
  // set the message type
	em_ulType = EMT_SETPLACEMENT_NOTIFY;
  // size is the size of a BOOL
	em_uwSize = sizeof(ubForceUpdate);
	em_ulEntityID = ulEntityID;	
  em_nmMessage << ubForceUpdate;

  // report to console
  if (net_bReportServerTraffic) {
		CPrintF("Server: SETPLACEMENT_NOTIFY:     EntityID: 0x%X  ",em_ulEntityID);
    if (ubForceUpdate) {
      CPrintF(" FORCED\n");
    } else {
      CPrintF(" not forced\n");
    }
  }

};

// read a notification of changed placement
void CEntityMessage::ReadPlacementNotify(ULONG &ulEntityID,UBYTE &ubForceUpdate)
{
	ASSERT (em_ulType == EMT_SETPLACEMENT_NOTIFY);

  // go to the start of the message
  em_nmMessage.Rewind();
  em_nmMessage >> ubForceUpdate;
  ulEntityID = em_ulEntityID;
};



// write a Ska animation message 
void CEntityMessage::WriteSka(ULONG &ulEntityID,UBYTE ubSkaCode,void* pvSkaData,SLONG &slDataSize)
{
  ASSERT (ubSkaCode != 0);

   // initialize the message
  em_nmMessage.Reinit();
  // set message type
	em_ulType = EMT_SKA_ANIM;
  em_ulEntityID = ulEntityID;

  // size of the message one UBYTE (Ska code) plus size of ska data to transport
  em_uwSize = sizeof (UBYTE) + slDataSize;

  // write the Ska code
  em_nmMessage << ubSkaCode;
  // copy data to this message
  em_nmMessage.Write(pvSkaData,slDataSize);

  // report to console
  if (net_bReportServerTraffic) {
	  CPrintF("Server: Ska Anim:            EntityID: 0x%X    Code: %d\n",ulEntityID,ubSkaCode);
  }

};

// read a Ska animation message
void CEntityMessage::ReadSka(ULONG &ulEntityID,UBYTE &ubSkaCode,void* pvSkaData,SLONG &slDataSize)
{
	ASSERT (em_ulType == EMT_SKA_ANIM);

  // go to the start of the message
  em_nmMessage.Rewind();

  slDataSize =  em_uwSize - sizeof(UBYTE);
  // read the Ska code
  em_nmMessage >> ubSkaCode;
  // extract data
  em_nmMessage.Read(pvSkaData,slDataSize);

  ulEntityID = em_ulEntityID;

};



// write a pause message
void CEntityMessage::WritePause(BOOL &bPaused,CTString &strPauser)
{
  UWORD uwStrLen = strPauser.Length() + 1;
  // initialize the message	
  em_nmMessage.Reinit();
  // set message type
	em_ulType = EMT_PAUSE;
  // all that is carried is the pauser's name
	em_uwSize = sizeof(uwStrLen) + uwStrLen;
  // put the paused state into the entity id (to waste less bandwidth)
  em_ulEntityID = bPaused;

  em_nmMessage << uwStrLen;
  em_nmMessage.Write(strPauser.str_String,uwStrLen);

  // report to console
  //if (net_bReportServerTraffic) {
    if (bPaused) {
	    CPrintF("Server: Pause game by %s\n",strPauser);
    } else {
      CPrintF("Server: Unpause game by %s\n",strPauser);
    }
  //}


};

// read a pause message
void CEntityMessage::ReadPause(BOOL &bPaused,CTString &strPauser)
{	
  ASSERT (em_ulType == EMT_PAUSE);
  UWORD uwStrLen;
  char* strString;

  // just set the id
  em_nmMessage.Rewind();
	bPaused = em_ulEntityID;;

  // get the pauser's name
  em_nmMessage >> uwStrLen;
  ASSERT(uwStrLen >= 0);
  strString = new char[uwStrLen];
  em_nmMessage.Read(strString,uwStrLen);
  strPauser = strString;
  delete []strString;

};


// write a destroy entity message - just the id of the entity to be destroyed
void CEntityMessage::WriteRemovePlayer(INDEX &iPlayer)
{
  // initialize the message	
  em_nmMessage.Reinit();
  // set message type
	em_ulType = EMT_REMPLAYER;
  // player index is carried in the entity ID
	em_uwSize = 0;
  em_ulEntityID = iPlayer;

  // report to console
  if (net_bReportServerTraffic) {
	  CPrintF("Server: REMPLAYER:        index: %ld\n",iPlayer);
  }

};

// read a destroy entity message - entitie's id
void CEntityMessage::ReadRemovePlayer(INDEX &iPlayer)
{
	ASSERT (em_ulType == EMT_REMPLAYER);
  
  // just set the id
  em_nmMessage.Rewind();
	iPlayer = em_ulEntityID;
};



// write a message to change a player's character
void CEntityMessage::WriteCharacterChange(ULONG &ulEntityID,CPlayerCharacter &pcCharacter)
{
  UWORD uwStrLen1;
  
  // initialize the message
  em_nmMessage.Reinit();
  // get the lengths of player name and player's team name
  uwStrLen1 = pcCharacter.pc_strName.Length() + 1;

  // set the message type
  em_ulType = EMT_CHANGECHARACTER;
  // size is: PLAYERGUIDSIZE bytes of gui data, length of player's name (UBYTE), ubStrLen1 bytes to hold the name string
  //          length of team's name (UBYTE), ubStrLen2 bytes to hold the team name string and MAX_PLAYERAPPEARANCE of appearance data 
	em_uwSize = PLAYERGUIDSIZE + sizeof(UWORD) + uwStrLen1 + MAX_PLAYERAPPEARANCE + sizeof(INDEX);//0217
	em_ulEntityID = ulEntityID;

  em_nmMessage.Write(pcCharacter.pc_aubGUID,PLAYERGUIDSIZE);

  em_nmMessage << uwStrLen1;
  em_nmMessage.Write(pcCharacter.pc_strName.str_String,uwStrLen1);

  em_nmMessage.Write(pcCharacter.pc_aubAppearance,MAX_PLAYERAPPEARANCE);
//0217
  em_nmMessage << pcCharacter.pc_iPlayerType;

  // report to console
  if (net_bReportServerTraffic) {
    CPrintF("Server: CHARACTERCHANGE:     EntityID: 0x%X, Name: %s\n",em_ulEntityID,RemoveSpecialCodes(pcCharacter.GetNameForPrinting()));
  }

};

// read a message to change a player's character
void CEntityMessage::ReadcharacterChange(ULONG &ulEntityID,CPlayerCharacter &pcCharacter)
{
  ASSERT (em_ulType == EMT_CHANGECHARACTER);
  UWORD uwStrLen;
  char* strString;

  // go to the start of the message 
  em_nmMessage.Rewind();

  em_nmMessage.Read(pcCharacter.pc_aubGUID,PLAYERGUIDSIZE);

  // get the player's name
  em_nmMessage >> uwStrLen;
  ASSERT(uwStrLen >= 0);
  strString = new char[uwStrLen];
  em_nmMessage.Read(strString,uwStrLen);
  pcCharacter.pc_strName = strString;
  delete []strString;

  // read appearance
  em_nmMessage.Read(pcCharacter.pc_aubAppearance,MAX_PLAYERAPPEARANCE);
//0217
  em_nmMessage >> pcCharacter.pc_iPlayerType;

  ulEntityID = em_ulEntityID;

};


// write a Ska animation message 
void CEntityMessage::WriteSound(ULONG &ulEntityID,UBYTE ubSoundCode,void* pvSoundData,SLONG &slDataSize)
{
  ASSERT (ubSoundCode != 0);

   // initialize the message
  em_nmMessage.Reinit();
  // set message type
	em_ulType = EMT_SOUND;
  em_ulEntityID = ulEntityID;

  // size of the message one UBYTE (Ska code) plus size of ska data to transport
  em_uwSize = sizeof (UBYTE) + slDataSize;

  // write the Ska code
  em_nmMessage << ubSoundCode;
  // copy data to this message
  em_nmMessage.Write(pvSoundData,slDataSize);

  // report to console
  if (net_bReportServerTraffic) {
	  CPrintF("Server: Sound:            EntityID: 0x%X    Code: %d\n",ulEntityID,ubSoundCode);
  }

};

// read a Ska animation message
void CEntityMessage::ReadSound(ULONG &ulEntityID,UBYTE &ubSoundCode,void* pvSoundData,SLONG &slDataSize)
{
	ASSERT (em_ulType == EMT_SOUND);

  // go to the start of the message
  em_nmMessage.Rewind();

  slDataSize =  em_uwSize - sizeof(UBYTE);
  // read the Ska code
  em_nmMessage >> ubSoundCode;
  // extract data
  em_nmMessage.Read(pvSoundData,slDataSize);

  ulEntityID = em_ulEntityID;

};


//! 기본위치값을 em_nmMessage에 쓴다. 서버전용
// write a base placement used for compression of entity placements
void CEntityMessage::WriteBasePosition(FLOAT3D &vBasePosition)
{
 
  // initialize the message
  em_nmMessage.Reinit();
  // set message type
	em_ulType = EMT_BASEPLACEMENT;
  // size is: one position vector (FLOAT3D), one compressed orientation (ULONG), and class id (UWORD)
	em_uwSize = sizeof(FLOAT3D);
	em_ulEntityID = 0;

  // write data to message
  em_nmMessage << vBasePosition(1) << vBasePosition(2) << vBasePosition(3);

  // report to console
  if (net_bReportServerTraffic) {
		CPrintF("Server: BASEPLACEMENT:           Position: %f, %f, %f\n",vBasePosition(1),vBasePosition(2),vBasePosition(3));
  }

};


// read a base placement used for compression of entity placements
void CEntityMessage::ReadBasePosition(FLOAT3D &vBasePosition)
{
	ASSERT (em_ulType == EMT_BASEPLACEMENT);

  // go to the start of the message
  em_nmMessage.Rewind();

  // extract data from the message
  em_nmMessage >> vBasePosition(1) >> vBasePosition(2) >> vBasePosition(3);
  
};



// write a message to add a new string to the ska string table
void CEntityMessage::WriteAddSkaString(INDEX iIndex,const CTString &strString)
{
  //ASSERT(strString.Length()<=255);
  UBYTE ubStrLen = strString.Length() + 1;
  // initialize the message	
  em_nmMessage.Reinit();
  // set message type
	em_ulType = EMT_ADDSKASTRING;
  // all that is carried is the pauser's name
	em_uwSize = sizeof(ubStrLen) + ubStrLen;
  // put the paused state into the entity id (to waste less bandwidth)
  em_ulEntityID = iIndex;

  em_nmMessage << ubStrLen;
  em_nmMessage.Write(strString.str_String,ubStrLen);

  // report to console
  if (net_bReportServerTraffic) {
    CPrintF("Server: String  '%s' added to ska table, ID is:  %ld\n",(const char*)strString,iIndex);
  }

};


// read a message to add a new string to the ska string table
void CEntityMessage::ReadAddSkaString(INDEX &iIndex,CTString &strString)
{	
  ASSERT (em_ulType == EMT_ADDSKASTRING);
  UBYTE ubStrLen;
  char* strTempString;

  // just set the id
  em_nmMessage.Rewind();
	iIndex = em_ulEntityID;

  // get the pauser's name
  em_nmMessage >> ubStrLen;
  ASSERT(ubStrLen >= 0);
  strTempString = new char[ubStrLen];
  em_nmMessage.Read(strTempString,ubStrLen);
  strString = strTempString;
  delete []strTempString;

};



// write a PlayerAction - used for recording demos
void CEntityMessage::WritePlayerAction(ULONG &ulEntityID,CPlayerAction &paAction)
{
  // initialize the message
  em_nmMessage.Reinit();
  // set message type
	em_ulType = EMT_PLAYERACTION;
  em_ulEntityID = ulEntityID;
  em_nmMessage << paAction;

  // size of the message is one byte less than the size of 'ostr', because the first byte in a network message
  // is the identifier of that message's type, and it's not used here
  em_uwSize = em_nmMessage.nm_slSize-1;

  // report to console
  if (net_bReportServerTraffic) {
	  CPrintF("Server: PLAYERACTION:            EntityID: 0x%X    \n",ulEntityID);
  }

};


// read a PlayerAction - used for demo playback
void CEntityMessage::ReadPlayerAction(ULONG &ulEntityID,CPlayerAction &paAction)
{  
	ASSERT (em_ulType == EMT_PLAYERACTION);

  // go to the start of the message
  em_nmMessage.Rewind();

  em_nmMessage >> paAction;

  ulEntityID = em_ulEntityID;

};







/*
*
* CEMsgBuffer - entity message buffer
*
*/


// constructor
CEMsgBuffer::CEMsgBuffer()
{				  
  // initialize marker indices
	emb_uwNumTickMarkers    = 0;
	emb_iFirstTickMarker    = 0;
  emb_iCurrentTickMarker  = 0;

  // reset all tick markers
	for (int i=0;i<MAX_TICKS_KEPT;i++) {
		emb_atmTickMarkers[i].tm_slTickOffset = -1;
		emb_atmTickMarkers[i].tm_tmTickTime    = -1;
		emb_atmTickMarkers[i].tm_ubAcknowledgesExpected = 0;
	}

};


// destructor - nothing to do
CEMsgBuffer::~CEMsgBuffer ()
{
};


// initialize the message buffer
void CEMsgBuffer::Clear(void)
{
  // initialize marker indices
  emb_uwNumTickMarkers    = 0;
	emb_iFirstTickMarker    = 0;
  emb_iCurrentTickMarker  = 0;

  // reset all tick markers
	for (int i=0;i<MAX_TICKS_KEPT;i++) {
		emb_atmTickMarkers[i].tm_slTickOffset = -1;
		emb_atmTickMarkers[i].tm_tmTickTime    = -1;
		emb_atmTickMarkers[i].tm_ubAcknowledgesExpected = 0;
	}

  // set buffer offsets to the start of buffer
  bu_slWriteOffset = 0;
  bu_slReadOffset = 0;

  // the whole buffer is empty
  bu_slFree = bu_slSize;

};

//! 버퍼에 메시지를 쓴다. 서버전용.
// write a message to the bufffer
BOOL CEMsgBuffer::WriteMessage(CEntityMessage  &emEntityMessage, BOOL bExpand /* = TRUE */) 
{
  // a tick must have started before any messages are generated
  ASSERT (emb_uwNumTickMarkers > 0);
  ASSERT(bu_slFree>=0 && bu_slFree<=bu_slSize);
  // entity id must fit into 24 bits
  ASSERT(emEntityMessage.em_ulEntityID < 0x10000000);
	ULONG ulTemp;

  int iTickMarker;  

  // if expanding the buffer is not allowed
  if (!bExpand) {
    // check if there is enough free space in the buffer for this message
    SLONG slSize = sizeof(ulTemp) + sizeof(emEntityMessage.em_tmMessageTime) + sizeof(emEntityMessage.em_uwSize);
    slSize += emEntityMessage.em_nmMessage.nm_slSize-1;
    // and if there isn't, return
    if (slSize > bu_slFree) {
      return FALSE;
    }
  }

  // combine the entity id (24 bits) and message type (8 bits) into one ULONG (32 bits)
	ulTemp = emEntityMessage.em_ulEntityID | (emEntityMessage.em_ulType << 24);
  // write ths to buffer 
	WriteBytes(&(ulTemp),sizeof(ULONG));	
  // write message time
  WriteBytes(&(emEntityMessage.em_tmMessageTime),sizeof(emEntityMessage.em_tmMessageTime));
  // write message data size
	WriteBytes(&(emEntityMessage.em_uwSize),sizeof(emEntityMessage.em_uwSize));
  // if any data present, write it into the buffer
	if (emEntityMessage.em_uwSize > 0) {
		WriteBytes(emEntityMessage.em_nmMessage.nm_pubMessage+1,emEntityMessage.em_nmMessage.nm_slSize-1);
	}

  // get the last tick marker
  iTickMarker = emb_iCurrentTickMarker-1;
  if (iTickMarker < 0) iTickMarker += MAX_TICKS_KEPT;
  // and increase the count of messages for it
  emb_atmTickMarkers[iTickMarker].tm_uwNumMessages++;

  return TRUE;
};



// read a message from the bufffer
int CEMsgBuffer::ReadMessage(CEntityMessage  &emEntityMessage)
{
	ULONG ulTemp;
  UBYTE* pubData;
  ASSERT (bu_slReadOffset <= bu_slWriteOffset);

  // if buffer is empty, return
  if (bu_slReadOffset == bu_slWriteOffset) {
    return EMB_ERR_BUFFER_EMPTY;
  }


  // read combined entity id and message type
	ReadBytes(&(ulTemp),sizeof(ULONG));
  // extract message type
	emEntityMessage.em_ulType = ulTemp >> 24;
  // extract entity id
	emEntityMessage.em_ulEntityID = ulTemp & 0x007FFFFF;
  // read time of the message
  ReadBytes(&(emEntityMessage.em_tmMessageTime),sizeof(emEntityMessage.em_tmMessageTime));
  // read the size of message data
	ReadBytes(&(emEntityMessage.em_uwSize),sizeof(emEntityMessage.em_uwSize));
  // initialize the message 
  emEntityMessage.em_nmMessage.Reinit();
  // if there is any message data to read, copy it from the buffer to the message
	if (emEntityMessage.em_uwSize > 0) {
    pubData = new UBYTE[emEntityMessage.em_uwSize];
		ReadBytes(pubData,emEntityMessage.em_uwSize);
    emEntityMessage.em_nmMessage.Write(pubData,emEntityMessage.em_uwSize);
    delete []pubData;
	}

  return EMB_SUCCESS_OK;
};


// peek at a message at a specified offset in the buffer - this does not advance the buffer beyond the read message
int CEMsgBuffer::PeekMessageAtOffset(CEntityMessage &emEntityMessage,SLONG &slTickOffset)
{
  ASSERT(slTickOffset >= 0 && slTickOffset <= bu_slSize);
  ULONG ulTemp;
  UBYTE* pubData;

  // get the combined entity id and message type
  PeekBytesAtOffset(&(ulTemp),sizeof(ULONG),slTickOffset);
  //! 메모리 버퍼에서 가져온 값을 emEntityMessage에 저장한다.
  // extract message type
	emEntityMessage.em_ulType = ulTemp >> 24;
  // extract entity id
	emEntityMessage.em_ulEntityID = ulTemp & 0x007FFFFF;
  // read time of the message
  PeekBytesAtOffset(&(emEntityMessage.em_tmMessageTime),sizeof(emEntityMessage.em_tmMessageTime),slTickOffset);
  // read the size of message data
	PeekBytesAtOffset(&(emEntityMessage.em_uwSize),sizeof(emEntityMessage.em_uwSize),slTickOffset);
  // initialize the message 
  emEntityMessage.em_nmMessage.Reinit();
  // if there is any message data to read, copy it from the buffer to the message
	if (emEntityMessage.em_uwSize > 0) {
    pubData = new UBYTE[emEntityMessage.em_uwSize];
		PeekBytesAtOffset(pubData,emEntityMessage.em_uwSize,slTickOffset);
    emEntityMessage.em_nmMessage.Write(pubData,emEntityMessage.em_uwSize);
    delete []pubData;
	}

  return EMB_SUCCESS_OK;
};




// read data for one tick - does not advance the read offset since access is random, not sequential (we can ask for any tick in the buffer,
// not necceesserily the first one)
int  CEMsgBuffer::ReadTick(float fTickTime,const void *pv, SLONG &slSize)
{
  ASSERT (slSize>=0);
  ASSERT (pv != NULL);
  ASSERT (fTickTime >= 0);
  int   iErr;
  INDEX iTickIndex,iNextTickIndex;
  
  
  // find the index of the specified tick in the buffer
  iErr = GetTickIndex(fTickTime,iTickIndex);

  // if it's not there, report an error
  if (iErr != EMB_SUCCESS_OK) {
    return iErr;
  }

  // if this is the index of the last tick marker, return zero bytes (this is because writing data this tick 
  // has not yet finished, so we should not process any of it's data)
  if (iTickIndex >= (emb_iFirstTickMarker + emb_uwNumTickMarkers - 1)) {
    slSize=0;
    return EMB_SUCCESS_OK;
  }

  // get the index of the next tick
  iNextTickIndex = (iTickIndex + 1) % MAX_TICKS_KEPT;
  // calculate the number of bytes to be read
  SLONG slTickSize = emb_atmTickMarkers[iNextTickIndex].tm_slTickOffset - emb_atmTickMarkers[iTickIndex].tm_slTickOffset;
  // if buffer is not wrapping 
  if ((slTickSize > 0) || ((slTickSize == 0) && (emb_atmTickMarkers[iTickIndex].tm_uwNumMessages == 0))) {
    // if not enough space in the destination
    if (slSize < slTickSize) {
      // report it
      return EMB_ERR_BUFFER_TOO_SMALL;
    }
    // set the ammount of data read
    slSize = slTickSize;
    // copy data from the buffer to the destination
    memcpy((UBYTE*)pv,bu_pubBuffer+emb_atmTickMarkers[iTickIndex].tm_slTickOffset,slTickSize);          
  // if buffer is wrapping
  } else {
    // if not enough space in the destination
    if (slSize < (bu_slSize - emb_atmTickMarkers[iTickIndex].tm_slTickOffset) + emb_atmTickMarkers[iNextTickIndex].tm_slTickOffset-1) {
      // report it
      return EMB_ERR_BUFFER_TOO_SMALL;
    }
    // set the ammount of data read
    slSize = (bu_slSize - emb_atmTickMarkers[iTickIndex].tm_slTickOffset) + emb_atmTickMarkers[iNextTickIndex].tm_slTickOffset;
    // copy data from the start of this tick to the end of the buffer
    memcpy((UBYTE*)pv,bu_pubBuffer+emb_atmTickMarkers[iTickIndex].tm_slTickOffset,bu_slSize - emb_atmTickMarkers[iTickIndex].tm_slTickOffset);
    // copy datafrom the start of the buffer to the start of next tick
    memcpy(((UBYTE*)pv)+bu_slSize - emb_atmTickMarkers[iTickIndex].tm_slTickOffset,bu_pubBuffer,emb_atmTickMarkers[iNextTickIndex].tm_slTickOffset);
  }

  // report success
  return EMB_SUCCESS_OK;

};


// write tick data from source memory to the buffer
void CEMsgBuffer::WriteTick(float tm_tmTickTime,const void *pv, SLONG slSize)
{
  ASSERT(slSize>=0 && pv!=NULL);
  ASSERT(bu_slFree>=0 && bu_slFree<=bu_slSize);
  // make a new tick marker with a specified tick time
  StartNewTick(tm_tmTickTime);
  // copy data to buffer
  WriteBytes(pv,slSize);
};


// set the first buffered tick to the specified tick time
int CEMsgBuffer::MoveToStartOfTick(float fTickTime) 
{
  int iErr;
  INDEX iTickIndex;

  // if fTickTime is -1 (all ticks empty) clear the buffer
  if (fTickTime<0) {
    Clear();
  }

  // check that this tick time is greater or equal than the time of first tick in the buffer
  ASSERT(fTickTime >= emb_atmTickMarkers[emb_iFirstTickMarker].tm_tmTickTime);

  // get the index of this tick marker
  iErr = GetTickIndex(fTickTime,iTickIndex);
  if (iErr != EMB_SUCCESS_OK) {
    return iErr;
  }  


  // if tick indices are not wrapping
  if (iTickIndex >= emb_iFirstTickMarker) { 
    // calculate the number of tick markers left in the buffer 
    emb_uwNumTickMarkers -= iTickIndex - emb_iFirstTickMarker;
  // if wrapping
  } else {
    emb_uwNumTickMarkers -= iTickIndex + (MAX_TICKS_KEPT - emb_iFirstTickMarker);
  }

  // set the index of the first tick marker
  emb_iFirstTickMarker = iTickIndex;

  // if data buffer is not wrapping
  if (bu_slReadOffset <= emb_atmTickMarkers[iTickIndex].tm_slTickOffset) {
    // calculate the new ammount of free bytes in the buffer
    bu_slFree += emb_atmTickMarkers[iTickIndex].tm_slTickOffset - bu_slReadOffset;
  // if data buffer is wrapping
  } else {
    bu_slFree += (bu_slSize - bu_slReadOffset) + emb_atmTickMarkers[iTickIndex].tm_slTickOffset;
  }

  // set new data read offset
  bu_slReadOffset = emb_atmTickMarkers[iTickIndex].tm_slTickOffset;
 

  return EMB_SUCCESS_OK;

};



// mark the start of a new tick with a specified tick time
int CEMsgBuffer::StartNewTick(float fTickTime)
{
	
  ASSERT(bu_slFree>=0 && bu_slFree<=bu_slSize);
  // if the maximum number of bufered ticks has been reached, report it and return
	if (emb_uwNumTickMarkers >= MAX_TICKS_KEPT) {
		return EMB_ERR_MAX_TICKS;
	}

  // set the tick time
  emb_atmTickMarkers[emb_iCurrentTickMarker].tm_tmTickTime = fTickTime;
  // remember the offset where we started writing this tick from
	emb_atmTickMarkers[emb_iCurrentTickMarker].tm_slTickOffset = bu_slWriteOffset;
  // clear tick stats
	emb_atmTickMarkers[emb_iCurrentTickMarker].tm_ubAcknowledgesExpected = 0;
  emb_atmTickMarkers[emb_iCurrentTickMarker].tm_uwNumMessages = 0;
	
  // update marker stats
  emb_uwNumTickMarkers++;
  emb_iCurrentTickMarker++;
  emb_iCurrentTickMarker %= MAX_TICKS_KEPT;

	return EMB_SUCCESS_OK;
};


// get the marker index for specified time
int CEMsgBuffer::GetTickIndex(float fTickTime,INDEX &iTickIndex)
{
  INDEX iTickMarker;

  // try to find a matching tick marker
	for (int i=0;i<emb_uwNumTickMarkers;i++) {
    iTickMarker = (i + emb_iFirstTickMarker) % MAX_TICKS_KEPT;
		if (fabs(emb_atmTickMarkers[iTickMarker].tm_tmTickTime - fTickTime) < _pTimer->TickQuantum/2) {
      // if found, return it's index
      iTickIndex = iTickMarker;			
			return EMB_SUCCESS_OK;
		}
	}

  // if not found return -1, and report an error
  iTickIndex = -1;
  return EMB_ERR_NOT_IN_BUFFER;
};



// get the buffer offset for a specified tick time
int CEMsgBuffer::GetTickOffset(float fTickTime,SLONG &slTickOffset)
{
  INDEX iTickMarker;
  
  // try to find a matching tick marker
	for (int i=0;i<emb_uwNumTickMarkers;i++) {  
    iTickMarker = (i + emb_iFirstTickMarker) % MAX_TICKS_KEPT;
		if (fabs(emb_atmTickMarkers[iTickMarker].tm_tmTickTime - fTickTime) < _pTimer->TickQuantum/2) {
      // if found, return it's offset
      slTickOffset = emb_atmTickMarkers[iTickMarker].tm_slTickOffset;			
			return EMB_SUCCESS_OK;
		}
	}

  // if not found return -1, and report an error
  slTickOffset = -1;
  return EMB_ERR_NOT_IN_BUFFER;
};



// find the first tick time after some specified time
int CEMsgBuffer::GetNextTickTime(float fTickTime,float &fNextTickTime)
{
  INDEX iTickIndex;
  int   iErr;

  if (emb_uwNumTickMarkers == 0) {
    return EMB_ERR_NOT_IN_BUFFER;
  }

  // if specified time is negative (not initialized), return the first time bufffered
  if (fTickTime < 0) {
    if (emb_uwNumTickMarkers > 0) {
      fNextTickTime = emb_atmTickMarkers[emb_iFirstTickMarker].tm_tmTickTime;
      return EMB_SUCCESS_OK;
    }
  }

  // if time is older than the time of the first tick in the buffer
  if (fTickTime < (emb_atmTickMarkers[emb_iFirstTickMarker].tm_tmTickTime-(_pTimer->TickQuantum/2))) {
    // return that time
    fNextTickTime = emb_atmTickMarkers[emb_iFirstTickMarker].tm_tmTickTime;
    return EMB_SUCCESS_OK;
  };

  // get the index of tick marker for the specified time
  iErr = GetTickIndex(fTickTime,iTickIndex);
  // if it does not exist, report an error
  if (iErr != EMB_SUCCESS_OK) {
    return iErr;
  }

  // get the index of the next marker in the buffer
  iTickIndex = (iTickIndex + 1) % MAX_TICKS_KEPT;

  // return it's tick time
  fNextTickTime = emb_atmTickMarkers[iTickIndex].tm_tmTickTime;

  return EMB_SUCCESS_OK;
};


// mark that an acknowledge is required for this tick (i.e. reference counting)
int CEMsgBuffer::RequestTickAcknowledge(float fTickTime,UBYTE ubNumAcknowledges) 
{
  ASSERT (fTickTime >= 0);
  INDEX iTickIndex;
  int   iErr;

  // if this tick really is in the buffer
  iErr = GetTickIndex(fTickTime,iTickIndex);
  if (iErr == EMB_SUCCESS_OK) {
    // increase it's count of expected acknowledges and return
    emb_atmTickMarkers[iTickIndex].tm_ubAcknowledgesExpected += ubNumAcknowledges;
		return EMB_SUCCESS_OK;
  }
  // else, report an error
  return iErr;
};


// acknowledge all ticks between given tick times
int CEMsgBuffer::ReceiveTickAcknowledge(float fLastAck, float fTickTime) 
{
  ASSERT (fTickTime >= 0);
//  ASSERT ((fLastAck >= 0) && (fLastAck<fTickTime));
  float fFirstToAck;
  INDEX iTickIndex,iFirstToAck;
  INDEX iFirst;
  int   iErr;
  int   iNumMark = emb_uwNumTickMarkers;

  // at what tick time will we start
  iErr = GetNextTickTime(fLastAck,fFirstToAck);
  if (iErr == EMB_ERR_NOT_IN_BUFFER) {
    return EMB_SUCCESS_OK;
  }

  // find the index of this tick in the buffer
  iErr = GetTickIndex(fFirstToAck,iFirstToAck);
  if (iErr != EMB_SUCCESS_OK) {
    iFirstToAck = emb_iFirstTickMarker;
  }

  // get the index of the last tick marker
  iErr = GetTickIndex(fTickTime,iTickIndex);
  if (iErr == EMB_SUCCESS_OK) {
    // start from the first tick marker
    iFirst = iFirstToAck;
    // if the first tick marker is expecting an acknowledge, decrease it's expected acknowledges count
    if (emb_atmTickMarkers[iFirst].tm_ubAcknowledgesExpected>0) emb_atmTickMarkers[iFirst].tm_ubAcknowledgesExpected--;
    // for each tick after from the first to the specified tick
    while (iFirst!=iTickIndex) {
      // move to te next tick
      iFirst++;
      iFirst%=MAX_TICKS_KEPT;
      // if it is expecting an acknowledge, decrease it's expected acknowledges count
      if (emb_atmTickMarkers[iFirst].tm_ubAcknowledgesExpected>0) emb_atmTickMarkers[iFirst].tm_ubAcknowledgesExpected--;
    }
    
    // start from the first tick again
    iFirst = iFirstToAck;
    // if the first tick is not expecting any more acknowledges, and there are some ticks in the buffer
    if (emb_atmTickMarkers[iFirst].tm_ubAcknowledgesExpected == 0 && iNumMark != 0) {
      // while that is so
      while (emb_atmTickMarkers[iFirst].tm_ubAcknowledgesExpected == 0 && iNumMark != 0) {
        if (iNumMark > 0) {
          // reset tick marker data
          emb_atmTickMarkers[iFirst].tm_tmTickTime = -1;
          emb_atmTickMarkers[iFirst].tm_slTickOffset = -1;
          emb_atmTickMarkers[iFirst].tm_uwNumMessages = 0;
          // decrease tick marker count
          iNumMark--;
          // advance the index of the first tick (we have just deleted it)
          iFirst++;
          iFirst %= MAX_TICKS_KEPT;
        }
      }
      // when done, advance the buffer to the start of the (new) first tick
      MoveToStartOfTick(emb_atmTickMarkers[iFirst].tm_tmTickTime);
    }

    // return success
		return EMB_SUCCESS_OK;
  }
  // if the specified tick time is not int he buffer, return the eror
  return iErr;
};



// expand buffer to be given number of bytes in size
void CEMsgBuffer::Expand(SLONG slNewSize)
{
  ASSERT(slNewSize>0);
  ASSERT(bu_slSize>=0);
  ASSERT(bu_slFree>=0 && bu_slFree<=bu_slSize);
  // if not already allocated
  if (bu_slSize==0) {
    // allocate a new empty buffer
    ASSERT(bu_pubBuffer==NULL);
    bu_pubBuffer = (UBYTE*)AllocMemory(slNewSize);
    bu_slWriteOffset = 0;
    bu_slReadOffset = 0;
    bu_slFree = slNewSize;
    bu_slSize = slNewSize;

  // if already allocated
  } else {
    // this should never be called if we have enough space already
    ASSERT(slNewSize>bu_slSize);
    SLONG slSizeDiff = slNewSize-bu_slSize;
    ASSERT(bu_pubBuffer!=NULL);
    // grow buffer
    GrowMemory((void**)&bu_pubBuffer, slNewSize);


    // if buffer is currently wrapping
    if (bu_slReadOffset>bu_slWriteOffset||bu_slFree==0) {
      // move part at the end of old buffer to the end of new memory
      memmove(bu_pubBuffer+bu_slReadOffset+slSizeDiff, bu_pubBuffer+bu_slReadOffset,bu_slSize-bu_slReadOffset);
      // update tick marker offsets to properly point to new data
 			for (int i=0;i<MAX_TICKS_KEPT;i++) {
				if (emb_atmTickMarkers[i].tm_slTickOffset >= bu_slReadOffset) {
					emb_atmTickMarkers[i].tm_slTickOffset += slSizeDiff;
				}
			}
      // set the proper position of data start
      bu_slReadOffset+=slSizeDiff;
    }
    
    ASSERT(bu_slFree>=0 && bu_slFree<=bu_slSize);
    // update ammound of free space and set new buffer size
    bu_slFree += slNewSize-bu_slSize;
    bu_slSize = slNewSize;

    // verify that all is well
    ASSERT(bu_slReadOffset>=0 && bu_slReadOffset<bu_slSize);
    ASSERT(bu_slFree>=0 && bu_slFree<=bu_slSize);
  }
}



//! Entity메시지를 버퍼 메모리에 바이트로 쓴다.
// write bytes to buffer 
void CEMsgBuffer::WriteBytes(const void *pv, SLONG slSize)
{
	BOOL bWraping = FALSE;
	SLONG slOldReadOffset = bu_slReadOffset;

  ASSERT(bu_slFree>=0 && bu_slFree<=bu_slSize);
	ASSERT(slSize>=0 && pv!=NULL);
  // if there is nothing to write
  if (slSize==0) {
    // do nothing
    return;
  }

  // if there is not enough free space
  if (bu_slFree<slSize) {
		SLONG slSizeDiff;
		SLONG slNewSize;
    // find how much memory we need
		slNewSize = bu_slSize + ((slSize-bu_slFree + bu_slAllocationStep - 1) / bu_slAllocationStep) * bu_slAllocationStep;
		slSizeDiff =  slNewSize - bu_slSize;

    // expand the buffer
    Expand(slNewSize);

    // verify that expansion has created enough free space
    ASSERT(bu_slFree>=slSize);
  }

  UBYTE *pub = (UBYTE*)pv;

  // write part of block at the end of buffer
  SLONG slSizeEnd = __min(bu_slSize-bu_slWriteOffset, slSize);
  memcpy(bu_pubBuffer+bu_slWriteOffset, pub, slSizeEnd);
  // if there is anything left, write it to the begining of the data buffer (buffer is now wrapping)
  pub+=slSizeEnd;
  memcpy(bu_pubBuffer, pub, slSize-slSizeEnd);
  // set write offset and adjust the ammount of free space
  bu_slWriteOffset+=slSize;
  bu_slWriteOffset%=bu_slSize;
  bu_slFree-=slSize;

  // verify that all is well
  ASSERT(bu_slWriteOffset>=0 && bu_slWriteOffset<bu_slSize);
  ASSERT(bu_slFree>=0 && bu_slFree<=bu_slSize);
};



// peek a number of bytes from the data buffer (does not advance the buffer read offset)
SLONG CEMsgBuffer::PeekBytes(const void *pv, SLONG slSize)
{
  ASSERT(slSize>0 && pv!=NULL);
  UBYTE *pub = (UBYTE*)pv;

  // clamp size to amount of bytes actually in the buffer
  SLONG slUsed = bu_slSize-bu_slFree;
  if (slUsed<slSize) {
    slSize = slUsed;
  }
  // if there is nothing to read
  if (slSize==0) {
    // do nothing
    return 0;
  }

  // read part of block after read pointer to the end of buffer
  SLONG slSizeEnd = __min(bu_slSize-bu_slReadOffset, slSize);
  memcpy(pub, bu_pubBuffer+bu_slReadOffset, slSizeEnd);
  pub+=slSizeEnd;
  // if that is not all (buffer is wrapping)
  if (slSizeEnd<slSize) {
    // read rest from start of buffer
    memcpy(pub, bu_pubBuffer, slSize-slSizeEnd);
  }

  // verify that all is well
  ASSERT(bu_slReadOffset>=0 && bu_slReadOffset<bu_slSize);
  ASSERT(bu_slFree>=0 && bu_slFree<=bu_slSize);

  return slSize;
}

//! 메모리 버퍼로부터 주어진 오프셋에서 바이트수 만큼 얻어낸다.
// peek a number of bytes from the data buffer at a given offset
SLONG CEMsgBuffer::PeekBytesAtOffset(const void *pv, SLONG slSize,SLONG &slTickOffset)
{
  ASSERT(slSize>0 && pv!=NULL);
  UBYTE *pub = (UBYTE*)pv;

  // clamp size to amount of bytes actually in the buffer
  SLONG slUsed = bu_slSize-bu_slFree;
  if (slUsed<slSize) {
    slSize = slUsed;
  }
  // if there is nothing to read
  if (slSize==0) {
    // do nothing
    return 0;
  }

  // read part of block after read pointer to the end of buffer
  SLONG slSizeEnd = __min(bu_slSize-slTickOffset, slSize);
  memcpy(pub, bu_pubBuffer+slTickOffset, slSizeEnd);
  pub+=slSizeEnd;
  slTickOffset += slSizeEnd;
  // if that is not all (buffer is wrapping)
  if (slSizeEnd<slSize) {
    // read rest from start of buffer
    memcpy(pub, bu_pubBuffer, slSize-slSizeEnd);
    slTickOffset = slSize - slSizeEnd;
  }   

  // what is the offset of the next byte
  slTickOffset %= bu_slSize;
  // verify that all is well
  ASSERT(slTickOffset>=0 && slTickOffset<bu_slSize);
  ASSERT(bu_slFree>=0 && bu_slFree<=bu_slSize);

  return slSize;
}
