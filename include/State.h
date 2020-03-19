#pragma once
#include "Enemy.h"
#include "Class.h"
#include "Building.h"

class State {
public:

	virtual void update(float dt, Enemy* enemy, Class* player, Building room) = 0;

	bool _changeState = false;
	std::string _newState = "";

	std::string _stateName;

private:

};