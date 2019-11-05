#include "MenuScene.h"
#include "Cappuccino/CappMacros.h"

MenuScene::MenuScene(bool isActive)
	:Cappuccino::Scene(isActive), _in(true, std::nullopt), cursorBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f)), startBox(glm::vec3(0.0f, -75.0f, 0.0f), glm::vec3(200.0f, 100.0f, 200.0f))
{
	menuShader = new Cappuccino::Shader("screenSpaceModel.vert", "screenSpace.frag");
	_uiShader = new Cappuccino::Shader("font.vert", "font.frag");
	ui._uiComponents.push_back(new Cappuccino::UIText("Start", glm::vec2(1600.0f, 1200.0f), *_uiShader, glm::vec2(-100.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));
	ui._uiComponents.push_back(new Cappuccino::UIText("P R I M O R D I A L", glm::vec2(1600.0f, 1200.0f), *_uiShader, glm::vec2(-600.0f, 600.0f), glm::vec3(0.0f, 1.0f, 0.0f), 2.5f));
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

	cursorBox._position = glm::vec3(cursorPos.x, cursorPos.y, 0.0f);
	//CAPP_PRINT("%f %f\n",cursorBox._position.x, cursorBox._position.y);
	//CAPP_PRINT("box %f %f\n",startBox._position.x, startBox._position.y);

	if (cursorBox.checkCollision(startBox, startBox._position, cursorBox._position)) {
		static_cast<Cappuccino::UIText*>(ui._uiComponents[0])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

		if (_in.clickListener.leftClicked())
			Cappuccino::SceneManager::changeScene(1);
	}
	else
		static_cast<Cappuccino::UIText*>(ui._uiComponents[0])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

	ui.update(dt);
}

void MenuScene::mouseFunction(double xpos, double ypos)
{
	cursorPos = glm::vec2(xpos, ypos);
	cursorPos -= glm::vec2(800.0f, 600.0f);
}

void MenuScene::clickFunction(int button, int action, int mods)
{
	_in.clickListener.checkClick(button, action, mods);
}
