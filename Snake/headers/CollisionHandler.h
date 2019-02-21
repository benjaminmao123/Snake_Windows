#pragma once

#include <iostream>

#include "Entity.h"
#include "Player.h"
#include "Food.h"
#include "Limb.h"

class CollisionHandler
{
public:
	CollisionHandler() { }

	bool CheckFoodCollision(std::deque<Limb> Body, std::shared_ptr<Entity> Cherry)
	{
		if (Cherry->Rect.getGlobalBounds().intersects(Body[0].Part.getGlobalBounds()))
		{
			Cherry->Eaten = true;
			return true;
		}

		return false;
	}

	bool CheckSelfCollision(std::deque<Limb> Body)
	{
		sf::Vector2f Temp;
		Temp.x = Body[0].Position.x;
		Temp.y = Body[0].Position.y;

		for (auto i = 1U; i < Body.size(); ++i)
			if (Temp.x == Body[i].Position.x && Temp.y == Body[i].Position.y)
				return true;

		return false;
	}
};