#pragma once
#include "Building.h"
#include "Cappuccino/RigidBody.h"
#include "Cappuccino/PointLight.h"
class LightManager
{
public:
	LightManager(Cappuccino::PointLight& light);
	void update(float dt);

private:
	Cappuccino::PointLight* _light;
};

class LevelManager
{
public:
	LevelManager(Cappuccino::PointLight& light);
	void update(float dt, Cappuccino::RigidBody& player);

	std::vector <Building*> rooms;
	std::vector <Building*> airlocks;
	unsigned _currentRoom = 0;
private:
	
	float _currentRotation = 0.0f;
	bool _start = true;
	LightManager _lightManager;
};

