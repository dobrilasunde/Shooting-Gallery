#include "Collision.hpp"
#include <algorithm>
#include <array>

LineSegment::LineSegment(const Vector3& start, const Vector3& end): mStart(start), mEnd(end)
{

}

Vector3 LineSegment::PointOnSegment(float t) const
{
	return mStart + (mEnd - mStart) * t;
}


AABB::AABB(const Vector3& min, const Vector3& max): mMin(min), mMax(max)
{

}

// When loading in a model, we have a sequence of vertices, and we can use this sequence to define an AABB for the model.
// This function takes in a point and updates min and max.
void AABB::UpdateMinMax(const Vector3& point)
{
	mMin.x = Math::Min(mMin.x, point.x);
	mMin.y = Math::Min(mMin.y, point.y);
	mMin.z = Math::Min(mMin.z, point.z);

	mMax.x = Math::Max(mMax.x, point.x);
	mMax.y = Math::Max(mMax.y, point.y);
	mMax.z = Math::Max(mMax.z, point.z);
}

void AABB::Rotate(const Quaternion& q)
{
	// Construct the 8 points for the corners of the box
	std::array<Vector3, 8> points;
	// Min point is always a corner
	points[0] = mMin;
	// Permutations with 2 min and 1 max
	points[1] = Vector3(mMax.x, mMin.y, mMin.z);
	points[2] = Vector3(mMin.x, mMax.y, mMin.z);
	points[3] = Vector3(mMin.x, mMin.y, mMax.z);
	// Permutations with 2 max and 1 min
	points[4] = Vector3(mMin.x, mMax.y, mMax.z);
	points[5] = Vector3(mMax.x, mMin.y, mMax.z);
	points[6] = Vector3(mMax.x, mMax.y, mMin.z);
	// Max point corner
	points[7] = Vector3(mMax);

	// Rotate first point
	Vector3 p = Vector3::Transform(points[0], q);
	// Reset min/max to first point rotated
	mMin = p;
	mMax = p;
	// Update min/max based on remaining points, rotated
	for (size_t i = 1; i < points.size(); i++)
	{
		p = Vector3::Transform(points[i], q);
		UpdateMinMax(p);
	}
}

bool AABB::Contains(const Vector3& point) const
{
	bool outside = point.x < mMin.x || point.y < mMin.y || point.z < mMin.z || point.x > mMax.x || point.y > mMax.y || point.z > mMax.z;
	// If none of these are true, the point is inside the box.
	return !outside;
}

float AABB::MinDistSq(const Vector3& point) const
{
	float dx = Math::Max(mMin.x - point.x, 0.0f);
	dx = Math::Max(dx, point.x - mMax.x);
	float dy = Math::Max(mMin.y - point.y, 0.0f);
	dy = Math::Max(dy, point.y - mMax.y);
	float dz = Math::Max(mMin.z - point.z, 0.0f);
	dz = Math::Max(dy, point.z - mMax.z);
	// Distance squared formula
	return dx * dx + dy * dy + dz * dz;
}


bool Intersect(const AABB& a, const AABB& b)
{
	bool no = a.mMax.x < b.mMin.x || a.mMax.y < b.mMin.y || a.mMax.z < b.mMin.z || b.mMax.x < a.mMin.x || b.mMax.y < a.mMin.y || b.mMax.z < a.mMin.z;
	// If none of these are true, they must intersect
	return !no;
}


bool TestSidePlane(float start, float end, float negd, const Vector3& norm, std::vector<std::pair<float, Vector3>>& out)
{
	float denom = end - start;
	if (Math::NearZero(denom))
	{
		return false;
	}
	else
	{
		float numer = -start + negd;
		float t = numer / denom;
		if (t >= 0.0f && t <= 1.0f)
		{
			out.emplace_back(t, norm);
			return true;
		}
		else
		{
			return false;
		}
	}
}

// Test if the line segment and the AABB intersect.
// If they do, return the t value in outT variable.
bool Intersect(const LineSegment& l, const AABB& b, float& outT, Vector3& outNorm)
{
	// Vector to save all possible t values, and normals for those sides.
	std::vector<std::pair<float, Vector3>> tValues;
	// Test the x planes.
	TestSidePlane(l.mStart.x, l.mEnd.x, b.mMin.x, Vector3::NegUnitX, tValues);
	TestSidePlane(l.mStart.x, l.mEnd.x, b.mMax.x, Vector3::UnitX, tValues);
	// Test the y planes.
	TestSidePlane(l.mStart.y, l.mEnd.y, b.mMin.y, Vector3::NegUnitY, tValues);
	TestSidePlane(l.mStart.y, l.mEnd.y, b.mMax.y, Vector3::UnitY, tValues);
	// Test the z planes.
	TestSidePlane(l.mStart.z, l.mEnd.z, b.mMin.z, Vector3::NegUnitZ, tValues);
	TestSidePlane(l.mStart.z, l.mEnd.z, b.mMax.z, Vector3::UnitZ, tValues);

	// Sort the t values in ascending order.
	std::sort(tValues.begin(), tValues.end(), [](const std::pair<float, Vector3>& a, const std::pair<float, Vector3>& b) 
	{
		return a.first < b.first;
	});
	// Test if the box contains any of these points of intersection.
	Vector3 point;
	for (auto& t : tValues)
	{
		point = l.PointOnSegment(t.first);
		if (b.Contains(point))
		{
			outT = t.first;
			outNorm = t.second;
			return true;
		}
	}

	//None of the intersections are within bounds of box
	return false;
}








