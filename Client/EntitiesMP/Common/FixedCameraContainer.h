#ifndef __FIXEDCAMERACONTAINER_H
#define __FIXEDCAMERACONTAINER_H

//////////////////////////////////////////////////////////////////////////
// 고정 시점 카메라 컨테이너.
//////////////////////////////////////////////////////////////////////////

#include<Engine/Math/ExtensionMath.h>
#include<EntitiesMP/FixedCameraField.h>
#include<EntitiesMP/Player.h>

// container functions
// insert and remove fields
void InsertCameraField(CFixedCameraField* field);
void RemoveCameraField(CFixedCameraField* field);

// lookup functions
// find the field the player is in
CFixedCameraField* FindPlayerInCameraField(const CPlayer* player);
bool IsFixedCameraSystemActive(void);

#endif