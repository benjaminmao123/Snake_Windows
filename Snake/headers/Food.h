#pragma once

#include <random>
#include <iostream>

#include "Entity.h"

static std::random_device rd;
static std::mt19937 rng(rd());

class Food : public Entity
{
public:
	Food(sf::RenderWindow & Window, sf::Vector2f PlayerSize)
	{
		Score = 0;
		Active = true;
		EntityID = 0;
		Eaten = 0;
		Size = PlayerSize;
		Rect.setSize(Size);
		Rect.setPosition(GeneratePosition(Window));
		Rect.setFillColor(sf::Color::Red);
	}

	sf::Vector2f GeneratePosition(sf::RenderWindow & Window)
	{
		static std::uniform_int_distribution<int> XPosition(0, static_cast<int>(Window.getSize().x - Size.x));
		int X = XPosition(rng);
		while (X % 16)
			X = XPosition(rng);
		std::uniform_int_distribution<int> YPosition(0, static_cast<int>(Window.getSize().y - Size.y));
		int Y = YPosition(rng);
		while (Y % 16)
			Y = YPosition(rng);

		return sf::Vector2f(static_cast<float>(X), static_cast<float>(Y));
	}

	sf::RectangleShape GetFood()
	{
		return Rect;
	}

	void Update(sf::RenderWindow & Window, float ElapsedTime)
	{
		if (Eaten)
		{
			Rect.setPosition(GeneratePosition(Window));
			Eaten = false;
		}
	}

	void Render(sf::RenderWindow & Window)
	{
		Window.draw(Rect);
	}
};