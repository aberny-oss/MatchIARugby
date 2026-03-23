#include "Projectile.h"

#include "PvZScene.h"

constexpr int ProjectileSpeed = 400;

void Projectile::OnInitialize()
{
	SetTag((int)PvZScene::Tag::Projectile);
	SetDirection(1, 0, ProjectileSpeed);
}