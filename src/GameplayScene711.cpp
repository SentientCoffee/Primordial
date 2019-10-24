#include "GameplayScene711.h"

GameplayScene::GameplayScene(bool isActive)
	:Cappuccino::Scene(isActive),_text("Primordial Alpha 0.0.1",_textShader,glm::vec2(-1500.0f,-1100.0f),glm::vec3(1.0f,1.0f,1.0f),1.0f)
{
	_testMesh = new Cappuccino::Mesh("./Assets/Meshes/Cube.obj");
	_testMesh->loadMesh();
	_testMesh2 = new Cappuccino::Mesh(*_testMesh);
	Cappuccino::FontManager::loadTypeFace("Viper Nora.ttf");
	_testCommando = new Commando(_textShader, std::vector<Cappuccino::Texture*>{}, std::vector<Cappuccino::Mesh*>{});


	//_floorMesh = new Cappuccino::Mesh("./Assets/Meshes/floor.obj");
	//floor.push_back(_floorMesh);
	//_floorObject = new Building(_basicShader, std::vector<Cappuccino::Texture*>{}, floor);
	//init members here
	
}

bool GameplayScene::init()
{
	//activate members here
	_initialized = true;
	_shouldExit = false;
	_testCommando->setActive(true);

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


	_basicShader.use();
	_basicShader.loadProjectionMatrix(1600.0f, 1200.0f);
	_basicShader.loadViewMatrix(*_testCommando->getCamera());

	Cappuccino::Transform transform;

	transform.translate(glm::vec3(5.0f,0.0f,0.0f));
	transform.update();

	_basicShader.loadModelMatrix(transform._transformMat);

	_testMesh->draw();
	
	transform.translate(glm::vec3(0.0f, 0.0f, 5.0f));
	transform.update();
	_basicShader.loadModelMatrix(transform._transformMat);
	_testMesh2->draw();

	//glm::mat4 projection = glm::mat4(1.0f);
	//projection = glm::perspective(glm::radians(45.0f), (float)1600 / (float)1200, 0.1f, 100.0f);
	//rigidTest.setViewProjMat(_testCommando->getCamera()->whereAreWeLooking(),projection);
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