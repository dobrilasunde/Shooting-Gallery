#include "BoxComponent.hpp"
#include "Actor.hpp"
#include "Game.hpp"
#include "PhysWorld.hpp"

BoxComponent::BoxComponent(Actor* owner, int updateOrder): Component(owner, updateOrder), mObjectBox(Vector3::Zero, Vector3::Zero), mWorldBox(Vector3::Zero, Vector3::Zero), mShouldRotate(true)
{
	mOwner->GetGame()->GetPhysWorld()->AddBox(this);
}

BoxComponent::~BoxComponent()
{
	mOwner->GetGame()->GetPhysWorld()->RemoveBox(this);
}

// To convert from object bounds to world bounds, we need to apply scale, rotation, and translation to the bounds.
// As when constructing the world transform matrix, the order is important because rotation is about the origin.
void BoxComponent::OnUpdateWorldTransform()
{
	// Reset to object space box
	mWorldBox = mObjectBox;
	// Scale
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();
	// Rotate (if we want to)
	if (mShouldRotate)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}
	// Translate
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();
}