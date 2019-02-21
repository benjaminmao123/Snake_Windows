#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <string>

#include "Entity.h"

class Entity;

class EntityManager
{
public:
	EntityManager() { }

	void AddEntity(std::shared_ptr<Entity> Entity)
	{
		EntityList.emplace_back(Entity);
	}

	const std::shared_ptr<Entity> GetEntity(int Index) const
	{
		if (!EntityList.size())
			return nullptr;

		if (Index >= EntityList.size() && EntityList.size())
			return EntityList[EntityList.size() - 1];
		else if (Index < 0 && EntityList.size())
			return EntityList[0];

		return EntityList[Index];
	}

	const unsigned int EntityListSize() const
	{
		return EntityList.size();
	}

	void Update()
	{
		int Index = 0;
		auto i = EntityList.begin();

		while (i != EntityList.end())
		{
			if (!(*i)->Active)
			{
				i = EntityList.erase(EntityList.begin() + Index);
				--Index;
			}
			else
			{
				++i;
				++Index;
			}
		}
	}

private:
	std::vector<std::shared_ptr<Entity>> EntityList;
};