#pragma once
#include "Building.h"
#include "Cappuccino/RigidBody.h"
#include "Cappuccino/PointLight.h"
#include "Enemy.h"
#include "Shop.h"
#include "Loot.h"



class EnemyManager
{
public:
	EnemyManager();
	void update(float dt);
	float spawnEnemy(glm::vec3 position,int type);

	std::vector<Enemy*> _enemies;
private:

	
	bool start = true;
};

class LightManager
{
public:
	LightManager(std::vector<Cappuccino::PointLight>& lights);
	void update(float dt);
	void resetLights(std::vector<glm::vec3>& lightPos);
private:
	std::vector<Cappuccino::PointLight>* _light;
};

class LevelManager
{
public:
	LevelManager(std::vector<Cappuccino::PointLight>& lights);
	void update(float dt, Cappuccino::RigidBody& player);

	std::vector <Building*> rooms;
	std::vector <Building*> airlocks;
	unsigned _currentRoom = 0;

	ShopTerminal* _testShopTerminal;

	std::vector<Chest*> _chests;

	EnemyManager _enemyManager;
private:
	
	float _currentRotation = 0.0f;
	bool _start = true;
	LightManager _lightManager;
};

