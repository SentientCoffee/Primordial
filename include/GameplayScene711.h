#pragma once

#include "Cappuccino/SceneManager.h"
#include "Cappuccino/FontManager.h"
#include "Cappuccino/DirLight.h"
#include "Cappuccino/PointLight.h"

#include "UIPointLight.h"
#include "Class.h"
#include "Building.h"
#include "Enemy.h"
#include "PlayerHUD.h"
#include "LevelManager.h"

class GameplayScene : public Cappuccino::Scene {
public:
	GameplayScene(bool isActive);

	bool init() override;
	bool exit() override;

	void childUpdate(float dt) override;

	void mouseFunction(double xpos, double ypos) override;
	void clickFunction(int button, int action, int mods) override;

private:
	//Cappuccino::Shader _basicShader{ "basicShader.vert","basicShader.frag" };
	Cappuccino::PointLight _pLight;
	Class* _testCommando = new Commando(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{}, std::vector<Cappuccino::Mesh*>{});
	Enemy* _testEnemy;
	Ghoul* _testGhoul;
	Sentinel* _testSentinel;
	Bullet* bullet;
	Bullet* bullet2;

	std::vector<Enemy*> _enemies;

	LevelManager _levelManager;
	//

	HUD* _hud;
	
	Cappuccino::RigidBody rigidTest = Cappuccino::RigidBody(glm::vec3(0.0f),glm::vec3(0.0f));

	//for an fps camera
	float lastX = 400, lastY = 300;
	float yaw = -90.0f;
	float pitch = 0.0f;
	bool firstMouse = true;
};