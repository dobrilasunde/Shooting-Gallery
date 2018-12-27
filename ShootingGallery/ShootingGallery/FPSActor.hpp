#pragma once
#include "Actor.hpp"

class FPSActor : public Actor
{
public:
	FPSActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;
	void Shoot();
	void SetVisible(bool visible);
	void FixCollisions();

private:
	class MoveComponent* mMoveComp;
	class MeshComponent* mMeshComp;
	class FPSCamera* mCameraComp;
	class BoxComponent* mBoxComp;
	class Actor* mFPSModel;
};