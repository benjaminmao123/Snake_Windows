#pragma once

#include <SFML\Graphics.hpp>
#include <string>

class State
{
public:
	State() { }

	virtual void Update(sf::RenderWindow & Window, float ElapsedTime) = 0;
	virtual void Render(sf::RenderWindow & Window) = 0;

	std::string Type;
};