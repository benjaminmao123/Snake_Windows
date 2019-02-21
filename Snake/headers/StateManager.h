#pragma once

#include <vector>
#include <memory>

#include "State.h"

std::vector<std::shared_ptr<State>> StateStack;

class StateManager
{
public:
	StateManager() { }

	void Update(sf::RenderWindow & Window, float ElapsedTime)
	{
		if (!StateStack.empty())
			StateStack.back()->Update(Window, ElapsedTime);
	}

	void Render(sf::RenderWindow & Window)
	{
		if (!StateStack.empty())
			StateStack.back()->Render(Window);
	}
};