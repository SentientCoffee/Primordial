#include "LevelManager.h"
#include "GameplayScene711.h"
#include "Options.h"

// -------------------------------------------------------------------------------------
// ----- Level Manager -----------------------------------------------------------------
// -------------------------------------------------------------------------------------

LevelManager::LevelManager(std::vector<Cappuccino::PointLight>& lights) :
	_lightManager(lights) {}

void LevelManager::update(float dt, Class* player) {
	_lightManager.update(dt);
	_enemyManager.update(dt);

	if (_start || Options::isTutorial()) {
		for (auto x : airlocks) {
			x->setActive(false);
		}
		for (auto x : _rooms) {
			x->setActive(false);
		}

		airlocks[0]->setActive(true);
		airlocks[0]->_rigidBody._position = { -30.0f, -2.0f, -5.0f };

		_entrancesL[1]->_originalLoc = airlocks[0]->_levelData._entrance._exitBox._position + airlocks[0]->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f);
		_entrancesR[1]->_originalLoc = airlocks[0]->_levelData._entrance._exitBox._position + airlocks[0]->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f);
		_entrancesL[1]->_rigidBody._position = _entrancesL[1]->_originalLoc;
		_entrancesR[1]->_rigidBody._position = _entrancesR[1]->_originalLoc;
		_entrancesL[1]->setActive(true);
		_entrancesR[1]->setActive(true);
		_entrancesL[1]->_locked = true;
		_entrancesR[1]->_locked = true;

		_entrancesL[0]->_originalLoc = airlocks[0]->_levelData._exits[0]._exitBox._position + airlocks[0]->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f);
		_entrancesR[0]->_originalLoc = airlocks[0]->_levelData._exits[0]._exitBox._position + airlocks[0]->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f);
		_entrancesL[0]->_rigidBody._position = _entrancesL[0]->_originalLoc;
		_entrancesR[0]->_rigidBody._position = _entrancesR[0]->_originalLoc;
		_entrancesL[0]->setActive(true);
		_entrancesR[0]->setActive(true);
		_entrancesL[0]->_locked = false;
		_entrancesR[0]->_locked = false;


		_lootRoom->_rigidBody._position = glm::vec3(-1000);
		_lootRoom->setActive(true);
		_lootChest->_rigidBody._position = _lootRoom->_rigidBody._position + _lootRoom->_levelData.chests[0];
		_lootChest->_rigidBody._position.y += 2;
		_lootChest->setActive(true);

		_teleporterA->_rigidBody._position = _lootRoom->_rigidBody._position + _lootRoom->_levelData._teleporterLoc[0]._position;
		_teleporterA->_rigidBody._position.y += 1;
		_teleporterA->setActive(true);


		_teleporterB->_rigidBody._position = glm::vec3(-15, 0, 0.0f);
		_teleporterB->setActive(true);

		_start = false;

		_currentRoom = 1;

		if (Options::isTutorial())
		{
			// tutorial room
			_currentRoom = 0;

			// enemies
			int tutTemp = 0;
			for (auto enemies : _enemyManager._enemies)
			{
				if (enemies->_enemyType == "Dummy")
				{
					enemies->setActive(true);
					if (tutTemp == 0)
						enemies->_rigidBody._position = glm::vec3(110.0f, 12.0f, -45.0f);
					else if (tutTemp == 1)
					{
						enemies->_rigidBody.rotateRigid(270.0f);
						enemies->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 270.0f);
						enemies->_rigidBody._position = glm::vec3(130.0f, 12.0f, -67.0f);
					}
					else if (tutTemp == 2)
					{
						enemies->_rigidBody.rotateRigid(270.0f);
						enemies->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 270.0f);
						enemies->_rigidBody._position = glm::vec3(132.0f, 12.0f, -65.0f);
					}
					else if (tutTemp == 3) {
						enemies->_rigidBody.rotateRigid(270.0f);
						enemies->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 270.0f);
						enemies->_rigidBody._position = glm::vec3(132.0f, 12.0f, -69.0f);
					}
					tutTemp++;
				}
			}

			ui._uiComponents.clear();
			ui._uiComponents.push_back(new Cappuccino::UIText("WASD to move", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 800.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
			ui._uiComponents.push_back(new Cappuccino::UIText("Spacebar to jump", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 700.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
			ui._uiComponents.push_back(new Cappuccino::UIText("Q for menu", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 600.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));

			ui._uiComponents.push_back(new Cappuccino::UIText("LMB to shoot", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 800.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
			ui._uiComponents.push_back(new Cappuccino::UIText("Enemy info displayed at top of screen", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 700.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
			ui._uiComponents.push_back(new Cappuccino::UIText("Ammo/Gun on bottom right of screen", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 600.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
			ui._uiComponents.push_back(new Cappuccino::UIText("The pistol has unlimited ammo", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 500.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));

			ui._uiComponents.push_back(new Cappuccino::UIText("HP/Shield on bottom left of screen", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 800.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
			ui._uiComponents.push_back(new Cappuccino::UIText("Shield recovers by being out of combat", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 700.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));

			ui._uiComponents.push_back(new Cappuccino::UIText("Enemies drop three loots:", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 800.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
			ui._uiComponents.push_back(new Cappuccino::UIText("Sednium, Health packs, and Ammo packs", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 700.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
			ui._uiComponents.push_back(new Cappuccino::UIText("Health packs recover health", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 600.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
			ui._uiComponents.push_back(new Cappuccino::UIText("Ammo packs refill your gun", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 500.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));

			ui._uiComponents.push_back(new Cappuccino::UIText("Gravlifts lift you to higher elevation", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 800.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
			ui._uiComponents.push_back(new Cappuccino::UIText("Teleporters send you to the treasure room", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 700.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
			ui._uiComponents.push_back(new Cappuccino::UIText("Activating (E) chests will drop loot", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 600.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));

			ui._uiComponents.push_back(new Cappuccino::UIText("Sednium can be used in the Shop", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 800.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
			ui._uiComponents.push_back(new Cappuccino::UIText("Shops appear in airlocks", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 700.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
			ui._uiComponents.push_back(new Cappuccino::UIText("Health, ammo, and upgrades can be purchased", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1500.0f, 600.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));

			for (auto x : ui._uiComponents)
				x->setVisible(false);

			Options::setTutorial(false);
		}

			_rooms[_currentRoom]->setActive(true);
			_rooms[_currentRoom]->_rigidBody._position = airlocks[0]->_rigidBody._position + airlocks[0]->_levelData._exits[0]._exitBox._position - _rooms[_currentRoom]->_levelData._entrance._exitBox._position;

			// Chests
			for (unsigned r = 0; r < _rooms[_currentRoom]->_levelData.chests.size(); r++) {
				_chests[r]->_rigidBody._position = _rooms[_currentRoom]->_levelData.chests[r] + _rooms[_currentRoom]->_rigidBody._position;
				_chests[r]->_rigidBody._position.y += 2;
				_chests[r]->setActive(true);
				_chests[r]->_opened = false;
			}

			/*
			lights
			*/
			std::vector <glm::vec4> tempLights;
			for (auto x : airlocks[0]->_levelData._lights)
				tempLights.push_back(x + glm::vec4(airlocks[0]->_rigidBody._position, 0.0f));
			for (auto x : _rooms[_currentRoom]->_levelData._lights)
				tempLights.push_back(x + glm::vec4(_rooms[_currentRoom]->_rigidBody._position, 0.0f));
			_lightManager.resetLights(tempLights);
		
	}

	while (_currentRotation >= 360.0f)
		_currentRotation -= 360.0f;

	//tutorial UI
	if (!ui._uiComponents.empty())
	{
		//movement
		if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(5.0f)), glm::vec3(-22.0f, 3.0f, -29.0f))) {

			for (unsigned i = 0; i < 3; i++)
				ui._uiComponents[i]->setVisible(true);
		}
		else
		{
			for (unsigned i = 0; i < 3; i++)
				ui._uiComponents[i]->setVisible(false);
		}
		//enemies and shooting
		if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(5.0f)), glm::vec3(109.0f, 12.0f, -27.0f))) {

			for (unsigned i = 3; i < 7; i++)
				ui._uiComponents[i]->setVisible(true);
		}
		else
		{
			for (unsigned i = 3; i < 7; i++)
				ui._uiComponents[i]->setVisible(false);
		}
		//player stats
		if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(5.0f)), glm::vec3(109.0f, 12.0f, -67.0f))) {

			for (unsigned i = 7; i < 9; i++)
				ui._uiComponents[i]->setVisible(true);
		}
		else
		{
			for (unsigned i = 7; i < 9; i++)
				ui._uiComponents[i]->setVisible(false);
		}
		//loot
		if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(5.0f)), glm::vec3(129.0f, 12.0f, -67.0f))) {

			for (unsigned i = 9; i < 13; i++)
				ui._uiComponents[i]->setVisible(true);
		}
		else
		{
			for (unsigned i = 9; i < 13; i++)
				ui._uiComponents[i]->setVisible(false);
		}
		//special tiles
		if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(5.0f)), glm::vec3(151.0f, 12.0f, -67.0f))) {

			for (unsigned i = 13; i < 16; i++)
				ui._uiComponents[i]->setVisible(true);
		}
		else
		{
			for (unsigned i = 13; i < 16; i++)
				ui._uiComponents[i]->setVisible(false);
		}
		//shop
		if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(5.0f)), glm::vec3(223.0f, 36.0f, -67.0f))) {

			for (unsigned i = 16; i < 19; i++)
				ui._uiComponents[i]->setVisible(true);
		}
		else
		{
			for (unsigned i = 16; i < 19; i++)
				ui._uiComponents[i]->setVisible(false);
		}
	}

	//gravlift triggers
	for (auto x : _rooms[_currentRoom]->_levelData._lifts) {
		if (player->checkCollision(x._areaOfAffect, _rooms[_currentRoom]->_rigidBody._position)) {
			player->_rigidBody.setVelocity(glm::vec3(player->_rigidBody._vel.x, x._liftPower, player->_rigidBody._vel.z));
			Options::Effects->playEvent(Effect::JumpPad);
		}
		for (auto y : _enemyManager._enemies) {
			if (y->isActive()) {
				if (y->checkCollision(x._areaOfAffect, _rooms[_currentRoom]->_rigidBody._position)) {
					y->_rigidBody.setVelocity(glm::vec3(y->_rigidBody._vel.x, x._liftPower, y->_rigidBody._vel.z));
					Options::Effects->playEvent(Effect::JumpPad);
				}
			}
		}
	}


	//teleporter triggers
	if (player->checkCollision(_teleporterA->_areaOfAffect, _teleporterA->_rigidBody._position) && _teleporterA->isActive() && !_teleporterA->_currentDelay) {
		_teleporterB->_currentDelay += dt;
		player->_rigidBody._position = _teleporterB->_rigidBody._position;
		player->_rigidBody._position.y += 2.1;
		_lootChest->_opened = false;
		_teleporterB->setActive(false);

		Options::Effects->playEvent(Effect::Teleport);
	}
	if (player->checkCollision(_teleporterB->_areaOfAffect, _teleporterB->_rigidBody._position) && _teleporterB->isActive() && !_teleporterB->_currentDelay) {
		_teleporterA->_currentDelay += dt;
		player->_rigidBody._position = _teleporterA->_rigidBody._position;
		player->_rigidBody._position.y += 2.1;

		Options::Effects->playEvent(Effect::Teleport);
	}

	//hurtbox triggers
	for (auto x : _rooms[_currentRoom]->_levelData._hurtboxes) {
		if (player->checkCollision(x._hurtBox, _rooms[_currentRoom]->_rigidBody._position)) {
			player->takeDamage(x._damagePerSecond * dt);
		}
		for (auto y : _enemyManager._enemies) {
			if (y->isActive()) {
				if (y->checkCollision(x._hurtBox, _rooms[_currentRoom]->_rigidBody._position)) {
					y->hurt(x._damagePerSecond * dt);
				}
			}
		}
	}

	// Door open/close lerping
	for (unsigned i = 0; i < _entrancesL.size(); i++) {
		if (_entrancesL[i]->isActive()) {
			if (!_entrancesL[i]->_locked) {
				auto dir = glm::vec3(0.0f);

				if (_entrancesL[i]->_rotation == 90.0f)
					dir = glm::vec3(-1.0f, 0.0f, 0.0f);
				else if (_entrancesL[i]->_rotation == 180.0f)
					dir = glm::vec3(0.0f, 0.0f, -1.0f);
				else if (_entrancesL[i]->_rotation == 270.0f)
					dir = glm::vec3(1.0f, 0.0f, 0.0f);
				else
					dir = glm::vec3(0.0f, 0.0f, 1.0f);

				if (glm::distance(_entrancesL[i]->_originalLoc, player->_rigidBody._position) <= 5.0f) {

					if (glm::distance(_entrancesL[i]->_rigidBody._position, _entrancesR[i]->_rigidBody._position) <= 2.5f) {
						_entrancesL[i]->_rigidBody.addPosition(dir * dt * 10.0f);
						_entrancesR[i]->_rigidBody.addPosition(-dir * dt * 10.0f);

					}
					if (!Options::Effects->isEventPlaying(Effect::AirlockDoor))
						Options::Effects->playEvent(Effect::AirlockDoor);
				}
				else if (glm::distance(_entrancesL[i]->_rigidBody._position, _entrancesL[i]->_originalLoc) != 0.0f) {
					_entrancesL[i]->_rigidBody.addPosition(-dir * dt * 10.0f);
					_entrancesR[i]->_rigidBody.addPosition(dir * dt * 10.0f);

					if (!Options::Effects->isEventPlaying(Effect::AirlockDoor))
						Options::Effects->playEvent(Effect::AirlockDoor);
				}
			}
			else {
				_entrancesL[i]->_rigidBody._position = _entrancesL[i]->_originalLoc;
				_entrancesR[i]->_rigidBody._position = _entrancesR[i]->_originalLoc;

				if (player->checkCollision(_entrancesL[i]->_rigidBody._hitBoxes[0], _entrancesL[i]->_rigidBody._position) || player->checkCollision(_entrancesR[i]->_rigidBody._hitBoxes[0], _entrancesR[i]->_rigidBody._position))
				{
					player->_rigidBody._accel = glm::vec3(0.0f, player->_rigidBody._accel.y, 0.0f);
					player->_rigidBody._vel = glm::vec3(0.0f, player->_rigidBody._vel.y, 0.0f);
				}
			}
		}
	}

	//room handling should be dynamic
	for (unsigned x = 0; x < _rooms.size(); x++) {
		if (_rooms[x]->isActive()) {
			for (auto y : _rooms[x]->_levelData._exits) {
				if (player->checkCollision(y._exitBox, _rooms[x]->_rigidBody._position)) {
					_rooms[x]->setActive(false);
					_rooms[x]->reset();
					ui._uiComponents.clear();
					for (unsigned i = 1; i < _entrancesL.size(); i++)
					{
						_entrancesL[i]->_locked = true;
						_entrancesR[i]->_locked = true;
					}
					for (auto z : airlocks) {
						if (z->isActive()) {
							if (player->checkCollision(z->_levelData._entrance._exitBox, z->_rigidBody._position)) {
								//std::cout << "Handling Room\n";
								unsigned temp = Cappuccino::randomInt(1, (int)_rooms.size() - 1);
								_currentRotation += y._rotation;
								_rooms[temp]->rotate(_currentRotation);
								_rooms[temp]->_rigidBody._position = z->_rigidBody._position + z->_levelData._exits[0]._exitBox._position - _rooms[temp]->_levelData._entrance._exitBox._position;
								_rooms[temp]->setActive(true);
								_currentRoom = temp;

								auto tempRotation = _entrancesL[0]->_rotation - z->_currentRotation;
								if (tempRotation <= 0.0f)
									tempRotation += 360.0f;
								else if (tempRotation >= 360.0f)
									tempRotation -= 360.0f;
								_entrancesL[0]->_originalLoc = z->_levelData._exits[0]._exitBox._position + z->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f);
								_entrancesL[0]->_rigidBody._position = _entrancesL[0]->_originalLoc;
								_entrancesL[0]->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), tempRotation);
								_entrancesL[0]->_rigidBody.rotateRigid(tempRotation);
								_entrancesL[0]->_rotation = z->_currentRotation;
								_entrancesL[0]->setActive(true);

								_entrancesR[0]->_originalLoc = z->_levelData._exits[0]._exitBox._position + z->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f);
								_entrancesR[0]->_rigidBody._position = _entrancesR[0]->_originalLoc;
								_entrancesR[0]->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), tempRotation);
								_entrancesR[0]->_rigidBody.rotateRigid(tempRotation);
								_entrancesR[0]->_rotation = z->_currentRotation;
								_entrancesR[0]->setActive(true);

								_entrancesL[0]->_locked = false;
								_entrancesR[0]->_locked = false;

								// Lights
								//std::cout << "Handling Lights\n";
								std::vector<glm::vec4> lightPos;
								for (auto g : _rooms[temp]->_levelData._lights)
									lightPos.push_back(g + glm::vec4(_rooms[temp]->_rigidBody._position, 0.0f));
								for (auto h : airlocks) {
									if (h->isActive())
										for (auto i : h->_levelData._lights)
											lightPos.push_back(i + glm::vec4(h->_rigidBody._position, 0.0f));
								}
								_lightManager.resetLights(lightPos);

								// Chests
								for (unsigned r = 0; r < _rooms[_currentRoom]->_levelData.chests.size(); r++) {
									_chests[r]->_rigidBody._position = _rooms[_currentRoom]->_levelData.chests[r] + _rooms[_currentRoom]->_rigidBody._position;
									_chests[r]->_rigidBody._position.y += 2;
									_chests[r]->setActive(true);
									_chests[r]->_opened = false;
								}


								// Enemy spawning
								//std::cout << "Spawning Enemies\n";
								for (unsigned r = 0; r < _enemyManager._enemies.size(); r++) {		//reset all enemies
									_enemyManager._enemies[r]->setActive(false);
									_enemyManager._enemies[r]->resetEnemy();
								}

								int factionType = Cappuccino::randomInt(0, 2);
								unsigned usedSpawnPoints = 0;
								while (_rooms[temp]->_spawnData._usedWeight < _rooms[temp]->_spawnData._weight) {
									int randomSpawnPoint = Cappuccino::randomInt(0, (int)_rooms[temp]->_spawnData._spawnPoints.size() - 1);
									if (!_rooms[temp]->_spawnData._spawnPoints[randomSpawnPoint]._spawned) {
										glm::vec3 enemySpawns = (_rooms[temp]->_rigidBody._position + _rooms[temp]->_spawnData._spawnPoints[randomSpawnPoint]._position);
										_rooms[temp]->_spawnData._usedWeight += _enemyManager.spawnEnemy(enemySpawns, factionType);
										usedSpawnPoints++;
										_rooms[temp]->_spawnData._spawnPoints[randomSpawnPoint]._spawned = true;
									}
									if (usedSpawnPoints >= _rooms[temp]->_spawnData._spawnPoints.size())
										break;
								}

								_roomEnemies.clear();
								for (auto e : _enemyManager._enemies) {
									if (e->isActive()) {
										_roomEnemies.push_back(e);
									}
								}
								break;
							}

							z->reset();
							z->setActive(false);
						}

					}
				}
			}
		}
	}
	// airlock
	for (unsigned x = 0; x < airlocks.size(); x++) {
		if (airlocks[x]->isActive()) {
			if (player->checkCollision(airlocks[x]->_levelData._exits[0]._exitBox, airlocks[x]->_rigidBody._position)) {
				ShopTerminal::damageBought = false;
				ShopTerminal::ammoBought = false;

				// std::cout << "Starting Airlock" << std::endl;
				for (auto airlock : airlocks) {
					airlock->reset();
					airlock->setActive(false);
				}
				_entrancesL[0]->_locked = true;
				_entrancesR[0]->_locked = true;
				_testShopTerminal->setActive(false);
				for (auto z : _rooms) {
					if (z->isActive())
					{

						// Teleporter spawning
						if constexpr (true)
						{
							_teleporterB->setActive(true);
							_teleporterB->_rigidBody._position = z->_levelData._teleporterLoc[0]._position + z->_rigidBody._position;
							_teleporterB->_rigidBody._position.y += 1;
							for (auto chestPos : _lootRoom->_levelData.chests)
								for (auto y : _chests)
									if (y->_rigidBody._position == chestPos)
										y->setClose(true);
						}
						else {
							_teleporterA->setActive(false);
						}


						for (unsigned n = 0; n < z->_levelData._exits.size(); n++)
						{
							for (unsigned i = 0; i < airlocks.size(); i++) {
								if (!airlocks[i]->isActive()) {
									airlocks[i]->rotate(_currentRotation + z->_levelData._exits[n]._rotation);
									airlocks[i]->_rigidBody._position = z->_rigidBody._position + z->_levelData._exits[n]._exitBox._position - airlocks[i]->_levelData._entrance._exitBox._position;
									airlocks[i]->setActive(true);

									auto tempRotation = _entrancesL[n + 1]->_rotation - (_currentRotation + z->_levelData._exits[n]._rotation);
									if (tempRotation >= 360.0f)
										tempRotation -= 360.0f;
									else if (tempRotation <= 0.0f)
										tempRotation += 360.0f;
									_entrancesL[n + 1]->_originalLoc = airlocks[i]->_levelData._entrance._exitBox._position + airlocks[i]->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f);
									_entrancesL[n + 1]->_rigidBody._position = _entrancesL[n + 1]->_originalLoc;
									_entrancesL[n + 1]->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), tempRotation);
									_entrancesL[n + 1]->_rigidBody.rotateRigid(tempRotation);
									_entrancesL[n + 1]->_rotation = _currentRotation + z->_levelData._exits[n]._rotation;
									_entrancesL[n + 1]->setActive(true);

									_entrancesR[n + 1]->_originalLoc = airlocks[i]->_levelData._entrance._exitBox._position + airlocks[i]->_rigidBody._position + glm::vec3(0.0f, 1.0f, 0.0f);
									_entrancesR[n + 1]->_rigidBody._position = _entrancesR[n + 1]->_originalLoc;
									_entrancesR[n + 1]->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), tempRotation);
									_entrancesR[n + 1]->_rigidBody.rotateRigid(tempRotation);
									_entrancesR[n + 1]->_rotation = _currentRotation + z->_levelData._exits[n]._rotation;
									_entrancesR[n + 1]->setActive(true);

									_entrancesL[n + 1]->_locked = false;
									_entrancesR[n + 1]->_locked = false;

									//std::cout << "Starting Lights" << std::endl;
									/*
									Lights
									*/
									// Teleporter spawning
									if constexpr (true)
									{
										_teleporterB->setActive(true);
										_teleporterB->_rigidBody._position = z->_levelData._teleporterLoc[0]._position + z->_rigidBody._position;
										_teleporterB->_rigidBody._position.y += 1;
										for (const auto& chestPos : _lootRoom->_levelData.chests)
											for (auto y : _chests)
												if (y->_rigidBody._position == chestPos)
													y->setClose(true);
									}
									else {
										_teleporterA->setActive(false);
									}

									// Lights
									// std::cout << "Starting Lights" << std::endl;
									std::vector<glm::vec4> lightPos;
									for (const auto y : z->_levelData._lights)
										lightPos.push_back(y + glm::vec4(z->_rigidBody._position, 0.0f));

									for (const auto y : airlocks[x]->_levelData._lights)
										lightPos.push_back(y + glm::vec4(airlocks[x]->_rigidBody._position, 0.0f));
									_lightManager.resetLights(lightPos);
									Options::Music->getEvent(MusicManager::getCurrentPlaying())->setParameterByName("parameter:/levelClear", 1);
									int ran = Cappuccino::randomInt(1, 2);
									Options::Music->getEvent(ran)->setParameterByName("parameter:/levelClear", 0);
									MusicManager::playSong(ran);
									Options::Music->stopEvent(ran == 1 ? 2 : 1);

									if (Cappuccino::randomBool()) {
										_testShopTerminal->setActive(true);
										_testShopTerminal->_rigidBody._position = airlocks[x]->_levelData._shopLocation + airlocks[x]->_rigidBody._position;
										_testShopTerminal->_rigidBody.rotateRigid(180.0f);
									}
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}



// -------------------------------------------------------------------------------------
// ----- Light Manager -----------------------------------------------------------------
// -------------------------------------------------------------------------------------

LightManager::LightManager(std::vector<Cappuccino::PointLight>& lights) {
	_light = &lights;
}

void LightManager::update(float dt) {}

void LightManager::resetLights(const std::vector<glm::vec4>& lightProperties) const {
	for (unsigned i = 0; i < lightProperties.size(); i++) {
		glm::vec4 newLight = lightProperties[i];
		newLight.z += 5;
		_light->at(i)._pos = { newLight.x, newLight.y, newLight.z };
		_light->at(i)._isActive = true;
		_light->at(i).setShadowCaster(lightProperties[i].w == 1.0f ? true : false);
	}

	// start at the back of the list, this is where inactive lights are
	for (unsigned i = lightProperties.size(); i < _light->size(); i++) {
		_light->at(i)._isActive = false;
		_light->at(i).setShadowCaster(false);
	}

	GameplayScene::resendLights();
}

// -------------------------------------------------------------------------------------
// ----- Enemy Manager -----------------------------------------------------------------
// -------------------------------------------------------------------------------------

void EnemyManager::update(float dt) {
	if (start) {
		for (auto enemy : _enemies) {
			enemy->setActive(false);
		}
		start = false;
	}
}

float EnemyManager::spawnEnemy(const glm::vec3 position, const int type) {
	const int enemy = Cappuccino::randomInt(0, 1);
	std::string myEnemy;

	if (type == 0) {//robot
		if (enemy == 0) {//Sentry
			myEnemy = "Sentry";
			for (unsigned i = 0; i < _enemies.size(); i++) {
				if (_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					_enemies[i]->_rigidBody._position.y += 2;
					break;
				}
			}
			return 3.0f;
		}
		if (enemy == 1) {//Sentinel
			myEnemy = "RoboGunner";
			for (unsigned i = 0; i < _enemies.size(); i++) {
				if (_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					break;
				}
			}
			return 1.5f;
		}
	}
	else if (type == 1)//raiders
	{
		if (enemy == 0) {//Grunt
			myEnemy = "Grunt";
			for (unsigned i = 0; i < _enemies.size(); i++) {
				if (_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					break;
				}
			}
			return 1.0f;
		}
		if (enemy == 1) {//Captain
			myEnemy = "Captain";
			for (unsigned i = 0; i < _enemies.size(); i++) {
				if (_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					break;
				}
			}
			return 2.0f;
		}
	}
	else if (type == 2)//aliens
	{
		if (enemy == 0) {//Ghoul
			myEnemy = "Ghoul";
			for (unsigned i = 0; i < _enemies.size(); i++) {
				if (_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					break;
				}
			}
			return 0.5f;
		}
		if (enemy == 1) {//Squelch
			myEnemy = "Squelch";
			for (unsigned i = 0; i < _enemies.size(); i++) {
				if (_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					break;
				}
			}
			return 1.0f;
		}
	}
	return 0.0f;
}

