#pragma once
#include "Cappuccino/SceneManager.h"
#include "Cappuccino/ShaderProgram.h"
#include "Cappuccino/UI.h"
#include "Cappuccino/CappInput.h"
#include "Cappuccino/HitBox.h"

class MenuScene : public Cappuccino::Scene {
public:
	MenuScene(bool isActive);

	bool init() override;
	bool exit() override;

	void childUpdate(float dt) override;

	void mouseFunction(double xpos, double ypos);
	void clickFunction(int button, int action, int mods) override;

private:
	Cappuccino::CappInput _in;

	Cappuccino::UserInterface ui;

	Cappuccino::Camera camera;
	Cappuccino::Shader* menuShader;

	glm::vec2 cursorPos;

	Cappuccino::HitBox cursorBox;
	Cappuccino::HitBox startBox;

};