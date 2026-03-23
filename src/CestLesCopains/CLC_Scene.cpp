#include "CLC_Scene.h"
#include "CLC_Ball.h"
#include "Debug.h"
#include <iostream>

void CLC_Scene::OnInitialize()
{
	GetSizeWindow();

	//Team1
	CreatePlayer(borderLeftMaxPosX, 60, sf::Color::Green, CLC_Players::Team::One, CLC_Players::Number::One);
	CreatePlayer(210, 90, sf::Color::Green, CLC_Players::Team::One, CLC_Players::Number::Two);
	CreatePlayer(300, m_midPosY, sf::Color::Green, CLC_Players::Team::One, CLC_Players::Number::Three);
	CreatePlayer(210, 640, sf::Color::Green, CLC_Players::Team::One, CLC_Players::Number::Four);
	CreatePlayer(borderLeftMaxPosX, 670, sf::Color::Green, CLC_Players::Team::One, CLC_Players::Number::Five);

	//Team2
	CreatePlayer(borderRightMaxPosX, 60, sf::Color::Red, CLC_Players::Team::Two, CLC_Players::Number::One);
	CreatePlayer(borderRightPosX, 90, sf::Color::Red, CLC_Players::Team::Two, CLC_Players::Number::Two);
	CreatePlayer(m_windowWidth - 300, m_midPosY, sf::Color::Red, CLC_Players::Team::Two, CLC_Players::Number::Three);
	CreatePlayer(borderRightPosX, 640, sf::Color::Red, CLC_Players::Team::Two, CLC_Players::Number::Four);
	CreatePlayer(borderRightMaxPosX, 670, sf::Color::Red, CLC_Players::Team::Two, CLC_Players::Number::Five);

	//Ball
	CreateBall(m_windowWidth / 2, m_windowHeight / 2, sf::Color{ 100, 60, 35, 255 });
}

void CLC_Scene::OnEvent(const sf::Event& event)
{
	EventMouse(event);
	EventKeyboards(event);
}

void CLC_Scene::EventMouse(const sf::Event& event)
{
	if (event.type != sf::Event::EventType::MouseButtonPressed)
		return;

	if (event.mouseButton.button == sf::Mouse::Button::Right)
	{
		for (CLC_Players* p : m_TeamPlayerOne)
		{
			TrySetSelectedEntity(p, event.mouseButton.x, event.mouseButton.y);
		}
		for (CLC_Players* p : m_TeamPlayerTwo)
		{
			TrySetSelectedEntity(p, event.mouseButton.x, event.mouseButton.y);
		}
	}

	if (event.mouseButton.button == sf::Mouse::Button::Left)
	{
		if (pEntitySelected != nullptr)
		{
			pEntitySelected->GoToPosition(event.mouseButton.x, event.mouseButton.y, pEntitySelected->GetSpeed());
		}
	}
}

void CLC_Scene::EventKeyboards(const sf::Event& event)
{
	if (event.type != sf::Event::EventType::KeyPressed)
	{
		return;
	}

	if (event.key.code == sf::Keyboard::A)
	{
		if (pEntitySelected != nullptr)
		{
			pEntitySelected->PassBall();
		}
	}
	if (event.key.code == sf::Keyboard::Z)
	{
		for (CLC_Players* p : m_TeamPlayerOne)
		{
			p->GoToPosition(m_Ball->GetPosition().x, m_Ball->GetPosition().y);
		}
	}
	if (event.key.code == sf::Keyboard::E)
	{
		for (CLC_Players* p : m_TeamPlayerTwo)
		{
			p->GoToPosition(m_Ball->GetPosition().x, m_Ball->GetPosition().y);
		}
	}
	if (event.key.code == sf::Keyboard::R)
	{
		ResetPos();
	}

	if (event.key.code == sf::Keyboard::D)
	{
		if (m_activeDebug == false)
		{
			m_activeDebug = true;
			return;
		}
		if (m_activeDebug == true)
		{
			m_activeDebug = false;
			return;
		}
	}
	
}

void CLC_Scene::OnUpdate()
{
	ShowScore();

	ActiveDebug();

	GetSizeWindow();

	// Droite
	Debug::DrawLine(m_windowWidth - 80, m_windowHeight, m_windowWidth - 80, m_windowHeight - m_windowHeight, sf::Color::White);

	// Gauche
	Debug::DrawLine(80, m_windowHeight, 80, m_windowHeight - m_windowHeight, sf::Color::White);

	if (pEntitySelected != nullptr)
	{
		sf::Vector2f position = pEntitySelected->GetPosition();
		Debug::DrawCircle(position.x, position.y, 6, sf::Color::Blue);
	}
	if (m_Ball->GetOwner() != nullptr)
	{
		CLC_Players* owner = (CLC_Players*)m_Ball->GetOwner();
		if (owner->GetTeam() == CLC_Players::Team::One)
		{
			if (owner->GetPosition().x > m_windowWidth - 80)
			{
				ResetPos();
				m_teamOneScore++;
				return;
			}
		}
		else if (owner->GetTeam() == CLC_Players::Team::Two)
		{
			if (owner->GetPosition().x < 80)
			{
				ResetPos();
				m_teamTwoScore++;
				return;
			}
		}
	}
	OutTheMap();

}

void CLC_Scene::GetSizeWindow()
{
	m_windowWidth = GetWindowWidth();
	m_windowHeight = GetWindowHeight();

	m_midPosY = m_windowHeight / 2;

	borderRightMaxPosX = m_windowWidth / 1.05;
	borderRightPosX = m_windowWidth / 1.20;

	borderLeftMaxPosX = m_windowWidth / 20;
	borderLeftPosX = m_windowWidth / 1.20;
}

void CLC_Scene::CreatePlayer(int x, int y, sf::Color color, CLC_Players::Team team, CLC_Players::Number number)
{
	CLC_Players* player = CreateEntity<CLC_Players>(40, color);
	player->SetPosition(x, y, 0.5f, 0.5f);
	player->SetRigidBody(true);
	player->SetTeam(team);
	player->SetNumber(number);
	//player->SetStats({222, 300, 150, 1, 2});

	if (team == CLC_Players::Team::One)
	{
		m_TeamPlayerOne.push_back(player);
	}
	else if (team == CLC_Players::Team::Two)
	{
		m_TeamPlayerTwo.push_back(player);
	}
}
void CLC_Scene::CreateBall(int x, int y, sf::Color color)
{
	m_Ball = CreateEntity<CLC_Ball>(20, color);
	m_Ball->SetPosition(x, y, 0.5f, 0.5f);
	m_Ball->SetRigidBody(false);
}

void CLC_Scene::ResetPos()
{
	pEntitySelected = nullptr;


	for (CLC_Players* p : m_TeamPlayerOne)
	{
		if (p->GetNumber() == CLC_Players::Number::One)
		{
			p->GoToPosition(borderLeftMaxPosX, 60);
			p->SetPosition(borderLeftMaxPosX, 60);
			
		}
		else if (p->GetNumber() == CLC_Players::Number::Two)
		{
			p->GoToPosition(210, 90);
			p->SetPosition(210, 90);
		}
		else if (p->GetNumber() == CLC_Players::Number::Three)
		{
			p->GoToPosition(300, m_midPosY);
			p->SetPosition(300, m_midPosY);
		}
		else if (p->GetNumber() == CLC_Players::Number::Four)
		{
			p->GoToPosition(210, 640);
			p->SetPosition(210, 640);
		}
		else if (p->GetNumber() == CLC_Players::Number::Five)
		{
			p->GoToPosition(borderLeftMaxPosX, 670);
			p->SetPosition(borderLeftMaxPosX, 670);
		}
	}
	for (CLC_Players* p : m_TeamPlayerTwo)
	{
		if (p->GetNumber() == CLC_Players::Number::One)
		{
			p->GoToPosition(borderRightMaxPosX, 60);
			p->SetPosition(borderRightMaxPosX, 60);
		}
		else if (p->GetNumber() == CLC_Players::Number::Two)
		{
			p->GoToPosition(borderRightPosX, 90);
			p->SetPosition(borderRightPosX, 90);
		}
		else if (p->GetNumber() == CLC_Players::Number::Three)
		{
			p->GoToPosition(m_windowWidth - 300, m_midPosY);
			p->SetPosition(m_windowWidth - 300, m_midPosY);
		}
		else if (p->GetNumber() == CLC_Players::Number::Four)
		{
			p->GoToPosition(borderRightPosX, 640);
			p->SetPosition(borderRightPosX, 640);
		}
		else if (p->GetNumber() == CLC_Players::Number::Five)
		{
			p->GoToPosition(borderRightMaxPosX, 670);
			p->SetPosition(borderRightMaxPosX, 670);
		}
	}
	m_Ball->SetOwner(nullptr);
	m_Ball->SetLastOwner(nullptr);
	m_Ball->GoToPosition(m_windowWidth / 2, m_windowHeight / 2);
	m_Ball->SetPosition(m_windowWidth / 2, m_windowHeight / 2);
}

void CLC_Scene::OutTheMap()
{
	int margin = 5;

	for (CLC_Players* p : m_TeamPlayerOne)
	{
		if (p->GetNumber() == CLC_Players::Number::One || p->GetNumber() == CLC_Players::Number::Two)
		{
			if (p->GetPosition().y > m_windowHeight / 2 - p->GetRadius() - margin)
			{
				p->SetPosition(p->GetPosition().x, m_windowHeight / 2 - p->GetRadius() - margin);
			}
		}
		if (p->GetNumber() == CLC_Players::Number::Three)
		{
			if (p->GetPosition().y < m_windowHeight / 5 + p->GetRadius() + margin)
			{
				p->SetPosition(p->GetPosition().x, m_windowHeight / 5 + p->GetRadius() + margin);
			}
			else if (p->GetPosition().y > m_windowHeight - (m_windowHeight / 5) - p->GetRadius() - margin)
			{
				p->SetPosition(p->GetPosition().x, m_windowHeight - (m_windowHeight / 5) - p->GetRadius() - margin);
			}
		}
		if (p->GetNumber() == CLC_Players::Number::Four || p->GetNumber() == CLC_Players::Number::Five)
		{
			if (p->GetPosition().y < m_windowHeight / 2 + p->GetRadius() + margin)
			{
				p->SetPosition(p->GetPosition().x, m_windowHeight / 2 + p->GetRadius() + margin);
			}
		}
		if (p->GetPosition().x > m_windowWidth - p->GetRadius() - margin)
		{
			p->SetPosition(m_windowWidth - p->GetRadius() - margin, p->GetPosition().y);
		}

		if (p->GetPosition().x < 0 + p->GetRadius() + margin)
		{
			p->SetPosition(0 + p->GetRadius() + margin, p->GetPosition().y);
		}

		if (p->GetPosition().y > m_windowHeight - p->GetRadius() - margin)
		{
			p->SetPosition(p->GetPosition().x, m_windowHeight - p->GetRadius() - margin);
		}

		if (p->GetPosition().y < 0 + p->GetRadius() + margin)
		{
			p->SetPosition(p->GetPosition().x, 0 + p->GetRadius() + margin);
		}
	}
	for (CLC_Players* p : m_TeamPlayerTwo)
	{
		if (p->GetNumber() == CLC_Players::Number::One || p->GetNumber() == CLC_Players::Number::Two)
		{
			if (p->GetPosition().y > m_windowHeight / 2 - p->GetRadius() - margin)
			{
				p->SetPosition(p->GetPosition().x, m_windowHeight / 2 - p->GetRadius() - margin);
			}
		}
		if (p->GetNumber() == CLC_Players::Number::Three)
		{
			if (p->GetPosition().y < m_windowHeight / 5 + p->GetRadius() + margin)
			{
				p->SetPosition(p->GetPosition().x, m_windowHeight / 5 + p->GetRadius() + margin);
			}
			else if (p->GetPosition().y > m_windowHeight - (m_windowHeight / 5) - p->GetRadius() - margin)
			{
				p->SetPosition(p->GetPosition().x, m_windowHeight - (m_windowHeight / 5) - p->GetRadius() - margin);
			}
		}
		if (p->GetNumber() == CLC_Players::Number::Four || p->GetNumber() == CLC_Players::Number::Five)
		{
			if (p->GetPosition().y < m_windowHeight / 2 + p->GetRadius() + margin)
			{
				p->SetPosition(p->GetPosition().x, m_windowHeight / 2 + p->GetRadius() + margin);
			}
		}
		if (p->GetPosition().x > m_windowWidth - p->GetRadius())
		{
			p->SetPosition(m_windowWidth - p->GetRadius(), p->GetPosition().y);
		}

		if (p->GetPosition().x < 0 + p->GetRadius())
		{
			p->SetPosition(0 + p->GetRadius(), p->GetPosition().y);
		}

		if (p->GetPosition().y > m_windowHeight - p->GetRadius())
		{
			p->SetPosition(p->GetPosition().x, m_windowHeight - p->GetRadius());
		}

		if (p->GetPosition().y < 0 + p->GetRadius())
		{
			p->SetPosition(p->GetPosition().x, 0 + p->GetRadius());
		}
	}
}

void CLC_Scene::TrySetSelectedEntity(CLC_Players* pEntity, int x, int y)
{
	if (pEntity->IsInside(x, y) == false)
		return;

	pEntitySelected = pEntity;
}

std::vector<CLC_Players*> CLC_Scene::GetAllPlayerByTeam(CLC_Players::Team team)
{
	if (team == CLC_Players::Team::One)
	{
		return m_TeamPlayerOne;
	}
	else if (team == CLC_Players::Team::Two)
	{
		return m_TeamPlayerTwo;
	}
}

CLC_Ball* CLC_Scene::GetBall()
{
	return m_Ball;
}

void CLC_Scene::ShowScore()
{

	std::string scoreText = "Score : " + std::to_string(m_teamOneScore) + " / " + std::to_string(m_teamTwoScore);

	Debug::DrawText(m_windowWidth / 2, m_windowHeight - (m_windowHeight - 20), scoreText, 0.5f, 0.5f, sf::Color::White);

}

void CLC_Scene::ActiveDebug()
{
	if (m_activeDebug == false)
		return;

	std::string playerStateText = "State :\n";


	// Millieu
	Debug::DrawLine(m_windowWidth - m_windowWidth, m_windowHeight / 2, m_windowWidth, m_windowHeight / 2, sf::Color::Blue);

	// Millieu Haut
	Debug::DrawLine(m_windowWidth - m_windowWidth, m_windowHeight / 5, m_windowWidth, m_windowHeight / 5, sf::Color::Blue);

	// Millieu Bas
	Debug::DrawLine(m_windowWidth - m_windowWidth, m_windowHeight - (m_windowHeight / 5), m_windowWidth, m_windowHeight - (m_windowHeight / 5), sf::Color::Blue);


	//State Team 1
	for (CLC_Players* p : m_TeamPlayerOne)
	{
		if (p->GetNumber() == CLC_Players::Number::One)
		{
			Debug::DrawText(p->GetPosition().x, p->GetPosition().y, playerStateText + p->StateToStr(), 0.5f, 0.5f, sf::Color::White);

		}
		if (p->GetNumber() == CLC_Players::Number::Two)
		{
			Debug::DrawText(p->GetPosition().x, p->GetPosition().y, playerStateText + p->StateToStr(), 0.5f, 0.5f, sf::Color::White);

		}
		if (p->GetNumber() == CLC_Players::Number::Three)
		{
			Debug::DrawText(p->GetPosition().x, p->GetPosition().y, playerStateText + p->StateToStr(), 0.5f, 0.5f, sf::Color::White);

		}
		if (p->GetNumber() == CLC_Players::Number::Four)
		{
			Debug::DrawText(p->GetPosition().x, p->GetPosition().y, playerStateText + p->StateToStr(), 0.5f, 0.5f, sf::Color::White);

		}
		if (p->GetNumber() == CLC_Players::Number::Five)
		{
			Debug::DrawText(p->GetPosition().x, p->GetPosition().y, playerStateText + p->StateToStr(), 0.5f, 0.5f, sf::Color::White);

		}
	}

	//State Team 2
	for (CLC_Players* p : m_TeamPlayerTwo)
	{
		if (p->GetNumber() == CLC_Players::Number::One)
		{
			Debug::DrawText(p->GetPosition().x, p->GetPosition().y, playerStateText + p->StateToStr(), 0.5f, 0.5f, sf::Color::White);

		}
		if (p->GetNumber() == CLC_Players::Number::Two)
		{
			Debug::DrawText(p->GetPosition().x, p->GetPosition().y, playerStateText + p->StateToStr(), 0.5f, 0.5f, sf::Color::White);

		}
		if (p->GetNumber() == CLC_Players::Number::Three)
		{
			Debug::DrawText(p->GetPosition().x, p->GetPosition().y, playerStateText + p->StateToStr(), 0.5f, 0.5f, sf::Color::White);

		}
		if (p->GetNumber() == CLC_Players::Number::Four)
		{
			Debug::DrawText(p->GetPosition().x, p->GetPosition().y, playerStateText + p->StateToStr(), 0.5f, 0.5f, sf::Color::White);

		}
		if (p->GetNumber() == CLC_Players::Number::Five)
		{
			Debug::DrawText(p->GetPosition().x, p->GetPosition().y, playerStateText + p->StateToStr(), 0.5f, 0.5f, sf::Color::White);

		}


		//Affiche les cercles de passe
		for (CLC_Players* p : m_TeamPlayerOne)
		{
			Debug::DrawOutCircle(p->GetPosition().x, p->GetPosition().y, (p->GetRadius() + p->GetZonePass()), sf::Color::Yellow);
		}

		for (CLC_Players* p : m_TeamPlayerTwo)
		{
			Debug::DrawOutCircle(p->GetPosition().x, p->GetPosition().y, (p->GetRadius() + p->GetZonePass()), sf::Color::Yellow);
		}

		//Affiche le cercle de god mod
		for (CLC_Players* p : m_TeamPlayerOne)
		{
			if (p->IsInvincible() == true)
			{
				Debug::DrawOutCircle(p->GetPosition().x, p->GetPosition().y, (p->GetRadius() + 5), sf::Color::Cyan);
			}
		}

		for (CLC_Players* p : m_TeamPlayerTwo)
		{
			if (p->IsInvincible() == true)
			{
				Debug::DrawOutCircle(p->GetPosition().x, p->GetPosition().y, (p->GetRadius() + 5), sf::Color::Cyan);
			}
			
			
		}
	}
}





