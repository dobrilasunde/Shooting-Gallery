#include "Game.hpp"
#include <algorithm>
#include "Renderer.hpp"
#include "PhysWorld.hpp"
#include "Actor.hpp"
#include "SpriteComponent.hpp"
#include "MeshComponent.hpp"
#include "FPSActor.hpp"
#include "PlaneActor.hpp"
#include "TargetActor.hpp"
#include "BallActor.hpp"
#include<iostream>

Game::Game() :mRenderer(nullptr), mPhysWorld(nullptr), mIsRunning(true), mUpdatingActors(false)
{

}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.0f, 768.0f))
	{
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	// Create the physics world
	mPhysWorld = new PhysWorld(this);

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::AddPlane(PlaneActor* plane)
{
	mPlanes.emplace_back(plane);
}

void Game::RemovePlane(PlaneActor* plane)
{
	auto iter = std::find(mPlanes.begin(), mPlanes.end(), plane);
	mPlanes.erase(iter);
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
			// This fires when a key's initially pressed
		case SDL_KEYDOWN:
			if (!event.key.repeat)
			{
				HandleKeyPress(event.key.keysym.sym);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			HandleKeyPress(event.button.button);
			break;
		default:
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
}

void Game::HandleKeyPress(int key)
{
	switch (key)
	{
	case SDL_BUTTON_LEFT:
	{
		// Fire weapon
		mFPSActor->Shoot();
		break;
	}
	default:
		break;
	}
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void AddColumn(class Game* game, int x, int y, class Actor * a, Quaternion& q, Quaternion& q2) {
	for (int i = 0;i < 9;i++) {
		a = new PlaneActor(game);
		a->SetScale(2.0f);
		a->SetPosition(Vector3(x, y, i * 50));
		a->SetRotation(q2);

		a = new PlaneActor(game);
		a->SetScale(2.0f);
		a->SetPosition(Vector3(x - 200, y, i * 50));
		a->SetRotation(q2);

		a = new PlaneActor(game);
		a->SetScale(2.0f);
		a->SetPosition(Vector3(x - 100, y - 100, i * 50));
		a->SetRotation(q);

		a = new PlaneActor(game);
		a->SetScale(2.0f);
		a->SetPosition(Vector3(x - 100, y + 100, i * 50));
		a->SetRotation(q);
	}
}

void Game::LoadData()
{
	// Create actors
	Actor* a = nullptr;
	Quaternion q, q2,q3;

	// Setup floor
	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}

	// Left/right walls
	q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		a->SetRotation(q);
	}

	q2 = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));

	// Forward/back walls
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		a->SetRotation(q2);
		a = new PlaneActor(this);
		a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		a->SetRotation(q2);
	}

	//prepreke

	//left wall
	for (int i = 0;i < 17;i++) {
		for (int j = 0;j < 9;j++) {
			a = new PlaneActor(this);
			a->SetScale(2.0f);
			a->SetPosition(Vector3(0, start-size/2-50+i*50, j*50));
			a->SetRotation(q2);
		}
	}

	for (int i = 1;i < 18;i++) {
		for (int j = 0;j < 9;j++) {
			a = new PlaneActor(this);
			a->SetScale(2.0f);
			a->SetPosition(Vector3((i * 50)-size*2+50, start + 3 * size, j * 50));
			a->SetRotation(q);

			a = new PlaneActor(this);
			a->SetScale(2.0f);
			a->SetPosition(Vector3((i * 50)-size*2+50, start + 3 * size - 200, j * 50));
			a->SetRotation(q);
		}
	}



	for (int i = 0;i < 9;i++) {
		a = new PlaneActor(this);
		a->SetScale(2.0f);
		a->SetPosition(Vector3(500, start + 3 * size - 100, i * 50));
		a->SetRotation(q2);

		a = new PlaneActor(this);
		a->SetScale(2.0f);
		a->SetPosition(Vector3(-500, start + 3 * size - 100, i * 50));
		a->SetRotation(q2);
	}

	AddColumn(this, 500, 500, a, q, q2);
	AddColumn(this, -1000, -1000, a, q, q2);
	AddColumn(this, -800, 200, a, q, q2);
	
	//right wall

	for (int i = 0;i < 10;i++) {
		for (int j = 0;j < 9;j++) {
			a = new PlaneActor(this);
			a->SetScale(2.0f);
			a->SetPosition(Vector3(-500, -start - size + i * 50, j * 50));
			a->SetRotation(q2);

			a = new PlaneActor(this);
			a->SetScale(2.0f);
			a->SetPosition(Vector3(-700, -start - size + i * 50, j * 50));
			a->SetRotation(q2);
		}
	}

	for (int i = 0;i < 9;i++) {
		a = new PlaneActor(this);
		a->SetScale(2.0f);
		a->SetPosition(Vector3(-600, -start-2*size+150, i * 50));
		a->SetRotation(q);
	}
	// Setup lights
	mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);
	
	auto yellowLight = PointLight();
	yellowLight.Position = Vector3(0.0f, 0.0f, 100.0f);
	yellowLight.DiffuseColor = Vector3(0.8f, 0.8f, 0.0f);
	yellowLight.SpecularColor = Vector3(0.8f, 0.8f, 0.0f);
	mRenderer->AddPointLight(yellowLight);

	auto redLight = PointLight();
	redLight.Position = Vector3(1000.0f, -1000.0f, 100.0f);
	redLight.DiffuseColor = Vector3(1.0f, 0.4f, 0.4f);
	redLight.SpecularColor = Vector3(0.8f, 0.3f, 0.3f);
	mRenderer->AddPointLight(redLight);

	auto greenLight = PointLight();
	greenLight.Position = Vector3(1000.0f, 1000.0f, 100.0f);
	greenLight.DiffuseColor = Vector3(0.4f, 1.0f, 0.4f);
	greenLight.SpecularColor = Vector3(0.3f, 0.8f, 0.3f);
	mRenderer->AddPointLight(greenLight);

	auto blueLight = PointLight();
	blueLight.Position = Vector3(-1000.0f, 1000.0f, 100.0f);
	blueLight.DiffuseColor = Vector3(0.4f, 0.4f, 1.0f);
	blueLight.SpecularColor = Vector3(0.3f, 0.3f, 0.8f);
	mRenderer->AddPointLight(blueLight);

	auto tealLight = PointLight();
	tealLight.Position = Vector3(-800.0f, -1000.0f, 100.0f);
	tealLight.DiffuseColor = Vector3(0.0f, 0.8f, 0.8f);
	tealLight.SpecularColor = Vector3(0.0f, 0.8f, 0.8f);
	mRenderer->AddPointLight(tealLight);

	// UI elements
	a = new Actor(this);
	a->SetScale(2.0f);
	mCrosshair = new SpriteComponent(a);
	mCrosshair->SetTexture(mRenderer->GetTexture("Assets/Crosshair.png"));

	// Enable relative mouse mode for camera look
	SDL_SetRelativeMouseMode(SDL_TRUE);
	// Make an initial call to get relative to clear out
	SDL_GetRelativeMouseState(nullptr, nullptr);

	// Different camera actors
	mFPSActor = new FPSActor(this);
	q3 = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::TwoPi));
	// Create target actors
	a = new TargetActor(this);
	a->SetPosition(Vector3(1450.0f, 0.0f, 200.0f));
	a = new TargetActor(this);
	a->SetRotation(q3);
	a->SetPosition(Vector3(-950.0f, -1000.0f, 300.0f));
	a = new TargetActor(this);
	a->SetRotation(q3);
	a->SetPosition(Vector3(-1450.0f, 500.0f, 200.0f));
	a = new TargetActor(this);
	a->SetRotation(q2);
	a->SetPosition(Vector3(-100.0f, -450.0f, 200.0f));
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	if (mRenderer)
	{
		mRenderer->UnloadData();
	}
}

void Game::Shutdown()
{
	UnloadData();
	delete mPhysWorld;
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}