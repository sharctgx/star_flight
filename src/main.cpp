#include "stdio.h"
#include "math.h"
#include "SFML/Graphics.hpp"
#include <cmath>
#include "GameSystem.h"
#include "Vector.h"
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
	GameSystem gameSystem;

	sf::RenderWindow window(sf::VideoMode(gameSystem.screenWidth, gameSystem.screenHeight), "STAR FLIGHT");
	//window.setFramerateLimit(30);
	float dt;
	sf::Clock clock;

	bool pause = false;
	float changingTime = 0;
	dt = 0.0f;
	int state = 0;

	sf::String text;

	float prevTime = clock.getElapsedTime().asSeconds();
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			window.close();

			if (event.type == sf::Event::KeyPressed&&event.key.code == sf::Keyboard::Space)
			{
				if (state == 0)
				{
					state = 1;
				}
				if (state == 2)
				{
					state = 1;
					gameSystem.NewGame();
				}
			}
			if (state==0&&event.type == sf::Event::TextEntered)
			{
				// Handle ASCII characters only
				if (event.text.unicode < 123 && event.text.unicode>47)
				{
					gameSystem.name	+= static_cast<char>(event.text.unicode);
				}
				if (event.text.unicode == 8 && gameSystem.name.length()>0)
				{
					gameSystem.name = gameSystem.name.substr(0, gameSystem.name.length() - 1);
				}
				
			}

		};

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		{
			if (prevTime - changingTime > 0.3)
			{
				pause = !pause;
				changingTime = clock.getElapsedTime().asSeconds();
			}
		}

		if (state == 0)
			gameSystem.StartScreen(&window);
		if (state == 1)
		{
			//if (gameSystem.collision)
			//if ((gameSystem.collision) && (clock.getElapsedTime().asSeconds() - gameSystem.collisionTime > 0))
			if ((gameSystem.collision) && (prevTime - gameSystem.collisionTime > 0))
			{
   				gameSystem.FinalScreen(&window);
				state = 2;
			}
			else
				gameSystem.Draw(&window, dt);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			gameSystem.NewGame();
			pause = true;
		}
		float currTime = clock.getElapsedTime().asSeconds();
		if (pause == true)
			dt = 0;
		else
		{
			dt = currTime - prevTime;
			if (dt > 0.1) dt = 0.1;
		}
		prevTime = currTime;

		//window.clear();
	}

	return 0;
}