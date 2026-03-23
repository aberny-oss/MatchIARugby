#include "Zombie.h"

#include "PvZScene.h"

constexpr int ZombieSpeed = 50;
constexpr int ZombieHP = 3;

void Zombie::OnInitialize()  
{
	SetTag((int)PvZScene::Tag::Zombie);
	SetDirection(-1, 0, ZombieSpeed);

	m_currentHealth = ZombieHP;
}

void Zombie::OnCollision(Entity* entity) 
{
	if(entity->IsTag((int)PvZScene::Tag::Projectile))
	{
		m_currentHealth -= 1;
		if (m_currentHealth <= 0)
		{
			Destroy();
			GetScene<PvZScene>()->RemoveZombie(this);
		}

		entity->Destroy();
	}
}