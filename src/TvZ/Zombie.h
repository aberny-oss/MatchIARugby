#include "Entity.h"

class Zombie : public Entity 
{
	float m_currentHealth;

protected:
	void OnInitialize() override;
	void OnCollision(Entity* entity) override;
};