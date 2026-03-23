#include "CLC_Ball.h"
#include "CLC_Scene.h"

void CLC_Ball::SetOwner(CLC_Players* owner)
{
    if (m_owner != nullptr)
    {
        m_lastOwner = m_owner;
    }
    if (m_owner != owner)
    {
        m_owner = owner;
    }
    
}

void CLC_Ball::SetLastOwner(CLC_Players* owner)
{
    m_lastOwner = nullptr;
}


void CLC_Ball::SetIsPass(bool isPass)
{
	m_isPass = isPass;
}

CLC_Players* CLC_Ball::GetOwner()
{
    return m_owner;
}

void CLC_Ball::OnInitialize()
{
    SetSpeed(888);
    m_isPass = false;
    SetTag((int)CLC_Scene::Tag::Ball);
}

void CLC_Ball::OnUpdate()
{
    if (m_owner != nullptr && m_isPass == false)
    {
        SetPosition(m_owner->GetPosition().x, m_owner->GetPosition().y);
	}
}

void CLC_Ball::OnCollision(Entity* collidedWith)
{
    CLC_Scene* scene = GetScene<CLC_Scene>();
    if (m_lastOwner == collidedWith)
        return;

    if (GetOwner() == collidedWith)
        return;

    if (collidedWith->IsTag((int)CLC_Scene::Tag::Player))
    {
        SetOwner((CLC_Players*)collidedWith);
        m_owner->SetInvincible();
		m_isPass = false;
    }
}


