#pragma once

#include <SFML\Graphics.hpp>

class Entity
{
public:
	Entity() { }

	virtual void Update(sf::RenderWindow & Window, float ElapsedTime) = 0;
	virtual void Render(sf::RenderWindow & Window) = 0;

	int EntityID;
	bool Active;
	bool Eaten;
	int Score;
	sf::Vector2f Size;
	sf::RectangleShape Rect;
};