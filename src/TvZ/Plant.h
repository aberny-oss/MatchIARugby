#include "Entity.h"
#include "StateMachine.h"

class Plant : public Entity
{
	enum class State
	{
		Idle,
		Shooting,
		Reloading,

		Count
	};

	StateMachine<Plant> m_stateMachine;
	int m_bullet;
	int m_capacity;

	
public:
	void Shoot();
	void Reload();

protected:
	void OnInitialize() override;
	void OnUpdate() override;

private:
	const char* StateToStr() const;

	friend class IdlePLantState;
	friend class ShootingPLantState;
	friend class ReloadingPLantState;

	friend class NoAmmoCondition;
	friend class HasZombieOnLaneCondition;
	friend class NotFullAmmoCondition;
};

class IdlePLantState : public StateBase<Plant>
{
public:
	void Start(Plant* type) override;
	void Update(Plant* type, float dt) override;
	void End(Plant* type) override;
};

class ShootingPLantState : public StateBase<Plant>
{
public:
	float m_delay;

private:
	float m_invincibleTimer;

public:
	void Start(Plant* type) override;
	void Update(Plant* type, float dt) override;
	void End(Plant* type) override;
};

class ReloadingPLantState : public StateBase<Plant>
{
public:
	float m_delay;

private:
	float m_invincibleTimer;

public:
	void Start(Plant* type) override;
	void Update(Plant* type, float dt) override;
	void End(Plant* type) override;
};

class NoAmmoCondition : public Condition<Plant> 
{
public:
	bool Test(Plant* plant) override;
};

class HasZombieOnLaneCondition : public Condition<Plant>
{
public:
	bool Test(Plant* plant) override;
};

class NotFullAmmoCondition : public Condition<Plant>
{
public:
	bool Test(Plant* plant) override;
};

class DelayCondition : public Condition<Plant>
{
	float m_delay;
public:
	bool Test(Plant* plant) override;
};