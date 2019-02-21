#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>

#include "State.h"
#include "StateManager.h"
#include "GameState.h"
#include "Player.h"
#include "ShowScoresState.h"

class MainMenuState : public State
{
public:
	MainMenuState() : Selected(0), Up(false), Down(false), Enter(false), A(false)
	{
		Type = "Main Menu";

		if (!Font.loadFromFile("Fonts/AlexisItalic.ttf"))
			std::cout << "Error opening file." << std::endl;
	}

	void SetMenu(sf::RenderWindow & Window)
	{
		Title.setFont(Font);
		Title.setString("Snake");
		Title.setOrigin(Title.getGlobalBounds().width / 2, Title.getGlobalBounds().height / 2);
		Title.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(Window.getSize().y / 8));

		Start.setFont(Font);
		Start.setString("Start");
		Start.setOrigin(Start.getGlobalBounds().width / 2, Start.getGlobalBounds().height / 2);
		Start.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(Window.getSize().y / 2));

		Scores.setFont(Font);
		Scores.setString("Scores");
		Scores.setOrigin(Scores.getGlobalBounds().width / 2, Scores.getGlobalBounds().height / 2);
		Scores.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(Window.getSize().y / 2) + Scores.getGlobalBounds().height * 2);

		Quit.setFont(Font);
		Quit.setString("Quit");
		Quit.setOrigin(Quit.getGlobalBounds().width / 2, Quit.getGlobalBounds().height / 2);
		Quit.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(Window.getSize().y / 2) + Quit.getGlobalBounds().height * 4);

		Start.setFillColor(sf::Color::White);
		Scores.setFillColor(sf::Color::White);
		Quit.setFillColor(sf::Color::White);
	}

	void Update(sf::RenderWindow & Window, float ElapsedTime) override
	{
		GameEnd = false;

		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) <= -100)
		{
			Selected -= 1;
			std::this_thread::sleep_for(std::chrono::milliseconds(125));
		}
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) >= 50)
		{
			Selected += 1;
			std::this_thread::sleep_for(std::chrono::milliseconds(125));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !Up)
			Selected -= 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !Down)
			Selected += 1;
		if (Selected > 2)
			Selected = 0;
		if (Selected < 0)
			Selected = 2;

		if (!sf::Joystick::isButtonPressed(0, 0))
			if (!Selected && A)
				StateStack.emplace_back(std::make_shared<GameState>(Window));
			else if (Selected == 1 && A)
				StateStack.emplace_back(std::make_shared<ShowScoresState>());
			else if (Selected == 2 && A)
				Window.close();

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
			if (!Selected && Enter)
				StateStack.emplace_back(std::make_shared<GameState>(Window));
			else if (Selected == 1 && Enter)
				StateStack.emplace_back(std::make_shared<ShowScoresState>());
			else if (Selected == 2 && Enter)
				Window.close();

		Up = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
		Down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
		Enter = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return);
		A = sf::Joystick::isButtonPressed(0, 0);
	}

	void Render(sf::RenderWindow & Window) override
	{
		SetMenu(Window);

		if (!Selected)
			Start.setFillColor(sf::Color::Green);
		else if (Selected == 1)
			Scores.setFillColor(sf::Color::Green);
		else if (Selected == 2)
			Quit.setFillColor(sf::Color::Green);

		Window.draw(Title);
		Window.draw(Start);
		Window.draw(Scores);
		Window.draw(Quit);
	}

private:
	sf::Font Font;
	sf::Text Title;
	sf::Text Quit;
	sf::Text Start;
	sf::Text Scores;
	int Selected;
	bool Up;
	bool Down;
	bool A;
	bool Enter;
};
