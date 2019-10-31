#include "GameplayScene711.h"

GameplayScene::GameplayScene(bool isActive)
	:Cappuccino::Scene(isActive), _text("Primordial Alpha 0.0.1", _textShader, glm::vec2(-1500.0f, -1100.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f),
	_light(glm::vec2(1600.0f, 1200.0f), glm::vec3(0.2f, -1.0f, 0.3f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(1.0f, 1.f, 1.f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f),
	_pLight(glm::vec2(1600.0f,1200.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.05f,0.05f,0.05f),glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.5f,0.5f,0.5f),32.0f)
{
	_testMesh = new Cappuccino::Mesh("Assets/Meshes/Cube.obj");
	_testMesh->loadMesh();
	_testMesh2 = new Cappuccino::Mesh(*_testMesh);
	Cappuccino::FontManager::loadTypeFace("Viper Nora.ttf");

	auto diffuse = new Cappuccino::Texture(std::string("./Assets/Textures/nut.png"), Cappuccino::TextureType::DiffuseMap);
	auto spec = new Cappuccino::Texture(std::string("./Assets/Textures/Metal_specmap.png"), Cappuccino::TextureType::SpecularMap);



	_testEnemy = new Enemy(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{diffuse,spec}, std::vector<Cappuccino::Mesh*>{new Cappuccino::Mesh("Assets/Meshes/NUTtest.obj")}, 1.0f);
	
	_floorObject = new Building(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{}, std::vector<Cappuccino::Mesh*>{new Cappuccino::Mesh("Assets/Meshes/floor.obj")});
	//init members here

}

bool GameplayScene::init()
{
	//activate members here
	_initialized = true;
	_shouldExit = false;
	_testCommando->setActive(true);
	_testEnemy->setActive(true);
	_floorObject->setActive(true);

	return true;
}

bool GameplayScene::exit()
{
	//deactivate members here
	_initialized = false;
	_shouldExit = true;
	_testCommando->setActive(false);
	_testEnemy->setActive(false);
	_floorObject->setActive(false);
	return true;
}

void GameplayScene::childUpdate(float dt)
{
	_textShader.use();
	_textShader.loadOrthoProjectionMatrix(1600.0f, 1200.0f);
	_textShader.setUniform("textColour", _text.getColour());

	_text.draw();



	_pLight._pointLightShader.use();
	_pLight._pointLightShader.loadViewMatrix(*_testCommando->getCamera());
	_pLight.updateViewPos(_testCommando->getCamera()->getPosition());

	Cappuccino::Transform transform;

	transform.translate(glm::vec3(5.0f, 0.0f, 0.0f));
	transform.update();

	_pLight._pointLightShader.loadModelMatrix(transform._transformMat);

	_testMesh->draw();

	transform.translate(glm::vec3(0.0f, 0.0f, 5.0f));
	transform.update();
	_pLight._pointLightShader.loadModelMatrix(transform._transformMat);
	_testMesh2->draw();


	//_testEnemy->trackGO(_testCommando,0.001f);

	if (_testCommando->_rigidBody.checkCollision(_testEnemy->_rigidBody))
		CAPP_PRINT("YOU FOOL\n");
	else
		CAPP_PRINT("Noice\n");

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)1600 / (float)1200, 0.1f, 100.0f);
	rigidTest.setViewProjMat(_testCommando->getCamera()->whereAreWeLooking(),projection);
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
