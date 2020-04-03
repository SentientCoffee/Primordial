#pragma once
#include "State.h"
#include <memory>


class StateMachine {
public:
	void update(float dt, Enemy* enemy, Class* player, Building* room);

	void addState(std::shared_ptr<State> newState) { _states.push_back(newState); }

	std::shared_ptr<State> _currentState;
private:
	std::vector<std::shared_ptr<State>> _states;
};