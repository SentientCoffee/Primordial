#pragma once
#include "Building.h"
#include "Class.h"
#include "Enemy.h"
#include "Loot.h"
#include "Shop.h"

#include <Cappuccino/PointLight.h>
#include <Cappuccino/Random.h>
#include <Cappuccino/RigidBody.h>


class EnemyManager
{
public:
	EnemyManager() = default;
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
	void resetLights(std::vector<glm::vec3>& lightPos) const;
private:
	std::vector<Cappuccino::PointLight>* _light;
};

class LevelManager
{
public:
	LevelManager(std::vector<Cappuccino::PointLight>& lights);
	void update(float dt, Class* player);

	std::vector <Building*> _rooms;
	std::vector <Building*> airlocks;
	std::vector<Door*> _entrancesL;
	std::vector<Door*> _entrancesR;
	unsigned _currentRoom = 1;
	Teleporter* _teleporterA = nullptr;
	Teleporter* _teleporterB = nullptr;

	Building* _lootRoom = nullptr;

	ShopTerminal* _testShopTerminal = nullptr;

	std::vector<Chest*> _chests;

	std::vector<Cappuccino::UIText*> _tutorial;
	Cappuccino::UserInterface ui;
	Chest* _lootChest = NULL;

	EnemyManager _enemyManager;
private:
	
	float _currentRotation = 0.0f;
	bool _start = true;
	LightManager _lightManager;
};

