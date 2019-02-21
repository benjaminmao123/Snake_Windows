#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\Audio.hpp>
#include <queue>
#include <iostream>

#include "Entity.h"
#include "CollisionHandler.h"
#include "Limb.h"

class CollisionHandler;

bool GameEnd = false;

class Player : public Entity
{
public:
	Player(sf::Vector2f Dimensions, sf::Vector2f Position, std::shared_ptr<Entity> Cherry)
	{
		if (!Font.loadFromFile("Fonts/AlexisItalic.ttf"))
			std::cout << "Error opening file." << std::endl;
		Points.setFillColor(sf::Color::Yellow);
		EntityID = 1;
		Eaten = 0;
		Active = true;
		this->Cherry = Cherry;
		this->ColHandler = ColHandler;
		Score = 0;
		Size.x = Dimensions.x;
		Size.y = Dimensions.y;
		Body.emplace_back(Limb(Size, sf::Vector2f(Position.x, Position.y)));
		Head = Body.front();
		Tail = Body.back();
		Direction = 4;
		if (!SndBuffer.loadFromFile("Sounds/Eat_Food.wav"))
			std::cout << "Error opening file." << std::endl;
		Snd.setBuffer(SndBuffer);
		Snd.setVolume(10);
	}

	void HandleMovement(sf::RenderWindow & Window)
	{
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) <= -100 && Direction != 2)
			Direction = 1;
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) >= 100 && Direction != 1)
			Direction = 2;
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) <= -100 && Direction != 4)
			Direction = 3;
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) >= 100 && Direction != 3)
			Direction = 4;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && Direction != 2)
			Direction = 1;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && Direction != 1)
			Direction = 2;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && Direction != 4)
			Direction = 3;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && Direction != 3)
			Direction = 4;

		switch (Direction)
		{
		case 1:
			Body.pop_back();
			if (Head.Position.y <= 0)
				Body.emplace_front(Limb(Size, sf::Vector2f(Head.Position.x, Window.getSize().y - Size.y)));
			else
				Body.emplace_front(Limb(Size, sf::Vector2f(Head.Position.x, Head.Position.y - Size.y)));
			Head = Body.front();
			Tail = Body.back();
			break;
		case 2:
			Body.pop_back();
			if (Head.Position.y >= (Window.getSize().y - Size.y))
				Body.emplace_front(Limb(Size, sf::Vector2f(Head.Position.x, 0)));
			else
				Body.emplace_front(Limb(Size, sf::Vector2f(Head.Position.x, Head.Position.y + Size.y)));
			Head = Body.front();
			Tail = Body.back();
			break;
		case 3:
			Body.pop_back();
			if (Head.Position.x <= 0)
				Body.emplace_front(Limb(Size, sf::Vector2f(Window.getSize().x - Size.x, Head.Position.y)));
			else
				Body.emplace_front(Limb(Size, sf::Vector2f(Head.Position.x - Size.x, Head.Position.y)));
			Head = Body.front();
			Tail = Body.back();
			break;
		case 4:
			Body.pop_back();
			if (Head.Position.x >= (Window.getSize().x - Size.x))
				Body.emplace_front(Limb(Size, sf::Vector2f(0, Head.Position.y)));
			else
				Body.emplace_front(Limb(Size, sf::Vector2f(Head.Position.x + Size.x, Head.Position.y)));
			Head = Body.front();
			Tail = Body.back();
			break;
		default:
			break;
		}
	}

	void HandleCollision(sf::RenderWindow & Window)
	{
		if (ColHandler.CheckFoodCollision(Body, Cherry))
		{
			Snd.play();
			++Score;
			switch (Direction)
			{
			case 1:
				if (Head.Position.y <= 0)
					Body.emplace_front(Limb(Size, sf::Vector2f(Head.Position.x, Window.getSize().y - Size.y)));
				else
					Body.emplace_front(Limb(Size, sf::Vector2f(Head.Position.x, Head.Position.y - Size.y)));
				Head = Body.front();
				break;
			case 2:
				if (Head.Position.y >= (Window.getSize().y - Size.y))
					Body.emplace_front(Limb(Size, sf::Vector2f(Head.Position.x, 0)));
				else
					Body.emplace_front(Limb(Size, sf::Vector2f(Head.Position.x, Head.Position.y + Size.y)));
				Head = Body.front();
				break;
			case 3:
				if (Head.Position.x <= 0)
					Body.emplace_front(Limb(Size, sf::Vector2f(Window.getSize().x - Size.x, Head.Position.y)));
				else
					Body.emplace_front(Limb(Size, sf::Vector2f(Head.Position.x - Size.x, Head.Position.y)));
				Head = Body.front();
				break;
			case 4:
				if (Head.Position.x >= (Window.getSize().x - Size.x))
					Body.emplace_front(Limb(Size, sf::Vector2f(0, Head.Position.y)));
				else
					Body.emplace_front(Limb(Size, sf::Vector2f(Head.Position.x + Size.x, Head.Position.y)));
				Head = Body.front();
				break;
			default:
				break;
			}
		}

		if (ColHandler.CheckSelfCollision(Body))
		{
			Active = false;
			GameEnd = true;
		}
	}

	void DisplayHUD(sf::RenderWindow & Window)
	{
		Points.setFont(Font);
		Points.setString(std::to_string(Score));
		Points.setOrigin(Points.getGlobalBounds().width / 2, Points.getGlobalBounds().height / 2);
		Points.setPosition(static_cast<float>(Window.getSize().x / 2), 0);
		Window.draw(Points);
	}

	void Update(sf::RenderWindow & Window, float ElapsedTime) override
	{
		HandleMovement(Window);
		HandleCollision(Window);
	}

	void Render(sf::RenderWindow & Window) override
	{
		for (auto i : Body)
			Window.draw(i.Part);

		DisplayHUD(Window);
	}

private:
	std::shared_ptr<Entity> Cherry;
	int Direction;
	std::deque<Limb> Body;
	Limb Head;
	Limb Tail;
	CollisionHandler ColHandler;
	sf::Font Font;
	sf::Text Points;
	sf::SoundBuffer SndBuffer;
	sf::Sound Snd;
};