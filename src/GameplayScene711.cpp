#include "GameplayScene711.h"
#include "Options.h"

#include <Cappuccino/ResourceManager.h>
#include <ctime>

//whew
#define LOAD_TEXTURE Cappuccino::TextureLibrary::loadTexture
#define LOAD_MESH Cappuccino::MeshLibrary::loadMesh

GameplayScene::GameplayScene(const bool isActive) :
	Scene(isActive),
	_pLight(glm::vec2(1600.0f, 1200.0f), { glm::vec3(0.0f,-100.0f,0.0f) },
		glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 16.0f),
	cursorBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f)),
	_levelManager(_pLight) {

	_levelManager._testShopTerminal = new ShopTerminal(_pLight._pointLightShader, {
		LOAD_TEXTURE("Shop terminal diffuse", "Shop/Shop Base/shopBase_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::DiffuseMap),
		LOAD_TEXTURE("Shop terminal diffuses", "Shop/Shop Base/shopBase_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::SpecularMap),
		LOAD_TEXTURE("Shop terminal diffusess", "Shop/Shop Base/shopBase_low_DefaultMaterial_Normal.png",Cappuccino::TextureType::NormalMap),
		LOAD_TEXTURE("Shop terminal diffusesss", "Shop/Shop Base/shopBase_low_DefaultMaterial_Emissive.png",Cappuccino::TextureType::EmissionMap),
		

		LOAD_TEXTURE("Shop terminal diffusee", "Shop/Large Ring/shopBigRing_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::DiffuseMap,1),
		LOAD_TEXTURE("Shop terminal diffuseee", "Shop/Large Ring/shopBigRing_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::SpecularMap,1),
		LOAD_TEXTURE("Shop terminal diffuseeee", "Shop/Large Ring/shopBigRing_low_DefaultMaterial_Normal.png",Cappuccino::TextureType::NormalMap,1),
		LOAD_TEXTURE("Shop terminal diffuseeeee", "Shop/Large Ring/shopBigRing_low_DefaultMaterial_Emissive.png",Cappuccino::TextureType::EmissionMap,1),
		
		LOAD_TEXTURE("Shop terminal diffusez",	"Shop/Medium Ring/shopMediumRing_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::DiffuseMap,2),
		LOAD_TEXTURE("Shop terminal diffuseze", "Shop/Medium Ring/shopMediumRing_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::SpecularMap,2),
		LOAD_TEXTURE("Shop terminal diffusewd", "Shop/Medium Ring/shopMediumRing_low_DefaultMaterial_Normal.png",Cappuccino::TextureType::NormalMap,2),
		LOAD_TEXTURE("Shop terminal diffusegfe","Shop/Medium Ring/shopMediumRing_low_DefaultMaterial_Emissive.png",Cappuccino::TextureType::EmissionMap,2),

		LOAD_TEXTURE("Shop terminal diffushqwe", "Shop/Shop Screen/shopScreen_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::DiffuseMap,3),
		LOAD_TEXTURE("Shop terminal diffusefqwf", "Shop/Shop Screen/shopScreen_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::SpecularMap,3),
		LOAD_TEXTURE("Shop terminal diffusegasg", "Shop/Shop Screen/shopScreen_low_DefaultMaterial_Normal.png",Cappuccino::TextureType::NormalMap,3),
		LOAD_TEXTURE("Shop terminal diffuseasxzwe", "Shop/Shop Screen/shopScreen_low_DefaultMaterial_Emissive.png",Cappuccino::TextureType::EmissionMap,3),

		LOAD_TEXTURE("Shop terminal diffuseasxzweeee",	 "Shop/Small Ring/shopSmallRing_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::DiffuseMap,4),
		LOAD_TEXTURE("Shop terminal diffuseasxzweeeeee", "Shop/Small Ring/shopSmallRing_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::SpecularMap,4),
		LOAD_TEXTURE("Shop terminal diffuseasxzwesdads", "Shop/Small Ring/shopSmallRing_low_DefaultMaterial_Normal.png",Cappuccino::TextureType::NormalMap,4),
		LOAD_TEXTURE("Shop terminal diffuseasxzweqqqwe", "Shop/Small Ring/shopSmallRing_low_DefaultMaterial_Emissive.png",Cappuccino::TextureType::EmissionMap,4),


		}, {
			LOAD_MESH("Shop Base",			"Shop/shopBase_low.obj"),
			LOAD_MESH("Shop Big Ring",		"Shop/shopBigRing_low.obj"),
			LOAD_MESH("Shop Medium Ring",	"Shop/shopMediumRing_low.obj"),
			LOAD_MESH("Shop Screen",		"Shop/shopScreen_low.obj"),
			LOAD_MESH("Shop Small Ring",	"Shop/shopSmallRing_low.obj")
		}, _testCommando, cursorBox);

	_levelManager._testShopTerminal->_rigidBody._position = glm::vec3(-10.0f, 0.0f, 0.0f);


	const auto matte = LOAD_TEXTURE("Level matte", "matte.png", Cappuccino::TextureType::DiffuseMap);
	const auto diffuse = LOAD_TEXTURE("Level metal", "metal.png", Cappuccino::TextureType::DiffuseMap);
	const auto spec = LOAD_TEXTURE("Level metal specular", "metal.png", Cappuccino::TextureType::SpecularMap);
	const auto red = LOAD_TEXTURE("Enemy red diffuse", "red.png", Cappuccino::TextureType::DiffuseMap);

	_sednium = new Sednium(_pLight._pointLightShader, { red, spec });

	_ammoPack = new AmmoPack(_pLight._pointLightShader, {
		LOAD_TEXTURE("Ammo pack diffuse",  "ammoPickup/ammoPickup-Diffuse.png",  Cappuccino::TextureType::DiffuseMap),
		LOAD_TEXTURE("Ammo pack specular", "ammoPickup/ammoPickup-Diffuse.png",  Cappuccino::TextureType::SpecularMap),
		LOAD_TEXTURE("Ammo pack normal",   "ammoPickup/ammoPickup-Normal.png",   Cappuccino::TextureType::NormalMap),
		LOAD_TEXTURE("Ammo pack emission", "ammoPickup/ammoPickup-Emission.png", Cappuccino::TextureType::EmissionMap)
		});

	_healthPack = new HealthPack(_pLight._pointLightShader, {
		LOAD_TEXTURE("Health pack diffuse",  "healthPickup/healthPickup-Diffuse.png",  Cappuccino::TextureType::DiffuseMap),
		LOAD_TEXTURE("Health pack specular", "healthPickup/healthPickup-Diffuse.png",  Cappuccino::TextureType::SpecularMap),
		LOAD_TEXTURE("Health pack normal",   "healthPickup/healthPickup-Normal.png",   Cappuccino::TextureType::NormalMap),
		LOAD_TEXTURE("Health pack emission", "healthPickup/healthPickup-Emission.png", Cappuccino::TextureType::EmissionMap)
		});

	_bullion = new Bullion(_pLight._pointLightShader, {
		LOAD_TEXTURE("Bullion diffuse",  "lootChestContents/lootChestContents-Diffuse.png", Cappuccino::TextureType::DiffuseMap),
		LOAD_TEXTURE("Bullion specular", "lootChestContents/lootChestContents-Diffuse.png", Cappuccino::TextureType::SpecularMap),
		LOAD_TEXTURE("Bullion normal",   "lootChestContents/lootChestContents-Normal.png",    Cappuccino::TextureType::NormalMap),
		LOAD_TEXTURE("Bullion emission", "lootChestContents/lootChestContents-Emission.png",  Cappuccino::TextureType::EmissionMap),
		LOAD_TEXTURE("Bullion height",   "lootChestContents/lootChestContents-Height.png",    Cappuccino::TextureType::HeightMap)
		});

	_chest = new Chest(_pLight._pointLightShader, {
		LOAD_TEXTURE("Loot chest closed diffuse",  "lootChest/Chest_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::DiffuseMap),
		LOAD_TEXTURE("Loot chest closed specular", "lootChest/Chest_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::SpecularMap),
		LOAD_TEXTURE("Loot chest closed normal",   "lootChest/Chest_DefaultMaterial_Normal.png",    Cappuccino::TextureType::NormalMap),
		LOAD_TEXTURE("Loot chest closed emission", "lootChest/Chest_DefaultMaterial_Emissive.png",  Cappuccino::TextureType::EmissionMap),
		LOAD_TEXTURE("Loot chest closed height",   "lootChest/Chest_DefaultMaterial_Height.png",    Cappuccino::TextureType::HeightMap)
		});

	_chest->_rigidBody._position = glm::vec3(10.0f, -2.0f, -8.5f);
	_chest->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), -90.0f);

	//handle room data here


	_levelManager.rooms.push_back(new Building("./Assets/LevelData/Room2LevelData.obj", "./Assets/SpawnData/Room2SpawnData.obj", "./Assets/Meshes/Hitboxes/Room2HitboxData.obj", &_pLight._pointLightShader, { diffuse, spec }, { LOAD_MESH("Room 2", "Room2/Room2_Low.obj") }));
	_levelManager.rooms.push_back(new Building("./Assets/LevelData/Room1LevelData.obj", "./Assets/SpawnData/Room1SpawnData.obj", "./Assets/Meshes/Hitboxes/Room1HitboxData.obj", &_pLight._pointLightShader, { diffuse, spec }, { LOAD_MESH("Room 1", "Room1/Room1_Low.obj") }));
	_levelManager.rooms.push_back(new Building("./Assets/LevelData/Room3LevelData.obj", "./Assets/SpawnData/Room3SpawnData.obj", "./Assets/Meshes/Hitboxes/Room3HitboxData.obj", &_pLight._pointLightShader, { diffuse, spec }, { LOAD_MESH("Room 3", "Room3/Room3_low.obj") }));

	for (unsigned i = 0; i < 5; i++)
		_levelManager.airlocks.push_back(new Building("./Assets/LevelData/AirLockLevelData.obj", "./Assets/SpawnData/AirLockSpawnData.obj", "./Assets/Meshes/Hitboxes/AirlockHitboxData.obj", &_pLight._pointLightShader, { matte, spec }, { LOAD_MESH("Airlock", "Airlock.obj") }));

	auto botMesh = LOAD_MESH("Bot", "Bot.obj");
	botMesh->loadMesh();
	auto botDiffuse = LOAD_TEXTURE("Bot-Diffuse.png", "Bot/Bot-Diffuse.png", Cappuccino::TextureType::DiffuseMap);
	auto botSpecular = LOAD_TEXTURE("Bot-Diffuse1.png", "Bot/Bot-Diffuse.png", Cappuccino::TextureType::SpecularMap);
	auto botEmission = LOAD_TEXTURE("Bot-Emission.png", "Bot/Bot-Emission.png", Cappuccino::TextureType::EmissionMap);
	auto botNormal = LOAD_TEXTURE("Bot-Normal.png", "Bot/Bot-Normal.png", Cappuccino::TextureType::NormalMap);

	auto gruntMesh = LOAD_MESH("Grunt", "Grunt.obj");
	gruntMesh->loadMesh();
	auto gruntDiffuse = LOAD_TEXTURE("Grunt-Diffuse",	"Grunt/Grunt_Low_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::DiffuseMap);
	auto gruntSpecular = LOAD_TEXTURE("Grunt-Diffuse1", "Grunt/Grunt_Low_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::SpecularMap);
	auto gruntEmissive = LOAD_TEXTURE("Grunt-Emissive", "Grunt/Grunt_Low_DefaultMaterial_Emissive.png", Cappuccino::TextureType::EmissionMap);
	auto gruntNormal = LOAD_TEXTURE("Grunt-Normal",		"Grunt/Grunt_Low_DefaultMaterial_Normal.png", Cappuccino::TextureType::NormalMap);

	auto sentryMesh = LOAD_MESH("Sentry", "Sentry.obj");
	auto sentryDiffuse = LOAD_TEXTURE("Sentry Diffuse", "Sentry/Sentry-Diffuse.png",Cappuccino::TextureType::DiffuseMap);
	auto sentrySpecular = LOAD_TEXTURE("Sentry Diffuse", "Sentry/Sentry-Diffuse.png",Cappuccino::TextureType::SpecularMap);
	auto sentryEmissive = LOAD_TEXTURE("Sentry Emissive", "Sentry/Sentry-Emission.png",Cappuccino::TextureType::EmissionMap);
	auto sentryNormal = LOAD_TEXTURE("Sentry Normal", "Sentry/Sentry-Normal.png", Cappuccino::TextureType::NormalMap);

	_primordial = new Primordial(&_pLight._pointLightShader, { red, spec }, { Cappuccino::MeshLibrary::loadMesh("Squelch", "Squelch.obj") });



	for (unsigned i = 0; i < 30; i++) {
		_pLight.getPositions().emplace_back(0, -100, 0);
	}
	_pLight.resendLights();

	for (unsigned i = 0; i < 10; i++)
		_levelManager._enemyManager._enemies.push_back(new Sentry(&_pLight._pointLightShader, { sentryDiffuse,sentrySpecular,sentryEmissive,sentryNormal }, { sentryMesh }, 1.0f));

	for (unsigned i = 0; i < 10; i++)
		_levelManager._enemyManager._enemies.push_back(new Ghoul(&_pLight._pointLightShader, {
		LOAD_TEXTURE("Crawler diffuse",  "Crawler/CrawlerDiffuse.png", Cappuccino::TextureType::DiffuseMap),
		LOAD_TEXTURE("Crawler specular", "Crawler/CrawlerDiffuse.png", Cappuccino::TextureType::SpecularMap),
		LOAD_TEXTURE("Crawler normal",   "Crawler/CrawlerNorm.png",    Cappuccino::TextureType::NormalMap)
			}, {
				LOAD_MESH("Crawler", "Crawler.obj")
			}, 1.0f));

	for (unsigned i = 0; i < 10; i++)
		_levelManager._enemyManager._enemies.push_back(new RoboGunner(&_pLight._pointLightShader, { botDiffuse,botSpecular,botEmission,botNormal }, { botMesh }));

		//_primordial->setBabies(_levelManager._enemyManager._enemies.back());
	for (unsigned i = 0; i < 10; i++)
		_levelManager._enemyManager._enemies.push_back(new Captain(&_pLight._pointLightShader, { botDiffuse,botSpecular,botEmission,botNormal }, { botMesh }));

	for (unsigned i = 0; i < 10; i++)
		_levelManager._enemyManager._enemies.push_back(new Grunt(&_pLight._pointLightShader, { gruntDiffuse,gruntSpecular,gruntEmissive,gruntNormal}, { gruntMesh }));

	auto squelchMesh = LOAD_MESH("Squelch", "Squelch.obj");
	squelchMesh->loadMesh();

	for (unsigned i = 0; i < 10; i++)
		_levelManager._enemyManager._enemies.push_back(new Squelch(&_pLight._pointLightShader, { LOAD_TEXTURE("Squelch Diff","Squelch/Squelch-Diffuse.png",Cappuccino::TextureType::DiffuseMap),
			LOAD_TEXTURE("Squelch Diff","Squelch/Squelch-Diffuse.png",Cappuccino::TextureType::SpecularMap),LOAD_TEXTURE("Squelch Diff","Squelch/Squelch-Normal.png",Cappuccino::TextureType::NormalMap)
			}, { squelchMesh }));

		//_primordial->setBabies(*_levelManager._enemyManager._enemies.back());
	resetObjects();

	//init members here
	auto mesh = LOAD_MESH("Bullet", "Bullet.obj");
	mesh->loadMesh();

	bullet = new Bullet(_pLight._pointLightShader, { matte, spec }, { mesh }, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2 = new Bullet(_pLight._pointLightShader, { matte, spec }, { mesh }, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2->_transform.scale(glm::vec3(1.0f), 0.1f);

	for (unsigned i = 0; i < _levelManager._enemyManager._enemies.size(); i++)
		if (_levelManager._enemyManager._enemies[i]->_enemyType != "Ghoul" && _levelManager._enemyManager._enemies[i]->_enemyType != "Squelch")
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
		createdPlayer = true;

		_levelManager._testShopTerminal->_player = _testCommando;

	}

	//activate members here
	_initialized = true;
	_shouldExit = false;
	_testCommando->setActive(true);
	for (auto x : _loot)
		x->setActive(true);
	for (auto x : lamps)
		x->setActive(true);

	_levelManager._testShopTerminal->setActive(false);
	_chest->setActive(true);

	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	return true;
}

bool GameplayScene::exit()
{
	//deactivate members here
	_initialized = false;
	_shouldExit = true;
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

	_levelManager._testShopTerminal->setActive(false);

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
	//update level manager and shader
	_levelManager.update(dt, _testCommando->_rigidBody);
	_pLight._pointLightShader.use();
	_pLight._pointLightShader.loadViewMatrix(*_testCommando->getCamera());


	///REMOVE AFTER TESTING
	{
		//add light button
		static bool pressed = false;
		if (_testCommando->_input.keyboard->keyPressed(Cappuccino::KeyEvent::L) && !pressed) {
			pressed = true;
			_pLight.getPositions().push_back(glm::vec3(_testCommando->_rigidBody._position.x, _testCommando->_rigidBody._position.y, _testCommando->_rigidBody._position.z + 5.0f));
			_pLight.resendLights();
		}
		else if (!_testCommando->_input.keyboard->keyPressed(Cappuccino::KeyEvent::L))
			pressed = false;
	}

	///REMOVE AFTER TESTING
	{
		//add hitbox render toggle
		static bool pressed = false;
		if (_testCommando->_input.keyboard->keyPressed(Cappuccino::KeyEvent::H) && !pressed) {
			pressed = true;
			Cappuccino::RigidBody::drawHitBox ^= 1;
		}
		else if (!_testCommando->_input.keyboard->keyPressed(Cappuccino::KeyEvent::H))
			pressed = false;

	}





	_testCommando->getUILight().getActives() = _pLight.getActives();
	_testCommando->getUILight().getPositions() = _pLight.getPositions();
	_testCommando->getUILight().setPlayerPosition(_testCommando->_rigidBody._position);
	_testCommando->getUILight().resendLights();


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

	for (auto x : _levelManager._enemyManager._enemies)
		if (x->intersecting(_testCommando->_testRay) && x->isActive())
		{
			x->_rigidBody._shaderColour = glm::vec4(0, 1, 0, 1);
			x->getHUD()->toggleHud(true);
		}
		else
		{
			x->getHUD()->toggleHud(false);
		}
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

	const GLfloat xOffset = xpos - lastX;
	const GLfloat yOffset = lastY - ypos;
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
		_testCommando->_rigidBody._position = _levelManager.rooms[_levelManager._currentRoom]->_levelData._respawnPoint + _levelManager.rooms[_levelManager._currentRoom]->_rigidBody._position;
		_testCommando->_rigidBody._position.y += 2;
		_testCommando->setHealth(_testCommando->getMaxHp());
		_testCommando->setShield(_testCommando->getMaxShield());
		_testCommando->_rigidBody._vel = glm::vec3(0.0f);
		_testCommando->_rigidBody._accel = glm::vec3(0.0f);
	}

	for (auto& x : _levelManager._enemyManager._enemies)
	{
		x->setHealth(x->getMaxHP());
		x->setShield(x->getMaxShield());
	}
}
