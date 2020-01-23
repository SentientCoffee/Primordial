#include "GameplayScene711.h"
#include "Options.h"
#include <ctime>

GameplayScene::GameplayScene(const bool isActive) :
	Scene(isActive),
	_pLight(glm::vec2(1600.0f, 1200.0f), { glm::vec3(0.0f,-100.0f,0.0f) }, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 16.0f)
	, cursorBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f)), _levelManager(_pLight)
{
	srand(time(0));

	_testShopTerminal = new ShopTerminal(_pLight._pointLightShader, { new Cappuccino::Texture("container2.png",Cappuccino::TextureType::DiffuseMap) }, { new Cappuccino::Mesh("Cube2.obj") }, _testCommando, cursorBox);
	_testShopTerminal->_rigidBody._position = glm::vec3(-10.0f, 0.0f, 0.0f);


	auto diffuse = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::DiffuseMap);
	auto matte = new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap);
	auto spec = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::SpecularMap);
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
	_bullion = new Bullion(_pLight._pointLightShader, {
		new Cappuccino::Texture("lootChest-contents-BaseColor.png", Cappuccino::TextureType::DiffuseMap),
		new Cappuccino::Texture("lootChest-contents-BaseColor.png", Cappuccino::TextureType::SpecularMap),
		new Cappuccino::Texture("lootChest-contents-Emissive.png", Cappuccino::TextureType::EmissionMap),
		new Cappuccino::Texture("lootChest-contents-Height.png", Cappuccino::TextureType::HeightMap),
		new Cappuccino::Texture("lootChest-contents-Normal.png", Cappuccino::TextureType::NormalMap)
		});
	_chest = new Chest(_pLight._pointLightShader, {
		new Cappuccino::Texture("lootChest-closed-BaseColor.png", Cappuccino::TextureType::DiffuseMap),
		new Cappuccino::Texture("lootChest-closed-BaseColor.png", Cappuccino::TextureType::SpecularMap),
		new Cappuccino::Texture("lootChest-closed-Emissive.png", Cappuccino::TextureType::EmissionMap),
		new Cappuccino::Texture("lootChest-closed-Height.png", Cappuccino::TextureType::HeightMap),
		new Cappuccino::Texture("lootChest-closed-Normal.png", Cappuccino::TextureType::NormalMap)
		});
	_openedChest = new Chest(_pLight._pointLightShader, {
		new Cappuccino::Texture("lootChest-opened-BaseColor.png", Cappuccino::TextureType::DiffuseMap),
		new Cappuccino::Texture("lootChest-opened-Emissive.png", Cappuccino::TextureType::EmissionMap),
		new Cappuccino::Texture("lootChest-opened-Height.png", Cappuccino::TextureType::HeightMap),
		new Cappuccino::Texture("lootChest-opened-Normal.png", Cappuccino::TextureType::NormalMap)
		}, { new Cappuccino::Mesh("lootChest-opened.obj") });

	_sednium->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), .5f);
	_healthPack->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), .5f);
	_ammoPack->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), .5f);

	_chest->_rigidBody._position = glm::vec3(10.0f, -2.0f, -8.5f);
	_openedChest->_rigidBody._position = glm::vec3(10.0f, -2.0f, -8.5f);

	//handle room data here
	
	_levelManager.rooms.push_back(new Building("./Assets/LevelData/Room1LevelData.obj", "./Assets/Meshes/Hitboxes/Room1HitboxData.obj", &_pLight._pointLightShader, { diffuse, spec }, { new Cappuccino::Mesh("Room1Low.obj") }));
	_levelManager.rooms.push_back(new Building("./Assets/LevelData/Room2LevelData.obj", "./Assets/Meshes/Hitboxes/Room2HitboxData.obj", &_pLight._pointLightShader, { diffuse, spec }, { new Cappuccino::Mesh("Room2Low.obj") }));
	_levelManager.rooms.push_back(new Building("./Assets/LevelData/Room3LevelData.obj", "./Assets/Meshes/Hitboxes/Room3HitboxData.obj", &_pLight._pointLightShader, { diffuse, spec }, { new Cappuccino::Mesh("Room3Low.obj") }));
	//_levelManager.rooms.push_back(new Building("./Assets/LevelData/Room1LevelData.obj", "./Assets/Meshes/Hitboxes/Room2Hitbox.obj", &_pLight._pointLightShader, { diffuse, spec }, { new Cappuccino::Mesh("room1.obj") }));

	for (unsigned i = 0; i < 5; i++)
		_levelManager.airlocks.push_back(new Building("./Assets/LevelData/AirLockData.obj", "./Assets/Meshes/Hitboxes/AirlockHitbox.obj", &_pLight._pointLightShader, { matte, spec }, { new Cappuccino::Mesh("Airlock.obj") }));

	auto botMesh = new Cappuccino::Mesh("Bot.obj");
	botMesh->loadMesh();

	
	for (unsigned i=0;i<30;i++)
	{
		_pLight.getPositions().push_back(glm::vec3(0, -100, 0));
	}

	_pLight.resendLights();

	_testEnemy = new Sentry(&_pLight._pointLightShader, { red, spec }, { new Cappuccino::Mesh("Sentry.obj") }, 1.0f);
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
		else if (Options::Demolitionist)
			_testCommando = new Demolitionist(&_pLight._pointLightShader, {}, {});
		else if (Options::Scout)
			_testCommando = new Scout(&_pLight._pointLightShader, {}, {});

		bullet->_transform.scale(glm::vec3(1.0f), 0.1f);
		_testCommando->addAmmo(bullet, bullet2);
		_testCommando->getUILight().getPositions().clear();
		for (unsigned i = 0; i < _pLight.getPositions().size(); i++)
			_testCommando->getUILight().getPositions().push_back(_pLight.getPositions()[i]);
		_testCommando->getUILight().resendLights();
		_testCommando->getUILight().setPlayerPosition(_testCommando->_rigidBody._position);
		_testCommando->_rigidBody._position = glm::vec3(-30.0f, 0.0f, -5.0f) + _levelManager.airlocks[0]->_levelData._spawnPoint;
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
	for (auto x : _loot)
		x->setActive(true);
	for (auto x : lamps)
		x->setActive(true);

	_testShopTerminal->setActive(true);
	_chest->setActive(true);

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

	for (auto x : lamps)
		x->setActive(false);

	for (auto x : _loot)
		x->setActive(false);

	_testShopTerminal->setActive(false);

	return true;
}

void GameplayScene::shootCollisionBehaviour(Enemy* enemy) {
	enemy->hurt(_testCommando->getGun()->getDamage());
	_testCommando->getGun()->specialCollisionBehaviour(_enemies);

	//special behaviour if the enemy dies
	if (enemy->dead())
	{
		auto rando = rand() % 3;
		if (rando == 0)
			_loot.push_back(_sednium->spawn(enemy->getWeight(), enemy->_rigidBody._position));
		else if (rando == 1)
			_loot.push_back(_healthPack->spawn(enemy->getWeight(), enemy->_rigidBody._position));
		else if (rando == 2)
			_loot.push_back(_ammoPack->spawn(enemy->getWeight(), enemy->_rigidBody._position));
	}
}

void GameplayScene::childUpdate(float dt)
{
	_levelManager.update(dt, _testCommando->_rigidBody);
	_pLight._pointLightShader.use();
	_pLight._pointLightShader.loadViewMatrix(*_testCommando->getCamera());

	/*
	///REMOVE AFTER TESTING <sets half the lights to on/off, example code>
	if (_testCommando->_input.keyboard->keyPressed(Cappuccino::KeyEvent::O)) {
		for (unsigned i = 0; i < _pLight.getActives().size() / 2; i++)
			_pLight.setActive(i, false);
	}
	else if (_testCommando->_input.keyboard->keyPressed(Cappuccino::KeyEvent::I)) {
		for (unsigned i = 0; i < _pLight.getActives().size() / 2; i++)
			_pLight.setActive(i, true);

	}
	for (unsigned i = 0; i < lamps.size(); i++) {
		lamps[i]->setActive(_pLight.getActives()[i]);
	}
	///REMOVE AFTER TESTING


	_testCommando->getUILight().getActives() = _pLight.getActives();
	_testCommando->getUILight().getPositions() = _pLight.getPositions();
	_testCommando->getUILight().setPlayerPosition(_testCommando->_rigidBody._position);
	_testCommando->getUILight().resendLights();



	//printf("%f,%f,%f\n", _testCommando->_rigidBody._position.x, _testCommando->_rigidBody._position.y, _testCommando->_rigidBody._position.z);

	//enemy logic
	for (auto& enemy : _enemies) {

		//activate enemy if within a trigger volume
		if (_testCommando->checkCollision(enemy->triggerVolume, enemy->_rigidBody._position) && enemy->isActive())
			enemy->setTrigger(true);
		else
			enemy->setTrigger(false);

		enemy->dead(); //checks for squelch 

		//bullet collision
		if (!_testCommando->getGun()->isHitscan()) {

			//loop through the player's bullets
			for (auto playerBullets : _testCommando->getGun()->getBullets()) {
				//check if the bullet touches an enemy
				if (playerBullets->_rigidBody.checkCollision(enemy->_rigidBody) && playerBullets->isActive() && enemy->isActive()) {
					shootCollisionBehaviour(enemy);
					playerBullets->setActive(false);
				}
			}
		}
		else {
			//	if (enemy->_rigidBody.intersecting(_testCommando->getGun()->getHitscanRay())) {
			//		shootCollisionBehaviour(enemy);
			//	}
		}
		enemy->attack(_testCommando, dt);

		for (auto bullet : enemy->getGun()->getBullets()) {
			if (bullet->checkCollision(_testCommando)) {
				_testCommando->takeDamage(enemy->getGun()->getDamage());
			}
		}
	}

	if (_testCommando->checkCollision(_chest->_triggerVolume, _chest->_rigidBody._position) && _testCommando->_input.keyboard->keyPressed('E') && !_chest->open())
	{
		_chest->setActive(false);
		_openedChest->setActive(true);
		std::vector<Loot*> _temp = _chest->spawn(10.0f, _chest->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f), _sednium, _healthPack, _ammoPack, _bullion);
		for (auto x : _temp)
		{
			_loot.push_back(x);
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
		_testCommando->_rigidBody._position = _levelManager.rooms[_levelManager._currentRoom]->_levelData._spawnPoint+_levelManager.rooms[_levelManager._currentRoom]->_rigidBody._position;
		_testCommando->_rigidBody._position.y += 2;
		_testCommando->setHealth(_testCommando->getMaxHp());
		_testCommando->setShield(_testCommando->getMaxShield());
		_testCommando->_rigidBody._vel = glm::vec3(0.0f);
		_testCommando->_rigidBody._accel = glm::vec3(0.0f);
	}

	_testEnemy->_rigidBody._position = glm::vec3(26.80f, 5.0f, -50.0f);
	_testGhoul->_rigidBody._position = glm::vec3(26.80f, -1.5f, -60.0f);
	_testRobo->_rigidBody._position = glm::vec3(30.0f, -1.5f, -50.0f);
	_testCaptain->_rigidBody._position = glm::vec3(32.0f, -1.5f, -50.0f);
	_testGrunt->_rigidBody._position = glm::vec3(34.0f, -1.5f, -50.0f);
	_testSquelch->_rigidBody._position = glm::vec3(35.0f, -1.5f, -48.0f);
	//_testSentinel->_rigidBody._position = glm::vec3(26.0f, 0.0f, -50.0f);

	for (auto& x : _enemies)
	{
		x->setHealth(x->getMaxHP());
		x->setShield(x->getMaxShield());
		x->setActive(true);
	}
	_testEnemy->setActive(false);
}
