#include "MenuScene.h"

MenuScene::MenuScene(bool isActive)
	:Cappuccino::Scene(isActive),_in(true,std::nullopt)
{
	menuShader = new Cappuccino::Shader("screenSpaceModel.vert", "screenSpace.frag");
	_uiShader = new Cappuccino::Shader("font.vert", "font.frag");
	ui._uiComponents.push_back(new Cappuccino::UIText("Start", glm::vec2(1600.0f, 1200.0f), *_uiShader, glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
	menuShader->use();
	menuShader->loadOrthoProjectionMatrix(1600.0f / 20.0f, 1200.0f / 20.0f);
	menuShader->loadViewMatrix(camera);
}

bool MenuScene::init()
{
	_initialized = true;
	_shouldExit = false;

	return true;
}

bool MenuScene::exit()
{
	_initialized = false;
	_shouldExit = true;

	return true;
}

void MenuScene::childUpdate(float dt)
{
	ui.update(dt);
	
	if (_in.keyboard->keyPressed(Events::Enter)) {

		Cappuccino::SceneManager::changeScene(1);
	}

}

void MenuScene::mouseFunction(double xpos, double ypos)
{
}
