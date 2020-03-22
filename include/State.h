#pragma once
#include "Class.h"
#include "Building.h"
#include "Cappuccino/Random.h"

class Enemy;

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
	bool _walking = false;
	void createNewDestination(Building* room);
	std::vector<std::shared_ptr<NavPoint>> _point2point;
	std::shared_ptr<NavPoint> objective;
	Cappuccino::HitBox box = Cappuccino::HitBox(glm::vec3(0.0f),0.2f);
};

class AttackState : public State {
public:
	AttackState();
	void update(float dt, Enemy* enemy, Class* player, Building* room)override;

private:

};