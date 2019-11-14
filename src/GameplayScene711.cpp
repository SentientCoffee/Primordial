#include "GameplayScene711.h"

GameplayScene::GameplayScene(bool isActive)
	:Cappuccino::Scene(isActive),
	_pLight(glm::vec2(1600.0f, 1200.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f)
{

	auto diffuse = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::DiffuseMap);
	auto spec = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::SpecularMap);

	_testEnemy = new Sentry(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap), spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("Sentry.obj") }, 1.0f);
	_testEnemy->_rigidBody._position = glm::vec3(26.80f,1.0f, -59.976f);
	_testEnemy->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f);

	_testGhoul = new Ghoul(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap), spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("Crawler.obj")}, 1.0f);
	_testGhoul->_rigidBody._position = glm::vec3(26.80f, 0.0f, -59.976f);
	//_testGhoul->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f);


//<<<<<<< Updated upstream
	_floorObject = new Building("./Assets/LevelData/Level1Data.obj","./Assets/Meshes/Hitboxes/floorHitBox.obj",&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ diffuse,spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("room1.obj") });
//=======
	//_floorObject = new Building(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{diffuse,spec}, std::vector<Cappuccino::Mesh*>{new Cappuccino::Mesh("Assets/Meshes/floor.obj")});
//>>>>>>> Stashed changes

	//init members here
	auto mesh = new Cappuccino::Mesh("Bullet.obj");

	bullet = new Bullet(_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap), spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh(*mesh) }, glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2 = new Bullet(_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap), spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh(*mesh) }, glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f));

	bullet->_transform.scale(glm::vec3(1.0f), 0.1f);
	bullet2->_transform.scale(glm::vec3(1.0f), 0.1f);
	_testCommando->addAmmo(bullet, bullet2);
	bullet->_transform.scale(glm::vec3(1.0f), 10.f);
	_testEnemy->getGun()->addBullets(bullet);

	_enemies.push_back(_testEnemy);
	_enemies.push_back(_testGhoul);
}

bool GameplayScene::init()
{
	//activate members here
	_initialized = true;
	_shouldExit = false;
	_testCommando->setActive(true);
	_testEnemy->setActive(true);
	_testGhoul->setActive(true);
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
	_testGhoul->setActive(false);
	_floorObject->setActive(false);
	return true;
}

void GameplayScene::childUpdate(float dt)
{

	_pLight._pointLightShader.use();
	_pLight._pointLightShader.loadViewMatrix(*_testCommando->getCamera());
	_pLight.updateViewPos(_testCommando->getCamera()->getPosition());

	_testCommando->getUILight().setPosition(_pLight.getPosition());

	for (unsigned i = 0; i < _enemies.size(); i++)
	{
	if (_testCommando->checkCollision(_enemies[i]->triggerVolume,_enemies[i]->_rigidBody._position) && _enemies[i]->isActive())
		_enemies[i]->setTrigger(true);
	else
		_enemies[i]->setTrigger(false);

	for (auto x : _testCommando->getGun()->getBullets()) {
		if (x->_rigidBody.checkCollision(_enemies[i]->_rigidBody) && x->isActive()) {
			_enemies[i]->hurt(_testCommando->getGun()->getDamage());
			x->setActive(false);
		}
	}
	_enemies[i]->attack(_testCommando, dt);
	}
	
	//if (_testCommando->_rigidBody.checkCollision(_floorObject->_rigidBody)) {
	//	_testCommando->_rigidBody.setGrav(false);
	//	_testCommando->_rigidBody._accel.y = 0.0f;
	//}
	//else
	//	_testCommando->_rigidBody.setGrav(true);

	//CAPP_PRINT("%f %f\n", _testCommando->_rigidBody._position.x, _testCommando->_rigidBody._position.z);


	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)1600 / (float)1200, 0.1f, 100.0f);
	rigidTest.setViewProjMat(_testCommando->getCamera()->whereAreWeLooking(), projection);

	//_hud->setHealth(_testCommando->getHealth());
	//_hud->setHealth(_testCommando->getShield());

	
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

void GameplayScene::clickFunction(int button, int action, int mods)
{
	_testCommando->_input.clickListener.checkClick(button, action, mods);
}
