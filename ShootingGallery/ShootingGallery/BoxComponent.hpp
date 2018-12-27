#pragma once

#include "Component.hpp"
#include "Collision.hpp"

class BoxComponent : public Component
{
public:
	BoxComponent(class Actor* owner, int updateOrder = 100);
	~BoxComponent();

	void OnUpdateWorldTransform() override;

	void SetObjectBox(const AABB& model) { mObjectBox = model; }
	const AABB& GetWorldBox() const { return mWorldBox; }
	void SetShouldRotate(bool value) { mShouldRotate = value; }

private:
	AABB mObjectBox; // One AABB for the object space bounds.
	AABB mWorldBox; // One AABB for the world space bounds.
	bool mShouldRotate;
};