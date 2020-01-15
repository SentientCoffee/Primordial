#include "GameplayScene711.h"
#include "Options.h"

GameplayScene::GameplayScene(const bool isActive) :
	Scene(isActive),
	_pLight(glm::vec2(1600.0f, 1200.0f), { glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(30.80f, 0.0f, -12.976f),glm::vec3(-6.0f,0.0f,-70.0f) }, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 16.0f)
	, cursorBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f))
	//_pLight(glm::vec2(1600.0f, 1200.0f), { glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(30.80f, 0.0f, -12.976f),glm::vec3(-6.0f,0.0f,-70.0f)}, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 16.0f)
	_pLight(glm::vec2(1600.0f, 1200.0f), { glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(30.80f, 0.0f, -59.976f),glm::vec3(-6.0f,0.0f,-70.0f)}, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 16.0f), _levelManager(_pLight)
{
	_testShopTerminal = new ShopTerminal(_pLight._pointLightShader, { new Cappuccino::Texture("container2.png",Cappuccino::TextureType::DiffuseMap) }, { new Cappuccino::Mesh("Cube2.obj") }, _testCommando, cursorBox);
	_testShopTerminal->_rigidBody._position = glm::vec3(-10.0f, 0.0f, 0.0f);


	auto diffuse = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::DiffuseMap);
	auto matte = new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap);
	auto spec = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::SpecularMap);
	auto norm = new Cappuccino::Texture(std::string("pistolNorm.png"), Cappuccino::TextureType::NormalMap);
	auto red = new Cappuccino::Texture(std::string("red.png"), Cappuccino::TextureType::DiffuseMap);

	_sednium = new Sednium(_pLight._pointLightShader, { red, spec });
	_ammoPack = new AmmoPack(_pLight._pointLightShader, {
		new Cappuccino::Texture("pickupAmmoDiffuse.png",Cappuccino::TextureType::DiffuseMap),
		new Cappuccino::Texture("pickupAmmoDiffuse.png",Cappuccino::TextureType::SpecularMap),
		new Cappuccino::Texture("pickupAmmoNormal.png",Cappuccino::TextureType::NormalMap),
		new Cappuccino::Texture("pickupAmmoEmission.png",Cappuccino::TextureType::EmissionMap) });
	_healthPack = new HealthPack(_pLight._pointLightShader, {
		new Cappuccino::Texture("healthPickupDiffuse.png",Cappuccino::TextureType::DiffuseMap),
		new Cappuccino::Texture("healthPickupDiffuse.png",Cappuccino::TextureType::SpecularMap),
		new Cappuccino::Texture("healthPickupNormal.png",Cappuccino::TextureType::NormalMap),
		new Cappuccino::Texture("healthPickupEmission.png",Cappuccino::TextureType::EmissionMap) });

	_testEnemy = new Sentry(&_pLight._pointLightShader, { matte, spec }, { new Cappuccino::Mesh("Sentry.obj") }, 1.0f);

	//handle room data here
	_levelManager.rooms.push_back(new Building("./Assets/LevelData/Room2LevelData.obj", "./Assets/Meshes/Hitboxes/Room2Hitbox.obj", &_pLight._pointLightShader, { diffuse, spec }, { new Cappuccino::Mesh("room1.obj") }));
	for (unsigned i = 0; i < 5; i++)
		_levelManager.airlocks.push_back(new Building("./Assets/LevelData/AirLockData.obj", "./Assets/Meshes/Hitboxes/AirlockHitbox.obj", &_pLight._pointLightShader, { diffuse, spec }, { new Cappuccino::Mesh("Airlock.obj") }));

	auto botMesh = new Cappuccino::Mesh("Bot.obj");
	botMesh->loadMesh();

	for (auto x : _levelManager.rooms)
	{
		for (auto y : x->_levelData.lights)
		{
			
		}
	}

	_testGhoul = new Ghoul(&_pLight._pointLightShader, { matte, spec }, { new Cappuccino::Mesh("Crawler.obj") }, 1.0f);
	_testRobo = new RoboGunner(&_pLight._pointLightShader, { red, spec }, { botMesh });
	_testCaptain = new Captain(&_pLight._pointLightShader, { red, spec }, { botMesh });
	_testGrunt = new Grunt(&_pLight._pointLightShader, { diffuse, spec }, { botMesh });
	_testSquelch = new Squelch(&_pLight._pointLightShader, { matte, spec }, { new Cappuccino::Mesh("Squelch.obj") });

	resetObjects();

	//init members here
	auto mesh = new Cappuccino::Mesh("Bullet.obj");
	mesh->loadMesh();

	bullet = new Bullet(_pLight._pointLightShader, { matte, spec }, { mesh }, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2 = new Bullet(_pLight._pointLightShader, { matte, spec }, { mesh }, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2->_transform.scale(glm::vec3(1.0f), 0.1f);
	_testEnemy->getGun()->addBullets(bullet);
	_testRobo->getGun()->addBullets(bullet);
	_testCaptain->getGun()->addBullets(bullet);
	_testGrunt->getGun()->addBullets(bullet);

	_enemies.push_back(_testEnemy);
	_enemies.push_back(_testGhoul);
	_enemies.push_back(_testRobo);
	_enemies.push_back(_testGrunt);
	_enemies.push_back(_testCaptain);
	_enemies.push_back(_testSquelch);


	for (unsigned i = 0; i < _pLight.getPositions().size(); i++) {
		lamps.push_back(new Billboard(&_pLight._pointLightShader, { matte }));
		lamps.back()->_rigidBody._position = _pLight.getPositions()[i];
	}
	

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
		_testCommando->getUILight().resendLights();
		_testCommando->getUILight().setPlayerPosition(_testCommando->_rigidBody._position);
		createdPlayer = true;

		_testShopTerminal->_player = _testCommando;
	}

	//activate members here
	_initialized = true;
	_shouldExit = false;
	_testCommando->setActive(true);
	_testEnemy->setActive(true);
	_testGhoul->setActive(true);
	_testRobo->setActive(true);
	_testGrunt->setActive(true);
	_testCaptain->setActive(true);
	_testSquelch->setActive(true);
	_levelManager.rooms[0]->setActive(true);
	for (auto& airlock : _levelManager.airlocks)
		airlock->setActive(true);
	_sednium->setPosition(_testCommando->_rigidBody._position);
	_sednium->setActive(true);
	for (auto x : _loot)
		x->setActive(true);
	for (auto x : lamps)
		x->setActive(true);

	_testShopTerminal->setActive(true);

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
	_testRobo->setActive(false);
	_testGrunt->setActive(false);
	_testCaptain->setActive(false);
	_testSquelch->setActive(false);
	for (auto& room : _levelManager.rooms)
		room->setActive(false);
	for (auto& airlock : _levelManager.airlocks)
		airlock->setActive(false);
	//for (auto x : _sednium)

	for (auto x : lamps)
		x->setActive(false);

	for (auto x : _loot)
		x->setActive(false);
	//	x->setActive(false);

	_testShopTerminal->setActive(false);

	return true;
}



void GameplayScene::childUpdate(float dt)
{

	_levelManager.update(dt, _testCommando->_rigidBody);

	//_pLight.updateViewPos(_testCommando->getCamera()->getPosition());
	_pLight._pointLightShader.use();
	_pLight._pointLightShader.loadViewMatrix(*_testCommando->getCamera());

	///make a function later
	_pLight.getPositions()[0] += glm::vec3(1.0f, 0.0f, 0.0f) * 2.f * dt;
	_testCommando->getUILight().getPositions() = _pLight.getPositions();
	//_pLight.getPositions().back() = _testCommando->_rigidBody._position;
	_pLight.resendLights();

	lamps.front()->_rigidBody._position = _pLight.getPositions()[0];
	///make a function later

	//_testCommando->getUILight().updateViewPos(_testCommando->getCamera()->getPosition());

	_testCommando->getUILight().setPlayerPosition(_testCommando->_rigidBody._position);
	_testCommando->getUILight().resendLights();

	//printf("%f,%f,%f\n", _testCommando->_rigidBody._position.x, _testCommando->_rigidBody._position.y, _testCommando->_rigidBody._position.z);

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
			if (bullet->checkCollision(_testCommando)) {
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

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1600) / static_cast<float>(1000), 0.1f, 100.0f);
	rigidTest.setViewProjMat(_testCommando->getCamera()->whereAreWeLooking(), projection);


	//deal with shop interface
	cursorBox._position = glm::vec3(cursorPos.x, cursorPos.y, 0.0f);




}

void GameplayScene::mouseFunction(const double xpos, const double ypos)
{
	if (ShopTerminal::_cursorLocked)
		firstMouse = true;

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

	if (!ShopTerminal::_cursorLocked)
		_testCommando->getCamera()->doMouseMovement(xOffset, yOffset);

	//keep track of the cursor's position
	cursorPos = glm::vec2(xpos, ypos);
	cursorPos -= glm::vec2(800.0f, 500.0f);
}

void GameplayScene::clickFunction(const int button, const int action, const int mods)
{
	_testCommando->_input.clickListener.checkClick(button, action, mods);
}

void GameplayScene::resetObjects() {
	if (_testCommando != nullptr)
	{
		_testCommando->_rigidBody._position = { -10.0f, 0.0f, 0.0f };
		_testCommando->setHealth(_testCommando->getMaxHp());
		_testCommando->setShield(_testCommando->getMaxShield());
	}

	_testEnemy->_rigidBody._position = glm::vec3(26.80f, -1.0f, -50.0f);
	_testEnemy->setHealth(50.0f);

	_testGhoul->_rigidBody._position = glm::vec3(26.80f, -1.0f, -60.0f);
	_testGhoul->setHealth(70.0f);

	_testRobo->_rigidBody._position = glm::vec3(30.0f, -1.0f, -50.0f);
	_testRobo->setHealth(200.0f);

	_testCaptain->_rigidBody._position = glm::vec3(32.0f, -1.0f, -50.0f);
	_testCaptain->setHealth(100.0f);

	_testGrunt->_rigidBody._position = glm::vec3(34.0f, -1.0f, -50.0f);
	_testGrunt->setHealth(75.0f);

	_testSquelch->_rigidBody._position = glm::vec3(36.0f, -1.0f, -50.0f);
	_testSquelch->setHealth(50.0f);


	_testEnemy->setActive(true);
	_testGhoul->setActive(true);
	_testRobo->setActive(true);
	_testGrunt->setActive(true);
	_testCaptain->setActive(true);
	_testSquelch->setActive(true);

}
