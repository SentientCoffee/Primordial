#pragma once
#include "Building.h"
#include "Cappuccino/RigidBody.h"
#include "Cappuccino/PointLight.h"
#include "Enemy.h"

class EnemyManager
{
public:
	EnemyManager();
	void update(float dt);
	void spawnEnemies();

	std::vector<Enemy*> _enemies;
private:

	
	bool start = true;
};

class LightManager
{
public:
	LightManager(Cappuccino::PointLight& light);
	void update(float dt);
	void resetLights(std::vector<glm::vec3>& lightPos);
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

	EnemyManager _enemyManager;
private:
	
	float _currentRotation = 0.0f;
	bool _start = true;
	LightManager _lightManager;
};

