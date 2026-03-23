#pragma once
#include "Entity.h"
#include "StateMachine.h"
#include <vector>

constexpr float s_invincibleTimer = 2.f; // durer d'invincibilit�

class CLC_Scene;
class CLC_Players : public Entity
{
public:

	enum class Team 
	{
		One,
		Two,

		Count
	};
	enum class Number
	{
		One,
		Two,
		Three,
		Four,
		Five,

		Count
	};
	enum class State
	{
		Base,
		Attack,
		Support,
		Defender,
		Pass,
		GapSide,

		Count
	};

private:
	
	struct Stats
	{
		float s_speed;
		float s_zonePass;
		float s_buffSpeed;
		int s_oppositionCount;
		float s_invincibleTimer;
	};
	Stats m_states;

	Team m_team;
	Number m_number;
	float m_invincibleTimer; // timer qui diminue avec delta time pour la durer du god mode
	float m_buffSpeed;
	int m_zonePass;
	int m_zoneGapSide;

	//StateMachine
	StateMachine<CLC_Players> m_stateMachine;

	friend class BasePlayersState;
	friend class AttackPlayersState;
	friend class SupportPlayersState;
	friend class DefenderPlayersState;
	friend class PassPlayersState;
	friend class GapSidePlayersState;

	friend class PlayerWithBallCondition;
	friend class OppositionAdversaryCondition;
	friend class TeammateStandOutNearbyCondition;
	friend class TeammateWithBallCondition;
	friend class AdversaryWithBallCondition;
	friend class BehindTeammateWithBallCondition;
	friend class PlayersWithoutBallCondition;

protected:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnCollision(Entity* collidedWith) override;

public:
	void SetStats(Stats stats);
	void PassBall();
	void GapSide();
	void SetTeam(Team team) { m_team = team; }
	Team GetTeam() const { return m_team; }
	bool IsSameTeam(CLC_Players* player) const { return m_team == player->GetTeam(); }

	void SetNumber(Number number) { m_number = number; }
	Number GetNumber() const { return m_number; }

	void SetInvincible();
	bool IsInvincible() const;
	int GetZonePass() const { return m_zonePass; }
	CLC_Players* GetOwner();

	const char* StateToStr() const;
};

// StateMachine

//State
class BasePlayersState : public StateBase<CLC_Players>
{
public:
	void Start(CLC_Players* type) override;
	void Update(CLC_Players* type, float dt) override;
	void End(CLC_Players* type) override;

};

class AttackPlayersState : public StateBase<CLC_Players>
{
public:

	void Start(CLC_Players* type) override;
	void Update(CLC_Players* type, float dt) override;
	void End(CLC_Players* type) override;
};

class SupportPlayersState : public StateBase<CLC_Players>
{
public:
	void Start(CLC_Players* type) override;
	void Update(CLC_Players* type, float dt) override;
	void End(CLC_Players* type) override;
};

class DefenderPlayersState : public StateBase<CLC_Players>
{
public:
	void Start(CLC_Players* type) override;
	void Update(CLC_Players* type, float dt) override;
	void End(CLC_Players* type) override;
};

class PassPlayersState : public StateBase<CLC_Players>
{
public:
	void Start(CLC_Players* type) override;
	void Update(CLC_Players* type, float dt) override;
	void End(CLC_Players* type) override;
};

class GapSidePlayersState : public StateBase<CLC_Players>
{
public:
	void Start(CLC_Players* type) override;
	void Update(CLC_Players* type, float dt) override;
	void End(CLC_Players* type) override;
};


//Condition
class PlayerWithBallCondition : public Condition<CLC_Players>
{
public:
	bool Test(CLC_Players* player) override;
};

class OppositionAdversaryCondition : public Condition<CLC_Players>
{
private:

	int m_distanceOppositionAdversary = 100;

public:
	bool Test(CLC_Players* player) override;
};

class TeammateStandOutNearbyCondition : public Condition<CLC_Players>
{
private:

	int m_distanceTeammateStandOut = 200;

public:
	bool Test(CLC_Players* player) override;
};

class TeammateWithBallCondition : public Condition<CLC_Players>
{
public:
	bool Test(CLC_Players* player) override;
};

class AdversaryWithBallCondition : public Condition<CLC_Players>
{
public:
	bool Test(CLC_Players* player) override;
};

class BehindTeammateWithBallCondition : public Condition<CLC_Players>
{
public:
	bool Test(CLC_Players* player) override;
};

class PlayersWithoutBallCondition : public Condition<CLC_Players>
{
public:
	bool Test(CLC_Players* player) override;
};