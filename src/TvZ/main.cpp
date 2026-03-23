
#include <SFML/Graphics.hpp>
#include <iostream>

#include "GameManager.h"
#include "PvZScene.h"

#include <cstdlib>
#include <crtdbg.h>

int main()
{
	GameManager* pInstance = GameManager::Get();

	pInstance->CreateWindow(1280, 720, "PvZ", 60, sf::Color::Black);

	pInstance->LaunchScene<PvZScene>();

	return 0;
}