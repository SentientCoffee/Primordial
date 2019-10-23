#pragma once
#include "Cappuccino/SceneManager.h"
#include "Cappuccino/FontManager.h"
#include "Commando.h"

class GameplayScene : public Cappuccino::Scene {
public:
	GameplayScene(bool isActive);

	bool init() override;
	bool exit() override;

	void childUpdate(float dt) override;

	void mouseFunction(double xpos, double ypos) override;

private:
	Cappuccino::Shader _dummyShader{ "","" };
	Commando* _testCommando;
	//for an fps camera
	float lastX = 400, lastY = 300;
	float yaw = -90.0f;
	float pitch = 0.0f;
	bool firstMouse = true;
};