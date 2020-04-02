#include "GameplayScene711.h"

#include "Options.h"
#include "PlayerHUD.h"

#include <Cappuccino/Application.h>
#include <Cappuccino/DirLight.h>
#include <Cappuccino/ResourceManager.h>

#include <ctime>

//whew
#define LOAD_SHADER Cappuccino::ShaderLibrary::loadShader
#define LOAD_TEXTURE Cappuccino::TextureLibrary::loadTexture
#define LOAD_MESH Cappuccino::MeshLibrary::loadMesh

Cappuccino::Shader* GameplayScene::_mainShader = nullptr;
Class* GameplayScene::_testCommando = nullptr;
std::vector<Cappuccino::PointLight> GameplayScene::_lights;

GameplayScene::GameplayScene(const bool isActive) :
	Scene(isActive),
	cursorBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f)),
	_levelManager(_lights) {

	_mainShader = LOAD_SHADER("PBR Shader", "PBR.vert", "PBR.frag");
	Cappuccino::Application::_lightingPassShader = _mainShader;

	//const auto diffuse = LOAD_TEXTURE("Level metal", "metal.png", Cappuccino::TextureType::PBRAlbedo);
	const auto matte = LOAD_TEXTURE("Level matte",          "matte.png", Cappuccino::TextureType::PBRAlbedo);
	const auto spec  = LOAD_TEXTURE("Level metal specular", "metal.png", Cappuccino::TextureType::PBRMetallic);
	const auto red   = LOAD_TEXTURE("Enemy red diffuse",    "red.png",   Cappuccino::TextureType::DiffuseMap);

	// ----------------------------------------------------
	// ----- LEVEL MANAGER INIT ---------------------------
	// ----------------------------------------------------
	{
		_levelManager._testShopTerminal = new ShopTerminal(*_mainShader, {
			LOAD_TEXTURE("Shop terminal diffuse",   "SCTD/SCTD_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::PBRAlbedo),
			LOAD_TEXTURE("Shop terminal AO",        "SCTD/SCTD_DefaultMaterial_AO.png",        Cappuccino::TextureType::PBRAmbientOcc),
			LOAD_TEXTURE("Shop terminal emissive",  "SCTD/SCTD_DefaultMaterial_Emissive.png",  Cappuccino::TextureType::PBREmission),
			LOAD_TEXTURE("Shop terminal metallic",  "SCTD/SCTD_DefaultMaterial_Metallic.png",  Cappuccino::TextureType::PBRMetallic),
			LOAD_TEXTURE("Shop terminal normal",    "SCTD/SCTD_DefaultMaterial_Normal.png",    Cappuccino::TextureType::PBRNormal),
			LOAD_TEXTURE("Shop terminal roughness", "SCTD/SCTD_DefaultMaterial_Roughness.png", Cappuccino::TextureType::PBRRoughness),
		}, {
			LOAD_MESH("Shop terminal", "Shop.obj")
		}, _testCommando, cursorBox);

		_levelManager._testShopTerminal->_rigidBody._position = glm::vec3(-10.0f, 0.0f, 0.0f);

		_sednium = new Sednium(*_mainShader, { red, spec });

		_ammoPack = new AmmoPack(*_mainShader, {
			LOAD_TEXTURE("Ammo pack diffuse",   "ammoPickup/ammoPickup-Diffuse.png",   Cappuccino::TextureType::PBRAlbedo),
			LOAD_TEXTURE("Ammo pack normal",    "ammoPickup/ammoPickup-Normal.png",    Cappuccino::TextureType::PBRNormal),
			LOAD_TEXTURE("Ammo pack Roughness", "ammoPickup/ammoPickup-Roughness.png", Cappuccino::TextureType::PBRRoughness),
			LOAD_TEXTURE("Ammo pack emission",  "ammoPickup/ammoPickup-Emission.png",  Cappuccino::TextureType::PBREmission)
		});

		_healthPack = new HealthPack(*_mainShader, {
			LOAD_TEXTURE("Health pack diffuse",   "healthPickup/healthPickup-Diffuse.png",   Cappuccino::TextureType::PBRAlbedo),
			LOAD_TEXTURE("Health pack specular",  "healthPickup/healthPickup-Metallic.png",  Cappuccino::TextureType::PBRMetallic),
			LOAD_TEXTURE("Health pack normal",    "healthPickup/healthPickup-Normal.png",    Cappuccino::TextureType::PBRNormal),
			LOAD_TEXTURE("Health pack Roughness", "healthPickup/healthPickup-Roughness.png", Cappuccino::TextureType::PBRRoughness),
			LOAD_TEXTURE("Health pack emission",  "healthPickup/healthPickup-Emission.png",  Cappuccino::TextureType::PBREmission)
		});

		_bullion = new Bullion(*_mainShader, {});

		for (unsigned i = 0; i < 7; i++) {
			_levelManager._chests.push_back(new Chest(*_mainShader, {
				LOAD_TEXTURE("Loot chest closed diffuse",  "lootChest/Chest_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::DiffuseMap),
				LOAD_TEXTURE("Loot chest closed specular", "lootChest/Chest_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::SpecularMap),
				LOAD_TEXTURE("Loot chest closed normal",   "lootChest/Chest_DefaultMaterial_Normal.png",    Cappuccino::TextureType::PBRNormal),
				LOAD_TEXTURE("Loot chest closed emission", "lootChest/Chest_DefaultMaterial_Emissive.png",  Cappuccino::TextureType::PBREmission),
				LOAD_TEXTURE("Loot chest closed height",   "lootChest/Chest_DefaultMaterial_Height.png",    Cappuccino::TextureType::HeightMap)
			}));
			_levelManager._chests[i]->setActive(false);
		}

		_levelManager._lootChest = new Chest(*_mainShader, {
			LOAD_TEXTURE("Loot chest closed diffuse",  "lootChest/Chest_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::DiffuseMap),
			LOAD_TEXTURE("Loot chest closed specular", "lootChest/Chest_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::SpecularMap),
			LOAD_TEXTURE("Loot chest closed normal",   "lootChest/Chest_DefaultMaterial_Normal.png",    Cappuccino::TextureType::PBRNormal),
			LOAD_TEXTURE("Loot chest closed emission", "lootChest/Chest_DefaultMaterial_Emissive.png",  Cappuccino::TextureType::PBREmission),
			LOAD_TEXTURE("Loot chest closed height",   "lootChest/Chest_DefaultMaterial_Height.png",    Cappuccino::TextureType::HeightMap)
		});
	}

	// ----------------------------------------------------
	// ----- ROOM DATA ------------------------------------
	// ----------------------------------------------------
	{
		// auto _levelDiffuse   = LOAD_TEXTURE("Room1 diffuse", "room/room1_low_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
		// auto _levelSpecular  = LOAD_TEXTURE("Room1 metallic", "room/room1_low_DefaultMaterial_Metallic.png", Cappuccino::TextureType::PBRMetallic);
		// auto _levelNormal    = LOAD_TEXTURE("Room1 normal", "room/room1_low_DefaultMaterial_Normal.png", Cappuccino::TextureType::PBRNormal);
		// auto _levelRoughness = LOAD_TEXTURE("Room1 roughness", "room/room1_low_DefaultMaterial_Roughness.png", Cappuccino::TextureType::PBRRoughness);

		const auto lOcc1 = LOAD_TEXTURE("Room var 1 AO",        "RoomVar1/Room_Texture_AO.png",        Cappuccino::TextureType::PBRAmbientOcc);
		const auto lAlb1 = LOAD_TEXTURE("Room var 1 diffuse",   "RoomVar1/Room_Texture_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
		const auto lEmi1 = LOAD_TEXTURE("Room var 1 emissive",  "RoomVar1/Room_Texture_Emissive.png",  Cappuccino::TextureType::PBREmission);
		const auto lMet1 = LOAD_TEXTURE("Room var 1 metallic",  "RoomVar1/Room_Texture_Metallic.png",  Cappuccino::TextureType::PBRMetallic);
		const auto lNor1 = LOAD_TEXTURE("Room var 1 normal",    "RoomVar1/Room_Texture_Normal.png",    Cappuccino::TextureType::PBRNormal);
		const auto lRou1 = LOAD_TEXTURE("Room var 1 roughness", "RoomVar1/Room_Texture_Roughness.png", Cappuccino::TextureType::PBRRoughness);

		const auto lOcc2 = LOAD_TEXTURE("Room var 2 AO",        "RoomVar2/Room_Texture_AO.png",        Cappuccino::TextureType::PBRAmbientOcc);
		const auto lAlb2 = LOAD_TEXTURE("Room var 2 diffuse",   "RoomVar2/Room_Texture_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
		const auto lEmi2 = LOAD_TEXTURE("Room var 2 emissive",  "RoomVar2/Room_Texture_Emissive.png",  Cappuccino::TextureType::PBREmission);
		const auto lMet2 = LOAD_TEXTURE("Room var 2 metallic",  "RoomVar2/Room_Texture_Metallic.png",  Cappuccino::TextureType::PBRMetallic);
		const auto lNor2 = LOAD_TEXTURE("Room var 2 normal",    "RoomVar2/Room_Texture_Normal.png",    Cappuccino::TextureType::PBRNormal);
		const auto lRou2 = LOAD_TEXTURE("Room var 2 roughness", "RoomVar2/Room_Texture_Roughness.png", Cappuccino::TextureType::PBRRoughness);

		const auto lOcc3 = LOAD_TEXTURE("Room var 3 AO",        "RoomVar3/Room_Texture_AO.png",        Cappuccino::TextureType::PBRAmbientOcc);
		const auto lAlb3 = LOAD_TEXTURE("Room var 3 diffuse",   "RoomVar3/Room_Texture_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
		const auto lEmi3 = LOAD_TEXTURE("Room var 3 emissive",  "RoomVar3/Room_Texture_Emissive.png",  Cappuccino::TextureType::PBREmission);
		const auto lMet3 = LOAD_TEXTURE("Room var 3 metallic",  "RoomVar3/Room_Texture_Metallic.png",  Cappuccino::TextureType::PBRMetallic);
		const auto lNor3 = LOAD_TEXTURE("Room var 3 normal",    "RoomVar3/Room_Texture_Normal.png",    Cappuccino::TextureType::PBRNormal);
		const auto lRou3 = LOAD_TEXTURE("Room var 3 roughness", "RoomVar3/Room_Texture_Roughness.png", Cappuccino::TextureType::PBRRoughness);

		// This is basically LOAD_TEXTURE and LOAD_MESH but for different levels (MEGA WHEW)
		// Code smell bad
		#define LOAD_LEVELS(...)\
			{																\
				_levelManager._rooms.push_back(new Building(				\
					"Assets/LevelData/TutorialRoomLevelData.obj",			\
					"Assets/SpawnData/TutorialRoomSpawnData.obj",			\
					"Assets/Meshes/Hitboxes/TutorialRoomHitboxData.obj",	\
					_mainShader,											\
					{ ##__VA_ARGS__ },										\
					{ LOAD_MESH("Tutorial", "Rooms/Tutorial_Room.obj") }	\
				));															\
																			\
				_levelManager._rooms.push_back(new Building(				\
					"Assets/LevelData/Room1LevelData.obj",					\
					"Assets/SpawnData/Room1SpawnData.obj",					\
					"Assets/Meshes/Hitboxes/Room1HitboxData.obj",			\
					_mainShader,											\
					{ ##__VA_ARGS__ },										\
					{ LOAD_MESH("Room 1", "Rooms/Room_1.obj") }				\
				));															\
																			\
				_levelManager._rooms.push_back(new Building(				\
					"Assets/LevelData/Room2LevelData.obj",					\
					"Assets/SpawnData/Room2SpawnData.obj",					\
					"Assets/Meshes/Hitboxes/Room2HitboxData.obj",			\
					_mainShader,											\
					{ ##__VA_ARGS__ },										\
					{ LOAD_MESH("Room 2", "Rooms/Room_2.obj") }				\
				));															\
																			\
				_levelManager._rooms.push_back(new Building(				\
					"Assets/LevelData/Room3LevelData.obj",					\
					"Assets/SpawnData/Room3SpawnData.obj",					\
					"Assets/Meshes/Hitboxes/Room3HitboxData.obj",			\
					_mainShader,											\
					{ ##__VA_ARGS__ },										\
					{ LOAD_MESH("Room 3", "Rooms/Room_3.obj") }				\
				));															\
																			\
				_levelManager._rooms.push_back(new Building(				\
					"Assets/LevelData/Room4LevelData.obj",					\
					"Assets/SpawnData/Room4SpawnData.obj",					\
					"Assets/Meshes/Hitboxes/Room4HitboxData.obj",			\
					_mainShader,											\
					{ ##__VA_ARGS__ },										\
					{ LOAD_MESH("Room 4", "Rooms/Room_4.obj") }				\
				));															\
																			\
				_levelManager._rooms.push_back(new Building(				\
					"Assets/LevelData/Room5LevelData.obj",					\
					"Assets/SpawnData/Room5SpawnData.obj",					\
					"Assets/Meshes/Hitboxes/Room5HitboxData.obj",			\
					_mainShader,											\
					{ ##__VA_ARGS__ },										\
					{ LOAD_MESH("Room 5", "Rooms/Room_5.obj") }				\
				));															\
																			\
				_levelManager._rooms.push_back(new Building(				\
					"Assets/LevelData/Room6LevelData.obj",					\
					"Assets/SpawnData/Room6SpawnData.obj",					\
					"Assets/Meshes/Hitboxes/Room6HitboxData.obj",			\
					_mainShader,											\
					{ ##__VA_ARGS__ },										\
					{ LOAD_MESH("Room 5", "Rooms/Room_6.obj") }				\
				));															\
			}

		LOAD_LEVELS(lOcc1, lAlb1, lEmi1, lMet1, lNor1, lRou1);
		LOAD_LEVELS(lOcc2, lAlb2, lEmi2, lMet2, lNor2, lRou2);
		LOAD_LEVELS(lOcc3, lAlb3, lEmi3, lMet3, lNor3, lRou3);

		// Airlocks
		for (unsigned i = 0; i < 5; i++) {
			_levelManager.airlocks.push_back(new Building(
				"Assets/LevelData/AirLockLevelData.obj",
				"Assets/SpawnData/AirLockSpawnData.obj",
				"Assets/Meshes/Hitboxes/AirlockHitboxData.obj",
				_mainShader,
				{ lAlb1, lMet1, lRou1, lOcc1, lEmi1, lNor1 },
				{ LOAD_MESH("Airlock", "Rooms/Airlock_low.obj") }
			));
		}

		//lootRoom
		_levelManager._lootRoom = new Building(
			"Assets/LevelData/ChestRoomLevelData.obj",
			"Assets/SpawnData/Room3SpawnData.obj",
			"Assets/Meshes/Hitboxes/Chest_Room_HitBoxData.obj",
			_mainShader,
			{ lAlb1, lMet1, lRou1, lOcc1, lEmi1, lNor1 },
			{ LOAD_MESH("Chest Room", "Rooms/Chest_Room.obj") }
		);

		const auto sctdOcc = LOAD_TEXTURE("SCTDOcc", "SCTD/SCTD_DefaultMaterial_AO.png", Cappuccino::TextureType::PBRAmbientOcc);
		const auto sctdAlb = LOAD_TEXTURE("SCTDAlb", "SCTD/SCTD_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
		const auto sctdEmi = LOAD_TEXTURE("SCTDEmi", "SCTD/SCTD_DefaultMaterial_Emissive.png", Cappuccino::TextureType::PBREmission);
		const auto sctdMet = LOAD_TEXTURE("SCTDMet", "SCTD/SCTD_DefaultMaterial_Metallic.png", Cappuccino::TextureType::PBRMetallic);
		const auto sctdNor = LOAD_TEXTURE("SCTDNor", "SCTD/SCTD_DefaultMaterial_Normal.png", Cappuccino::TextureType::PBRNormal);
		const auto sctdRou = LOAD_TEXTURE("SCTDRou", "SCTD/SCTD_DefaultMaterial_Roughness.png", Cappuccino::TextureType::PBRRoughness);

		const auto doorLeft = LOAD_MESH("doorLeft", "DoorLeft.obj");
		const auto doorRight = LOAD_MESH("doorRight", "doorRight.obj");
		for (int i = 0; i < 6; i++) {
			_levelManager._entrancesL.push_back(new Door(0.0f, Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(3.5f, 2.0f, 1.0f)), _mainShader, { sctdAlb, sctdMet, sctdRou, sctdOcc, sctdEmi, sctdNor }, { doorLeft }));
			_levelManager._entrancesR.push_back(new Door(0.0f, Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(3.5f, 2.0f, 1.0f)), _mainShader, { sctdAlb, sctdMet, sctdRou, sctdOcc, sctdEmi, sctdNor }, { doorRight }));
		}

		const auto teleporter = LOAD_MESH("Teleporter", "Teleporter.obj");
		_levelManager._teleporterA = new Teleporter(_mainShader, { sctdAlb, sctdMet, sctdRou, sctdOcc, sctdEmi, sctdNor }, { teleporter });
		_levelManager._teleporterB = new Teleporter(_mainShader, { sctdAlb, sctdMet, sctdRou, sctdOcc, sctdEmi, sctdNor }, { teleporter });
	}

	// ----------------------------------------------------
	// ----- ENEMIES --------------------------------------
	// ----------------------------------------------------
	{
		auto botMesh = LOAD_MESH("Bot", "Bot.obj");
		botMesh->loadMesh();
		const auto botDiffuse   = LOAD_TEXTURE("Bot diffuse",   "Bot/Bot-Diffuse.png",   Cappuccino::TextureType::PBRAlbedo);
		const auto botMetallic  = LOAD_TEXTURE("Bot metallic",  "Bot/Bot-Metallic.png",  Cappuccino::TextureType::PBRMetallic);
		const auto botEmission  = LOAD_TEXTURE("Bot emission",  "Bot/Bot-Emission.png",  Cappuccino::TextureType::PBREmission);
		const auto botNormal    = LOAD_TEXTURE("Bot normal",    "Bot/Bot-Normal.png",    Cappuccino::TextureType::PBRNormal);
		const auto botRoughness = LOAD_TEXTURE("Bot roughness", "Bot/Bot-Roughness.png", Cappuccino::TextureType::PBRRoughness);
		const auto botAO        = LOAD_TEXTURE("Bot AO",        "Bot/Bot-AO.png",        Cappuccino::TextureType::PBRAmbientOcc);

		auto captainMesh = LOAD_MESH("Captain", "Captain.obj");
		captainMesh->loadMesh();
		const auto captainDiffuse   = LOAD_TEXTURE("Captain diffuse",   "Captain/Captain_Low_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
		const auto captainMetallic  = LOAD_TEXTURE("Captain metallic",  "Captain/Captain_Low_DefaultMaterial_Metallic.png",  Cappuccino::TextureType::PBRMetallic);
		const auto captainEmission  = LOAD_TEXTURE("Captain emission",  "Captain/Captain_Low_DefaultMaterial_Emissive.png",  Cappuccino::TextureType::PBREmission);
		const auto captainNormal    = LOAD_TEXTURE("Captain normal",    "Captain/Captain_Low_DefaultMaterial_Normal.png",    Cappuccino::TextureType::PBRNormal);
		const auto captainRoughness = LOAD_TEXTURE("Captain roughness", "Captain/Captain_Low_DefaultMaterial_Roughness.png", Cappuccino::TextureType::PBRRoughness);
		const auto captainAo        = LOAD_TEXTURE("Captain AO",        "Captain/Captain_Low_DefaultMaterial_AO.png",        Cappuccino::TextureType::PBRAmbientOcc);

		auto gruntMesh = LOAD_MESH("Grunt", "Grunt.obj");
		gruntMesh->loadMesh();
		const auto gruntDiffuse   = LOAD_TEXTURE("Grunt diffuse",   "Grunt/Grunt_Low_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
		const auto gruntMetallic  = LOAD_TEXTURE("Grunt metallic",  "Grunt/Grunt_Low_DefaultMaterial_Metallic.png",  Cappuccino::TextureType::PBRMetallic);
		const auto gruntEmissive  = LOAD_TEXTURE("Grunt emissive",  "Grunt/Grunt_Low_DefaultMaterial_Emissive.png",  Cappuccino::TextureType::PBREmission);
		const auto gruntNormal    = LOAD_TEXTURE("Grunt normal",    "Grunt/Grunt_Low_DefaultMaterial_Normal.png",    Cappuccino::TextureType::PBRNormal);
		const auto gruntRoughness = LOAD_TEXTURE("Grunt roughness", "Grunt/Grunt_Low_DefaultMaterial_Roughness.png", Cappuccino::TextureType::PBRRoughness);
		const auto gruntAO        = LOAD_TEXTURE("Grunt AO",        "Grunt/Grunt_Low_DefaultMaterial_AO.png",        Cappuccino::TextureType::PBRAmbientOcc);

		auto squelchMesh = LOAD_MESH("Squelch", "Squelch.obj");
		squelchMesh->loadMesh();
		const auto squelchDiffuse   = LOAD_TEXTURE("Squelch diffuse",   "Squelch/Squelch-Diffuse.png",    Cappuccino::TextureType::PBRAlbedo);
		const auto squelchNorm      = LOAD_TEXTURE("Squelch norm",      "Squelch/Squelch-Norm.png",      Cappuccino::TextureType::PBRNormal);
		const auto squelchRoughness = LOAD_TEXTURE("Squelch roughness", "Squelch/Squelch-Roughness.png", Cappuccino::TextureType::PBRRoughness);
		const auto squelchAO        = LOAD_TEXTURE("Squelch AO",        "Squelch/Squelch-AO.png",        Cappuccino::TextureType::PBRAmbientOcc);

		auto sentryMesh = LOAD_MESH("Sentry", "Sentry.obj");
		sentryMesh->loadMesh();
		auto sentryDiffuse   = LOAD_TEXTURE("Sentry diffuse",   "Sentry/Sentry_Low_DefaultMaterial_BaseColor.png", Cappuccino::TextureType::PBRAlbedo);
		auto sentryMetallic  = LOAD_TEXTURE("Sentry metallic",  "Sentry/Sentry_Low_DefaultMaterial_Metallic.png",  Cappuccino::TextureType::PBRMetallic);
		auto sentryEmissive  = LOAD_TEXTURE("Sentry emissive",  "Sentry/Sentry_Low_DefaultMaterial_Emissive.png",  Cappuccino::TextureType::PBREmission);
		auto sentryNormal    = LOAD_TEXTURE("Sentry normal",    "Sentry/Sentry_Low_DefaultMaterial_Normal.png",    Cappuccino::TextureType::PBRNormal);
		auto sentryRoughness = LOAD_TEXTURE("Sentry roughness", "Sentry/Sentry_Low_DefaultMaterial_Roughness.png", Cappuccino::TextureType::PBRRoughness);
		auto sentryAO        = LOAD_TEXTURE("Sentry AO",        "Sentry/Sentry_Low_DefaultMaterial_AO.png",        Cappuccino::TextureType::PBRAmbientOcc);

		auto crawlerMesh = LOAD_MESH("Crawler", "Crawler.obj");
		crawlerMesh->loadMesh();
		const auto crawlerDiffuse   = LOAD_TEXTURE("Crawler diffuse",   "Crawler/CrawlerDiffuse.png",    Cappuccino::TextureType::PBRAlbedo);
		const auto crawlerNorm      = LOAD_TEXTURE("Crawler normal",    "Crawler/CrawlerNorm.png",       Cappuccino::TextureType::PBRNormal);
		const auto crawlerRoughness = LOAD_TEXTURE("Crawler roughness", "Crawler/Crawler-Roughness.png", Cappuccino::TextureType::PBRRoughness);
		const auto crawlerAO        = LOAD_TEXTURE("Crawler AO",        "Crawler/Crawler-AO.png",        Cappuccino::TextureType::PBRAmbientOcc);

		for (unsigned i = 0; i < 30; i++) {
			_lights.emplace_back(glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.52f, 0.37f, 0.11f) * 0.95f);
		}
		resendLights();


		for (unsigned i = 0; i < 10; i++) {
			_levelManager._enemyManager._enemies.push_back(new Sentry(_mainShader, { sentryDiffuse, sentryMetallic, sentryEmissive, sentryNormal, sentryRoughness, sentryAO }, { sentryMesh }, 1.0f));
		}

		for (unsigned i = 0; i < 10; i++) {
			_levelManager._enemyManager._enemies.push_back(new Ghoul(_mainShader, { crawlerDiffuse, crawlerRoughness, crawlerAO, crawlerNorm }, { crawlerMesh }, 1.0f));
		}

		for (unsigned i = 0; i < 10; i++) {
			_levelManager._enemyManager._enemies.push_back(new RoboGunner(_mainShader, { botDiffuse, botMetallic, botEmission, botNormal, botAO, botRoughness }, { botMesh }));
		}

		for (unsigned i = 0; i < 10; i++) {
			_levelManager._enemyManager._enemies.push_back(new Captain(_mainShader, { captainDiffuse, captainMetallic, captainEmission, captainNormal, captainAo, captainRoughness }, { captainMesh }));
		}

		for (unsigned i = 0; i < 10; i++) {
			_levelManager._enemyManager._enemies.push_back(new Grunt(_mainShader, { gruntDiffuse, gruntMetallic, gruntEmissive, gruntNormal, gruntAO, gruntRoughness }, { gruntMesh }));
		}

		for (unsigned i = 0; i < 10; i++) {
			_levelManager._enemyManager._enemies.push_back(new Squelch(_mainShader, { squelchDiffuse, squelchNorm, squelchRoughness, squelchAO }, { squelchMesh }));
		}

		for (unsigned i = 0; i < 4; i++) {
			if (i == 0)
				_levelManager._enemyManager._enemies.push_back(new Dummy(_mainShader, { botDiffuse, botMetallic, botEmission, botNormal, botAO, botRoughness }, { botMesh }, false));
			else
				_levelManager._enemyManager._enemies.push_back(new Dummy(_mainShader, { botDiffuse, botMetallic, botEmission, botNormal, botAO, botRoughness }, { botMesh }, true));
		}

		resetObjects();
	}

	// ----------------------------------------------------
	// ----- BULLETS --------------------------------------
	// ----------------------------------------------------
	{
		//init members here
		auto mesh = LOAD_MESH("Bullet", "Bullet.obj");
		mesh->loadMesh();

		bullet = new Bullet(*_mainShader, { matte, spec }, { mesh }, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		bullet2 = new Bullet(*_mainShader, { matte, spec }, { mesh }, glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		bullet->_transform.scale(glm::vec3(1.0f), 0.1f);
		bullet2->_transform.scale(glm::vec3(1.0f), 0.1f);

		for (auto& enemy : _levelManager._enemyManager._enemies)
			if (enemy->_enemyType != "Ghoul" && enemy->_enemyType != "Squelch")
				enemy->getGun()->addBullets(bullet);
	}

	// ----------------------------------------------------
	// ----- LIGHTS ---------------------------------------
	// ----------------------------------------------------
	{
		for (auto& light : _lights) {
			lamps.push_back(new Billboard(_mainShader, { matte }));
			lamps.back()->_rigidBody._position = light._pos;
		}
	}

	// ----------------------------------------------------
	// ----- SKYBOX ---------------------------------------
	// ----------------------------------------------------
	{
		_skybox = new Cappuccino::Cubemap({ 1600.0f, 1200.0f }, {
			"Assets/Textures/Skybox/x7/px.png",
			"Assets/Textures/Skybox/x7/nx.png",
			"Assets/Textures/Skybox/x7/ny.png",
			"Assets/Textures/Skybox/x7/py.png",
			"Assets/Textures/Skybox/x7/pz.png",
			"Assets/Textures/Skybox/x7/nz.png"
		});
	}

	// ----------------------------------------------------
	// ----- HUD/UI ELEMENTS ------------------------------
	// ----------------------------------------------------
	{
		Class::_uiLights.clear();
		for (const auto& light : _lights)
			Class::_uiLights.push_back(light);

		//pause menu stuff
		ui._uiComponents.push_back(new Cappuccino::UIText("Resume", glm::vec2(1600.0f, 1000.0f), glm::vec2(-250.0f, 100.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));
		ui._uiComponents.push_back(new Cappuccino::UIText("Options", glm::vec2(1600.0f, 1000.0f), glm::vec2(-250.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));

		ui._uiComponents.push_back(new Cappuccino::UIText("Restart", glm::vec2(1600.0f, 1000.0f), glm::vec2(-250.0f, -200.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));
		ui._uiComponents.push_back(new Cappuccino::UIText("Main Menu", glm::vec2(1600.0f, 1000.0f), glm::vec2(-250.0f, -300.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));

		ui._uiComponents.push_back(new Cappuccino::UIText("Exit Game", glm::vec2(1600.0f, 1000.0f), glm::vec2(-250.0f, -500.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));

		for (auto x : ui._uiComponents)
			x->setVisible(false);

		resumeBox = Cappuccino::HitBox(glm::vec3(-20.0f, -60.0f, 0.0f), glm::vec3(175.0f, 20.0f, 200.0f));
		optionsBox = Cappuccino::HitBox(glm::vec3(-20.0f, 0.0f, 0.0f), glm::vec3(175.0f, 20.0f, 200.0f));
		restartBox = Cappuccino::HitBox(glm::vec3(-20.0f, 80.0f, 0.0f), glm::vec3(175.0f, 20.0f, 200.0f));
		menuBox = Cappuccino::HitBox(glm::vec3(-20.0f, 140.0f, 0.0f), glm::vec3(175.0f, 20.0f, 200.0f));
		exitBox = Cappuccino::HitBox(glm::vec3(-20.0f, 220.0f, 0.0f), glm::vec3(175.0f, 20.0f, 200.0f));

		ui._uiComponents.push_back(new Cappuccino::UIScreenQuad({ LOAD_TEXTURE("Shield flare", "flareShield.png", Cappuccino::TextureType::DiffuseMap) }, 0.0f));
		ui._uiComponents.push_back(new Cappuccino::UIScreenQuad({ LOAD_TEXTURE("Health flare", "flareHealth.png", Cappuccino::TextureType::DiffuseMap) }, 0.0f));
	}
}

bool GameplayScene::init() {
	static bool createdPlayer = false;
	if (Options::Assault)
		_testCommando = new Assault(_mainShader, {}, {});
	else if (Options::Commando)
		_testCommando = new Commando(_mainShader, {}, {});
	else if (Options::Demolitionist)
		_testCommando = new Demolitionist(_mainShader, {}, {});
	else if (Options::Scout)
		_testCommando = new Scout(_mainShader, {}, {});

	_testCommando->addAmmo(bullet, bullet2);


	Class::resendLights();
	_testCommando->_rigidBody._position = glm::vec3(-30.0f, 0.0f, -5.0f) + _levelManager.airlocks[_levelManager._currentRoom]->_levelData._respawnPoint;


	//activate members here
	_initialized = true;
	_shouldExit = false;
	_testCommando->setActive(true);

	if (Options::isTutorial())
		_levelManager._currentRoom = 0;

	_levelManager._rooms[_levelManager._currentRoom]->setActive(true);
	_levelManager.airlocks[_levelManager._currentRoom]->setActive(true);


	for (auto& enemy : _enemies)
		enemy->setActive(true);
	for (auto& chests : _chests)
		chests->setActive(true);
	for (auto x : _loot)
		x->setActive(true);
	for (auto x : lamps)
		x->setActive(true);

	_levelManager._testShopTerminal->setActive(false);

	//_chest->setActive(true);

	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	_enemies.clear();
	_chests.clear();

	if (createdPlayer)
		resetObjects();


	_levelManager._testShopTerminal->_player = _testCommando;

	createdPlayer = true;



	return true;
}

bool GameplayScene::exit() {
	//deactivate members here
	_initialized = false;
	_shouldExit = true;
	_testCommando->setActive(false);
	_testCommando->toggleHud(false);

	Options::Assault = false;
	Options::Commando = false;
	Options::Scout = false;
	Options::Demolitionist = false;
	Options::setTutorial(false);

	for (auto& room : _levelManager._rooms) {
		room->setActive(false);
	}
	for (auto& airlock : _levelManager.airlocks) {
		airlock->setActive(false);
	}
	for (auto& chests : _levelManager._chests) {
		if (chests->isActive()) {
			chests->setActive(false);
			_chests.push_back(chests);
		}
	}
	for (auto& enemy : _levelManager._enemyManager._enemies) {
		if (enemy->isActive()) {
			enemy->setActive(false);
			_enemies.push_back(enemy);
		}
	}

	for (auto x : lamps) {
		x->setActive(false);
	}

	for (auto x : _loot) {
		x->setActive(false);
	}
	for (auto x : ui._uiComponents) {
		x->setVisible(false);
	}

	_levelManager._testShopTerminal->setActive(false);

	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);


	return true;
}

void GameplayScene::shootCollisionBehaviour(Enemy* enemy) {
	if (_testCommando->getGun()->getDamage() != 0.0f)
		enemy->hurt(_testCommando->getGun()->getDamage());
	_testCommando->getGun()->specialCollisionBehaviour(_levelManager._enemyManager._enemies);

	//special behaviour if the enemy dies
	if (enemy->dead()) {
		if (!_testCommando->_voiceLines->isEventPlaying((int)VoiceLine::KillingEnemy) &&
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
	if (first) {
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

	if (first) {
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

	if (pause) {

		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		for (auto x : Cappuccino::GameObject::gameObjects)
			x->setPaused(pause);

		if (_testCommando->getHealth() <= 0)
			for (unsigned i = 2; i < ui._uiComponents.size(); i++)
				ui._uiComponents[i]->setVisible(true);
		else
			for (auto& x : ui._uiComponents)
				x->setVisible(true);
		//resume button
		if (cursorBox.checkCollision(resumeBox, resumeBox._position, cursorBox._position) && ui._uiComponents[0]->isVisible()) {
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[0])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

			if (_testCommando->_input.clickListener.leftClicked()) {
				pause = !pause;
				glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				for (auto& x : ui._uiComponents)
					x->setVisible(false);
				for (auto x : Cappuccino::GameObject::gameObjects)
					x->setPaused(pause);
				_testCommando->togglePauseScreen();
			}
		}
		else if (!cursorBox.checkCollision(resumeBox, resumeBox._position, cursorBox._position) && ui._uiComponents[0]->isVisible())
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[0])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

		//options button
		if (cursorBox.checkCollision(optionsBox, optionsBox._position, cursorBox._position) && ui._uiComponents[1]->isVisible()) {
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[1])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

			static bool done = false;
			if (_testCommando->_input.clickListener.leftClicked() && !done) {
				done = true;
				Goptions::toggleGoptions();
			}
			else if (_testCommando->_input.clickListener.leftClicked() && done)
				done = false;

		}
		else if (!cursorBox.checkCollision(optionsBox, optionsBox._position, cursorBox._position) && ui._uiComponents[1]->isVisible())
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[1])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

		//restart button
		if (cursorBox.checkCollision(restartBox, restartBox._position, cursorBox._position)) {
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[2])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

			if (_testCommando->_input.clickListener.leftClicked()) {
				pause = !pause;
				glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				resetObjects();
				for (auto& x : ui._uiComponents)
					x->setVisible(false);
				for (auto x : Cappuccino::GameObject::gameObjects)
					x->setPaused(pause);
				_testCommando->togglePauseScreen();
			}
		}
		else if (!cursorBox.checkCollision(restartBox, restartBox._position, cursorBox._position))
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[2])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

		//menu button
		if (cursorBox.checkCollision(menuBox, menuBox._position, cursorBox._position)) {
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[3])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

			if (_testCommando->_input.clickListener.leftClicked()) {
				pause = !pause;
				resetObjects();
				for (auto& x : ui._uiComponents)
					x->setVisible(false);
				for (auto x : Cappuccino::GameObject::gameObjects)
					x->setPaused(pause);
				_testCommando->togglePauseScreen();
				Cappuccino::SceneManager::changeScene(0);
			}
		}
		else if (!cursorBox.checkCollision(menuBox, menuBox._position, cursorBox._position))
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[3])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

		//exit button
		if (cursorBox.checkCollision(exitBox, exitBox._position, cursorBox._position)) {
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[4])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

			if (_testCommando->_input.clickListener.leftClicked()) {
				for (auto& x : ui._uiComponents)
					x->setVisible(false);
				_testCommando->togglePauseScreen();
				_testCommando->setClose(true);
				exit();
			}
		}
		else if (!cursorBox.checkCollision(exitBox, exitBox._position, cursorBox._position))
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[4])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

	}
	if (!pause) {
		using namespace Cappuccino;

		//update level manager and shader
		_levelManager.update(dt, _testCommando);
		_mainShader->use();
		_mainShader->setUniform("camPos", _testCommando->getCamera()->getPosition());
		Application::_gBufferShader->use();
		Application::_gBufferShader->loadViewMatrix(*_testCommando->getCamera());
		sendGBufferShaderUniforms();

		///REMOVE AFTER TESTING
		{
			//add light button
			static bool pressed = false;
			if (_testCommando->_input.keyboard->keyPressed(KeyEvent::L) && !pressed) {
				pressed = true;
				_lights.emplace_back(glm::vec3(_testCommando->_rigidBody._position.x, _testCommando->_rigidBody._position.y, _testCommando->_rigidBody._position.z + 5.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				resendLights();
			}
			else if (!_testCommando->_input.keyboard->keyPressed(KeyEvent::L))
				pressed = false;
		}

		//enemy logic
		GameObject* hitObject = _testCommando->getFirstIntersect(_testCommando->_testRay);//first object hit
		static float flareAlpha = 0.0f;
		flareAlpha -= dt * 2.0f;

		for (auto& enemy : _levelManager._enemyManager._enemies) {
			if (!enemy->isActive())
				continue;
			enemy->_stateMachine.update(dt, enemy, _testCommando, _levelManager._rooms[_levelManager._currentRoom]);

			Ray enemyRay(normalize(_testCommando->_rigidBody._position - enemy->_rigidBody._position), enemy->_rigidBody._position);
			GameObject* enemyRayObject = enemy->getFirstIntersect(enemyRay);
			//activate enemy if within a trigger volume
			if (_testCommando->checkCollision(enemy->triggerVolume, _testCommando->_rigidBody._position) || enemy->getMaxHP() != enemy->getHP() || enemy->getMaxShield() != enemy->getShield()) {
				if (enemyRayObject == _testCommando)
					enemy->setTrigger(true);
				else
					enemy->setTrigger(false);
			}
			else
				enemy->setTrigger(false);

			{
				static float delay = 0.0f;
				//checks for squelch 
				if (enemy->dead()
					&& !_testCommando->_voiceLines->isEventPlaying((int)VoiceLine::KillingEnemy)
					&& delay < 0.0f) {
					_testCommando->_voiceLines->playEvent((int)VoiceLine::KillingEnemy);
					delay = randomFloat(5.0f, 10.0f);
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
					flareAlpha = 0.25f;
					_testCommando->takeDamage(enemy->getGun()->getDamage());
					bullet->setActive(false);
				}
			}
		}
		if (flareAlpha > 0.0f) {
			if (_testCommando->getShield() > 0.0f) {
				auto shieldFlare = dynamic_cast<UIScreenQuad*>(ui._uiComponents.at(5));
				shieldFlare->setAlpha(flareAlpha);
				shieldFlare->setVisible(true);

				ui._uiComponents.at(6)->setVisible(false);
			}
			else {
				auto healthFlare = dynamic_cast<UIScreenQuad*>(ui._uiComponents.at(6));
				healthFlare->setAlpha(flareAlpha);
				healthFlare->setVisible(true);

				ui._uiComponents.at(5)->setVisible(false);
			}

		}
		else {
			auto shieldFlare = dynamic_cast<UIScreenQuad*>(ui._uiComponents.at(5));
			auto healthFlare = dynamic_cast<UIScreenQuad*>(ui._uiComponents.at(6));

			shieldFlare->setAlpha(0.0f);    healthFlare->setAlpha(0.0f);
			shieldFlare->setVisible(false); healthFlare->setVisible(false);
		}

		for (auto chest : _levelManager._chests) {
			if (_testCommando->checkCollision(chest->_triggerVolume, chest->_rigidBody._position) && _testCommando->_input.keyboard->keyPressed(KeyEvent::E) && !chest->open()) {
				std::vector<Loot*> temp = chest->spawn(10.0f, chest->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f), _sednium, _healthPack, _ammoPack, _bullion);
				for (auto x : temp) {
					_loot.push_back(x);
				}
			}
		}

		if (_testCommando->checkCollision(_levelManager._lootChest->_triggerVolume, _levelManager._lootChest->_rigidBody._position) && _testCommando->_input.keyboard->keyPressed(KeyEvent::E) && !_levelManager._lootChest->open()) {
			std::vector<Loot*> temp = _levelManager._lootChest->spawn(10.0f, _levelManager._lootChest->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f), _sednium, _healthPack, _ammoPack, _bullion);
			for (auto x : temp) {
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
			if (x->isActive()) {
				x->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), dt * 20.0f);
				x->pickup(_testCommando);
			}
		}

		if (_testCommando->getHealth() <= 0) {
			_testCommando->setHealth(0.0f);
			_testCommando->_voiceLines->playEvent((int)VoiceLine::GettingKilled);
			pause = true;
			_testCommando->togglePauseScreen();
		}

		const glm::mat4 view = glm::mat4(glm::mat3(_testCommando->getCamera()->whereAreWeLooking()));
		_skybox->getShader().use();
		_skybox->getShader().setUniform("view", view);

		//Cappuccino::GameObject* hitObject = _testCommando->getFirstIntersect(_testCommando->_testRay);//first object hit

		bool spotted = false;
		for (auto y : GameObject::gameObjects) {//for all game objects
			if (y->id == "Enemy") {//if the object is an enemy
				if (y->isActive() && y == hitObject) {
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

			if (spotted && !_testCommando->_voiceLines->isEventPlaying((int)VoiceLine::SeeingEnemy)
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
	if (_testCommando->_input.keyboard->keyPressed(Cappuccino::KeyEvent::Q) && _pauseDelay <= 0.0f) {
		_pauseDelay = 0.5f;
		pause ^= 1;
		_testCommando->togglePauseScreen();
		if (!pause) {
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			for (unsigned i = 0; i < 5; ++i) {
				ui._uiComponents[i]->setVisible(false);
			}
			for (auto x : Cappuccino::GameObject::gameObjects)
				x->setPaused(pause);
		}
		else {
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	Goptions::update(dt);
}

void GameplayScene::mouseFunction(const double xpos, const double ypos) {
	if (ShopTerminal::cursorLocked)
		firstMouse = true;

	if (firstMouse) {
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}

	const GLfloat xOffset = xpos - lastX;
	const GLfloat yOffset = lastY - ypos;
	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	if (!ShopTerminal::cursorLocked)
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

	for (auto& x : _levelManager._enemyManager._enemies) {
		x->setHealth(x->getMaxHP());
		x->setShield(x->getMaxShield());
		x->setTrigger(false);
	}
}
