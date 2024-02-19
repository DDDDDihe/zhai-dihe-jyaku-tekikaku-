#pragma once
#include "Math.h"
#include <vector>

struct LineSegment
{
	LineSegment(const Vector3& start, const Vector3& end);
	//0=t<=1の時、線分上の点を取得する
	Vector3 PointOnSegment(float t) const;
	//点と線の最小距離の平方
	float MinDistSq(const Vector3& point) const;
	//線と線の最小距離の平方
	static float MinDistSq(const LineSegment& s1, const LineSegment& s2);

	Vector3 mStart;
	Vector3 mEnd;
};

struct Plane
{
	Plane(const Vector3& normal, float d);
	Plane(const Vector3& a, const Vector3& b, const Vector3& c);
	
	//plane and pointの距離をもらう
	float SignedDist(const Vector3& point) const;

	Vector3 mNormal;
	float mD;
};

struct Sphere
{
	Sphere(const Vector3& center, float radius);
	bool Contains(const Vector3& point) const;

	Vector3 mCenter;
	float mRadius;
};

struct AABB
{
	AABB(const Vector3& min, const Vector3& max);

	void UpdateMinMax(const Vector3& point);

	void Rotate(const Quaternion& q);

	bool Contains(const Vector3& point) const;

	float MinDistSq(const Vector3& point) const;

	Vector3 mMin;
	Vector3 mMax;
};

struct OBB
{
	Vector3 mCenter;
	Quaternion mRotation;
	Vector3 mExtents;
};

struct Capsule
{
	Capsule(const Vector3& start, const Vector3& end, float radius);

	Vector3 PointOnSegment(float t) const;
	
	bool Contains(const Vector3& point) const;

	LineSegment mSegment;
	float mRadius;
};

struct ConvexPolygon
{
	bool Contains(const Vector2& point) const;

	//頂点の順序は時計回り
	std::vector<Vector2> mVertices;
};

//交差関数
bool Intersect(const Sphere& a, const Sphere& b);
bool Intersect(const AABB& a, const AABB& b);
bool Intersect(const Capsule& a, const Capsule& b);
bool Intersect(const Sphere& s, const AABB& box);
bool Intersect(const LineSegment& l, const Sphere& s , float& outT);
bool Intersect(const LineSegment& l, const Plane& p, float& outT);
bool Intersect(const LineSegment& l, const AABB& b , float& outT , Vector3& outNorm);
bool Intersect(const OBB& o1, const OBB& o2);

bool SweptSphere(const Sphere& P0, const Sphere& P1, 
	const Sphere& Q0, const Sphere& Q1, float& t);