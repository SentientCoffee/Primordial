#include "GameplayScene711.h"

GameplayScene::GameplayScene(bool isActive)
	:Cappuccino::Scene(isActive)
{
	_testCommando = new Commando(_dummyShader, std::vector<Cappuccino::Texture*>{}, std::vector<Cappuccino::Mesh*>{});
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
