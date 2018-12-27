#pragma once
#include "Actor.hpp"

class BallActor : public Actor
{
public:
	BallActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void SetPlayer(Actor* player);
	void HitTarget();

private:
	class BallMove* mMyMove;
	float mLifeSpan;
};