#pragma once
#include <vector>
#include <functional>
#include "Math.hpp"
#include "Collision.hpp"

class PhysWorld
{
public:
	PhysWorld(class Game* game);

	struct CollisionInfo
	{
		// Point of collision
		Vector3 mPoint;
		// Normal at collision
		Vector3 mNormal;
		// Component collided with
		class BoxComponent* mBox;
		// Owning actor of component
		class Actor* mActor;
	};

	// Test a line segment against boxes
	// Returns true if it collides against a box
	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);

	// Add/remove box components from world
	void AddBox(class BoxComponent* box);
	void RemoveBox(class BoxComponent* box);

private:
	class Game* mGame;
	std::vector<class BoxComponent*> mBoxes;
};