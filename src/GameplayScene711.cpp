#include "GameplayScene711.h"
#include "Options.h"
#include "Cappuccino/FrameBuffer.h"

#include <Cappuccino/ResourceManager.h>
#include <ctime>

//whew
#define LOAD_TEXTURE Cappuccino::TextureLibrary::loadTexture
#define LOAD_MESH Cappuccino::MeshLibrary::loadMesh
 
Cappuccino::Shader* GameplayScene::_mainShader = nullptr;
Class* GameplayScene::_testCommando = nullptr;
std::vector<Cappuccino::PointLight> GameplayScene::_lights = {};
GameplayScene::GameplayScene(const bool isActive) :
	Scene(isActive),
	cursorBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f)),
	_levelManager(_lights) {
	
	_mainShader = new Cappuccino::Shader{ std::string("PBRshader"), "PBR.vert","PBR.frag" };
	Cappuccino::Application::_lightingPassShader = _mainShader;

	_levelManager._testShopTerminal = new ShopTerminal(*_mainShader, {
		LOAD_TEXTURE("Shop terminal diffuse", "Shop/Shop Base/shopBase_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::PBRAlbedo),
		LOAD_TEXTURE("Shop terminal diffuses", "Shop/Shop Base/shopBase_low_DefaultMaterial_Metallic.png",Cappuccino::TextureType::PBRMetallic),
		LOAD_TEXTURE("Shop terminal diffusess", "Shop/Shop Base/shopBase_low_DefaultMaterial_Normal.png",Cappuccino::TextureType::PBRNormal),
		LOAD_TEXTURE("Shop terminal diffusesss", "Shop/Shop Base/shopBase_low_DefaultMaterial_Emissive.png",Cappuccino::TextureType::PBREmission),
		LOAD_TEXTURE("Shop terminal Roughness", "Shop/Shop Base/shopBase_low_DefaultMaterial_Roughness.png",Cappuccino::TextureType::PBRRoughness),


		LOAD_TEXTURE("Shop terminal diffusee", "Shop/Large Ring/shopBigRing_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::PBRAlbedo,1),
		LOAD_TEXTURE("Shop terminal diffuseee", "Shop/Large Ring/shopBigRing_low_DefaultMaterial_Metallic.png",Cappuccino::TextureType::PBRMetallic,1),
		LOAD_TEXTURE("Shop terminal diffuseeee", "Shop/Large Ring/shopBigRing_low_DefaultMaterial_Normal.png",Cappuccino::TextureType::PBRNormal,1),
		LOAD_TEXTURE("Shop terminal diffuseeeee", "Shop/Large Ring/shopBigRing_low_DefaultMaterial_Emissive.png",Cappuccino::TextureType::PBREmission,1),
		LOAD_TEXTURE("Shop terminal ROOughness", "Shop/Large Ring/shopBigRing_low_DefaultMaterial_Roughness.png",Cappuccino::TextureType::PBRRoughness,1),

		LOAD_TEXTURE("Shop terminal diffusez",	"Shop/Medium Ring/shopMediumRing_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::PBRAlbedo,2),
		LOAD_TEXTURE("Shop terminal diffuseze", "Shop/Medium Ring/shopMediumRing_low_DefaultMaterial_Metallic.png",Cappuccino::TextureType::PBRMetallic,2),
		LOAD_TEXTURE("Shop terminal diffusewd", "Shop/Medium Ring/shopMediumRing_low_DefaultMaterial_Normal.png",Cappuccino::TextureType::PBRNormal,2),
		LOAD_TEXTURE("Shop terminal diffusegfe","Shop/Medium Ring/shopMediumRing_low_DefaultMaterial_Emissive.png",Cappuccino::TextureType::PBREmission,2),
		LOAD_TEXTURE("Shop terminal diffusegferre","Shop/Medium Ring/shopMediumRing_low_DefaultMaterial_Roughness.png",Cappuccino::TextureType::PBRRoughness,2),

		LOAD_TEXTURE("Shop terminal diffushqwe", "Shop/Shop Screen/shopScreen_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::PBRAlbedo,3),
		LOAD_TEXTURE("Shop terminal diffusefqwf", "Shop/Shop Screen/shopScreen_low_DefaultMaterial_Metallic.png",Cappuccino::TextureType::PBRMetallic,3),
		LOAD_TEXTURE("Shop terminal diffusegasg", "Shop/Shop Screen/shopScreen_low_DefaultMaterial_Normal.png",Cappuccino::TextureType::PBRNormal,3),
		LOAD_TEXTURE("Shop terminal diffuseasxzwe", "Shop/Shop Screen/shopScreen_low_DefaultMaterial_Emissive.png",Cappuccino::TextureType::PBREmission,3),
		LOAD_TEXTURE("Shop terminal diffuseasxzewewwe", "Shop/Shop Screen/shopScreen_low_DefaultMaterial_Roughness.png",Cappuccino::TextureType::PBRRoughness,3),

		LOAD_TEXTURE("Shop terminal diffuseasxzweeee",	 "Shop/Small Ring/shopSmallRing_low_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::PBRAlbedo,4),
		LOAD_TEXTURE("Shop terminal diffuseasxzweeeeee", "Shop/Small Ring/shopSmallRing_low_DefaultMaterial_Metallic.png",Cappuccino::TextureType::PBRMetallic,4),
		LOAD_TEXTURE("Shop terminal diffuseasxzwesdads", "Shop/Small Ring/shopSmallRing_low_DefaultMaterial_Normal.png",Cappuccino::TextureType::PBRNormal,4),
		LOAD_TEXTURE("Shop terminal diffuseasxzweqqqwe", "Shop/Small Ring/shopSmallRing_low_DefaultMaterial_Emissive.png",Cappuccino::TextureType::PBREmission,4),
		LOAD_TEXTURE("Shop terminal diffuseasxzweqqqweeee", "Shop/Small Ring/shopSmallRing_low_DefaultMaterial_Roughness.png",Cappuccino::TextureType::PBRRoughness,4),


		}, {
			LOAD_MESH("Shop Base",			"Shop/shopBase_low.obj"),
			LOAD_MESH("Shop Big Ring",		"Shop/shopBigRing_low.obj"),
			LOAD_MESH("Shop Medium Ring",	"Shop/shopMediumRing_low.obj"),
			LOAD_MESH("Shop Screen",		"Shop/shopScreen_low.obj"),
			LOAD_MESH("Shop Small Ring",	"Shop/shopSmallRing_low.obj")
		}, _testCommando, cursorBox);

	_levelManager._testShopTerminal->_rigidBody._position = glm::vec3(-10.0f, 0.0f, 0.0f);


	const auto matte = LOAD_TEXTURE("Level matte", "matte.png", Cappuccino::TextureType::PBRAlbedo);
	const auto diffuse = LOAD_TEXTURE("Level metal", "metal.png", Cappuccino::TextureType::PBRAlbedo);
	const auto spec = LOAD_TEXTURE("Level metal specular", "metal.png", Cappuccino::TextureType::PBRMetallic);
	const auto red = LOAD_TEXTURE("Enemy red diffuse", "red.png", Cappuccino::TextureType::DiffuseMap);

	_sednium = new Sednium(*_mainShader, { red, spec });

	_ammoPack = new AmmoPack(*_mainShader, {
		LOAD_TEXTURE("Ammo pack diffuse",  "ammoPickup/ammoPickup-Diffuse.png",  Cappuccino::TextureType::PBRAlbedo),
		LOAD_TEXTURE("Ammo pack normal",   "ammoPickup/ammoPickup-Normal.png",   Cappuccino::TextureType::PBRNormal),
		LOAD_TEXTURE("Ammo pack Roughness",   "ammoPickup/ammoPickup-Roughness.png",   Cappuccino::TextureType::PBRRoughness),
		LOAD_TEXTURE("Ammo pack emission", "ammoPickup/ammoPickup-Emission.png", Cappuccino::TextureType::PBREmission)
		});

	_healthPack = new HealthPack(*_mainShader, {
		LOAD_TEXTURE("Health pack diffuse",  "healthPickup/healthPickup-Diffuse.png",  Cappuccino::TextureType::PBRAlbedo),
		LOAD_TEXTURE("Health pack specular", "healthPickup/healthPickup-Metallic.png",  Cappuccino::TextureType::PBRMetallic),
		LOAD_TEXTURE("Health pack normal",   "healthPickup/healthPickup-Normal.png",   Cappuccino::TextureType::PBRNormal),
		LOAD_TEXTURE("Health pack Roughness",   "healthPickup/healthPickup-Roughness.png",   Cappuccino::TextureType::PBRRoughness),
		LOAD_TEXTURE("Health pack emission", "healthPickup/healthPickup-Emission.png", Cappuccino::TextureType::PBREmission)
		});

	_bullion = new Bullion(*_mainShader, {
		});

	for (unsigned i = 0; i < 7; i++)
	{
		_levelManager._chests.push_back(new Chest(*_mainShader, {
		LOAD_TEXTURE("Loot chest closed diffuse",  "lootChest/Chest_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::DiffuseMap),
		LOAD_TEXTURE("Loot chest closed specular", "lootChest/Chest_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::SpecularMap),
		LOAD_TEXTURE("Loot chest closed normal",   "lootChest/Chest_DefaultMaterial_Normal.png",    Cappuccino::TextureType::PBRNormal),
		LOAD_TEXTURE("Loot chest closed emission", "lootChest/Chest_DefaultMaterial_Emissive.png",  Cappuccino::TextureType::PBREmission),
		LOAD_TEXTURE("Loot chest closed height",   "lootChest/Chest_DefaultMaterial_Height.png",    Cappuccino::TextureType::HeightMap)
			}));
		_levelManager._chests[i]->setActive(false);
	}
	//_chest = new Chest(_mainShader, {
	//	LOAD_TEXTURE("Loot chest closed diffuse",  "lootChestClosed/lootChestClosed-Diffuse.png", Cappuccino::TextureType::DiffuseMap),
	//	LOAD_TEXTURE("Loot chest closed specular", "lootChestClosed/lootChestClosed-Diffuse.png", Cappuccino::TextureType::SpecularMap),
	//	LOAD_TEXTURE("Loot chest closed normal",   "lootChestClosed/lootChestClosed-Normal.png",    Cappuccino::TextureType::PBRNormal),
	//	LOAD_TEXTURE("Loot chest closed emission", "lootChestClosed/lootChestClosed-Emission.png",  Cappuccino::TextureType::PBREmission),
	//	LOAD_TEXTURE("Loot chest closed height",   "lootChestClosed/lootChestClosed-Height.png",    Cappuccino::TextureType::HeightMap)
	//	});

	//_chest->_rigidBody._position = glm::vec3(10.0f, -2.0f, -8.5f);
	//_chest->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), -90.0f);

	//handle room data here

	auto _levelDiffuse = LOAD_TEXTURE("room1 diffuse", "room/room1_low_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
	auto _levelSpecular = LOAD_TEXTURE("room1 diffuse3", "room/room1_low_DefaultMaterial_Metallic.png", Cappuccino::TextureType::PBRMetallic);
	auto _levelNormal = LOAD_TEXTURE("room1 diffuse2", "room/room1_low_DefaultMaterial_Normal.png", Cappuccino::TextureType::PBRNormal);
	auto _levelRoughness = LOAD_TEXTURE("room1 Rough", "room/room1_low_DefaultMaterial_Roughness.png", Cappuccino::TextureType::PBRRoughness);

	auto _lOcc = LOAD_TEXTURE("lAlb", "RoomVar1/Tileset(for textures)_DefaultMaterial_AO.png",Cappuccino::TextureType::PBRAmbientOcc);
	auto _lAlb = LOAD_TEXTURE("lAlb", "RoomVar1/Tileset(for textures)_DefaultMaterial_BaseColor.png",Cappuccino::TextureType::PBRAlbedo);
	auto _lEmi = LOAD_TEXTURE("lAlb", "RoomVar1/Tileset(for textures)_DefaultMaterial_Emissive.png",Cappuccino::TextureType::PBREmission);
	auto _lMet = LOAD_TEXTURE("lAlb", "RoomVar1/Tileset(for textures)_DefaultMaterial_Metallic.png",Cappuccino::TextureType::PBRMetallic);
	auto _lNor = LOAD_TEXTURE("lAlb", "RoomVar1/Tileset(for textures)_DefaultMaterial_Normal.png",Cappuccino::TextureType::PBRNormal);
	auto _lRou = LOAD_TEXTURE("lAlb", "RoomVar1/Tileset(for textures)_DefaultMaterial_Roughness.png",Cappuccino::TextureType::PBRRoughness);
	_levelManager._rooms.push_back(new Building("./Assets/LevelData/Room1LevelData.obj", "./Assets/SpawnData/Room1SpawnData.obj", "./Assets/Meshes/Hitboxes/Room1HitboxData.obj", _mainShader, { _levelDiffuse,_levelSpecular,_levelNormal,_levelRoughness }, { LOAD_MESH("Room 1", "Room1/Room1_Low.obj") }));
	_levelManager._rooms.push_back(new Building("./Assets/LevelData/Room2LevelData.obj", "./Assets/SpawnData/Room2SpawnData.obj", "./Assets/Meshes/Hitboxes/Room2HitboxData.obj", _mainShader, { _levelDiffuse,_levelSpecular,_levelNormal,_levelRoughness }, { LOAD_MESH("Room 2", "Room2/Room2_Low.obj") }));
	_levelManager._rooms.push_back(new Building("./Assets/LevelData/Room3LevelData.obj", "./Assets/SpawnData/Room3SpawnData.obj", "./Assets/Meshes/Hitboxes/Room3HitboxData.obj", _mainShader, { diffuse,_levelSpecular }, { LOAD_MESH("Room 3", "Room3/Room3_low.obj") }));
	_levelManager._rooms.push_back(new Building("./Assets/LevelData/Room4LevelData.obj", "./Assets/SpawnData/Room4SpawnData.obj", "./Assets/Meshes/Hitboxes/Room4HitboxData.obj", _mainShader, { _lAlb,_lMet,_lRou,_lOcc,_lEmi,_lNor }, { LOAD_MESH("Room 4", "Room4/Room4_low.obj") }));
	for (unsigned i = 0; i < 7; i++)
		_levelManager.airlocks.push_back(new Building("./Assets/LevelData/AirLockLevelData.obj", "./Assets/SpawnData/AirLockSpawnData.obj", "./Assets/Meshes/Hitboxes/AirlockHitboxData.obj", _mainShader, { matte, spec }, { LOAD_MESH("Airlock", "Airlock.obj") }));

	auto botMesh = LOAD_MESH("Bot", "Bot.obj");
	botMesh->loadMesh();
	auto botDiffuse = LOAD_TEXTURE("Bot-Diffuse.png", "Bot/Bot-Diffuse.png", Cappuccino::TextureType::PBRAlbedo);
	auto botMetallic = LOAD_TEXTURE("Bot-Metallic.png", "Bot/Bot-Metallic.png", Cappuccino::TextureType::PBRMetallic);
	auto botEmission = LOAD_TEXTURE("Bot-Emission.png", "Bot/Bot-Emission.png", Cappuccino::TextureType::PBREmission);
	auto botNormal = LOAD_TEXTURE("Bot-Normal.png", "Bot/Bot-Normal.png", Cappuccino::TextureType::PBRNormal);
	auto botRoughness = LOAD_TEXTURE("Bot-Roughness.png", "Bot/Bot-Roughness.png", Cappuccino::TextureType::PBRRoughness);
	auto botAO = LOAD_TEXTURE("Bot-AO.png", "Bot/Bot-AO.png", Cappuccino::TextureType::PBRAmbientOcc);

	auto gruntMesh = LOAD_MESH("Grunt", "Grunt.obj");
	gruntMesh->loadMesh();
	auto gruntDiffuse = LOAD_TEXTURE("Grunt-Diffuse", "Grunt/Grunt_Low_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
	auto gruntMetallic = LOAD_TEXTURE("Grunt-Diffuse1", "Grunt/Grunt_Low_DefaultMaterial_Metallic.png", Cappuccino::TextureType::PBRMetallic);
	auto gruntEmissive = LOAD_TEXTURE("Grunt-Emissive", "Grunt/Grunt_Low_DefaultMaterial_Emissive.png", Cappuccino::TextureType::PBREmission);
	auto gruntNormal = LOAD_TEXTURE("Grunt-Normal", "Grunt/Grunt_Low_DefaultMaterial_Normal.png", Cappuccino::TextureType::PBRNormal);
	auto gruntRoughness = LOAD_TEXTURE("Grunt-Roughness", "Grunt/Grunt_Low_DefaultMaterial_Roughness.png", Cappuccino::TextureType::PBRRoughness);
	auto gruntAO = LOAD_TEXTURE("Grunt-AO", "Grunt/Grunt_Low_DefaultMaterial_AO.png", Cappuccino::TextureType::PBRAmbientOcc);

	auto sentryMesh = LOAD_MESH("Sentry", "Sentry.obj");
	sentryMesh->loadMesh();
	auto sentryDiffuse = LOAD_TEXTURE("Sentry Diffuse", "Sentry/Sentry-Diffuse.png", Cappuccino::TextureType::PBRAlbedo);
	auto sentryMetallic = LOAD_TEXTURE("Sentry Metallic", "Sentry/Sentry-Metallic.png", Cappuccino::TextureType::PBRMetallic);
	auto sentryEmissive = LOAD_TEXTURE("Sentry Emissive", "Sentry/Sentry-Emission.png", Cappuccino::TextureType::PBREmission);
	auto sentryNormal = LOAD_TEXTURE("Sentry Normal", "Sentry/Sentry-Normal.png", Cappuccino::TextureType::PBRNormal);
	auto sentryRoughness = LOAD_TEXTURE("Sentry Roughness", "Sentry/Sentry-Roughness.png", Cappuccino::TextureType::PBRRoughness);

	auto squelchMesh = LOAD_MESH("Squelch", "Squelch.obj");
	squelchMesh->loadMesh();
	auto squelchDiffuse = LOAD_TEXTURE("Squelch Diff", "Squelch/Squelch-Diffuse.png", Cappuccino::TextureType::PBRAlbedo);
	auto squelchNorm = LOAD_TEXTURE("Squelch Norm", "Squelch/Squelch-Norm.png", Cappuccino::TextureType::PBRNormal);
	auto squelchRoughness = LOAD_TEXTURE("Squelch Roughness", "Squelch/Squelch-Roughness.png", Cappuccino::TextureType::PBRRoughness);
	auto squelchAO = LOAD_TEXTURE("Squelch AO", "Squelch/Squelch-AO.png", Cappuccino::TextureType::PBRAmbientOcc);

	_primordial = new Primordial(_mainShader, { red, spec }, { Cappuccino::MeshLibrary::loadMesh("Squelch", "Squelch.obj") });

	auto crawlerMesh = LOAD_MESH("Crawler", "Crawler.obj");
	crawlerMesh->loadMesh();
	auto crawlerDiffuse = LOAD_TEXTURE("Crawler diffuse", "Crawler/CrawlerDiffuse.png", Cappuccino::TextureType::PBRAlbedo);
	auto crawlerRoughness = LOAD_TEXTURE("Crawler Roughness", "Crawler/Crawler-Roughness.png", Cappuccino::TextureType::PBRRoughness);
	auto crawlerAO = LOAD_TEXTURE("Crawler AO", "Crawler/Crawler-AO.png", Cappuccino::TextureType::PBRAmbientOcc);
	auto crawlerNorm = LOAD_TEXTURE("Crawler normal", "Crawler/CrawlerNorm.png", Cappuccino::TextureType::PBRNormal);

	for (unsigned i = 0; i < 30; i++) {
		_lights.push_back({ glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) });
	}
	resendLights();


	for (unsigned i = 0; i < 10; i++) {
		_levelManager._enemyManager._enemies.push_back(new Sentry(_mainShader, { sentryDiffuse,sentryMetallic,sentryEmissive,sentryNormal,sentryRoughness }, { sentryMesh }, 1.0f));
	}

	for (unsigned i = 0; i < 10; i++) {

		_levelManager._enemyManager._enemies.push_back(new Ghoul(_mainShader, {
			crawlerDiffuse,
			crawlerRoughness,
			crawlerAO,
			crawlerNorm,
			}, {
				crawlerMesh
			}, 1.0f));
	}

	for (unsigned i = 0; i < 10; i++) {
		_levelManager._enemyManager._enemies.push_back(new RoboGunner(_mainShader, { botDiffuse,botMetallic,botEmission,botNormal,botAO,botRoughness }, { botMesh }));
	}

	//_primordial->setBabies(_levelManager._enemyManager._enemies.back());
	for (unsigned i = 0; i < 10; i++) {
		_levelManager._enemyManager._enemies.push_back(new Captain(_mainShader, { botDiffuse,botMetallic,botEmission,botNormal }, { botMesh }));
	}

	for (unsigned i = 0; i < 10; i++) {
		_levelManager._enemyManager._enemies.push_back(new Grunt(_mainShader, { gruntDiffuse,gruntMetallic,gruntEmissive,gruntNormal,gruntAO,gruntRoughness }, { gruntMesh }));
	}

	for (unsigned i = 0; i < 10; i++) {
		_levelManager._enemyManager._enemies.push_back(new Squelch(_mainShader, { squelchDiffuse,squelchNorm,squelchRoughness,squelchAO }, { squelchMesh }));
	}

	//_primordial->setBabies(*_levelManager._enemyManager._enemies.back());
	resetObjects();

	//init members here
	auto mesh = LOAD_MESH("Bullet", "Bullet.obj");
	mesh->loadMesh();

	bullet = new Bullet(*_mainShader, { matte, spec }, { mesh }, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2 = new Bullet(*_mainShader, { matte, spec }, { mesh }, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2->_transform.scale(glm::vec3(1.0f), 0.1f);

	for (unsigned i = 0; i < _levelManager._enemyManager._enemies.size(); i++)
		if (_levelManager._enemyManager._enemies[i]->_enemyType != "Ghoul" && _levelManager._enemyManager._enemies[i]->_enemyType != "Squelch")
			_levelManager._enemyManager._enemies[i]->getGun()->addBullets(bullet);


	for (unsigned i = 0; i < _lights.size(); i++) {
		lamps.push_back(new Billboard(_mainShader, { matte }));
		lamps.back()->_rigidBody._position = _lights[i]._pos;
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
			_testCommando = new Assault(_mainShader, {}, {});
		else if (Options::Commando)
			_testCommando = new Commando(_mainShader, {}, {});
		else if (Options::Demolitionist)
			_testCommando = new Demolitionist(_mainShader, {}, {});
		else if (Options::Scout)
			_testCommando = new Scout(_mainShader, {}, {});

		bullet->_transform.scale(glm::vec3(1.0f), 0.1f);
		_testCommando->addAmmo(bullet, bullet2);

		Class::_uiLights.clear();
		for (unsigned i = 0; i < _lights.size(); i++)
			Class::_uiLights.push_back(_lights[i]);

		Class::resendLights();
		_testCommando->_rigidBody._position = glm::vec3(-30.0f, 0.0f, -5.0f) + _levelManager.airlocks[0]->_levelData._respawnPoint;
		//Class::_uiLightShader->setUniform("PlayerPosition", _testCommando->_rigidBody._position);
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

	//_chest->setActive(true);

	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);



	return true;
}

bool GameplayScene::exit()
{
	//deactivate members here
	_initialized = false;
	_shouldExit = true;
	_testCommando->setActive(false);
	for (auto& room : _levelManager._rooms)
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
	if (_testCommando->getGun()->getDamage() != 0.0f)
		enemy->hurt(_testCommando->getGun()->getDamage());
	_testCommando->getGun()->specialCollisionBehaviour(_levelManager._enemyManager._enemies);

	//special behaviour if the enemy dies
	if (enemy->dead())
	{
		if (!_testCommando->_voiceLines->isEventPlaying((int)voiceLine::KillingEnemy) &&
			Cappuccino::randomInt(0, 10) <= 4)
			_testCommando->_voiceLines->playEvent((int)voiceLine::KillingEnemy);
		_loot.push_back(_sednium->spawn(enemy->getWeight(), enemy->_rigidBody._position));
		_loot.push_back(_healthPack->spawn(enemy->getWeight(), enemy->_rigidBody._position));
		_loot.push_back(_ammoPack->spawn(enemy->getWeight(), enemy->_rigidBody._position));
	}
}

void GameplayScene::resendLights()
{
	static bool first = true;
	_mainShader->use();
	if (first) {
		first = false;
		_mainShader->setUniform("gBuffer.gPos", 0);
		_mainShader->setUniform("gBuffer.gNormal", 1);
		_mainShader->setUniform("gBuffer.gAlbedo", 2);
		_mainShader->setUniform("gBuffer.gMetalRoughnessAO", 3);
		_mainShader->setUniform("gBuffer.gEmissive", 4);

	}
	_mainShader->setUniform("numLights", (int)_lights.size());
	for (unsigned i = 0; i < _lights.size(); i++) {
		_mainShader->setUniform("lights[" + std::to_string(i) + "].position", _lights[i]._pos);
		_mainShader->setUniform("lights[" + std::to_string(i) + "].colour", _lights[i]._col);
		//_mainShader->setUniform("lights[" + std::to_string(i) + "].active", _lights[i]._isActive);

	}

}

void GameplayScene::sendGBufferShaderUniforms()
{
	using namespace Cappuccino;
	Application::_gBufferShader->use();
	static bool first = true;

	if (first) {
		first = false;
		Application::_gBufferShader->loadProjectionMatrix(1600.0f, 1000.0f);
		Application::_gBufferShader->setUniform("material.albedo", (int)Cappuccino::TextureType::PBRAlbedo);
		Application::_gBufferShader->setUniform("material.normalMap", (int)Cappuccino::TextureType::PBRNormal);
		Application::_gBufferShader->setUniform("material.metallic", (int)Cappuccino::TextureType::PBRMetallic);
		Application::_gBufferShader->setUniform("material.roughness", (int)Cappuccino::TextureType::PBRRoughness);
		Application::_gBufferShader->setUniform("material.ambientOcc", (int)Cappuccino::TextureType::PBRAmbientOcc);
		Application::_gBufferShader->setUniform("material.emission", (int)Cappuccino::TextureType::PBREmission);
	}
}

void GameplayScene::childUpdate(float dt)
{
	//update level manager and shader
	_levelManager.update(dt, _testCommando);
	_mainShader->use();
	_mainShader->setUniform("camPos", _testCommando->getCamera()->getPosition());
	using namespace Cappuccino;
	Application::_gBufferShader->use();
	Application::_gBufferShader->loadViewMatrix(*_testCommando->getCamera());
	sendGBufferShaderUniforms();


	///REMOVE AFTER TESTING
	{
		//add light button
		static bool pressed = false;
		if (_testCommando->_input.keyboard->keyPressed(Cappuccino::KeyEvent::L) && !pressed) {
			pressed = true;
			_lights.push_back({ glm::vec3(_testCommando->_rigidBody._position.x, _testCommando->_rigidBody._position.y, _testCommando->_rigidBody._position.z + 5.0f),glm::vec3(1.0f,1.0f,1.0f) });
			resendLights();
		}
		else if (!_testCommando->_input.keyboard->keyPressed(Cappuccino::KeyEvent::L))
			pressed = false;
	}


	//enemy logic
	Cappuccino::GameObject* hitObject = _testCommando->getFirstIntersect(_testCommando->_testRay);//first object hit

	for (auto& enemy : _levelManager._enemyManager._enemies) {
		if (!enemy->isActive())
			continue;
		Cappuccino::Ray enemyRay(glm::normalize(_testCommando->_rigidBody._position - enemy->_rigidBody._position), enemy->_rigidBody._position);
		Cappuccino::GameObject* enemyRayObject = enemy->getFirstIntersect(enemyRay);
		//activate enemy if within a trigger volume
		if ((_testCommando->checkCollision(enemy->triggerVolume, enemy->_rigidBody._position) || enemy->getMaxHP() != enemy->getHP() || enemy->getShield() != enemy->getShield())&&enemyRayObject==_testCommando)
			enemy->setTrigger(true);

		{
			static float delay = 0.0f;
				//checks for squelch 
			if (enemy->dead()
				&& !_testCommando->_voiceLines->isEventPlaying((int)voiceLine::KillingEnemy)
				&& delay < 0.0f) {
					_testCommando->_voiceLines->playEvent((int)voiceLine::KillingEnemy);
					delay = Cappuccino::randomFloat(5.0f, 10.0f);
				}
			delay -= dt;
		}
		//bullet collision
		if (!_testCommando->getGun()->isHitscan()) {

			//loop through the player's bullets
			for (auto playerBullets : _testCommando->getGun()->getBullets()) {
				//check if the bullet touches an enemy
				if (playerBullets->_rigidBody.checkCollision(enemy->_rigidBody) && playerBullets->isActive()) {
					shootCollisionBehaviour(enemy);
					playerBullets->setActive(false);
				}
			}
		}
		else {
			if (enemy == hitObject) {
				shootCollisionBehaviour(enemy);
			}
		}
		enemy->attack(_testCommando, dt);

		for (auto bullet : enemy->getGun()->getBullets()) {
			if (bullet->checkCollision(_testCommando) && bullet->isActive()) {
				_testCommando->takeDamage(enemy->getGun()->getDamage());
				bullet->setActive(false);
			}
		}
	}

	for (auto y : _levelManager._chests)
		if (_testCommando->checkCollision(y->_triggerVolume, y->_rigidBody._position) && _testCommando->_input.keyboard->keyPressed('E') && !y->open())
		{
			std::vector<Loot*> _temp = y->spawn(10.0f, y->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f), _sednium, _healthPack, _ammoPack, _bullion);
			for (auto x : _temp)
			{
				_loot.push_back(x);
			}
		}
	//loot chest interaction, this should probably be a function inside the chest class
	//if (_testCommando->checkCollision(_chest->_triggerVolume, _chest->_rigidBody._position) && _testCommando->_input.keyboard->keyPressed('E') && !_chest->open())
	//{
	//	std::vector<Loot*> _temp = _chest->spawn(10.0f, _chest->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f), _sednium, _healthPack, _ammoPack, _bullion);
	//	for (auto x : _temp)
	//	{
	//		_loot.push_back(x);
	//	}
	//}

	for (auto& x : _loot) {
		if (x->isActive())
		{
			x->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), dt * 20.0f);
			x->pickup(_testCommando);
		}
	}

	if (_testCommando->getHealth() <= 0) {
		_testCommando->_voiceLines->playEvent((int)voiceLine::GettingKilled);
		resetObjects();
	}


	//deal with shop interface
	cursorBox._position = glm::vec3(cursorPos.x, cursorPos.y, 0.0f);


	const glm::mat4 view = glm::mat4(glm::mat3(_testCommando->getCamera()->whereAreWeLooking()));
	_skybox->getShader().use();
	_skybox->getShader().setUniform("view", view);

	//Cappuccino::GameObject* hitObject = _testCommando->getFirstIntersect(_testCommando->_testRay);//first object hit

	bool spotted = false;
	for (auto y : _testCommando->gameObjects) {//for all gameobjects

		if (y->id == "Enemy") {//if the object is an enemy
			if (y->isActive() && y == hitObject) {
				static_cast<Enemy*>(y)->getHUD()->toggleHud(true);//toggle the hud
				spotted = true;
			}
			else {
				static_cast<Enemy*>(y)->getHUD()->toggleHud(false);
			}
		}
	}
	{
		static float delay = 0.0f;

		if (spotted && !_testCommando->_voiceLines->isEventPlaying((int)voiceLine::SeeingEnemy)
			&& delay < 0.0f) {
			delay = Cappuccino::randomFloat(5.0f, 10.0f);
			_testCommando->_voiceLines->playEvent((int)voiceLine::SeeingEnemy);
		}
		delay -= dt;
	}



	//for (auto x : _levelManager._enemyManager._enemies)
	//	if (x->intersecting(_testCommando->_testRay) && x->isActive())
	//	{
	//		x->_rigidBody._shaderColour = glm::vec4(0, 1, 0, 1);
	//		x->getHUD()->toggleHud(true);
	//	}
	//	else
	//	{
	//		x->getHUD()->toggleHud(false);
	//	}


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
		_testCommando->_rigidBody._position = _levelManager._rooms[_levelManager._currentRoom]->_levelData._respawnPoint + _levelManager._rooms[_levelManager._currentRoom]->_rigidBody._position;
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
