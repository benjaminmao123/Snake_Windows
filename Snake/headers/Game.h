#pragma once

#include <memory>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <chrono>
#include <thread>
#include <iostream>
#include <string>

#include "StateManager.h"
#include "GameState.h"
#include "PauseState.h"
#include "MainMenuState.h"

bool HasController = false;

class Game
{
public:
	Game(unsigned int Width, unsigned int Height)
	{
		Window = std::make_shared<sf::RenderWindow>(sf::VideoMode(Width, Height), "Snake");
	}

	bool Run()
	{
		HasController = sf::Joystick::isConnected(0);
		StateStack.emplace_back(std::make_shared<MainMenuState>());
		Window->setKeyRepeatEnabled(false);

		while (Window->isOpen())
		{
			ElapsedTime = Clock.restart().asSeconds();
			Window->clear(sf::Color::Black);
			SManager.Update(*Window, ElapsedTime);
			SManager.Render(*Window);
			ProcessEvents();
			Window->display();
			std::this_thread::sleep_for(std::chrono::milliseconds(35));
		}

		return 1;
	}

	void ProcessEvents()
	{
		while (Window->pollEvent(Event))
			switch (Event.type)
			{
			case sf::Event::Closed:
				Window->close();
				break;
			case sf::Event::KeyPressed:
				if (Event.key.code == sf::Keyboard::Escape)
					if (StateStack.back()->Type == "Main Menu")
						Window->close();
					else if (StateStack.back()->Type == "Game")
						StateStack.emplace_back(std::make_shared<PauseState>());
					else if (StateStack.back()->Type == "Paused")
						StateStack.pop_back();
					else if (StateStack.back()->Type == "Display Score")
						StateStack.pop_back();
					else if (StateStack.back()->Type == "Replace Score")
						StateStack.pop_back();
					else if (StateStack.back()->Type == "Game Over")
						while (StateStack.size() > 1)
							StateStack.pop_back();
					else if (StateStack.back()->Type == "Control Setup")
						StateStack.pop_back();
				break;
			case sf::Event::JoystickButtonPressed:
				if (Event.joystickButton.button == 7)
					if (StateStack.back()->Type == "Game")
						StateStack.emplace_back(std::make_shared<PauseState>());
				else if (Event.joystickButton.button == 1)
					if (StateStack.back()->Type == "Paused")
						StateStack.pop_back();
					else if (StateStack.back()->Type == "Display Score")
						StateStack.pop_back();
					else if (StateStack.back()->Type == "Replace Score")
						StateStack.pop_back();
					else if (StateStack.back()->Type == "Control Setup")
						StateStack.pop_back();
				break;
			default:
				break;
			}
	}

private:
	StateManager SManager;
	sf::Event Event;
	std::shared_ptr<sf::RenderWindow> Window;
	sf::Clock Clock;
	float ElapsedTime;
};