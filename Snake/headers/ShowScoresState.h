#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "State.h"
#include "StateManager.h"

class ShowScoresState : public State
{
public:
	ShowScoresState() : Selected(0), Up(false), Down(false), Enter(false), A(false)
	{
		Type = "Display Score";

		if (!Font.loadFromFile("Fonts/AlexisItalic.ttf"))
			std::cout << "Error opening file." << std::endl;

		std::ifstream Input("Scores/scoreslist.txt", std::ios_base::in);
		int Temp;
		while (Input >> Temp)
			ScoreList.emplace_back(Temp);
		std::sort(ScoreList.begin(), ScoreList.end(), std::greater<int>());
	}

	void SetMenu(sf::RenderWindow & Window)
	{
		Title.setFont(Font);
		Title.setString("Scores");
		Title.setOrigin(Title.getGlobalBounds().width / 2, Title.getGlobalBounds().height / 2);
		Title.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(Window.getSize().y / 8));
		
		int Factor = 0;
		std::vector<sf::Text> TempList;
		TextList.swap(TempList);

		for (auto i = 0U; i < ScoreList.size(); ++i)
		{
			sf::Text T;

			T.setFont(Font);
			T.setString(std::to_string(i + 1) + ". " + std::to_string(ScoreList[i]));
			T.setOrigin(T.getGlobalBounds().width / 2, 0);
			T.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(144 + Factor));
			T.setFillColor(sf::Color::White);
			Factor += 24;

			TextList.emplace_back(T);
		}

		Back.setFont(Font);
		Back.setString("Back");
		Back.setOrigin(Back.getGlobalBounds().width / 2, Back.getGlobalBounds().height / 2);
		Back.setPosition(static_cast<float>(Window.getSize().x / 2), static_cast<float>(Window.getSize().y / 2) + Back.getGlobalBounds().height * 16);
		Back.setFillColor(sf::Color::White);
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
		if (Selected > static_cast<int>(ScoreList.size()))
			Selected = 0;
		if (Selected < 0)
			Selected = ScoreList.size();

		if (!sf::Joystick::isButtonPressed(0, 0))
			if (Selected == ScoreList.size() && A)
				StateStack.pop_back();

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
			if (Selected == ScoreList.size() && Enter)
				StateStack.pop_back();

		Up = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
		Down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
		Enter = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return);
		A = sf::Joystick::isButtonPressed(0, 0);
	}

	void Render(sf::RenderWindow & Window) override
	{
		SetMenu(Window);

		if (Selected >= 0 && Selected < static_cast<int>(TextList.size()))
			TextList[Selected].setFillColor(sf::Color::Green);
		if (Selected >= static_cast<int>(ScoreList.size()))
			Back.setFillColor(sf::Color::Green);

		Window.draw(Title);
		for (auto i : TextList)
			Window.draw(i);
		Window.draw(Back);
	}

private:
	sf::Font Font;
	sf::Text Title;
	std::vector<sf::Text> TextList;
	sf::Text Back;
	int Selected;
	bool Up;
	bool Down;
	bool Enter;
	bool A;
	StateManager SManager;
	std::vector<int> ScoreList;
};