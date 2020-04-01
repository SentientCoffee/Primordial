#include "LevelManager.h"
#include "GameplayScene711.h"

// -------------------------------------------------------------------------------------
// ----- Level Manager -----------------------------------------------------------------
// -------------------------------------------------------------------------------------

LevelManager::LevelManager(std::vector<Cappuccino::PointLight>& lights) :
	_lightManager(lights) {}

void LevelManager::update(float dt, Class* player) {
	_lightManager.update(dt);
	_enemyManager.update(dt);

	if (_start || _tutorial) {
		for(auto x : airlocks) {
			x->setActive(false);
		}
		for(auto x : _rooms) {
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


		_start = false;

		if (_tutorial)
		{
			// tutorial room
			_rooms[0]->setActive(true);
			_rooms[0]->_rigidBody._position = airlocks[0]->_rigidBody._position + airlocks[0]->_levelData._exits[0]._exitBox._position - _rooms[0]->_levelData._entrance._exitBox._position;

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

			// chests
			for (unsigned r = 0; r < _rooms[0]->_levelData.chests.size(); r++) {
				_chests[r]->_rigidBody._position = _rooms[0]->_levelData.chests[r] + _rooms[0]->_rigidBody._position;
				_chests[r]->_rigidBody._position.y += 2;
				_chests[r]->setActive(true);
				_chests[r]->_opened = false;
			}

			// lights
			std::vector <glm::vec3> tempLights;
			for (auto x : airlocks[0]->_levelData._lights)
				tempLights.push_back(x + airlocks[0]->_rigidBody._position);
			for (auto x : _rooms[0]->_levelData._lights)
				tempLights.push_back(x + _rooms[0]->_rigidBody._position);
			_lightManager.resetLights(tempLights);

			_tutorial = false;
		}
		else
		{

			_rooms[_currentRoom]->setActive(true);
			_rooms[_currentRoom]->_rigidBody._position = airlocks[0]->_rigidBody._position + airlocks[0]->_levelData._exits[0]._exitBox._position - _rooms[_currentRoom]->_levelData._entrance._exitBox._position;

			/*
			Chests
			*/
			for (unsigned r = 0; r < _rooms[_currentRoom]->_levelData.chests.size(); r++) {
				_chests[r]->_rigidBody._position = _rooms[_currentRoom]->_levelData.chests[r] + _rooms[_currentRoom]->_rigidBody._position;
				_chests[r]->_rigidBody._position.y += 2;
				_chests[r]->setActive(true);
				_chests[r]->_opened = false;
			}

			/*
			lights
			*/
			std::vector <glm::vec3> tempLights;
			for (auto x : airlocks[0]->_levelData._lights)
				tempLights.push_back(x + airlocks[0]->_rigidBody._position);
			for (auto x : _rooms[_currentRoom]->_levelData._lights)
				tempLights.push_back(x + _rooms[_currentRoom]->_rigidBody._position);
			_lightManager.resetLights(tempLights);
		}
	}

	while(_currentRotation >= 360.0f)
		_currentRotation -= 360.0f;

	for(auto x : _rooms[_currentRoom]->_levelData._lifts) {
		if(player->checkCollision(x._areaOfAffect, _rooms[_currentRoom]->_rigidBody._position)) {
			player->_rigidBody.setVelocity(glm::vec3(player->_rigidBody._vel.x, x._liftPower, player->_rigidBody._vel.z));
		}
		for(auto y : _enemyManager._enemies) {
			if(y->isActive()) {
				if(y->checkCollision(x._areaOfAffect, _rooms[_currentRoom]->_rigidBody._position)) {
					y->_rigidBody.setVelocity(glm::vec3(y->_rigidBody._vel.x, x._liftPower, y->_rigidBody._vel.z));
				}
			}
		}
	}
	for(auto x : _rooms[_currentRoom]->_levelData._hurtboxes) {
		if(player->checkCollision(x._hurtBox, _rooms[_currentRoom]->_rigidBody._position)) {
			player->takeDamage(x._damagePerSecond * dt);
		}
		for(auto y : _enemyManager._enemies) {
			if(y->isActive()) {
				if(y->checkCollision(x._hurtBox, _rooms[_currentRoom]->_rigidBody._position)) {
					y->hurt(x._damagePerSecond * dt);
				}
			}
		}
	}

	// Door open/close lerping
	for(unsigned i = 0; i < _entrancesL.size(); i++) {
		if(_entrancesL[i]->isActive()) {
			if(!_entrancesL[i]->_locked) {
				auto dir = glm::vec3(0.0f);

				if(_entrancesL[i]->_rotation == 90.0f)
					dir = glm::vec3(-1.0f, 0.0f, 0.0f);
				else if(_entrancesL[i]->_rotation == 180.0f)
					dir = glm::vec3(0.0f, 0.0f, -1.0f);
				else if(_entrancesL[i]->_rotation == 270.0f)
					dir = glm::vec3(1.0f, 0.0f, 0.0f);
				else
					dir = glm::vec3(0.0f, 0.0f, 1.0f);

				if(glm::distance(_entrancesL[i]->_originalLoc, player->_rigidBody._position) <= 5.0f) {

					if(glm::distance(_entrancesL[i]->_rigidBody._position, _entrancesR[i]->_rigidBody._position) <= 2.5f) {
						_entrancesL[i]->_rigidBody.addPosition(dir * dt * 10.0f);
						_entrancesR[i]->_rigidBody.addPosition(-dir * dt * 10.0f);
					}
				}
				else if(glm::distance(_entrancesL[i]->_rigidBody._position, _entrancesL[i]->_originalLoc) != 0.0f) {
					_entrancesL[i]->_rigidBody.addPosition(-dir * dt * 10.0f);
					_entrancesR[i]->_rigidBody.addPosition(dir * dt * 10.0f);
				}
			}
			else {
				_entrancesL[i]->_rigidBody._position = _entrancesL[i]->_originalLoc;
				_entrancesR[i]->_rigidBody._position = _entrancesR[i]->_originalLoc;

				if(player->checkCollision(_entrancesL[i]->_rigidBody._hitBoxes[0], _entrancesL[i]->_rigidBody._position) || player->checkCollision(_entrancesR[i]->_rigidBody._hitBoxes[0], _entrancesR[i]->_rigidBody._position)) {
					player->_rigidBody._accel = glm::vec3(0.0f, player->_rigidBody._accel.y, 0.0f);
				}
			}
		}
	}

	//room handling should be dynamic
	for(unsigned x = 0; x < _rooms.size(); x++) {
		if(_rooms[x]->isActive()) {
			for(auto y : _rooms[x]->_levelData._exits) {
				if(player->checkCollision(y._exitBox, _rooms[x]->_rigidBody._position)) {
					_rooms[x]->setActive(false);
					_rooms[x]->reset();
					for(auto z : airlocks) {
						if(z->isActive()) {
							if(player->checkCollision(z->_levelData._entrance._exitBox, z->_rigidBody._position)) {
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
								std::vector<glm::vec3> lightPos;
								for(auto g : _rooms[temp]->_levelData._lights)
									lightPos.push_back(g + _rooms[temp]->_rigidBody._position);
								for(auto h : airlocks) {
									if(h->isActive())
										for(auto i : h->_levelData._lights)
											lightPos.push_back(i + h->_rigidBody._position);
								}
								_lightManager.resetLights(lightPos);
								
								// Chests
								for(unsigned r = 0; r < _rooms[_currentRoom]->_levelData.chests.size(); r++) {
									_chests[r]->_rigidBody._position = _rooms[_currentRoom]->_levelData.chests[r] + _rooms[_currentRoom]->_rigidBody._position;
									_chests[r]->_rigidBody._position.y += 2;
									_chests[r]->setActive(true);
									_chests[r]->_opened = false;
								}

								// Enemy spawning
								//std::cout << "Spawning Enemies\n";
								for(unsigned r = 0; r < _enemyManager._enemies.size(); r++) {		//reset all enemies
									_enemyManager._enemies[r]->setActive(false);
									_enemyManager._enemies[r]->resetEnemy();
								}

								int factionType = Cappuccino::randomInt(0, 2);
								unsigned usedSpawnPoints = 0;
								while(_rooms[temp]->_spawnData._usedWeight < _rooms[temp]->_spawnData._weight) {
									int randomSpawnPoint = Cappuccino::randomInt(0, (int)_rooms[temp]->_spawnData._spawnPoints.size() - 1);
									if(!_rooms[temp]->_spawnData._spawnPoints[randomSpawnPoint]._spawned) {
										glm::vec3 enemySpawns = (_rooms[temp]->_rigidBody._position + _rooms[temp]->_spawnData._spawnPoints[randomSpawnPoint]._position);
										_rooms[temp]->_spawnData._usedWeight += _enemyManager.spawnEnemy(enemySpawns, factionType);
										usedSpawnPoints++;
										_rooms[temp]->_spawnData._spawnPoints[randomSpawnPoint]._spawned = true;
									}
									if(usedSpawnPoints >= _rooms[temp]->_spawnData._spawnPoints.size())
										break;
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

	////airlock
	for(unsigned x = 0; x < airlocks.size(); x++) {
		if(airlocks[x]->isActive()) {
			if(player->checkCollision(airlocks[x]->_levelData._exits[0]._exitBox, airlocks[x]->_rigidBody._position)) {
				ShopTerminal::damageBought = false;
				
				// std::cout << "Starting Airlock" << std::endl;
				for(auto& airlock : airlocks) {
					airlock->reset();
					airlock->setActive(false);
				}
				_entrancesL[0]->_locked = true;
				_entrancesR[0]->_locked = true;
				_testShopTerminal->setActive(false);
				for(auto room : _rooms) {
					if(room->isActive()) {
						for(unsigned n = 0; n < room->_levelData._exits.size(); n++) {
							for(auto airlock : airlocks) {
								if(!airlock->isActive()) {
									airlock->rotate(_currentRotation + room->_levelData._exits[n]._rotation);
									airlock->_rigidBody._position = room->_rigidBody._position + room->_levelData._exits[n]._exitBox._position - airlock->_levelData._entrance._exitBox._position;
									airlock->setActive(true);

									// Lights
									// std::cout << "Starting Lights" << std::endl;
									std::vector<glm::vec3> lightPos;
									for(auto y : room->_levelData._lights)
										lightPos.push_back(y + room->_rigidBody._position);
									for(auto y : airlock->_levelData._lights)
										lightPos.push_back(y + airlock->_rigidBody._position);
									_lightManager.resetLights(lightPos);

									if(Cappuccino::randomBool()) {
										_testShopTerminal->setActive(true);
										_testShopTerminal->_rigidBody._position = airlock->_levelData._shopLocation + airlock->_rigidBody._position;
									}
									break;
								}
							}
						}
					}
				}
				//std::cout << "Airlock Done" << std::endl;
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

void LightManager::resetLights(std::vector<glm::vec3>& lightPos) const {
	for(auto& pointLight : *_light) {
		pointLight._pos = glm::vec3(0, -10000, 0);
	}

	for(unsigned i = 0; i < lightPos.size(); i++) {
		glm::vec3 newLightPos = lightPos[i];
		newLightPos.z += 5;
		_light->at(i)._pos = newLightPos;
		_light->at(i)._isActive = true;
	}

	//start at the back of the list, this is where inactive lights are
	for (unsigned i = lightPos.size() - 1; i < _light->size(); i++)
		_light->at(i)._isActive = false;

	GameplayScene::resendLights();
}

// -------------------------------------------------------------------------------------
// ----- Enemy Manager -----------------------------------------------------------------
// -------------------------------------------------------------------------------------

void EnemyManager::update(float dt) {
	if(start) {
		for(auto& enemy : _enemies) {
			enemy->setActive(false);
		}
		start = false;
	}
}

float EnemyManager::spawnEnemy(const glm::vec3 position, const int type) {
	const int enemy = Cappuccino::randomInt(0, 1);
	std::string myEnemy;
	
	if(type == 0) {//robot
		if(enemy == 0) {//Sentry
			myEnemy = "Sentry";
			for(unsigned i = 0; i < _enemies.size(); i++) {
				if(_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					_enemies[i]->_rigidBody._position.y += 2;
					break;
				}
			}
			return 3.0f;
		}
		if(enemy == 1) {//Sentinel
			myEnemy = "RoboGunner";
			for(unsigned i = 0; i < _enemies.size(); i++) {
				if(_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					break;
				}
			}
			return 1.5f;
		}
	}
	else if(type == 1)//raiders
	{
		if(enemy == 0) {//Grunt
			myEnemy = "Grunt";
			for(unsigned i = 0; i < _enemies.size(); i++) {
				if(_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					break;
				}
			}
			return 1.0f;
		}
		if(enemy == 1) {//Captain
			myEnemy = "Captain";
			for(unsigned i = 0; i < _enemies.size(); i++) {
				if(_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					break;
				}
			}
			return 2.0f;
		}
	}
	else if(type == 2)//aliens
	{
		if(enemy == 0) {//Ghoul
			myEnemy = "Ghoul";
			for(unsigned i = 0; i < _enemies.size(); i++) {
				if(_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					break;
				}
			}
			return 0.5f;
		}
		if(enemy == 1) {//Squelch
			myEnemy = "Squelch";
			for(unsigned i = 0; i < _enemies.size(); i++) {
				if(_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
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

