#pragma once
#include "State.h"
#include <memory>

class StateMachine {
public:
	void update(float dt, Enemy* enemy, Class* player, Building room);

private:

	std::shared_ptr<State> _currentState;
	std::vector<std::shared_ptr<State>> _states;
};