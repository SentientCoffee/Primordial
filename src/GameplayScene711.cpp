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
std::vector<Cappuccino::PointLight> GameplayScene::_lights;

GameplayScene::GameplayScene(const bool isActive) :
	Scene(isActive),
	cursorBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f)),
	_levelManager(_lights) {

	_mainShader = new Cappuccino::Shader(std::string("PBR Shader"), "PBR.vert", "PBR.frag");
	Cappuccino::Application::_lightingPassShader = _mainShader;

	_levelManager._testShopTerminal = new ShopTerminal(*_mainShader, {
		LOAD_TEXTURE("Shop terminal diffuse", "SCTD/SCTD_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::PBRAlbedo),
		LOAD_TEXTURE("Shop terminal AO", "SCTD/SCTD_DefaultMaterial_AO.png", Cappuccino::TextureType::PBRAmbientOcc),
		LOAD_TEXTURE("Shop terminal emissive", "SCTD/SCTD_DefaultMaterial_Emissive.png", Cappuccino::TextureType::PBREmission),
		LOAD_TEXTURE("Shop terminal metallic", "SCTD/SCTD_DefaultMaterial_Metallic.png", Cappuccino::TextureType::PBRMetallic),
		LOAD_TEXTURE("Shop terminal normal", "SCTD/SCTD_DefaultMaterial_Normal.png", Cappuccino::TextureType::PBRNormal),
		LOAD_TEXTURE("Shop terminal roughness", "SCTD/SCTD_DefaultMaterial_Roughness.png", Cappuccino::TextureType::PBRRoughness),
	}, {
		//LOAD_MESH("Shop Base",			"Shop.obj")
		//LOAD_MESH("Shop Big Ring",		"Shop/shopBigRing_low.obj"),
		//LOAD_MESH("Shop Medium Ring",	"Shop/shopMediumRing_low.obj"),
		//LOAD_MESH("Shop Screen",		"Shop/shopScreen_low.obj"),
		//LOAD_MESH("Shop Small Ring",	"Shop/shopSmallRing_low.obj")
	}, _testCommando, cursorBox);

	_levelManager._testShopTerminal->_rigidBody._position = glm::vec3(-10.0f, 0.0f, 0.0f);


	const auto matte = LOAD_TEXTURE("Level matte", "matte.png", Cappuccino::TextureType::PBRAlbedo);
	//const auto diffuse = LOAD_TEXTURE("Level metal", "metal.png", Cappuccino::TextureType::PBRAlbedo);
	const auto spec  = LOAD_TEXTURE("Level metal specular", "metal.png", Cappuccino::TextureType::PBRMetallic);
	const auto red   = LOAD_TEXTURE("Enemy red diffuse", "red.png", Cappuccino::TextureType::DiffuseMap);

	_sednium = new Sednium(*_mainShader, { red, spec });

	_ammoPack = new AmmoPack(*_mainShader, {
		LOAD_TEXTURE("Ammo pack diffuse", "ammoPickup/ammoPickup-Diffuse.png", Cappuccino::TextureType::PBRAlbedo),
		LOAD_TEXTURE("Ammo pack normal", "ammoPickup/ammoPickup-Normal.png", Cappuccino::TextureType::PBRNormal),
		LOAD_TEXTURE("Ammo pack Roughness", "ammoPickup/ammoPickup-Roughness.png", Cappuccino::TextureType::PBRRoughness),
		LOAD_TEXTURE("Ammo pack emission", "ammoPickup/ammoPickup-Emission.png", Cappuccino::TextureType::PBREmission)
	});

	_healthPack = new HealthPack(*_mainShader, {
		LOAD_TEXTURE("Health pack diffuse", "healthPickup/healthPickup-Diffuse.png", Cappuccino::TextureType::PBRAlbedo),
		LOAD_TEXTURE("Health pack specular", "healthPickup/healthPickup-Metallic.png", Cappuccino::TextureType::PBRMetallic),
		LOAD_TEXTURE("Health pack normal", "healthPickup/healthPickup-Normal.png", Cappuccino::TextureType::PBRNormal),
		LOAD_TEXTURE("Health pack Roughness", "healthPickup/healthPickup-Roughness.png", Cappuccino::TextureType::PBRRoughness),
		LOAD_TEXTURE("Health pack emission", "healthPickup/healthPickup-Emission.png", Cappuccino::TextureType::PBREmission)
	});

	_bullion = new Bullion(*_mainShader, {
	});

	for(unsigned i = 0; i < 7; i++) {
		_levelManager._chests.push_back(new Chest(*_mainShader, {
			LOAD_TEXTURE("Loot chest closed diffuse", "lootChest/Chest_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::DiffuseMap),
			LOAD_TEXTURE("Loot chest closed specular", "lootChest/Chest_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::SpecularMap),
			LOAD_TEXTURE("Loot chest closed normal", "lootChest/Chest_DefaultMaterial_Normal.png", Cappuccino::TextureType::PBRNormal),
			LOAD_TEXTURE("Loot chest closed emission", "lootChest/Chest_DefaultMaterial_Emissive.png", Cappuccino::TextureType::PBREmission),
			LOAD_TEXTURE("Loot chest closed height", "lootChest/Chest_DefaultMaterial_Height.png", Cappuccino::TextureType::HeightMap)
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

	auto _levelDiffuse   = LOAD_TEXTURE("Room1 diffuse", "room/room1_low_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
	auto _levelSpecular  = LOAD_TEXTURE("Room1 metallic", "room/room1_low_DefaultMaterial_Metallic.png", Cappuccino::TextureType::PBRMetallic);
	auto _levelNormal    = LOAD_TEXTURE("Room1 normal", "room/room1_low_DefaultMaterial_Normal.png", Cappuccino::TextureType::PBRNormal);
	auto _levelRoughness = LOAD_TEXTURE("Room1 roughness", "room/room1_low_DefaultMaterial_Roughness.png", Cappuccino::TextureType::PBRRoughness);

	auto _lOcc = LOAD_TEXTURE("Room 1 var AO", "RoomVar1/RoomVar1_DefaultMaterial_AO.png", Cappuccino::TextureType::PBRAmbientOcc);
	auto _lAlb = LOAD_TEXTURE("Room 1 var diffuse", "RoomVar1/RoomVar1_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
	auto _lEmi = LOAD_TEXTURE("Room 1 var emissive", "RoomVar1/RoomVar1_DefaultMaterial_Emissive.png", Cappuccino::TextureType::PBREmission);
	auto _lMet = LOAD_TEXTURE("Room 1 var metallic", "RoomVar1/RoomVar1_DefaultMaterial_Metallic.png", Cappuccino::TextureType::PBRMetallic);
	auto _lNor = LOAD_TEXTURE("Room 1 var normal", "RoomVar1/RoomVar1_DefaultMaterial_Normal.png", Cappuccino::TextureType::PBRNormal);
	auto _lRou = LOAD_TEXTURE("Room 1 var roughness", "RoomVar1/RoomVar1_DefaultMaterial_Roughness.png", Cappuccino::TextureType::PBRRoughness);

	auto _lOcc2 = LOAD_TEXTURE("Room 2 var AO", "RoomVar2/RoomVar2_DefaultMaterial_AO.png", Cappuccino::TextureType::PBRAmbientOcc);
	auto _lAlb2 = LOAD_TEXTURE("Room 2 var diffuse", "RoomVar2/RoomVar2_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
	auto _lEmi2 = LOAD_TEXTURE("Room 2 var emissive", "RoomVar2/RoomVar2_DefaultMaterial_Emissive.png", Cappuccino::TextureType::PBREmission);
	auto _lMet2 = LOAD_TEXTURE("Room 2 var metallic", "RoomVar2/RoomVar2_DefaultMaterial_Metallic.png", Cappuccino::TextureType::PBRMetallic);
	auto _lNor2 = LOAD_TEXTURE("Room 2 var normal", "RoomVar2/RoomVar2_DefaultMaterial_Normal.png", Cappuccino::TextureType::PBRNormal);
	auto _lRou2 = LOAD_TEXTURE("Room 2 var roughness", "RoomVar2/RoomVar2_DefaultMaterial_Roughness.png", Cappuccino::TextureType::PBRRoughness);


	_levelManager._rooms.push_back(new Building("Assets/LevelData/Room1LevelData.obj", "Assets/SpawnData/Room1SpawnData.obj", "Assets/Meshes/Hitboxes/Room1HitboxData.obj", _mainShader, { _levelDiffuse, _levelSpecular, _levelNormal, _levelRoughness }, { LOAD_MESH("Room 1", "Rooms/Room1_Low.obj") }));
	_levelManager._rooms.push_back(new Building("Assets/LevelData/Room2LevelData.obj", "Assets/SpawnData/Room2SpawnData.obj", "Assets/Meshes/Hitboxes/Room2HitboxData.obj", _mainShader, { _levelDiffuse, _levelSpecular, _levelNormal, _levelRoughness }, { LOAD_MESH("Room 2", "Rooms/Room2_Low.obj") }));
	_levelManager._rooms.push_back(new Building("Assets/LevelData/Room4LevelData.obj", "Assets/SpawnData/Room4SpawnData.obj", "Assets/Meshes/Hitboxes/Room4HitboxData.obj", _mainShader, { _lAlb, _lMet, _lRou, _lOcc, _lEmi, _lNor }, { LOAD_MESH("Room 4", "Rooms/Room4_low.obj") }));
	for(unsigned i = 0; i < 7; i++)
		_levelManager.airlocks.push_back(new Building("Assets/LevelData/AirLockLevelData.obj", "Assets/SpawnData/AirLockSpawnData.obj", "Assets/Meshes/Hitboxes/AirlockHitboxData.obj", _mainShader, { _lAlb2, _lMet2, _lRou2, _lOcc2, _lEmi2, _lNor2 }, { LOAD_MESH("Airlock", "Rooms/Airlock_low.obj") }));

	auto botMesh = LOAD_MESH("Bot", "Bot.obj");
	botMesh->loadMesh();
	auto botDiffuse   = LOAD_TEXTURE("Bot diffuse.png", "Bot/Bot-Diffuse.png", Cappuccino::TextureType::PBRAlbedo);
	auto botMetallic  = LOAD_TEXTURE("Bot metallic.png", "Bot/Bot-Metallic.png", Cappuccino::TextureType::PBRMetallic);
	auto botEmission  = LOAD_TEXTURE("Bot emission.png", "Bot/Bot-Emission.png", Cappuccino::TextureType::PBREmission);
	auto botNormal    = LOAD_TEXTURE("Bot normal.png", "Bot/Bot-Normal.png", Cappuccino::TextureType::PBRNormal);
	auto botRoughness = LOAD_TEXTURE("Bot roughness.png", "Bot/Bot-Roughness.png", Cappuccino::TextureType::PBRRoughness);
	auto botAO        = LOAD_TEXTURE("Bot AO.png", "Bot/Bot-AO.png", Cappuccino::TextureType::PBRAmbientOcc);

	auto CaptainMesh = LOAD_MESH("Captain", "Captain.obj");
	CaptainMesh->loadMesh();
	auto CaptainDiffuse   = LOAD_TEXTURE("Captain diffuse.png", "Captain/Captain_Low_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
	auto CaptainMetallic  = LOAD_TEXTURE("Captain metallic.png", "Captain/Captain_Low_DefaultMaterial_Metallic.png", Cappuccino::TextureType::PBRMetallic);
	auto CaptainEmission  = LOAD_TEXTURE("Captain emission.png", "Captain/Captain_Low_DefaultMaterial_Emissive.png", Cappuccino::TextureType::PBREmission);
	auto CaptainNormal    = LOAD_TEXTURE("Captain normal.png", "Captain/Captain_Low_DefaultMaterial_Normal.png", Cappuccino::TextureType::PBRNormal);
	auto CaptainRoughness = LOAD_TEXTURE("Captain roughness.png", "Captain/Captain_Low_DefaultMaterial_Roughness.png", Cappuccino::TextureType::PBRRoughness);
	auto CaptainAO        = LOAD_TEXTURE("Captain AO.png", "Captain/Captain_Low_DefaultMaterial_AO.png", Cappuccino::TextureType::PBRAmbientOcc);

	auto gruntMesh = LOAD_MESH("Grunt", "Grunt.obj");
	gruntMesh->loadMesh();
	auto gruntDiffuse   = LOAD_TEXTURE("Grunt diffuse", "Grunt/Grunt_Low_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
	auto gruntMetallic  = LOAD_TEXTURE("Grunt metallic", "Grunt/Grunt_Low_DefaultMaterial_Metallic.png", Cappuccino::TextureType::PBRMetallic);
	auto gruntEmissive  = LOAD_TEXTURE("Grunt emissive", "Grunt/Grunt_Low_DefaultMaterial_Emissive.png", Cappuccino::TextureType::PBREmission);
	auto gruntNormal    = LOAD_TEXTURE("Grunt normal", "Grunt/Grunt_Low_DefaultMaterial_Normal.png", Cappuccino::TextureType::PBRNormal);
	auto gruntRoughness = LOAD_TEXTURE("Grunt roughness", "Grunt/Grunt_Low_DefaultMaterial_Roughness.png", Cappuccino::TextureType::PBRRoughness);
	auto gruntAO        = LOAD_TEXTURE("Grunt AO", "Grunt/Grunt_Low_DefaultMaterial_AO.png", Cappuccino::TextureType::PBRAmbientOcc);

	auto sentryMesh = LOAD_MESH("Sentry", "Sentry.obj");
	sentryMesh->loadMesh();
	auto sentryDiffuse   = LOAD_TEXTURE("Sentry diffuse", "Sentry/Sentry-Diffuse.png", Cappuccino::TextureType::PBRAlbedo);
	auto sentryMetallic  = LOAD_TEXTURE("Sentry metallic", "Sentry/Sentry-Metallic.png", Cappuccino::TextureType::PBRMetallic);
	auto sentryEmissive  = LOAD_TEXTURE("Sentry emissive", "Sentry/Sentry-Emission.png", Cappuccino::TextureType::PBREmission);
	auto sentryNormal    = LOAD_TEXTURE("Sentry normal", "Sentry/Sentry-Normal.png", Cappuccino::TextureType::PBRNormal);
	auto sentryRoughness = LOAD_TEXTURE("Sentry roughness", "Sentry/Sentry-Roughness.png", Cappuccino::TextureType::PBRRoughness);

	auto squelchMesh = LOAD_MESH("Squelch", "Squelch.obj");
	squelchMesh->loadMesh();
	auto squelchDiffuse   = LOAD_TEXTURE("Squelch diffuse", "Squelch/Squelch-Diffuse.png", Cappuccino::TextureType::PBRAlbedo);
	auto squelchNorm      = LOAD_TEXTURE("Squelch norm", "Squelch/Squelch-Norm.png", Cappuccino::TextureType::PBRNormal);
	auto squelchRoughness = LOAD_TEXTURE("Squelch roughness", "Squelch/Squelch-Roughness.png", Cappuccino::TextureType::PBRRoughness);
	auto squelchAO        = LOAD_TEXTURE("Squelch AO", "Squelch/Squelch-AO.png", Cappuccino::TextureType::PBRAmbientOcc);

	_primordial = new Primordial(_mainShader, { red, spec }, { Cappuccino::MeshLibrary::loadMesh("Squelch", "Squelch.obj") });

	auto crawlerMesh = LOAD_MESH("Crawler", "Crawler.obj");
	crawlerMesh->loadMesh();
	auto crawlerDiffuse   = LOAD_TEXTURE("Crawler diffuse", "Crawler/CrawlerDiffuse.png", Cappuccino::TextureType::PBRAlbedo);
	auto crawlerNorm      = LOAD_TEXTURE("Crawler normal", "Crawler/CrawlerNorm.png", Cappuccino::TextureType::PBRNormal);
	auto crawlerRoughness = LOAD_TEXTURE("Crawler roughness", "Crawler/Crawler-Roughness.png", Cappuccino::TextureType::PBRRoughness);
	auto crawlerAO        = LOAD_TEXTURE("Crawler AO", "Crawler/Crawler-AO.png", Cappuccino::TextureType::PBRAmbientOcc);

	for(unsigned i = 0; i < 30; i++) {
		_lights.emplace_back(glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	}
	resendLights();


	for(unsigned i = 0; i < 10; i++) {
		_levelManager._enemyManager._enemies.push_back(new Sentry(_mainShader, { sentryDiffuse, sentryMetallic, sentryEmissive, sentryNormal, sentryRoughness }, { sentryMesh }, 1.0f));
	}

	for(unsigned i = 0; i < 10; i++) {

		_levelManager._enemyManager._enemies.push_back(new Ghoul(_mainShader, {
			crawlerDiffuse,
			crawlerRoughness,
			crawlerAO,
			crawlerNorm,
		}, {
			crawlerMesh
		}, 1.0f));
	}

	for(unsigned i = 0; i < 10; i++) {
		_levelManager._enemyManager._enemies.push_back(new RoboGunner(_mainShader, { botDiffuse, botMetallic, botEmission, botNormal, botAO, botRoughness }, { botMesh }));
	}

	for(unsigned i = 0; i < 10; i++) {
		_levelManager._enemyManager._enemies.push_back(new Captain(_mainShader, { CaptainDiffuse, CaptainMetallic, CaptainEmission, CaptainNormal, CaptainAO, CaptainRoughness }, { CaptainMesh }));
	}

	for(unsigned i = 0; i < 10; i++) {
		_levelManager._enemyManager._enemies.push_back(new Grunt(_mainShader, { gruntDiffuse, gruntMetallic, gruntEmissive, gruntNormal, gruntAO, gruntRoughness }, { gruntMesh }));
	}

	for(unsigned i = 0; i < 10; i++) {
		_levelManager._enemyManager._enemies.push_back(new Squelch(_mainShader, { squelchDiffuse, squelchNorm, squelchRoughness, squelchAO }, { squelchMesh }));
	}

	resetObjects();

	//init members here
	auto mesh = LOAD_MESH("Bullet", "Bullet.obj");
	mesh->loadMesh();

	bullet = new Bullet(*_mainShader, { matte, spec }, { mesh }, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	bullet2 = new Bullet(*_mainShader, { matte, spec }, { mesh }, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	bullet->_transform.scale(glm::vec3(1.0f), 0.1f);
	bullet2->_transform.scale(glm::vec3(1.0f), 0.1f);

	for(auto& enemy : _levelManager._enemyManager._enemies)
		if(enemy->_enemyType != "Ghoul" && enemy->_enemyType != "Squelch")
			enemy->getGun()->addBullets(bullet);


	for(auto& light : _lights) {
		lamps.push_back(new Billboard(_mainShader, { matte }));
		lamps.back()->_rigidBody._position = light._pos;
	}

	_skybox = new Cappuccino::Cubemap({ 1600.0f, 1200.0f }, {
		"./Assets/Textures/Skybox/x7/px.png",
		"./Assets/Textures/Skybox/x7/nx.png",
		"./Assets/Textures/Skybox/x7/ny.png",
		"./Assets/Textures/Skybox/x7/py.png",
		"./Assets/Textures/Skybox/x7/pz.png",
		"./Assets/Textures/Skybox/x7/nz.png"
	});

	Class::_uiLights.clear();
	for(const auto& light : _lights)
		Class::_uiLights.push_back(light);

	//pause menu stuff
	ui._uiComponents.push_back(new Cappuccino::UIText("Resume", glm::vec2(1600.0f, 1000.0f), glm::vec2(-250.0f, 100.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));
	ui._uiComponents.push_back(new Cappuccino::UIText("Options", glm::vec2(1600.0f, 1000.0f), glm::vec2(-250.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));

	ui._uiComponents.push_back(new Cappuccino::UIText("Restart", glm::vec2(1600.0f, 1000.0f), glm::vec2(-250.0f, -200.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));
	ui._uiComponents.push_back(new Cappuccino::UIText("Main Menu", glm::vec2(1600.0f, 1000.0f), glm::vec2(-250.0f, -300.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));

	ui._uiComponents.push_back(new Cappuccino::UIText("Exit Game", glm::vec2(1600.0f, 1000.0f), glm::vec2(-250.0f, -500.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));
	
	for (auto x : ui._uiComponents)
		x->setVisible(false);
	
	resumeBox  = Cappuccino::HitBox(glm::vec3(-20.0f, -60.0f, 0.0f), glm::vec3(175.0f, 20.0f, 200.0f));
	optionsBox = Cappuccino::HitBox(glm::vec3(-20.0f, 0.0f, 0.0f), glm::vec3(175.0f, 20.0f, 200.0f));
	restartBox = Cappuccino::HitBox(glm::vec3(-20.0f, 80.0f, 0.0f), glm::vec3(175.0f, 20.0f, 200.0f));
	menuBox    = Cappuccino::HitBox(glm::vec3(-20.0f, 140.0f, 0.0f), glm::vec3(175.0f, 20.0f, 200.0f));
	exitBox    = Cappuccino::HitBox(glm::vec3(-20.0f, 220.0f, 0.0f), glm::vec3(175.0f, 20.0f, 200.0f));
}

bool GameplayScene::init() {
	static bool createdPlayer = false;
	if(Options::Assault)
		_testCommando = new Assault(_mainShader, {}, {});
	else if(Options::Commando)
		_testCommando = new Commando(_mainShader, {}, {});
	else if(Options::Demolitionist)
		_testCommando = new Demolitionist(_mainShader, {}, {});
	else if(Options::Scout)
		_testCommando = new Scout(_mainShader, {}, {});

	_testCommando->addAmmo(bullet, bullet2);


	Class::resendLights();
	_testCommando->_rigidBody._position = glm::vec3(-30.0f, 0.0f, -5.0f) + _levelManager.airlocks[0]->_levelData._respawnPoint;


	//activate members here
	_initialized = true;
	_shouldExit  = false;
	_testCommando->setActive(true);

	_levelManager._rooms[_levelManager._currentRoom]->setActive(true);
	for(auto& airlock : _levelManager.airlocks)
		airlock->setActive(true);
	for(auto& enemy : _enemies)
		enemy->setActive(true);
	for(auto& chests : _chests)
		chests->setActive(true);
	for(auto x : _loot)
		x->setActive(true);
	for(auto x : lamps)
		x->setActive(true);

	_levelManager._testShopTerminal->setActive(false);

	//_chest->setActive(true);

	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	_enemies.clear();
	_chests.clear();

	if(createdPlayer)
		resetObjects();


	_levelManager._testShopTerminal->_player = _testCommando;

	createdPlayer = true;

	return true;
}

bool GameplayScene::exit() {
	//deactivate members here
	_initialized = false;
	_shouldExit  = true;
	_testCommando->setActive(false);
	_testCommando->toggleHud(false);

	Options::Assault       = false;
	Options::Commando      = false;
	Options::Scout         = false;
	Options::Demolitionist = false;

	for(auto& room : _levelManager._rooms)
		room->setActive(false);
	for(auto& airlock : _levelManager.airlocks)
		airlock->setActive(false);
	for(auto& chests : _levelManager._chests)
		if(chests->isActive()) {
			chests->setActive(false);
			_chests.push_back(chests);
		}
	for(auto& enemy : _levelManager._enemyManager._enemies) {
		if(enemy->isActive()) {
			enemy->setActive(false);
			_enemies.push_back(enemy);
		}
	}

	for(auto x : lamps)
		x->setActive(false);

	for(auto x : _loot)
		x->setActive(false);
	for(auto x : ui._uiComponents)
		x->setVisible(false);

	_levelManager._testShopTerminal->setActive(false);

	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);


	return true;
}

void GameplayScene::shootCollisionBehaviour(Enemy* enemy) {
	if(_testCommando->getGun()->getDamage() != 0.0f)
		enemy->hurt(_testCommando->getGun()->getDamage());
	_testCommando->getGun()->specialCollisionBehaviour(_levelManager._enemyManager._enemies);

	//special behaviour if the enemy dies
	if(enemy->dead()) {
		if(!_testCommando->_voiceLines->isEventPlaying((int)VoiceLine::KillingEnemy) &&
			Cappuccino::randomInt(0, 10) <= 4)
			_testCommando->_voiceLines->playEvent((int)VoiceLine::KillingEnemy);
		_loot.push_back(_sednium->spawn(enemy->getWeight(), enemy->_rigidBody._position));
		_loot.push_back(_healthPack->spawn(enemy->getWeight(), enemy->_rigidBody._position));
		_loot.push_back(_ammoPack->spawn(enemy->getWeight(), enemy->_rigidBody._position));
	}
}

void GameplayScene::resendLights() {
	static bool first = true;
	_mainShader->use();
	if(first) {
		first = false;
		_mainShader->setUniform("gBuffer.gPos", 0);
		_mainShader->setUniform("gBuffer.gNormal", 1);
		_mainShader->setUniform("gBuffer.gAlbedo", 2);
		_mainShader->setUniform("gBuffer.gMetalRoughnessAO", 3);
		_mainShader->setUniform("gBuffer.gEmissive", 4);

		_mainShader->setUniform("shadowBias", 0.05f);
		_mainShader->setUniform("farPlane", 400.0f);
	}
	//_mainShader->setUniform("numLights", (int)_lights.size());
	//for(unsigned i = 0; i < _lights.size(); i++) {
	//	_mainShader->setUniform("lights[" + std::to_string(i) + "].position", _lights[i]._pos);
	//	_mainShader->setUniform("lights[" + std::to_string(i) + "].colour", _lights[i]._col);
	//	_mainShader->setUniform("lights[" + std::to_string(i) + "].depthMap", (int)(5 + i));
	//	//_mainShader->setUniform("lights[" + std::to_string(i) + "].active", _lights[i]._isActive);
	//}

	Cappuccino::Application::allLights = _lights;
}

void GameplayScene::sendGBufferShaderUniforms() {
	using namespace Cappuccino;
	Application::_gBufferShader->use();
	static bool first = true;

	if(first) {
		first = false;
		Application::_gBufferShader->loadProjectionMatrix(1600.0f, 1000.0f);
		Application::_gBufferShader->setUniform("material.albedo", (int)TextureType::PBRAlbedo);
		Application::_gBufferShader->setUniform("material.normalMap", (int)TextureType::PBRNormal);
		Application::_gBufferShader->setUniform("material.metallic", (int)TextureType::PBRMetallic);
		Application::_gBufferShader->setUniform("material.roughness", (int)TextureType::PBRRoughness);
		Application::_gBufferShader->setUniform("material.ambientOcc", (int)TextureType::PBRAmbientOcc);
		Application::_gBufferShader->setUniform("material.emission", (int)TextureType::PBREmission);
	}
}

void GameplayScene::childUpdate(float dt) {
	static bool pause = false;
	static float _pauseDelay = 0.5f;

	if(pause) {

		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		for(auto x : Cappuccino::GameObject::gameObjects)
			x->setPaused(pause);

		if(_testCommando->getHealth() <= 0)
			for(unsigned i = 2; i < ui._uiComponents.size(); i++)
				ui._uiComponents[i]->setVisible(true);
		else
			for(auto& x : ui._uiComponents)
				x->setVisible(true);
		//resume button
		if(cursorBox.checkCollision(resumeBox, resumeBox._position, cursorBox._position) && ui._uiComponents[0]->isVisible()) {
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[0])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

			if(_testCommando->_input.clickListener.leftClicked()) {
				pause = !pause;
				glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				for(auto& x : ui._uiComponents)
					x->setVisible(false);
				for(auto x : Cappuccino::GameObject::gameObjects)
					x->setPaused(pause);
				_testCommando->togglePauseScreen();
			}
		}
		else if(!cursorBox.checkCollision(resumeBox, resumeBox._position, cursorBox._position) && ui._uiComponents[0]->isVisible())
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[0])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

		//options button
		if(cursorBox.checkCollision(optionsBox, optionsBox._position, cursorBox._position) && ui._uiComponents[1]->isVisible()) {
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[1])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

			//if (_testCommando->_input.clickListener.leftClicked())

		}
		else if(!cursorBox.checkCollision(optionsBox, optionsBox._position, cursorBox._position) && ui._uiComponents[1]->isVisible())
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[1])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

		//restart button
		if(cursorBox.checkCollision(restartBox, restartBox._position, cursorBox._position)) {
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[2])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

			if(_testCommando->_input.clickListener.leftClicked()) {
				pause = !pause;
				glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				resetObjects();
				for(auto& x : ui._uiComponents)
					x->setVisible(false);
				for(auto x : Cappuccino::GameObject::gameObjects)
					x->setPaused(pause);
				_testCommando->togglePauseScreen();
			}
		}
		else if(!cursorBox.checkCollision(restartBox, restartBox._position, cursorBox._position))
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[2])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

		//menu button
		if(cursorBox.checkCollision(menuBox, menuBox._position, cursorBox._position)) {
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[3])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

			if(_testCommando->_input.clickListener.leftClicked()) {
				pause = !pause;
				resetObjects();
				for(auto& x : ui._uiComponents)
					x->setVisible(false);
				for(auto x : Cappuccino::GameObject::gameObjects)
					x->setPaused(pause);
				_testCommando->togglePauseScreen();
				Cappuccino::SceneManager::changeScene(0);
			}
		}
		else if(!cursorBox.checkCollision(menuBox, menuBox._position, cursorBox._position))
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[3])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

		//exit button
		if(cursorBox.checkCollision(exitBox, exitBox._position, cursorBox._position)) {
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[4])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

			if (_testCommando->_input.clickListener.leftClicked()) {
				for (auto& x : ui._uiComponents)
					x->setVisible(false);
				_testCommando->togglePauseScreen();
				_testCommando->setClose(true);
				exit();
			}
		}
		else if(!cursorBox.checkCollision(exitBox, exitBox._position, cursorBox._position))
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[4])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

	}
	if(!pause) {
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
			if(_testCommando->_input.keyboard->keyPressed(KeyEvent::L) && !pressed) {
				pressed = true;
				_lights.emplace_back(glm::vec3(_testCommando->_rigidBody._position.x, _testCommando->_rigidBody._position.y, _testCommando->_rigidBody._position.z + 5.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				resendLights();
			}
			else if(!_testCommando->_input.keyboard->keyPressed(KeyEvent::L))
				pressed = false;
		}


		//enemy logic
		GameObject* hitObject = _testCommando->getFirstIntersect(_testCommando->_testRay);//first object hit

		for(auto& enemy : _levelManager._enemyManager._enemies) {
			if(!enemy->isActive())
				continue;
			Ray enemyRay(normalize(_testCommando->_rigidBody._position - enemy->_rigidBody._position), enemy->_rigidBody._position);
			GameObject* enemyRayObject = enemy->getFirstIntersect(enemyRay);
			//activate enemy if within a trigger volume
			if(_testCommando->checkCollision(enemy->triggerVolume, _testCommando->_rigidBody._position) || enemy->getMaxHP() != enemy->getHP() || enemy->getMaxShield() != enemy->getShield()) {
				if(enemyRayObject == _testCommando)
					enemy->setTrigger(true);
				else
					enemy->setTrigger(false);
			}
			else
				enemy->setTrigger(false);

			{
				static float delay = 0.0f;
				//checks for squelch 
				if(enemy->dead()
					&& !_testCommando->_voiceLines->isEventPlaying((int)VoiceLine::KillingEnemy)
					&& delay < 0.0f) {
					_testCommando->_voiceLines->playEvent((int)VoiceLine::KillingEnemy);
					delay = randomFloat(5.0f, 10.0f);
				}
				delay -= dt;
			}
			//bullet collision
			if(!_testCommando->getGun()->isHitscan()) {

				//loop through the player's bullets
				for(auto playerBullets : _testCommando->getGun()->getBullets()) {
					//check if the bullet touches an enemy
					if(playerBullets->_rigidBody.checkCollision(enemy->_rigidBody) && playerBullets->isActive()) {
						shootCollisionBehaviour(enemy);
						playerBullets->setActive(false);
					}
				}
			}
			else {
				if(enemy == hitObject) {
					shootCollisionBehaviour(enemy);
				}
			}
			enemy->attack(_testCommando, dt);

			for(auto bullet : enemy->getGun()->getBullets()) {
				if(bullet->checkCollision(_testCommando) && bullet->isActive()) {
					_testCommando->takeDamage(enemy->getGun()->getDamage());
					bullet->setActive(false);
				}
			}
		}

		for(auto y : _levelManager._chests)
			if(_testCommando->checkCollision(y->_triggerVolume, y->_rigidBody._position) && _testCommando->_input.keyboard->keyPressed(KeyEvent::E) && !y->open()) {
				std::vector<Loot*> _temp = y->spawn(10.0f, y->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f), _sednium, _healthPack, _ammoPack, _bullion);
				for(auto x : _temp) {
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

		for(auto& x : _loot) {
			if(x->isActive()) {
				x->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), dt * 20.0f);
				x->pickup(_testCommando);
			}
		}

		if(_testCommando->getHealth() <= 0) {
			_testCommando->_voiceLines->playEvent((int)VoiceLine::GettingKilled);
			pause = true;
			_testCommando->togglePauseScreen();
		}

		const glm::mat4 view = glm::mat4(glm::mat3(_testCommando->getCamera()->whereAreWeLooking()));
		_skybox->getShader().use();
		_skybox->getShader().setUniform("view", view);

		//Cappuccino::GameObject* hitObject = _testCommando->getFirstIntersect(_testCommando->_testRay);//first object hit

		bool spotted = false;
		for(auto y : GameObject::gameObjects) {//for all game objects
			if(y->id == "Enemy") {//if the object is an enemy
				if(y->isActive() && y == hitObject) {
					dynamic_cast<Enemy*>(y)->getHUD()->toggleHud(true);//toggle the hud
					spotted = true;
				}
				else {
					dynamic_cast<Enemy*>(y)->getHUD()->toggleHud(false);
				}
			}
		}
		{
			static float delay = 0.0f;

			if(spotted && !_testCommando->_voiceLines->isEventPlaying((int)VoiceLine::SeeingEnemy)
				&& delay < 0.0f) {
				delay = randomFloat(5.0f, 10.0f);
				_testCommando->_voiceLines->playEvent((int)VoiceLine::SeeingEnemy);
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

	//deal with shop interface
	cursorBox._position = glm::vec3(cursorPos.x, cursorPos.y, 0.0f);

	_pauseDelay -= dt;

	//pause button
	if(_testCommando->_input.keyboard->keyPressed(Cappuccino::KeyEvent::Q) && _pauseDelay <= 0.0f) {
		_pauseDelay = 0.5f;
		pause = !pause;
		_testCommando->togglePauseScreen();
		if(!pause) {
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			for(auto& x : ui._uiComponents)
				x->setVisible(false);
			for(auto x : Cappuccino::GameObject::gameObjects)
				x->setPaused(pause);
		}
		else
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void GameplayScene::mouseFunction(const double xpos, const double ypos) {
	if(ShopTerminal::cursorLocked)
		firstMouse = true;

	if(firstMouse) {
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}

	const GLfloat xOffset = xpos - lastX;
	const GLfloat yOffset = lastY - ypos;
	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	if(!ShopTerminal::cursorLocked)
		_testCommando->getCamera()->doMouseMovement(xOffset, yOffset);

	//keep track of the cursor's position
	cursorPos = glm::vec2(xpos, ypos);
	cursorPos -= glm::vec2(800.0f, 500.0f);

	//auto temp = cursorPos;
	//printf("%f %f\n", temp.x, -temp.y);
	
}

void GameplayScene::clickFunction(const int button, const int action, const int mods) {
	_testCommando->_input.clickListener.checkClick(button, action, mods);
}

void GameplayScene::resetObjects() {
	if (_testCommando != nullptr)
	{
		_testCommando->_rigidBody._position = _levelManager._rooms[_levelManager._currentRoom]->_levelData._respawnPoint + _levelManager._rooms[_levelManager._currentRoom]->_rigidBody._position;
		_testCommando->_rigidBody._position.y += 2;
		_testCommando->setHealth(_testCommando->getMaxHp());
		_testCommando->setShield(_testCommando->getMaxShield());
		_testCommando->getPrimary()->setAmmoCount(0);
		_testCommando->setCurrency(0);
		_testCommando->_rigidBody._vel = glm::vec3(0.0f);
		_testCommando->_rigidBody._accel = glm::vec3(0.0f);
	}

	for(auto& x : _levelManager._enemyManager._enemies) {
		x->setHealth(x->getMaxHP());
		x->setShield(x->getMaxShield());
		x->setTrigger(false);
	}
}
