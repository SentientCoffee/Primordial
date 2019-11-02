#pragma once
#include "Cappuccino/SceneManager.h"
#include "Cappuccino/FontManager.h"
#include "Cappuccino/DirLight.h"
#include "Cappuccino/PointLight.h"
#include "UIPointLight.h"
#include "Commando.h"
#include "Building.h"
#include "Enemy.h"

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
	Cappuccino::DirLight _light;
	Cappuccino::PointLight _pLight;
	Cappuccino::Mesh* _testMesh;
	Cappuccino::Mesh* _testMesh2;
	Cappuccino::Shader _textShader{ "font.vert","font.frag" };
	Cappuccino::Text _text;
	Commando* _testCommando = new Commando(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{}, std::vector<Cappuccino::Mesh*>{});
	Enemy* _testEnemy;
	Bullet* bullet;

	Building* _floorObject;

	Cappuccino::RigidBody rigidTest = Cappuccino::RigidBody(glm::vec3(0.0f),glm::vec3(0.0f));

	//for an fps camera
	float lastX = 400, lastY = 300;
	float yaw = -90.0f;
	float pitch = 0.0f;
	bool firstMouse = true;
};