#pragma once

#include "Billboard.h"
#include "Building.h"
#include "Class.h"
#include "Enemy.h"
#include "LevelManager.h"
#include "Loot.h"
#include "Shop.h"

#include <Cappuccino/Cubemap.h>
#include <Cappuccino/FontManager.h>
#include <Cappuccino/LUT.h>
#include <Cappuccino/SceneManager.h>

class GameplayScene : public Cappuccino::Scene {

	using LightVector = std::vector<Cappuccino::PointLight>;
	
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
	static void sendGBufferShaderUniforms();


	static LightVector _lights;
	static Class* _testCommando;
private:

	static Cappuccino::Shader* _mainShader;

	Cappuccino::Cubemap* _skybox;

	Cappuccino::UserInterface ui;
	Cappuccino::HitBox resumeBox;
	Cappuccino::HitBox optionsBox;
	Cappuccino::HitBox restartBox;
	Cappuccino::HitBox menuBox;
	Cappuccino::HitBox exitBox;

	Cappuccino::HitBox cursorBox;
	glm::vec2 cursorPos;

	std::vector<Billboard*> lamps;
	Bullet* bullet  = nullptr;
	Bullet* bullet2 = nullptr;

	Primordial* _primordial = nullptr;

	Sednium* _sednium;
	HealthPack* _healthPack;
	AmmoPack* _ammoPack;
	Bullion* _bullion;

	//Chest* _chest;
	std::vector<Loot*> _loot;

	LevelManager _levelManager;
	std::vector<Enemy*> _enemies;
	std::vector <Building*> _airlocks;
	std::vector<Chest*> _chests;

	Cappuccino::RigidBody rigidTest = Cappuccino::RigidBody(glm::vec3(0.0f));

	//for an fps camera
	float lastX = 400, lastY = 300;
	float yaw = -90.0f;
	float pitch = 0.0f;
	bool firstMouse = true;
};
