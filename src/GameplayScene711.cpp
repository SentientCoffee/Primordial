#include "GameplayScene711.h"
#include "Options.h"

#include <Cappuccino/ResourceManager.h>
#include <ctime>

GameplayScene::GameplayScene(const bool isActive) :
	Scene(isActive),
	_pLight(glm::vec2(1600.0f, 1200.0f), { glm::vec3(0.0f,-100.0f,0.0f) },
	        glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 16.0f),
	cursorBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f)),
	_levelManager(_pLight) {

	_testShopTerminal = new ShopTerminal(_pLight._pointLightShader, {
		Cappuccino::TextureLibrary::loadTexture("Shop terminal diffuse", "container2.png",Cappuccino::TextureType::DiffuseMap)
	}, {
		Cappuccino::MeshLibrary::loadMesh("Shop terminal", "Cube2.obj")
	}, _testCommando, cursorBox);

	_testShopTerminal->_rigidBody._position = glm::vec3(-10.0f, 0.0f, 0.0f);


	const auto matte   = Cappuccino::TextureLibrary::loadTexture("Level matte",          "matte.png", Cappuccino::TextureType::DiffuseMap);
	const auto diffuse = Cappuccino::TextureLibrary::loadTexture("Level metal",          "metal.png", Cappuccino::TextureType::DiffuseMap);
	const auto spec    = Cappuccino::TextureLibrary::loadTexture("Level metal specular", "metal.png", Cappuccino::TextureType::SpecularMap);
	const auto red     = Cappuccino::TextureLibrary::loadTexture("Enemy red diffuse",    "red.png",   Cappuccino::TextureType::DiffuseMap);

	_sednium  = new Sednium(_pLight._pointLightShader, { red, spec });
	
	_ammoPack = new AmmoPack(_pLight._pointLightShader, {
		Cappuccino::TextureLibrary::loadTexture("Ammo pack diffuse",  "pickupAmmoDiffuse.png",  Cappuccino::TextureType::DiffuseMap),
		Cappuccino::TextureLibrary::loadTexture("Ammo pack specular", "pickupAmmoDiffuse.png",  Cappuccino::TextureType::SpecularMap),
		Cappuccino::TextureLibrary::loadTexture("Ammo pack normal",   "pickupAmmoNormal.png",   Cappuccino::TextureType::NormalMap),
		Cappuccino::TextureLibrary::loadTexture("Ammo pack emission", "pickupAmmoEmission.png", Cappuccino::TextureType::EmissionMap)
	});
	
	_healthPack = new HealthPack(_pLight._pointLightShader, {
		Cappuccino::TextureLibrary::loadTexture("Health pack diffuse",  "healthPickupDiffuse.png",  Cappuccino::TextureType::DiffuseMap),
		Cappuccino::TextureLibrary::loadTexture("Health pack specular", "healthPickupDiffuse.png",  Cappuccino::TextureType::SpecularMap),
		Cappuccino::TextureLibrary::loadTexture("Health pack normal",   "healthPickupNormal.png",   Cappuccino::TextureType::NormalMap),
		Cappuccino::TextureLibrary::loadTexture("Health pack emission", "healthPickupEmission.png", Cappuccino::TextureType::EmissionMap)
	});
	
	_bullion = new Bullion(_pLight._pointLightShader, {
		Cappuccino::TextureLibrary::loadTexture("Bullion diffuse",  "lootChest-contents-BaseColor.png", Cappuccino::TextureType::DiffuseMap),
		Cappuccino::TextureLibrary::loadTexture("Bullion specular", "lootChest-contents-BaseColor.png", Cappuccino::TextureType::SpecularMap),
		Cappuccino::TextureLibrary::loadTexture("Bullion normal",   "lootChest-contents-Normal.png",    Cappuccino::TextureType::NormalMap),
		Cappuccino::TextureLibrary::loadTexture("Bullion emission", "lootChest-contents-Emissive.png",  Cappuccino::TextureType::EmissionMap),
		Cappuccino::TextureLibrary::loadTexture("Bullion height",   "lootChest-contents-Height.png",    Cappuccino::TextureType::HeightMap)
	});
	
	_chest = new Chest(_pLight._pointLightShader, {
		Cappuccino::TextureLibrary::loadTexture("Loot chest closed diffuse",  "lootChest-closed-BaseColor.png", Cappuccino::TextureType::DiffuseMap),
		Cappuccino::TextureLibrary::loadTexture("Loot chest closed specular", "lootChest-closed-BaseColor.png", Cappuccino::TextureType::SpecularMap),
		Cappuccino::TextureLibrary::loadTexture("Loot chest closed normal",   "lootChest-closed-Normal.png",    Cappuccino::TextureType::NormalMap),
		Cappuccino::TextureLibrary::loadTexture("Loot chest closed emission", "lootChest-closed-Emissive.png",  Cappuccino::TextureType::EmissionMap),
		Cappuccino::TextureLibrary::loadTexture("Loot chest closed height",   "lootChest-closed-Height.png",    Cappuccino::TextureType::HeightMap)
	});

	_chest->_rigidBody._position       = glm::vec3(10.0f, -2.0f, -8.5f);

	//handle room data here

	
	_levelManager.rooms.push_back(new Building("./Assets/LevelData/Room1LevelData.obj", "./Assets/SpawnData/Room1SpawnData.obj", "./Assets/Meshes/Hitboxes/Room1HitboxData.obj", &_pLight._pointLightShader, { diffuse, spec }, { Cappuccino::MeshLibrary::loadMesh("Room 1", "Room1Low.obj") }));
	_levelManager.rooms.push_back(new Building("./Assets/LevelData/Room2LevelData.obj", "./Assets/SpawnData/Room2SpawnData.obj", "./Assets/Meshes/Hitboxes/Room2HitboxData.obj", &_pLight._pointLightShader, { diffuse, spec }, { Cappuccino::MeshLibrary::loadMesh("Room 2", "Room2Low.obj") }));
	_levelManager.rooms.push_back(new Building("./Assets/LevelData/Room3LevelData.obj", "./Assets/SpawnData/Room3SpawnData.obj", "./Assets/Meshes/Hitboxes/Room3HitboxData.obj", &_pLight._pointLightShader, { diffuse, spec }, { Cappuccino::MeshLibrary::loadMesh("Room 3", "Room3Low.obj") }));

	for (unsigned i = 0; i < 5; i++)
		_levelManager.airlocks.push_back(new Building("./Assets/LevelData/AirLockData.obj", "./Assets/SpawnData/AirLockSpawnData.obj", "./Assets/Meshes/Hitboxes/AirlockHitbox.obj", &_pLight._pointLightShader, { matte, spec }, { Cappuccino::MeshLibrary::loadMesh("Airlock", "Airlock.obj") }));

	auto botMesh = Cappuccino::MeshLibrary::loadMesh("Bot", "Bot.obj");
	botMesh->loadMesh();


	for (unsigned i = 0; i < 30; i++) {
		_pLight.getPositions().emplace_back(0, -100, 0);
	}
	_pLight.resendLights();

	for (unsigned i = 0; i < 10; i++)
		_levelManager._enemyManager._enemies.push_back(new Sentry(&_pLight._pointLightShader, { red, spec }, { Cappuccino::MeshLibrary::loadMesh("Sentry", "Sentry.obj") }, 1.0f));
	
	for (unsigned i = 0; i < 10; i++)
		_levelManager._enemyManager._enemies.push_back(new Ghoul(&_pLight._pointLightShader, {
		Cappuccino::TextureLibrary::loadTexture("Crawler diffuse",  "CrawlerDiffuse.png", Cappuccino::TextureType::DiffuseMap),
		Cappuccino::TextureLibrary::loadTexture("Crawler specular", "CrawlerDiffuse.png", Cappuccino::TextureType::SpecularMap),
		Cappuccino::TextureLibrary::loadTexture("Crawler normal",   "CrawlerNorm.png",    Cappuccino::TextureType::NormalMap)
			}, {
				Cappuccino::MeshLibrary::loadMesh("Crawler", "Crawler.obj")
			}, 1.0f));

	for (unsigned i = 0; i < 10; i++)
		_levelManager._enemyManager._enemies.push_back(new RoboGunner(&_pLight._pointLightShader, { red, spec }, { botMesh }));
	
	for (unsigned i = 0; i < 10; i++)
		_levelManager._enemyManager._enemies.push_back(new Captain(&_pLight._pointLightShader, { red, spec }, { botMesh }));

	for (unsigned i = 0; i < 10; i++)
		_levelManager._enemyManager._enemies.push_back(new Grunt(&_pLight._pointLightShader, { diffuse, spec }, { botMesh }));
	
	for (unsigned i = 0; i < 10; i++)
		_levelManager._enemyManager._enemies.push_back(new Squelch(&_pLight._pointLightShader, { matte, spec }, { Cappuccino::MeshLibrary::loadMesh("Squelch", "Squelch.obj") }));
	
	resetObjects();

	//init members here
	auto mesh = Cappuccino::MeshLibrary::loadMesh("Bullet", "Bullet.obj");
	mesh->loadMesh();

	bullet = new Bullet(_pLight._pointLightShader, { matte, spec }, { mesh }, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2 = new Bullet(_pLight._pointLightShader, { matte, spec }, { mesh }, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2->_transform.scale(glm::vec3(1.0f), 0.1f);

	for (unsigned i = 0; i < _levelManager._enemyManager._enemies.size(); i++)
		if(_levelManager._enemyManager._enemies[i]->_enemyType!="Ghoul"&& _levelManager._enemyManager._enemies[i]->_enemyType != "Squelch")
			_levelManager._enemyManager._enemies[i]->getGun()->addBullets(bullet);


	for (unsigned i = 0; i < _pLight.getPositions().size(); i++) {
		lamps.push_back(new Billboard(&_pLight._pointLightShader, { matte }));
		lamps.back()->_rigidBody._position = _pLight.getPositions()[i];
	}

	_skybox = new Cappuccino::Cubemap({ 1600.0f, 1200.0f }, {
		"./Assets/Textures/Skybox/x7/px.png",
		"./Assets/Textures/Skybox/x7/nx.png",
		"./Assets/Textures/Skybox/x7/ny.png",
		"./Assets/Textures/Skybox/x7/py.png",
		"./Assets/Textures/Skybox/x7/pz.png",
		"./Assets/Textures/Skybox/x7/nz.png"
	});
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
		_testCommando->_rigidBody._position = glm::vec3(-30.0f, 0.0f, -5.0f) + _levelManager.airlocks[0]->_levelData._respawnPoint;
		createdPlayer                       = true;

		_testShopTerminal->_player = _testCommando;

	}

	//activate members here
	_initialized = true;
	_shouldExit  = false;
	_testCommando->setActive(true);
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
	_shouldExit  = true;
	_testCommando->setActive(false);
	for (auto& room : _levelManager.rooms)
		room->setActive(false);
	for (auto& airlock : _levelManager.airlocks)
		airlock->setActive(false);
	for (auto& enemy : _levelManager._enemyManager._enemies)
		enemy->setActive(false);

	for (auto x : lamps)
		x->setActive(false);

	for (auto x : _loot)
		x->setActive(false);

	_testShopTerminal->setActive(false);

	return true;
}

void GameplayScene::shootCollisionBehaviour(Enemy* enemy) {
	enemy->hurt(_testCommando->getGun()->getDamage());
	_testCommando->getGun()->specialCollisionBehaviour(_levelManager._enemyManager._enemies);

	//special behaviour if the enemy dies
	if (enemy->dead())
	{
		_loot.push_back(_sednium->spawn(enemy->getWeight(), enemy->_rigidBody._position));
		_loot.push_back(_healthPack->spawn(enemy->getWeight(), enemy->_rigidBody._position));
		_loot.push_back(_ammoPack->spawn(enemy->getWeight(), enemy->_rigidBody._position));
	}
}

void GameplayScene::childUpdate(float dt)
{
	_levelManager.update(dt, _testCommando->_rigidBody);
	_pLight._pointLightShader.use();
	_pLight._pointLightShader.loadViewMatrix(*_testCommando->getCamera());

	_testCommando->getUILight().getActives()   = _pLight.getActives();
	_testCommando->getUILight().getPositions() = _pLight.getPositions();
	_testCommando->getUILight().setPlayerPosition(_testCommando->_rigidBody._position);
	_testCommando->getUILight().resendLights();

	//printf("%f,%f,%f\n", _testCommando->_rigidBody._position.x, _testCommando->_rigidBody._position.y, _testCommando->_rigidBody._position.z);

	//enemy logic
	for (auto& enemy : _levelManager._enemyManager._enemies) {

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
			if (bullet->checkCollision(_testCommando) && bullet->isActive()) {
				_testCommando->takeDamage(enemy->getGun()->getDamage());
				bullet->setActive(false);
			}
		}
	}

	//loot chest interaction, this should probably be a function inside the chest class
	if (_testCommando->checkCollision(_chest->_triggerVolume, _chest->_rigidBody._position) && _testCommando->_input.keyboard->keyPressed('E') && !_chest->open())
	{
		std::vector<Loot*> _temp = _chest->spawn(10.0f, _chest->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f), _sednium, _healthPack, _ammoPack, _bullion);
		for (auto x : _temp)
		{
			_loot.push_back(x);
		}
	}

	for (auto& x : _loot) {
		if (x->isActive())
		{
			x->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), dt * 20.0f);
			x->pickup(_testCommando);
		}
	}

	if (_testCommando->getHealth() <= 0) {
		resetObjects();
	}

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1600) / static_cast<float>(1000), 0.1f, 100.0f);
	rigidTest.setViewProjMat(_testCommando->getCamera()->whereAreWeLooking(), projection);


	//deal with shop interface
	cursorBox._position = glm::vec3(cursorPos.x, cursorPos.y, 0.0f);


	const glm::mat4 view = glm::mat4(glm::mat3(_testCommando->getCamera()->whereAreWeLooking()));
	_skybox->getShader().use();
	_skybox->getShader().setUniform("view", view);

}

void GameplayScene::mouseFunction(const double xpos, const double ypos)
{
	if (ShopTerminal::_cursorLocked)
		firstMouse = true;

	if (firstMouse)
	{
		lastX      = static_cast<float>(xpos);
		lastY      = static_cast<float>(ypos);
		firstMouse = false;
	}

	const GLfloat xOffset = xpos - lastX;
	const GLfloat yOffset = lastY - ypos;
	lastX                 = static_cast<float>(xpos);
	lastY                 = static_cast<float>(ypos);

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
		_testCommando->_rigidBody._position = _levelManager.rooms[_levelManager._currentRoom]->_levelData._respawnPoint + _levelManager.rooms[_levelManager._currentRoom]->_rigidBody._position;
		_testCommando->_rigidBody._position.y += 2;
		_testCommando->setHealth(_testCommando->getMaxHp());
		_testCommando->setShield(_testCommando->getMaxShield());
		_testCommando->_rigidBody._vel   = glm::vec3(0.0f);
		_testCommando->_rigidBody._accel = glm::vec3(0.0f);
	}

	for (auto& x : _levelManager._enemyManager._enemies)
	{
		x->setHealth(x->getMaxHP());
		x->setShield(x->getMaxShield());
		x->setActive(true);
	}
}
