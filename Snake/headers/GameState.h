#pragma once

#include <memory>
#include <SFML\Graphics.hpp>
#include <string>

#include "State.h"
#include "Player.h"
#include "EntityManager.h"
#include "Food.h"
#include "CollisionHandler.h"
#include "GameOverState.h"

class GameState : public State
{
public:
	GameState(sf::RenderWindow & Window)
	{
		Type = "Game";

		Ent = std::make_shared<Food>(Window, sf::Vector2f(16, 16));
		EntManager.AddEntity(Ent);
		Ent = std::make_shared<Player>(sf::Vector2f(16, 16), sf::Vector2f(0, 0), EntManager.GetEntity(0));
		EntManager.AddEntity(Ent);
	}

	void Update(sf::RenderWindow & Window, float ElapsedTime) override
	{
		for (auto i = 0U; i < EntManager.EntityListSize(); ++i)
			EntManager.GetEntity(i)->Update(Window, ElapsedTime);

		int Score = EntManager.GetEntity(1)->Score;

		EntManager.Update();

		if (GameEnd)
			StateStack.emplace_back(std::make_shared<GameOverState>(Score));
	}

	void Render(sf::RenderWindow & Window) override
	{
		for (auto i = 0U; i < EntManager.EntityListSize(); ++i)
			EntManager.GetEntity(i)->Render(Window);
	}

private:
	std::shared_ptr<Entity> Ent;
	EntityManager EntManager;
};