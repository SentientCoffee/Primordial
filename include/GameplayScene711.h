#pragma once
#include "Cappuccino/SceneManager.h"
#include "Cappuccino/FontManager.h"
#include "Commando.h"
#include "Building.h"

class GameplayScene : public Cappuccino::Scene {
public:
	GameplayScene(bool isActive);

	bool init() override;
	bool exit() override;

	void childUpdate(float dt) override;

	void mouseFunction(double xpos, double ypos) override;

private:
	Cappuccino::Mesh* _testMesh;
	Cappuccino::Mesh* _testMesh2;
	Cappuccino::Shader _basicShader{ "basicShader.vert","basicShader.frag" };
	Cappuccino::Shader _textShader{ "font.vert","font.frag" };
	Cappuccino::Text _text;
	Commando* _testCommando;

	//Building* _floorObject;
	//Cappuccino::Mesh* _floorMesh;
	//std::vector<Cappuccino::Mesh*> floor;
	//Cappuccino::RigidBody rigidTest = Cappuccino::RigidBody(glm::vec3(0.0f),glm::vec3(0.0f));

	//for an fps camera
	float lastX = 400, lastY = 300;
	float yaw = -90.0f;
	float pitch = 0.0f;
	bool firstMouse = true;
};