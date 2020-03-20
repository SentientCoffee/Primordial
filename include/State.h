#pragma once
#include "Class.h"
#include "Building.h"

class State {
public:
	State(std::string statename);
	virtual void update(float dt, Enemy* enemy, Class* player, Building* room) = 0;

	bool _changeState = false;
	std::string _newState = "";

	std::string _stateName;

private:

};

class BaseState : public State {
public:
	BaseState();
	void update(float dt, Enemy* enemy, Class* player, Building* room)override;

private:

};

class WanderState : public State {
public:
	WanderState();
	void update(float dt, Enemy* enemy, Class* player, Building* room)override;

private:

};