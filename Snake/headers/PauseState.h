#pragma once

#include <SFML\Graphics.hpp>
#include <memory>
#include <iostream>

#include "State.h"
#include "GameState.h"
#include "Game.h"

class PauseState : public State
{
public:
	PauseState() : Selected(0), Up(false), Down(false), A(false)
	{
		Type = "Paused";

		if (!Font.loadFromFile("Fonts/AlexisItalic.ttf"))
			std::cout << "Error opening file." << std::endl;
	}

	void SetMenu(sf::RenderWindow & Window)
	{
		Title.setFont(Font);
		Title.setString("Paused");
		Title.setOrigin(Title.getGlobalBounds().width / 2, Title.getGlobalBounds().height / 2);
		Title.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(Window.getSize().y / 8));

		Resume.setFont(Font);
		Resume.setString("Resume");
		Resume.setOrigin(Resume.getGlobalBounds().width / 2, Resume.getGlobalBounds().height / 2);
		Resume.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(Window.getSize().y / 2));

		QuitToMenu.setFont(Font);
		QuitToMenu.setString("Quit to Main Menu");
		QuitToMenu.setOrigin(QuitToMenu.getGlobalBounds().width / 2, QuitToMenu.getGlobalBounds().height / 2);
		QuitToMenu.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(Window.getSize().y / 2) + QuitToMenu.getGlobalBounds().height * 2);

		Quit.setFont(Font);
		Quit.setString("Quit");
		Quit.setOrigin(Quit.getGlobalBounds().width / 2, Quit.getGlobalBounds().height / 2);
		Quit.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(Window.getSize().y / 2) + Quit.getGlobalBounds().height * 4);

		Resume.setFillColor(sf::Color::White);
		QuitToMenu.setFillColor(sf::Color::White);
		Quit.setFillColor(sf::Color::White);
	}

	void Update(sf::RenderWindow & Window, float ElapsedTime) override
	{
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
			Selected = 1;

		if (!sf::Joystick::isButtonPressed(0, 0))
			if (!Selected && A)
				StateStack.pop_back();
			else if (Selected == 1 && A)
				while (StateStack.size() > 1)
					StateStack.pop_back();
			else if (Selected == 2 && A)
				Window.close();

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
			if (!Selected && Enter)
				StateStack.pop_back();
			else if (Selected == 1 && Enter)
				while (StateStack.size() > 1)
					StateStack.pop_back();
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
			Resume.setFillColor(sf::Color::Green);
		else if (Selected == 1)
			QuitToMenu.setFillColor(sf::Color::Green);
		else if (Selected == 2)
			Quit.setFillColor(sf::Color::Green);

		Window.draw(Title);
		Window.draw(Resume);
		Window.draw(QuitToMenu);
		Window.draw(Quit);
	}

private:
	sf::Font Font;
	sf::Text Title;
	sf::Text QuitToMenu;
	sf::Text Quit;
	sf::Text Resume;
	int Selected;
	bool Up;
	bool Down;
	bool Enter;
	bool A;
	StateManager SManager;
};