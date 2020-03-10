#pragma once

#include "Cappuccino/SceneManager.h"
#include "Cappuccino/FontManager.h"
#include "Cappuccino/DirLight.h"
#include "Cappuccino/Cubemap.h"
#include "Cappuccino/LUT.h"
#include "Cappuccino/Application.h"

#include "Class.h"
#include "Building.h"
#include "Enemy.h"
#include "PlayerHUD.h"
#include "LevelManager.h"
#include "Loot.h"
#include "Billboard.h"

#include "Shop.h"

class GameplayScene : public Cappuccino::Scene {
public:

	GameplayScene(bool isActive);

	bool init() override;
	bool exit() override;

	void childUpdate(float dt) override;

	void mouseFunction(double xpos, double ypos) override;
	void clickFunction(int button, int action, int mods) override;

	inline void resetObjects();

	void shootCollisionBehaviour(Enemy* enemy);

	static void resendLights();
	void sendGBufferShaderUniforms();


	static std::vector<Cappuccino::PointLight> _lights;
	static Class* _testCommando;
private:

	static Cappuccino::Shader* _mainShader;


	Cappuccino::Cubemap* _skybox;

	Cappuccino::HitBox cursorBox;
	glm::vec2 cursorPos;

	std::vector<Billboard*> lamps;
	Bullet* bullet;
	Bullet* bullet2;

	Primordial* _primordial;

	Sednium* _sednium;
	HealthPack* _healthPack;
	AmmoPack* _ammoPack;
	Bullion* _bullion;

	//Chest* _chest;
	std::vector<Loot*> _loot;

	LevelManager _levelManager;
	std::vector<Enemy*> _enemies;

	Cappuccino::RigidBody rigidTest = Cappuccino::RigidBody(glm::vec3(0.0f));

	//for an fps camera
	float lastX = 400, lastY = 300;
	float yaw = -90.0f;
	float pitch = 0.0f;
	bool firstMouse = true;
};