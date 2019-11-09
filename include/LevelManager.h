#pragma once
#include "Building.h"
#include "Cappuccino/RigidBody.h"
class LevelManager
{
public:
	LevelManager();
	void update(float dt,Cappuccino::RigidBody player);


private:


	std::vector <Building> rooms;
	std::vector <Building> airlocks;
};