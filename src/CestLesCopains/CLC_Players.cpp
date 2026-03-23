#include "CLC_Players.h"
#include "CLC_Scene.h"
#include "CLC_Ball.h"
#include "Debug.h"

void CLC_Players::OnInitialize()
{
    SetSpeed(222);
    m_buffSpeed = 100;
    m_zonePass = 200;
    m_invincibleTimer = 0;
    m_zoneGapSide = GetRadius() + 20;
    SetTag((int)CLC_Scene::Tag::Player);

    //BASE_STATE
    {
        m_stateMachine.AddState(new BasePlayersState());

		//TRANSITION avec condition
        {
            //->Attack
			Transition<CLC_Players>* t = m_stateMachine.AddTransition((int)State::Base, (int)State::Attack);
			t->AddCondition(new PlayerWithBallCondition());
        }
        {
            //->Support
            Transition<CLC_Players>* t = m_stateMachine.AddTransition((int)State::Base, (int)State::Support);
            t->AddCondition(new TeammateWithBallCondition());
        }
        {
            //->Defender
            Transition<CLC_Players>* t = m_stateMachine.AddTransition((int)State::Base, (int)State::Defender);
            t->AddCondition(new AdversaryWithBallCondition());
        }
    }
    //ATTACK_STATE
    {
        m_stateMachine.AddState(new AttackPlayersState());

        //TRANSITION avec condition
        {
            //->Pass
            Transition<CLC_Players>* t = m_stateMachine.AddTransition((int)State::Attack, (int)State::Pass);
            t->AddCondition(new PlayerWithBallCondition());
            t->AddCondition(new OppositionAdversaryCondition());
            t->AddCondition(new TeammateStandOutNearbyCondition());
        }
        {
            //->Base
            Transition<CLC_Players>* t = m_stateMachine.AddTransition((int)State::Attack, (int)State::Base);
            Condition<CLC_Players>* c = new PlayerWithBallCondition();
            c->ExpectedValue = false;
            t->AddCondition(c);
        }
        {
            //->GapSide
            Transition<CLC_Players>* t = m_stateMachine.AddTransition((int)State::Attack, (int)State::GapSide);
            t->AddCondition(new PlayerWithBallCondition());
            t->AddCondition(new OppositionAdversaryCondition());
            Condition<CLC_Players>* c = new TeammateStandOutNearbyCondition();
            c->ExpectedValue = false;
            t->AddCondition(c);
        }
	}
    //SUPPORT_STATE
    {
        m_stateMachine.AddState(new SupportPlayersState());

        {
            //->Base
            Transition<CLC_Players>* t = m_stateMachine.AddTransition((int)State::Support, (int)State::Base);
            Condition<CLC_Players>* c = new TeammateWithBallCondition();
            c->ExpectedValue = false;
            t->AddCondition(c);
        }
    }
    //DEFENDER_STATE
    {
        m_stateMachine.AddState(new DefenderPlayersState());

        {
            //->Base
            Transition<CLC_Players>* t = m_stateMachine.AddTransition((int)State::Defender, (int)State::Base);
            t->AddCondition(new TeammateWithBallCondition());
        }
    }
    //PASS_STATE
    {
        m_stateMachine.AddState(new PassPlayersState());

        {
            //->Base
            Transition<CLC_Players>* t = m_stateMachine.AddTransition((int)State::Pass, (int)State::Base);
            Condition<CLC_Players>* c = new PlayerWithBallCondition();
            c->ExpectedValue = false;
            t->AddCondition(c);
        }
        {
			//->Attack
			Transition<CLC_Players>* t = m_stateMachine.AddTransition((int)State::Pass, (int)State::Attack);
            t->AddCondition(new PlayerWithBallCondition());
            t->AddCondition(new OppositionAdversaryCondition());
            Condition<CLC_Players>* c = new TeammateStandOutNearbyCondition();
            c->ExpectedValue = false;
            t->AddCondition(c);
        }
	}
    //GAP_SIDE_STATE
    {
        m_stateMachine.AddState(new GapSidePlayersState());

        {
            //->Base
            Transition<CLC_Players>* t = m_stateMachine.AddTransition((int)State::GapSide, (int)State::Base);
            Condition<CLC_Players>* c = new PlayerWithBallCondition();
            c->ExpectedValue = false;
            t->AddCondition(c);
        }
        {
            //->Attack
            Transition<CLC_Players>* t = m_stateMachine.AddTransition((int)State::GapSide, (int)State::Attack);
            t->AddCondition(new PlayerWithBallCondition());
            Condition<CLC_Players>* c = new OppositionAdversaryCondition();
            c->ExpectedValue = false;
            t->AddCondition(c);
        }
    }
}

void CLC_Players::OnUpdate()
{
    float dt = GetDeltaTime();
    m_invincibleTimer -= dt;

    if (IsInvincible() == false)
    {
        SetSpeed(222);
    }
    m_stateMachine.Update(this, GetDeltaTime());
}

void CLC_Players::SetStats(Stats stats)
{
    m_states.s_speed = stats.s_speed;
    m_states.s_zonePass = stats.s_zonePass;
    m_states.s_buffSpeed = stats.s_buffSpeed;
    m_states.s_oppositionCount = stats.s_oppositionCount;
    m_states.s_invincibleTimer = stats.s_invincibleTimer;
}

void CLC_Players::PassBall()
{
    CLC_Scene* scene = GetScene<CLC_Scene>();
    CLC_Ball* ball = scene->GetBall();

    if (ball->GetOwner() != this)
        return;

    CLC_Players* findPlayer = nullptr;
    float tinyDistance = FLT_MAX;

    for (CLC_Players* player : scene->GetAllPlayerByTeam(this->GetTeam()))
    {
        if (player == this)
        {
            continue;
        }
        float distance = GetDistanceTo(this, player);
        if (distance > m_zonePass)
        {
            continue;
        }
        if (distance < tinyDistance)
        {
            tinyDistance = distance;
			findPlayer = player;
        }
    }
    if (findPlayer != nullptr)
    {
        ball->SetOwner(nullptr);
        ball->SetIsPass(true);
        ball->GoToDirection(findPlayer->GetPosition().x, findPlayer->GetPosition().y, ball->mSpeed);
    }
}

void CLC_Players::GapSide()
{
    CLC_Scene* scene = GetScene<CLC_Scene>();
    
    if (GetTeam() == Team::One)
    {
        for (CLC_Players* p : scene->GetAllPlayerByTeam(CLC_Players::Team::Two))
        {
            if (GetDistanceTo(this, p) > m_zoneGapSide)
            {
                continue;
            }

            if (this->GetPosition().y < p->GetPosition().y) 
            {
                this->GoToDirection(1, -1);
            }
            else 
            {
                this->GoToDirection(1, 1);
            }
        }
    }
    else if (GetTeam() == Team::Two)
    {
        for (CLC_Players* p : scene->GetAllPlayerByTeam(CLC_Players::Team::Two))
        {
            if (GetDistanceTo(this, p) > m_zoneGapSide)
            {
                continue;
            }

            if (this->GetPosition().y < p->GetPosition().y)
            {
                this->GoToDirection(-1, -1);
            }
            else
            {
                this->GoToDirection(-1, 1);
            }
        }
    }
}

void CLC_Players::SetInvincible()
{
    m_invincibleTimer = s_invincibleTimer;
    SetSpeed(333);
}

bool CLC_Players::IsInvincible() const
{
    if (m_invincibleTimer > 0)
    {
        return true;
    }
    return false;
}

CLC_Players* CLC_Players::GetOwner()
{
    CLC_Scene* scene = GetScene<CLC_Scene>();
    CLC_Ball* ball = scene->GetBall();
    if (ball->GetOwner() == this)
        return this;
    return nullptr;
}

void CLC_Players::OnCollision(Entity* collidedWith)
{
    if (collidedWith->IsTag(mTag) == false)
        return;

    CLC_Players* player = (CLC_Players*)collidedWith;

    if (IsSameTeam(player))
        return;

    CLC_Scene* scene = GetScene<CLC_Scene>();
    CLC_Ball* ball = scene->GetBall();

    if (ball->GetOwner() != player)
        return;

    //is invincible ?
    if (player->IsInvincible())
    {
        return;
    }

    //set ball new owner
    ball->SetOwner(this);
    SetInvincible();
}

const char* CLC_Players::StateToStr() const
{
    switch ((State)m_stateMachine.GetState())
    {
    case State::Base:
        return "Base";
    case State::Attack:
        return "Attack";
    case State::Support:
        return "Support";
    case State::Defender:
        return "Defender";
    case State::Pass:
        return "Pass";
    case State::GapSide:
        return "GapSide";
    }
}

//StateMachine

//Base State
void BasePlayersState::Start(CLC_Players* type)
{

}
void BasePlayersState::Update(CLC_Players* type, float dt)
{
    CLC_Scene* scene = type->GetScene<CLC_Scene>();
    CLC_Ball* ball = scene->GetBall();
    type->GoToPosition(ball->GetPosition().x, ball->GetPosition().y);
}
void BasePlayersState::End(CLC_Players* type)
{

}

//Attack State
void AttackPlayersState::Start(CLC_Players* type)
{

}
void AttackPlayersState::Update(CLC_Players* type, float dt)
{
    CLC_Scene* scene = type->GetScene<CLC_Scene>();
    if (type->GetTeam() == CLC_Players::Team::One)
    {
        type->GoToPosition(scene->GetWindowWidth(), type->GetPosition().y);
    }
    else if (type->GetTeam() == CLC_Players::Team::Two)
    {
        type->GoToPosition(0, type->GetPosition().y);
    }
}
void AttackPlayersState::End(CLC_Players* type)
{

}

//Support State
void SupportPlayersState::Start(CLC_Players* type)
{

}
void SupportPlayersState::Update(CLC_Players* type, float dt)
{

}
void SupportPlayersState::End(CLC_Players* type)
{

}

//Defender State
void DefenderPlayersState::Start(CLC_Players* type)
{
    //CLC_Ball* b;
    //type->GoToPosition(b->GetPosition().x, b->GetPosition().y);
}
void DefenderPlayersState::Update(CLC_Players* type, float dt)
{

}
void DefenderPlayersState::End(CLC_Players* type)
{

}

//Pass State
void PassPlayersState::Start(CLC_Players* type)
{

}
void PassPlayersState::Update(CLC_Players* type, float dt)
{
    CLC_Scene* scene = type->GetScene<CLC_Scene>();
    type->PassBall();
}
void PassPlayersState::End(CLC_Players* type)
{

}

//GapSide State
void GapSidePlayersState::Start(CLC_Players* type)
{

}
void GapSidePlayersState::Update(CLC_Players* type, float dt)
{
    type->GapSide();
}
void GapSidePlayersState::End(CLC_Players* type)
{

}


//Condition
bool PlayerWithBallCondition::Test(CLC_Players* player)
{
	CLC_Scene* scene = player->GetScene<CLC_Scene>();
	CLC_Ball* ball = scene->GetBall();
    if (player == ball->GetOwner())
    {
		return true;
    }
    return false;
}
bool OppositionAdversaryCondition::Test(CLC_Players* player)
{
    CLC_Scene* scene = player->GetScene<CLC_Scene>();
	//retourne si le player est team one ou team two et retourne la liste des adversaires en fonction de ça
	std::vector<CLC_Players*> adversaries = scene->GetAllPlayerByTeam(player->GetTeam() == CLC_Players::Team::One ? CLC_Players::Team::Two : CLC_Players::Team::One);
    for(CLC_Players* adversary : adversaries)
    {
        if (player->GetTeam() == CLC_Players::Team::One)
        {
            if (player->GetPosition().x > adversary->GetPosition().x - adversary->GetRadius())
            {
                continue;
            }
            if (player->GetDistanceTo(player, adversary) > m_distanceOppositionAdversary)
            {
                continue;
            }
        }
        else if (player->GetTeam() == CLC_Players::Team::Two)
        {
            if (player->GetPosition().x < adversary->GetPosition().x + adversary->GetRadius())
            {
                continue;
            }
            if (player->GetDistanceTo(player, adversary) > m_distanceOppositionAdversary)
            {
                continue;
            }
        }
        return true;
	}
    return false;
}
bool TeammateStandOutNearbyCondition::Test(CLC_Players* player)
{
    CLC_Scene* scene = player->GetScene<CLC_Scene>();
	std::vector<CLC_Players*> teammates = scene->GetAllPlayerByTeam(player->GetTeam());
    CLC_Players* findPlayer = nullptr;
    float tinyDistance = FLT_MAX;

    for (CLC_Players* teammate : teammates)
    {
        if (player == teammate)
        {
            continue;
        }
        if(player->GetTeam() == CLC_Players::Team::One)
        {
            if (player->GetPosition().x < teammate->GetPosition().x + teammate->GetRadius())
            {
                continue;
            }
        }
		else if (player->GetTeam() == CLC_Players::Team::Two)
        {
            if (player->GetPosition().x > teammate->GetPosition().x - teammate->GetRadius())
            {
                continue;
            }
		}
        float distance = player->GetDistanceTo(player, teammate);
        if (distance > m_distanceTeammateStandOut)
        {
            continue;
        }
        if (distance < tinyDistance)
        {
            tinyDistance = distance;
            findPlayer = teammate;
        }
    }
    if (findPlayer != nullptr)
    {
        return true;
    }
    return false;
}
bool TeammateWithBallCondition::Test(CLC_Players* player)
{
    CLC_Scene* scene = player->GetScene<CLC_Scene>();
    CLC_Ball* ball = scene->GetBall();
    if (ball->GetOwner() == nullptr)
    {
        return false;
    }
    if(player->GetTeam() == ball->GetOwner()->GetTeam())
    {
        return true;
	}
    return false;
}
bool AdversaryWithBallCondition::Test(CLC_Players* player)
{
    CLC_Scene* scene = player->GetScene<CLC_Scene>();
    CLC_Ball* ball = scene->GetBall();
    if (ball->GetOwner() == nullptr)
    {
        return false;
    }
    if (player->GetTeam() != ball->GetOwner()->GetTeam())
    {
        return true;
    }
    return false;
}
bool BehindTeammateWithBallCondition::Test(CLC_Players* player)
{
    CLC_Scene* scene = player->GetScene<CLC_Scene>();
    CLC_Ball* ball = scene->GetBall();
	CLC_Players* owner = ball->GetOwner();

    if (owner->GetTeam() == CLC_Players::Team::One)
    {
        if (player->GetPosition().x > owner->GetPosition().x - owner->GetRadius())
        {
            return false;
        }
		return true;
    }
    else if (owner->GetTeam() == CLC_Players::Team::Two)
    {
        if (player->GetPosition().x < owner->GetPosition().x + owner->GetRadius())
        {
            return false;
		}
		return true;
	}
}
bool PlayersWithoutBallCondition::Test(CLC_Players* player)
{
    CLC_Scene* scene = player->GetScene<CLC_Scene>();
    CLC_Ball* ball = scene->GetBall();
    CLC_Players* owner = ball->GetOwner();

    if (owner == nullptr)
    {
        return true;
    }

    return false;
}
