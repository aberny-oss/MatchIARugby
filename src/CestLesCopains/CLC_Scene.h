#pragma once
#include "Scene.h"
#include "CLC_Players.h"
#include <Vector>

class CLC_Ball;

class CLC_Scene : public Scene
{
private:
	std::vector<CLC_Players*> m_TeamPlayerOne;
	std::vector<CLC_Players*> m_TeamPlayerTwo;
	CLC_Ball* m_Ball;

	CLC_Players* pEntitySelected;

	int m_teamOneScore;
	int m_teamTwoScore;

	float m_windowWidth;
	float m_windowHeight;

	float m_midPosY;

	float borderRightMaxPosX;
	float borderRightPosX;

	float borderLeftMaxPosX;
	float borderLeftPosX;

	bool m_activeDebug = true;


protected:
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void EventMouse(const sf::Event& event);
	void EventKeyboards(const sf::Event& event);
	void OnUpdate() override;
	void GetSizeWindow();

public:
	
	enum class Tag
	{
		Player,
		Ball,

		Count
	};

	void CreatePlayer(int x, int y, sf::Color color, CLC_Players::Team team, CLC_Players::Number number);
	void CreateBall(int x, int y, sf::Color color);

	void ResetPos();
	void OutTheMap();

	void TrySetSelectedEntity(CLC_Players* pEntity, int x, int y);

	std::vector<CLC_Players*> GetAllPlayerByTeam(CLC_Players::Team team);

	CLC_Ball* GetBall();

	void ShowScore();

	void ActiveDebug();




};	