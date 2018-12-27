#pragma once
#include "MoveComponent.hpp"

class BallMove : public MoveComponent
{
public:
	BallMove(class Actor* owner);
	void SetPlayer(Actor* player) { mPlayer = player; }
	void Update(float deltaTime) override;

protected:
	class Actor* mPlayer;
};