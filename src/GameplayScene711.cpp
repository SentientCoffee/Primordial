#include "GameplayScene711.h"

GameplayScene::GameplayScene(bool isActive)
	:Cappuccino::Scene(isActive),
	_pLight(glm::vec2(1600.0f, 1200.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f) * 10.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 16.0f)
{


	auto diffuse = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::DiffuseMap);
	auto matte = new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap);
	auto spec = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::SpecularMap);
	auto norm = new Cappuccino::Texture(std::string("pistolNorm.png"), Cappuccino::TextureType::NormalMap);
	auto red = new Cappuccino::Texture(std::string("red.png"), Cappuccino::TextureType::DiffuseMap);

	

	//_sednium.push_back(new Sednium(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{red}));
	//_sednium.back()->_rigidBody._position = glm::vec3(26.80f, -1.0f, -59.976f);
	_sednium = new Sednium(_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ diffuse, spec });
	_ammoPack = new AmmoPack(_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ diffuse, spec });
	_healthPack = new HealthPack(_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ diffuse, spec });

	_testEnemy = new Sentry(&_pLight._pointLightShader, { matte, spec, norm }, { new Cappuccino::Mesh("Sentry.obj") }, 1.0f);
	_testEnemy->_rigidBody._position = glm::vec3(26.80f, 1.0f, -59.976f);
	_testEnemy->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f);

	//handle room data here
	_levelManager.rooms.push_back(new Building("./Assets/LevelData/Room2LevelData.obj", "./Assets/Meshes/Hitboxes/Room2Hitbox.obj", &_pLight._pointLightShader, { diffuse, spec }, { new Cappuccino::Mesh("room1.obj") }));
	for (unsigned i = 0; i < 5; i++)
		_levelManager.airlocks.push_back(new Building("./Assets/LevelData/AirLockData.obj", "./Assets/Meshes/Hitboxes/AirlockHitbox.obj", &_pLight._pointLightShader, { diffuse, spec }, { new Cappuccino::Mesh("Airlock.obj") }));



	_testGhoul = new Ghoul(&_pLight._pointLightShader, { matte, spec }, { new Cappuccino::Mesh("Crawler.obj") }, 1.0f);
	_testGhoul->_rigidBody._position = glm::vec3(26.80f, 0.0f, -59.976f);

	_testRobo = new RoboGunner(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ red, spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("Crawler.obj")});
	_testRobo->_rigidBody._position = glm::vec3(30.0f, 0.0f, -50.0f);

	_testCaptain = new Captain(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ matte, spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("Crawler.obj")});
	_testCaptain->_rigidBody._position = glm::vec3(32.0f, 0.0f, -50.0f);

	_testGrunt = new Grunt(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ red, spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("Crawler.obj")});
	_testGrunt->_rigidBody._position = glm::vec3(34.0f, 0.0f, -50.0f);

	_testSquelch = new Squelch(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ matte, spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("Crawler.obj")});
	_testSquelch->_rigidBody._position = glm::vec3(36.0f, 0.0f, -50.0f);

	_testSentinel = new Sentinel(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{matte, spec, norm}, std::vector<Cappuccino::Mesh*>{new Cappuccino::Mesh("Sentinel.obj")}, 1.0f);
	_testSentinel->_rigidBody._position = glm::vec3(26.0f, 0.0f, -50.0f);
	//init members here
	auto mesh = new Cappuccino::Mesh("Bullet.obj");
	mesh->loadMesh();

	bullet = new Bullet(_pLight._pointLightShader, { matte, spec, norm },
		{mesh}, glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2 = new Bullet(_pLight._pointLightShader, { matte, spec, norm },
		{mesh}, glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f));

	bullet->_transform.scale(glm::vec3(1.0f), 0.1f);
	bullet2->_transform.scale(glm::vec3(1.0f), 0.1f);
	_testCommando->addAmmo(bullet, bullet2);
	bullet->_transform.scale(glm::vec3(1.0f), 10.f);
	_testEnemy->getGun()->addBullets(bullet);
	_testSentinel->getGun()->addBullets(bullet);
	_testRobo->getGun()->addBullets(bullet);
	_testCaptain->getGun()->addBullets(bullet);
	_testGrunt->getGun()->addBullets(bullet);

	_enemies.push_back(_testEnemy);
	_enemies.push_back(_testGhoul);
	_enemies.push_back(_testSentinel);
	_enemies.push_back(_testRobo);
	_enemies.push_back(_testGrunt);
	_enemies.push_back(_testCaptain);
	_enemies.push_back(_testSquelch);
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
	_testRobo->setActive(true);
	_testGrunt->setActive(true);
	_testCaptain->setActive(true);
	_testSquelch->setActive(true);
	_levelManager.rooms[0]->setActive(true);
	for (unsigned i = 0; i < _levelManager.airlocks.size(); i++)
		_levelManager.airlocks[i]->setActive(true);
	//for (auto x : _sednium)
		//x->setActive(true);
	_sednium->setPosition(_testCommando->_rigidBody._position);
	_sednium->setActive(true);
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
	_testRobo->setActive(false);
	_testGrunt->setActive(false);
	_testCaptain->setActive(false);
	_testSquelch->setActive(false);
	for (int i = 0; i < _levelManager.rooms.size(); i++)
		_levelManager.rooms[i]->setActive(false);
	for (unsigned i = 0; i < _levelManager.airlocks.size(); i++)
		_levelManager.airlocks[i]->setActive(false);
	//for (auto x : _sednium)
		//x->setActive(false);
	return true;
}

void GameplayScene::childUpdate(float dt)
{
	_levelManager.update(dt, _testCommando->_rigidBody);

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

		_enemies[i]->dead(); //checks for squelch 

		for (auto x : _testCommando->getGun()->getBullets()) {
			if (x->_rigidBody.checkCollision(_enemies[i]->_rigidBody) && x->isActive() && _enemies[i]->isActive()) {
				_enemies[i]->hurt(_testCommando->getGun()->getDamage());
				if (_enemies[i]->dead())
				{
					Sednium* temp = _sednium;
					temp->setActive(true);
					temp->_rigidBody.setGrav(false);
					temp->setPosition(_enemies[i]->_rigidBody._position);
					_loot.push_back(temp);
					//_sednium->spawn(_enemies[i]->getWeight(), _loot, _enemies[i]->_rigidBody._position);
					//_healthPack->spawn(_enemies[i]->getWeight(), _loot, _enemies[i]->_rigidBody._position);
					//_ammoPack->spawn(_enemies[i]->getWeight(), _loot, _enemies[i]->_rigidBody._position);
				}
				x->setActive(false);
			}
		}
		_enemies[i]->attack(_testCommando, dt);
	}

	for (unsigned i = 0; i < _loot.size(); i++)
	{
		_loot[i]->pickup(_testCommando);
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
