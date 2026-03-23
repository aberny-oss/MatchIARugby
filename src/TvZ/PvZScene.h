#pragma once

#include "Scene.h"
#include <vector>


class Plant;
class Zombie;

class PvZScene : public Scene 
{
public:
	enum class Tag
	{
		Plant, 
		Zombie, 
		Projectile,

		Count
	};

private:
	float m_laneHeight;

	Plant* m_plants[3];

	std::vector<Zombie*> m_zombies[3];

public:

	friend class CLC_Players;

	bool IsZombieOnLane(int indexLane);
	int GetLaneIndex(int y) const;
	void RemoveZombie(Zombie* zombie);

protected:
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;

private:
	
	Zombie* SpawnZombie(int x, int laneIndex);
	
};