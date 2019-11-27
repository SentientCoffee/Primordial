#include "GameplayScene711.h"
#include "Options.h"

GameplayScene::GameplayScene(const bool isActive) :
	Scene(isActive),
	_pLight(glm::vec2(1600.0f, 1200.0f), { glm::vec3(0.0f, 0.0f, 0.0f) }, glm::vec3(0.05f, 0.05f, 0.05f) * 10.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 16.0f)
{

	auto diffuse = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::DiffuseMap);
	auto matte = new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap);
	auto spec = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::SpecularMap);
	auto norm = new Cappuccino::Texture(std::string("pistolNorm.png"), Cappuccino::TextureType::NormalMap);
	auto red = new Cappuccino::Texture(std::string("red.png"), Cappuccino::TextureType::DiffuseMap);

	_sednium = new Sednium(_pLight._pointLightShader, { diffuse, spec });
	_ammoPack = new AmmoPack(_pLight._pointLightShader, { diffuse, spec });
	_healthPack = new HealthPack(_pLight._pointLightShader, { diffuse, spec });

	_testEnemy = new Sentry(&_pLight._pointLightShader, { matte, spec, norm }, { new Cappuccino::Mesh("Sentry.obj") }, 1.0f);

	//handle room data here
	_levelManager.rooms.push_back(new Building("./Assets/LevelData/Room2LevelData.obj", "./Assets/Meshes/Hitboxes/Room2Hitbox.obj", &_pLight._pointLightShader, { diffuse, spec }, { new Cappuccino::Mesh("room1.obj") }));
	for (unsigned i = 0; i < 5; i++)
		_levelManager.airlocks.push_back(new Building("./Assets/LevelData/AirLockData.obj", "./Assets/Meshes/Hitboxes/AirlockHitbox.obj", &_pLight._pointLightShader, { diffuse, spec }, { new Cappuccino::Mesh("Airlock.obj") }));


	_testGhoul = new Ghoul(&_pLight._pointLightShader, { matte, spec }, { new Cappuccino::Mesh("Crawler.obj") }, 1.0f);
	_testRobo = new RoboGunner(&_pLight._pointLightShader, { red, spec }, { new Cappuccino::Mesh("Crawler.obj") });
	_testCaptain = new Captain(&_pLight._pointLightShader, { matte, spec }, { new Cappuccino::Mesh("Crawler.obj") });
	_testGrunt = new Grunt(&_pLight._pointLightShader, { red, spec }, { new Cappuccino::Mesh("Crawler.obj") });
	_testSquelch = new Squelch(&_pLight._pointLightShader, { matte, spec }, { new Cappuccino::Mesh("Crawler.obj") });
	_testSentinel = new Sentinel(&_pLight._pointLightShader, { matte, spec, norm }, { new Cappuccino::Mesh("Sentinel.obj") }, 1.0f);

	resetObjects();

	//init members here
	auto mesh = new Cappuccino::Mesh("Bullet.obj");
	mesh->loadMesh();

	bullet = new Bullet(_pLight._pointLightShader, { matte, spec, norm }, { mesh }, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2 = new Bullet(_pLight._pointLightShader, { matte, spec, norm }, { mesh }, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2->_transform.scale(glm::vec3(1.0f), 0.1f);
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

	static bool createdPlayer = false;
	if (!createdPlayer) {
		if (Options::Assault)
			_testCommando = new Assault(&_pLight._pointLightShader, {}, {});

		else if (Options::Commando)
			_testCommando = new Commando(&_pLight._pointLightShader, {}, {});
		bullet->_transform.scale(glm::vec3(1.0f), 0.1f);

		_testCommando->addAmmo(bullet, bullet2);
		_testCommando->getUILight().getPositions().clear();
		for (unsigned i = 0; i < _pLight.getPositions().size(); i++)
			_testCommando->getUILight().getPositions().push_back(_pLight.getPositions()[i]);
		_testCommando->getUILight().resendData();
		createdPlayer = true;

	}

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
	for (auto& airlock : _levelManager.airlocks)
		airlock->setActive(true);
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
	for (auto& room : _levelManager.rooms)
		room->setActive(false);
	for (auto& airlock : _levelManager.airlocks)
		airlock->setActive(false);
	for (auto x : _loot)
		x->setActive(false);
	return true;
}

void GameplayScene::childUpdate(float dt)
{
	_levelManager.update(dt, _testCommando->_rigidBody);

	_pLight.updateViewPos(_testCommando->getCamera()->getPosition());
	_pLight._pointLightShader.loadViewMatrix(*_testCommando->getCamera());

	_testCommando->getUILight().updateViewPos(_testCommando->getCamera()->getPosition());

	for (auto& enemy : _enemies) {
		if (_testCommando->checkCollision(enemy->triggerVolume, enemy->_rigidBody._position) && enemy->isActive())
			enemy->setTrigger(true);
		else
			enemy->setTrigger(false);

		enemy->dead(); //checks for squelch 

		for (auto x : _testCommando->getGun()->getBullets()) {
			if (x->_rigidBody.checkCollision(enemy->_rigidBody) && x->isActive() && enemy->isActive()) {
				enemy->hurt(_testCommando->getGun()->getDamage());
				if (enemy->dead())
				{
					_loot.push_back(_sednium->spawn(enemy->getWeight(), enemy->_rigidBody._position));
					_loot.push_back(_healthPack->spawn(enemy->getWeight(), enemy->_rigidBody._position));
					_loot.push_back(_ammoPack->spawn(enemy->getWeight(), enemy->_rigidBody._position));
				}
				x->setActive(false);
			}
		}
		enemy->attack(_testCommando, dt);

		for (auto bullet : enemy->getGun()->getBullets()) {
			if (bullet->checkCollision(*_testCommando)) {
				_testCommando->takeDamage(enemy->getGun()->getDamage());
			}
		}
	}

	for (auto& x : _loot) {
		if (x->isActive())
			x->pickup(_testCommando);
	}

	if (_testCommando->getHealth() <= 0) {
		resetObjects();
	}
	static bool playing = false;
	if (isEvent(Events::F) && !playing)
		playing = true;
	if (playing)
		_testEnemy->getAnimation()->animate(dt);

	//glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1600) / static_cast<float>(1200), 0.1f, 100.0f);
	//rigidTest.setViewProjMat(_testCommando->getCamera()->whereAreWeLooking(), projection);
}

void GameplayScene::mouseFunction(const double xpos, const double ypos)
{
	if (firstMouse)
	{
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}

	GLfloat xOffset = xpos - lastX;
	GLfloat yOffset = lastY - ypos;
	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);


	if (!_testCommando->_input.keyboard->keyPressed(Events::G))
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	_testCommando->getCamera()->doMouseMovement(xOffset, yOffset);
}

void GameplayScene::clickFunction(const int button, const int action, const int mods)
{
	_testCommando->_input.clickListener.checkClick(button, action, mods);
}

void GameplayScene::resetObjects() {
	_testCommando->_rigidBody._position = { -10.0f, 0.0f, 0.0f };
	_testCommando->setHealth(_testCommando->getMaxHp());
	_testCommando->setShield(_testCommando->getMaxShield());

	_testEnemy->_rigidBody._position = glm::vec3(26.80f, 1.0f, -59.976f);
	_testEnemy->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f);
	_testEnemy->setHealth(100.0f);

	_testGhoul->_rigidBody._position = glm::vec3(26.80f, 0.0f, -59.976f);
	_testGhoul->setHealth(70.0f);

	_testRobo->_rigidBody._position = glm::vec3(30.0f, 0.0f, -50.0f);
	_testRobo->setHealth(200.0f);

	_testCaptain->_rigidBody._position = glm::vec3(32.0f, 0.0f, -50.0f);
	_testCaptain->setHealth(100.0f);

	_testGrunt->_rigidBody._position = glm::vec3(34.0f, 0.0f, -50.0f);
	_testGrunt->setHealth(75.0f);

	_testSquelch->_rigidBody._position = glm::vec3(36.0f, 0.0f, -50.0f);
	_testSquelch->setHealth(0.0f);

	_testSentinel->_rigidBody._position = glm::vec3(26.0f, 0.0f, -50.0f);
	_testSentinel->setHealth(1000.0f);

	_testEnemy->setActive(true);
	_testGhoul->setActive(true);
	_testSentinel->setActive(true);
	_testRobo->setActive(true);
	_testGrunt->setActive(true);
	_testCaptain->setActive(true);
	_testSquelch->setActive(true);

}
