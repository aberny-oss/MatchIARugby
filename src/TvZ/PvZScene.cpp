#include "PvZScene.h"

#include "Zombie.h"
#include "Plant.h"
#include "Projectile.h"
#include "Debug.h"

#include <iostream>

bool PvZScene::IsZombieOnLane(int indexLane)
{
	return m_zombies[indexLane].size() > 0;
}

void PvZScene::OnInitialize()
{
	float m_windowWidth = GetWindowWidth();
	float m_windowHeight = GetWindowHeight();

	m_laneHeight = m_windowHeight / 3;

	float plantSize = m_laneHeight * 0.8f;
	float plantRadius = plantSize / 2;

	float plantStartX = m_windowWidth * 0.01;
	float plantStartY = m_laneHeight / 2;

	m_plants[0] = CreateEntity<Plant>(plantRadius, sf::Color::Green);
	m_plants[0]->SetPosition(plantStartX, plantStartY, 0.f, 0.5f);

	m_plants[1] = CreateEntity<Plant>(plantRadius, sf::Color::Green);
	m_plants[1]->SetPosition(plantStartX, m_laneHeight + plantStartY, 0.f, 0.5f);

	m_plants[2] = CreateEntity<Plant>(plantRadius, sf::Color::Green);
	m_plants[2]->SetPosition(plantStartX, m_laneHeight * 2 + plantStartY, 0.f, 0.5f);
}

void PvZScene::OnEvent(const sf::Event& event)
{
	if (event.type != sf::Event::EventType::MouseButtonPressed)
		return;

	int laneIndex = GetLaneIndex(event.mouseButton.y);

	if (event.mouseButton.button == sf::Mouse::Button::Right)
	{
		SpawnZombie(event.mouseButton.x, laneIndex);
	}

	if (event.mouseButton.button == sf::Mouse::Button::Left)
	{
		m_plants[laneIndex]->Shoot();
	}
}

void PvZScene::OnUpdate()
{
	float m_windowWidth = GetWindowWidth();

	Debug::DrawRectangle(0.f, 0.f, m_windowWidth, m_laneHeight, sf::Color::Green);
	Debug::DrawRectangle(0.f, m_laneHeight, m_windowWidth, m_laneHeight, sf::Color::Green);
	Debug::DrawRectangle(0.f, m_laneHeight * 2, m_windowWidth, m_laneHeight, sf::Color::Green);
}

int PvZScene::GetLaneIndex(int y) const
{
	if (y <= m_laneHeight)
		return 0;

	if (y <= m_laneHeight * 2)
		return 1;

	if (y <= m_laneHeight * 3)
		return 2;

	_ASSERT(false);
	return -1;
}

void PvZScene::RemoveZombie(Zombie* zombie)
{
	int laneIndex = GetLaneIndex(zombie->GetPosition().y);

	for (int i = 0; i < m_zombies[laneIndex].size(); i++)
	{
		if (m_zombies[laneIndex][i] == zombie)
		{
			m_zombies[laneIndex].erase(m_zombies[laneIndex].begin() + i);
			break;
		}
	}
}

Zombie* PvZScene::SpawnZombie(int x, int laneIndex)
{
	float m_windowWidth = GetWindowWidth();

	float zombieSize = m_laneHeight * 0.5f;
	float zombieRadius = zombieSize / 2;

	float zombieStartY = m_laneHeight / 2;

	Zombie* z = CreateEntity<Zombie>(zombieRadius, sf::Color::Red);
	z->SetPosition(x, m_laneHeight * laneIndex + zombieStartY, 1.f, 0.5f );

	m_zombies[laneIndex].push_back(z);

	return z;
}

