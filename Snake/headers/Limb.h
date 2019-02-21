#pragma once

#include <SFML\Graphics.hpp>

struct Limb
{
	Limb() { }

	Limb(sf::Vector2f Size, sf::Vector2f Position) : Position(Position)
	{
		Part.setSize(Size);
		Part.setPosition(Position);
		Part.setOutlineColor(sf::Color::Green);
		Part.setOutlineThickness(1);
	}

	sf::Vector2f Position;
	sf::RectangleShape Part;
};