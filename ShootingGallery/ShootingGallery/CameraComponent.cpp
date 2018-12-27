#include "CameraComponent.hpp"
#include "Actor.hpp"
#include "Renderer.hpp"
#include "Game.hpp"

CameraComponent::CameraComponent(Actor* owner, int updateOrder):Component(owner, updateOrder)
{
}

void CameraComponent::SetViewMatrix(const Matrix4& view)
{
	Game* game = mOwner->GetGame();
	game->GetRenderer()->SetViewMatrix(view);
}