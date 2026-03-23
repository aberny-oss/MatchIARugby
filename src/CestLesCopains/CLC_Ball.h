#pragma once
#include "Entity.h"
#include "StateMachine.h"
#include "CLC_Players.h"

class CLC_Ball : public Entity
{
private:
	bool m_isPass;

protected:

	CLC_Players* m_owner = nullptr;
	CLC_Players* m_lastOwner = nullptr;
	void OnInitialize() override;
	void OnUpdate() override;


public:

	friend class CLC_Players;

	void SetOwner(CLC_Players* owner);
	void SetLastOwner(CLC_Players* owner);
	void SetIsPass(bool isPass);
	CLC_Players* GetOwner();

	void OnCollision(Entity* collidedWith) override;


};
