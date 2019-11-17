#include "GameplayScene711.h"

GameplayScene::GameplayScene(bool isActive)
	:Cappuccino::Scene(isActive),
	_pLight(glm::vec2(1600.0f, 1200.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f)
{

	auto diffuse = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::DiffuseMap);
	auto matte = new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap);
	auto spec = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::SpecularMap);

	_testEnemy = new Sentry(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ matte, spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("Sentry.obj") }, 1.0f);
	_testEnemy->_rigidBody._position = glm::vec3(26.80f, 1.0f, -59.976f);
	_testEnemy->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f);

	_testGhoul = new Ghoul(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ matte, spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("Crawler.obj")}, 1.0f);
	_testGhoul->_rigidBody._position = glm::vec3(26.80f, 0.0f, -59.976f);

	_testSentinel = new Sentinel(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{matte, spec}, std::vector<Cappuccino::Mesh*>{new Cappuccino::Mesh("Sentinel.obj")},1.0f);
	_testSentinel->_rigidBody._position = glm::vec3(26.0f, 0.0f, -50.0f);

	_floorObject = new Building("./Assets/LevelData/Level1Data.obj", "./Assets/Meshes/Hitboxes/floorHitBox.obj", &_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ diffuse, spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("room1.obj") });

	//init members here
	auto mesh = new Cappuccino::Mesh("Bullet.obj");
	mesh->loadMesh();

	bullet = new Bullet(_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ matte, spec }, 
		std::vector<Cappuccino::Mesh*>{mesh}, glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2 = new Bullet(_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ matte, spec },
		std::vector<Cappuccino::Mesh*>{mesh}, glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f));

	bullet->_transform.scale(glm::vec3(1.0f), 0.1f);
	bullet2->_transform.scale(glm::vec3(1.0f), 0.1f);
	_testCommando->addAmmo(bullet, bullet2);
	bullet->_transform.scale(glm::vec3(1.0f), 10.f);
	_testEnemy->getGun()->addBullets(bullet);
	_testSentinel->getGun()->addBullets(bullet);

	_enemies.push_back(_testEnemy);
	_enemies.push_back(_testGhoul);
	_enemies.push_back(_testSentinel);
}

bool GameplayScene::init()
{
	//activate members here
	_initialized = true;
	_shouldExit = false;
	_testCommando->setActive(true);
	_testEnemy->setActive(true);
	_testGhoul->setActive(true);
	_testSentinel->setActive(true);
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
	_testSentinel->setActive(false);
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
		if (_testCommando->checkCollision(_enemies[i]->triggerVolume, _enemies[i]->_rigidBody._position) && _enemies[i]->isActive())
			_enemies[i]->setTrigger(true);
		else
			_enemies[i]->setTrigger(false);

		for (auto x : _testCommando->getGun()->getBullets()) {
			if (x->_rigidBody.checkCollision(_enemies[i]->_rigidBody) && x->isActive() && _enemies[i]->isActive()) {
				_enemies[i]->hurt(_testCommando->getGun()->getDamage());
				x->setActive(false);
			}
		}
		_enemies[i]->attack(_testCommando, dt);
	}



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
