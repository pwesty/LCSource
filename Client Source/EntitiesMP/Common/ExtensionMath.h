#ifndef __EXTENSIONMATH_H
#define __EXTENSIONMATH_H

// mathematical functions
// is a point (the player) in an oriented bounding box (the field)
struct vector3D 
{
	float x, y, z;
	vector3D() { x = y = z = 0.0f; }
	vector3D(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
};

inline float dot_product(const vector3D& v1, const vector3D& v2) 
{
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

struct OBB 
{
	vector3D p;
	vector3D u, v, w;
	vector3D l;
	bool point_in(const vector3D& point);
};

inline bool OBB::point_in(const vector3D& point) 
{
	vector3D diff;
	diff.x = point.x - p.x;
	diff.y = point.y - p.y;
	diff.z = point.z - p.z;
	float test;
	test = dot_product(u, diff); if(test < 0 || test > l.x) return false;
	test = dot_product(v, diff); if(test < 0 || test > l.y) return false;
	test = dot_product(w, diff); if(test < 0 || test > l.z) return false;
	return true;
}

#endif