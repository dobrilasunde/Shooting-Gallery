#pragma once
#include "Math.hpp"
#include <vector>

struct LineSegment
{
	LineSegment(const Vector3& start, const Vector3& end);
	Vector3 PointOnSegment(float t) const;
	Vector3 mStart;
	Vector3 mEnd;
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

bool Intersect(const AABB& a, const AABB& b);
bool Intersect(const LineSegment& l, const AABB& b, float& outT, Vector3& outNorm);

