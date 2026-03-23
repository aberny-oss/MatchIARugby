#include "main.h"
#include "GameManager.h"
#include "CLC_Scene.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include <cstdlib>
#include <crtdbg.h>

	int main()
{
	GameManager* game = GameManager::Get();

	game->CreateWindow(1280, 720, "C'est Les Copains", 60, sf::Color{ 40, 120, 25,255 });

	game->LaunchScene<CLC_Scene>();

	return 0;
}
