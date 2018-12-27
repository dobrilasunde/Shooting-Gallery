#pragma once
#include <cstdint>

class Component
{
public:
	// The lower the update order, the earlier the component updates.
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();
	virtual void Update(float deltaTime);
	virtual void ProcessInput(const uint8_t* keyState) {}
	virtual void OnUpdateWorldTransform() { }

	class Actor* GetOwner() { return mOwner; }
	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	class Actor* mOwner;
	int mUpdateOrder;
};