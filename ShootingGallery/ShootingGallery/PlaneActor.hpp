#pragma once
#include "Actor.hpp"

class PlaneActor : public Actor
{
public:
	PlaneActor(class Game* game);
	~PlaneActor();
	class BoxComponent* GetBox() { return mBox; }

private:
	class BoxComponent* mBox;
};