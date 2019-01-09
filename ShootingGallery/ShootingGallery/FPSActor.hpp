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

	bool isJumping = false;
	bool isCrouchingOld = false;
	bool isCrouchingNew = false;
	float jumpDuration = 1.0f; // how long will jump last
	float jumpSpeed = 5.0f;
	float jumpTime = 0.0f; // time since jumping started
};