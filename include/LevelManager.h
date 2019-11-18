#pragma once
#include "Building.h"
#include "Cappuccino/RigidBody.h"
class LevelManager
{
public:
	LevelManager();
	void update(float dt, Cappuccino::RigidBody& player);

	std::vector <Building*> rooms;
	std::vector <Building*> airlocks;
private:
	float _currentRotation = 0.0f;
	bool _start = true;
	
};