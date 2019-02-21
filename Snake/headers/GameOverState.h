#pragma once

#include <iostream>
#include <SFML\Graphics.hpp>
#include <fstream>
#include <algorithm>

#include "State.h"
#include "StateManager.h"
#include "ReplaceScoreState.h"

class GameOverState : public State
{
public:
	GameOverState(int Score) : Selected(0), Up(false), Down(false), Enter(false), Score(Score), A(false)
	{
		Type = "Game Over";

		if (!Font.loadFromFile("Fonts/AlexisItalic.ttf"))
			std::cout << "Error opening file." << std::endl;
	}

	void SetMenu(sf::RenderWindow & Window)
	{
		Title.setFont(Font);
		Title.setString("Game Over");
		Title.setOrigin(Title.getGlobalBounds().width / 2, Title.getGlobalBounds().height / 2);
		Title.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(Window.getSize().y / 8));

		MainMenu.setFont(Font);
		MainMenu.setString("Main Menu");
		MainMenu.setOrigin(MainMenu.getGlobalBounds().width / 2, MainMenu.getGlobalBounds().height / 2);
		MainMenu.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(Window.getSize().y / 2));

		SaveScore.setFont(Font);
		SaveScore.setString("Save Score");
		SaveScore.setOrigin(SaveScore.getGlobalBounds().width / 2, SaveScore.getGlobalBounds().height / 2);
		SaveScore.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(Window.getSize().y / 2) + SaveScore.getGlobalBounds().height * 2);

		Quit.setFont(Font);
		Quit.setString("Quit");
		Quit.setOrigin(Quit.getGlobalBounds().width / 2, Quit.getGlobalBounds().height / 2);
		Quit.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(Window.getSize().y / 2) + Quit.getGlobalBounds().height * 4);

		MainMenu.setFillColor(sf::Color::White);
		SaveScore.setFillColor(sf::Color::White);
		Quit.setFillColor(sf::Color::White);
	}

	bool WriteScore()
	{
		std::ifstream Ifs("Scores/scoreslist.txt", std::ios_base::in);
		std::vector<int> ScoresRead;
		int ScoreHolder;
		while (Ifs >> ScoreHolder)
			ScoresRead.emplace_back(ScoreHolder);
		if (ScoresRead.size() >= 16)
		{
			StateStack.emplace_back(std::make_shared<ReplaceScoreState>(Score));
			return false;
		}
		else
		{
			std::ofstream Output("Scores/scoreslist.txt", std::ios_base::app | std::ios_base::out);
			Output << Score << "\n";
		}

		return true;
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
				while (StateStack.size() > 1)
					StateStack.pop_back();
			else if (Selected == 1 && A)
			{
				if (WriteScore())
					while (StateStack.size() > 1)
						StateStack.pop_back();
			}
			else if (Selected == 2 && A)
				Window.close();

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
			if (!Selected && Enter)
				while (StateStack.size() > 1)
					StateStack.pop_back();
			else if (Selected == 1 && Enter)
			{
				if (WriteScore())
					while (StateStack.size() > 1)
						StateStack.pop_back();
			}
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
			MainMenu.setFillColor(sf::Color::Green);
		else if (Selected == 1)
			SaveScore.setFillColor(sf::Color::Green);
		else if (Selected == 2)
			Quit.setFillColor(sf::Color::Green);

		Window.draw(Title);
		Window.draw(MainMenu);
		Window.draw(SaveScore);
		Window.draw(Quit);
	}

private:
	sf::Font Font;
	sf::Text Title;
	sf::Text MainMenu;
	sf::Text SaveScore;
	sf::Text Quit;
	int Selected;
	bool Up;
	bool Down;
	bool Enter;
	bool A;
	StateManager SManager;
	int Score;
	std::vector<int> ScoreList;
};