#ifndef __RESTRICTEDCONTAINER_H
#define __RESTRICTEDCONTAINER_H

#include<Engine/Math/ExtensionMath.h>
#include<EntitiesMP/RestrictedField.h>
#include<EntitiesMP/Player.h>

//////////////////////////////////////////////////////////////////////////
//				제한 영역 컨테이너.
// Desc :	해당 영역 내부에 플레이어가 있을때 특정 행동을 취하도록 함.
//////////////////////////////////////////////////////////////////////////

// container functions
// insert and remove fields
void InsertRestrictedField(CRestrictedField* field);
void RemoveRestrictedField(CRestrictedField* field);

// lookup functions
// find the field the player is in
CRestrictedField* FindPlayerInRestrictedField(const CPlayer* player);
bool IsRestrictedSystemActive(void);

#endif