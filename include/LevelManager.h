#pragma once
#include "Building.h"
#include "Cappuccino/RigidBody.h"
#include "Cappuccino/PointLight.h"
#include "Cappuccino/Random.h"
#include "Enemy.h"
#include "Shop.h"
#include "Loot.h"
#include "Class.h"


class doorMesh : public Cappuccino::GameObject {
public:
	doorMesh(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs);

	void setHitbox(Cappuccino::HitBox hitbox);
	void childUpdate(float dt) override;
};

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
	void update(float dt,Class *player);

	std::vector <Building*> _rooms;
	std::vector <Building*> airlocks;
	std::vector <doorMesh*> _doors;
	unsigned _currentRoom = 0;

	ShopTerminal* _testShopTerminal = NULL;

	std::vector<Chest*> _chests;

	EnemyManager _enemyManager;
private:
	
	float _currentRotation = 0.0f;
	bool _start = true;
	LightManager _lightManager;
};

