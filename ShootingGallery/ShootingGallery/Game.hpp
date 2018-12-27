#pragma once

#include <SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.hpp"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	class Renderer* GetRenderer() { return mRenderer; }
	class PhysWorld* GetPhysWorld() { return mPhysWorld; }

	void AddPlane(class PlaneActor* plane);
	void RemovePlane(class PlaneActor* plane);
	std::vector<class PlaneActor*>& GetPlanes() { return mPlanes; }
private:
	void ProcessInput();
	void HandleKeyPress(int key);
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;

	class Renderer* mRenderer;
	class PhysWorld* mPhysWorld;

	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors;

	std::vector<class PlaneActor*> mPlanes;
	class FPSActor* mFPSActor;
	class SpriteComponent* mCrosshair;
};