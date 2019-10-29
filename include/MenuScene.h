#pragma once
#include "Cappuccino/SceneManager.h"
#include "Cappuccino/ShaderProgram.h"
#include "Cappuccino/UI.h"

class MenuScene : public Cappuccino::Scene {
public:
	MenuScene(bool isActive);

	bool init() override;
	bool exit() override;

	void childUpdate(float dt) override;

	void mouseFunction(double xpos, double ypos);

private:
	Cappuccino::UserInterface ui;
	Cappuccino::Shader* _uiShader;

	Cappuccino::Camera camera;
	Cappuccino::Shader* menuShader;

};