#include "GameplayScene711.h"

GameplayScene::GameplayScene(bool isActive)
	:Cappuccino::Scene(isActive),
	_pLight(glm::vec2(1600.0f, 1200.0f), { glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(26.80f, 1.0f, -100.976f) }, glm::vec3(0.05f, 0.05f, 0.05f) * 10.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f)
{
	auto diffuse = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::DiffuseMap);
	auto matte = new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap);
	auto spec = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::SpecularMap);
	auto norm = new Cappuccino::Texture(std::string("pistolNorm.png"), Cappuccino::TextureType::NormalMap);
	auto red = new Cappuccino::Texture(std::string("red.png"), Cappuccino::TextureType::DiffuseMap);

	_sednium.push_back(new Sednium(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{red}));
	_sednium.back()->_rigidBody._position = glm::vec3(26.80f, -1.0f, -59.976f);


	_testEnemy = new Sentry(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ matte, spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("Sentry.obj") }, 1.0f);
	_testEnemy->_rigidBody._position = glm::vec3(26.80f, 1.0f, -59.976f);
	_testEnemy->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f);

	//handle room data here
	_levelManager.rooms.push_back(new Building("./Assets/LevelData/Room2LevelData.obj", "./Assets/Meshes/Hitboxes/Room2Hitbox.obj", &_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ diffuse, spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("room1.obj") }));
	for (unsigned i = 0; i < 5; i++)
		_levelManager.airlocks.push_back(new Building("./Assets/LevelData/AirLockData.obj", "./Assets/Meshes/Hitboxes/AirlockHitbox.obj", &_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ diffuse, spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("Airlock.obj") }));



	_testGhoul = new Ghoul(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ matte, spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("Crawler.obj")}, 1.0f);
	_testGhoul->_rigidBody._position = glm::vec3(26.80f, 0.0f, -59.976f);

	_testSentinel = new Sentinel(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{matte, spec, norm}, std::vector<Cappuccino::Mesh*>{new Cappuccino::Mesh("Sentinel.obj")}, 1.0f);
	_testSentinel->_rigidBody._position = glm::vec3(26.0f, 0.0f, -50.0f);
	//init members here
	auto mesh = new Cappuccino::Mesh("Bullet.obj");
	mesh->loadMesh();

	bullet = new Bullet(_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ matte, spec, norm },
		std::vector<Cappuccino::Mesh*>{mesh}, glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2 = new Bullet(_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ matte, spec, norm },
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

	_testCommando->getUILight().getPositions().clear();
	for (unsigned i = 0; i < _pLight.getPositions().size(); i++)
		_testCommando->getUILight().getPositions().push_back(_pLight.getPositions()[i]);

	_testCommando->getUILight().resendData();

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
	_levelManager.rooms[0]->setActive(true);
	for (unsigned i = 0; i < _levelManager.airlocks.size(); i++)
		_levelManager.airlocks[i]->setActive(true);
	for (auto x : _sednium)
		x->setActive(true);

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
	for (int i = 0; i < _levelManager.rooms.size(); i++)
		_levelManager.rooms[i]->setActive(false);
	for (unsigned i = 0; i < _levelManager.airlocks.size(); i++)
		_levelManager.airlocks[i]->setActive(false);
	for (auto x : _sednium)
		x->setActive(false);
	return true;
}

void GameplayScene::childUpdate(float dt)
{
	_levelManager.update(dt, _testCommando->_rigidBody);

	_pLight.updateViewPos(_testCommando->getCamera()->getPosition());
	_pLight._pointLightShader.loadViewMatrix(*_testCommando->getCamera());

	_testCommando->getUILight().updateViewPos(_testCommando->getCamera()->getPosition());


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


	if (!_testCommando->_input.keyboard->keyPressed(Events::G))
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	_testCommando->getCamera()->doMouseMovement(xOffset, yOffset);
}

void GameplayScene::clickFunction(int button, int action, int mods)
{
	_testCommando->_input.clickListener.checkClick(button, action, mods);
}
