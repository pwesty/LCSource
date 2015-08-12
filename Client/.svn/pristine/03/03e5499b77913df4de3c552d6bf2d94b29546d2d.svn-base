#include "stdh.h"
#include "FixedCameraContainer.h"

#include<deque>
#include<algorithm>
using namespace std;

// Variables
// the container to hold all our fields
// the last field the local player was known to be in (optimization)
static deque<CFixedCameraField*> field_list;
static CFixedCameraField* field_last;

// InsertCameraField
// Adds the field to the global container.
void InsertCameraField(CFixedCameraField* field) 
{
	field_list.insert(field_list.end(), field);
}

// RemoveCameraField
// Removes the field from the global container.
void RemoveCameraField(CFixedCameraField* field) 
{
	deque<CFixedCameraField*>::iterator i = find(field_list.begin(), field_list.end(), field);
	if(i != field_list.end()) field_list.erase(i);
}

// FindPlayerInCameraField
// Find the camera field that the player is in, if
// he is not in one, it returns 0.
CFixedCameraField* FindPlayerInCameraField(const CPlayer* player)
{
	// no player, no can do
	if(!player) return 0;
	
	// get player placement, offsetting the y value a little bit
	// so that the point doesn't lie on the floor, where two boxes
	// may overlap, confusing the system.
	vector3D point;
	point.x = player->GetPlacement().pl_PositionVector(1);
	point.y = player->GetPlacement().pl_PositionVector(2) + 0.001f;
	point.z = player->GetPlacement().pl_PositionVector(3);
	
	// if the player is in the last known field, which is most
	// likely, return it. this check makes the algorithm fast.
	if(field_last)
	{
		// get field's placement
		FLOAT ox = (field_last->GetPlacement()).pl_PositionVector(1);
		FLOAT oy = (field_last->GetPlacement()).pl_PositionVector(2);
		FLOAT oz = (field_last->GetPlacement()).pl_PositionVector(3);
		
		// get field's axis vectors from HBP values
		Quaternion<FLOAT> q;
		q.FromEuler((field_last->GetPlacement()).pl_OrientationAngle);
		Matrix<FLOAT, 3, 3> matrix;
		q.ToMatrix(matrix);
		
		// get field's oriented bounding box
		OBB obb;
		obb.p = vector3D(ox, oy, oz);
		obb.u = vector3D(matrix(1,1), matrix(2,1), matrix(3,1));
		obb.v = vector3D(matrix(1,2), matrix(2,2), matrix(3,2));
		obb.w = vector3D(matrix(1,3), matrix(2,3), matrix(3,3));
		obb.l = vector3D(field_last->du, field_last->dv, field_last->dw);
		
		// if the player is still in the last known field, return it
		bool inside = obb.point_in(point);
		if(inside) return field_last;
	}
	
	// player has changed fields, so we must look through the
	// field list to find the field the player is in
	for(size_t i = 0; i < field_list.size(); i++)
	{
		CFixedCameraField* field_curr = field_list[i];
		
		// get field's placement
		FLOAT ox = (field_curr->GetPlacement()).pl_PositionVector(1);
		FLOAT oy = (field_curr->GetPlacement()).pl_PositionVector(2);
		FLOAT oz = (field_curr->GetPlacement()).pl_PositionVector(3);
		
		// get field's axis vectors from HBP values
		Quaternion<FLOAT> q;
		q.FromEuler((field_curr->GetPlacement()).pl_OrientationAngle);
		Matrix<FLOAT, 3, 3> matrix;
		q.ToMatrix(matrix);
		
		// get field's oriented bounding box
		OBB obb;
		obb.p = vector3D(ox, oy, oz);
		obb.u = vector3D(matrix(1,1), matrix(2,1), matrix(3,1));
		obb.v = vector3D(matrix(1,2), matrix(2,2), matrix(3,2));
		obb.w = vector3D(matrix(1,3), matrix(2,3), matrix(3,3));
		obb.l = vector3D(field_curr->du, field_curr->dv, field_curr->dw);
		
		// if the player is in the current field, set it as the
		// last known field the player was in and return it
		bool inside = obb.point_in(point);
		if(inside) 
		{ 
			field_last = field_curr; 
			return field_curr; 
		}
	}
	
	// the player is not in any field
	field_last = 0;
	return 0;
}

// IsFixedCameraSystemActive
// If the local player is in a field, the field_last variable
// will not be zero. This function is needed so that the
// crosshair isn't drawn in RenderHUD in Player.es.
bool IsFixedCameraSystemActive(void) 
{
	return field_last != 0;
}