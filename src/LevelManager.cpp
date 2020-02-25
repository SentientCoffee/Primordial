#include "..\include\LevelManager.h"
#include "Cappuccino/CappMacros.h"
#include "GameplayScene711.h"

LevelManager::LevelManager(std::vector<Cappuccino::PointLight>& lights)
	:_lightManager(lights)
{
	
}

void LevelManager::update(float dt, Cappuccino::RigidBody& player)
{
	_lightManager.update(dt);
	_enemyManager.update(dt);

	if (_start){
		for (auto x : airlocks)
			x->setActive(false);
		for (auto x : rooms)
			x->setActive(false);
		
		airlocks[0]->setActive(true);
		airlocks[0]->_rigidBody._position = (glm::vec3(-30.0f, -2.0f, -5.0f));
		
		rooms[_currentRoom]->setActive(true);
		rooms[_currentRoom]->_rigidBody._position = airlocks[0]->_rigidBody._position+airlocks[0]->_levelData._exits[0]._exitBox._position - rooms[_currentRoom]->_levelData._entrance._exitBox._position;
		_start = false;

		/*
		Chests
		*/
		for (unsigned r = 0; r < rooms[_currentRoom]->_levelData.chests.size(); r++) {
			_chests[r]->_rigidBody._position = rooms[_currentRoom]->_levelData.chests[r] + rooms[_currentRoom]->_rigidBody._position;
			_chests[r]->_rigidBody._position.y += 2;
			_chests[r]->setActive(true);
			_chests[r]->_opened = false;
		}
		
		/*
		lights
		*/
		std::vector <glm::vec3> tempLights;
		for (auto x : airlocks[0]->_levelData._lights)
			tempLights.push_back(x+airlocks[0]->_rigidBody._position);
		for(auto x : rooms[_currentRoom]->_levelData._lights)
			tempLights.push_back(x+rooms[_currentRoom]->_rigidBody._position);
		_lightManager.resetLights(tempLights);
	}

	while (_currentRotation >= 360.0f)
		_currentRotation -= 360.0f;
	
	//room handling should be dynamic
	for (unsigned x = 0; x < rooms.size(); x++)
		if (rooms[x]->isActive()) {
			for (auto y : rooms[x]->_levelData._exits)
				if (player.checkCollision(y._exitBox, rooms[x]->_rigidBody._position)){
					rooms[x]->setActive(false);
					rooms[x]->reset();
					for (auto z : airlocks){
						if(z->isActive()){
							if (player.checkCollision(z->_levelData._entrance._exitBox, z->_rigidBody._position)) {
								//std::cout << "Handling Room\n";
								unsigned temp = Cappuccino::randomInt(0, rooms.size()-1);
								_currentRotation += y.rotation;
								rooms[temp]->rotate(_currentRotation);
								rooms[temp]->_rigidBody._position = z->_rigidBody._position + z->_levelData._exits[0]._exitBox._position - rooms[temp]->_levelData._entrance._exitBox._position;
								rooms[temp]->setActive(true);
								_currentRoom = temp;

								/*
								Lights
								*/
								//std::cout << "Handling Lights\n";
								std::vector<glm::vec3> lightPos;
								for (auto g : rooms[temp]->_levelData._lights)
									lightPos.push_back(g+rooms[temp]->_rigidBody._position);
								for (auto h : airlocks) {
									if(h->isActive())
										for(auto i : h->_levelData._lights)
											lightPos.push_back(i+h->_rigidBody._position);
								}
								_lightManager.resetLights(lightPos);
								/*
								Chests
								*/

								for (unsigned r = 0; r < rooms[_currentRoom]->_levelData.chests.size(); r++) {
									_chests[r]->_rigidBody._position = rooms[_currentRoom]->_levelData.chests[r] + rooms[_currentRoom]->_rigidBody._position;
									_chests[r]->_rigidBody._position.y += 2;
									_chests[r]->setActive(true);
									_chests[r]->_opened = false;
								}
								/*
								Enemy spawning
								*/
								//std::cout << "Spawning Enemies\n";
								for (unsigned r = 0; r < _enemyManager._enemies.size(); r++)//reset all enemies
									_enemyManager._enemies[r]->setActive(false);
								unsigned factionType = Cappuccino::randomInt(0,2);
								unsigned usedSpawnPoints = 0;
								while (rooms[temp]->_spawnData._usedWeight < rooms[temp]->_spawnData._weight) {
									int randomSpawnPoint = Cappuccino::randomInt(0, rooms[temp]->_spawnData._spawnPoints.size()-1);
									if (!rooms[temp]->_spawnData._spawnPoints[randomSpawnPoint]._spawned) {
										glm::vec3 enemySpawns =(rooms[temp]->_rigidBody._position+rooms[temp]->_spawnData._spawnPoints[randomSpawnPoint]._position);
										rooms[temp]->_spawnData._usedWeight+= _enemyManager.spawnEnemy(enemySpawns, (factionType));
										usedSpawnPoints++;
										rooms[temp]->_spawnData._spawnPoints[randomSpawnPoint]._spawned = true;
									}
									if (usedSpawnPoints >= rooms[temp]->_spawnData._spawnPoints.size())
										break;
								}
								break;
							}
							else {
								z->reset();
								z->setActive(false);
							}
						}
					}
					
				}					
		}
	
	////airlock
	for (unsigned x = 0; x < airlocks.size(); x++){
		if (airlocks[x]->isActive()){
			if (player.checkCollision(airlocks[x]->_levelData._exits[0]._exitBox, airlocks[x]->_rigidBody._position)){
				for (unsigned i = 0; i < airlocks.size(); i++){
					airlocks[i]->reset();
					airlocks[i]->setActive(false);
				}
				_testShopTerminal->setActive(false);
				for (auto z : rooms){
					if (z->isActive())
						for (unsigned n = 0; n < z->_levelData._exits.size();n++)
							for (unsigned i = 0; i < airlocks.size(); i++)
								if (!airlocks[i]->isActive()){
									airlocks[i]->rotate(_currentRotation + z->_levelData._exits[n].rotation);
									airlocks[i]->_rigidBody._position = z->_rigidBody._position + z->_levelData._exits[n]._exitBox._position - airlocks[i]->_levelData._entrance._exitBox._position;
									airlocks[i]->setActive(true);

									/*
									Lights
									*/
									std::vector<glm::vec3> lightPos;
									for (auto y : z->_levelData._lights)
										lightPos.push_back(y+z->_rigidBody._position);
									for (auto y : airlocks[i]->_levelData._lights)
										lightPos.push_back(y+airlocks[i]->_rigidBody._position);
									_lightManager.resetLights(lightPos);

									if (Cappuccino::randomInt(0, 1) == 0) {
										_testShopTerminal->setActive(true);
										_testShopTerminal->_rigidBody._position = airlocks[i]->_levelData._shopLocation+ airlocks[i]->_rigidBody._position;
									}

										
									break;
								}
				}
			}
		}
	}
		
	
				
}

LightManager::LightManager(std::vector<Cappuccino::PointLight>& light)
{
	_light = &light;
}

void LightManager::update(float dt)
{
}

void LightManager::resetLights(std::vector<glm::vec3>& lightPos)
{
	for (unsigned i = 0; i < _light->size(); i++)
		_light->at(i)._pos = glm::vec3(0,-10000,0);

	for (unsigned i = 0; i < lightPos.size(); i++) {
		glm::vec3 newLightPos = lightPos[i];
		newLightPos.z += 5;
		_light->at(i)._pos = newLightPos;
	}
		

	GameplayScene::resendLights();
}

EnemyManager::EnemyManager()
{
	
}

void EnemyManager::update(float dt)
{
	if (start) {
		for (unsigned i = 0; i < _enemies.size(); i++)
			_enemies[i]->setActive(false);
		start = false;
	}
}

float EnemyManager::spawnEnemy(glm::vec3 position, int type)
{
	int enemy = Cappuccino::randomInt(0, 1);
	std::string myEnemy = "";
	if (type == 0) {//robot
		if (enemy == 0) {//Sentry
			myEnemy = "Sentry";
			for (unsigned i = 0; i < _enemies.size(); i++){
				if (_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					_enemies[i]->_rigidBody._position.y += 2;
					break;
				}
			}
			return 3;
		}
		else if (enemy == 1) {//Sentinel
			myEnemy = "RoboGunner";
			for (unsigned i = 0; i < _enemies.size(); i++) {
				if (_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					break;
				}
			}
			return 1.5;
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
			return 1;
		}
		else if (enemy == 1) {//Captain
			myEnemy = "Captain";
			for (unsigned i = 0; i < _enemies.size(); i++) {
				if (_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					break;
				}
			}
			return 2;
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
			return 0.5;
		}
		else if (enemy == 1) {//Squelch
			myEnemy = "Squelch";
			for (unsigned i = 0; i < _enemies.size(); i++) {
				if (_enemies[i]->_enemyType == myEnemy && !_enemies[i]->isActive()) {
					_enemies[i]->setActive(true);
					_enemies[i]->_rigidBody._position = position;
					break;
				}
			}
			return 1;
		}
	}
	return 0.0f;
}
