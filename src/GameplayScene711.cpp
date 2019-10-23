#include "GameplayScene711.h"

GameplayScene::GameplayScene(bool isActive)
	:Cappuccino::Scene(isActive),_text("Primordial Alpha 0.0.1",_textShader,glm::vec2(-1500.0f,-1100.0f),glm::vec3(1.0f,1.0f,1.0f),1.0f)
{
	Cappuccino::FontManager::loadTypeFace("Viper Nora.ttf");
	_testCommando = new Commando(_textShader, std::vector<Cappuccino::Texture*>{}, std::vector<Cappuccino::Mesh*>{});
	//init members here
}

bool GameplayScene::init()
{
	//activate members here
	_initialized = true;
	_shouldExit = false;

	return true;
}

bool GameplayScene::exit()
{
	//deactivate members here
	_initialized = false;
	_shouldExit = true;

	return true;
}

void GameplayScene::childUpdate(float dt)
{
	
	_textShader.use();
	_textShader.loadOrthoProjectionMatrix(1600.0f, 1200.0f);
	_textShader.setUniform("textColour", _text.getColour());

	_text.draw();

}

void GameplayScene::mouseFunction(double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xOffset = xpos - lastX;
	GLfloat yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	_testCommando->getCamera()->doMouseMovement(xOffset, yOffset);
}
