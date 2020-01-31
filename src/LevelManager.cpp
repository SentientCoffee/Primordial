#include "..\include\LevelManager.h"
#include "Cappuccino/CappMacros.h"


LevelManager::LevelManager(Cappuccino::PointLight& light)
	:_lightManager(light)
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
								unsigned temp = std::rand() % rooms.size();
								_currentRotation += y.rotation;
								rooms[temp]->rotate(_currentRotation);
								rooms[temp]->_rigidBody._position = z->_rigidBody._position + z->_levelData._exits[0]._exitBox._position - rooms[temp]->_levelData._entrance._exitBox._position;
								rooms[temp]->setActive(true);
								_currentRoom = temp;

								std::vector<glm::vec3> lightPos;
								for (auto g : rooms[temp]->_levelData._lights)
									lightPos.push_back(g+rooms[temp]->_rigidBody._position);
								for (auto h : airlocks) {
									if(h->isActive())
										for(auto i : h->_levelData._lights)
											lightPos.push_back(i+h->_rigidBody._position);
								}
								_lightManager.resetLights(lightPos);
								int factionType = rand() % 3;
								glm::vec3 enemySpawns;
								while (rooms[temp]->_spawnData._usedWeight < rooms[temp]->_spawnData._weight) {
									int randomSpawnPoint = rand()%rooms[temp]->_spawnData._spawnPoints.size();
									if (!rooms[temp]->_spawnData._spawnPoints[randomSpawnPoint]._spawned) {
										enemySpawns=(rooms[temp]->_rigidBody._position+rooms[temp]->_spawnData._spawnPoints[randomSpawnPoint]._position);
									}

									_enemyManager.spawnEnemy(enemySpawns, (factionType));
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
				for (auto z : rooms){
					if (z->isActive())
						for (unsigned n = 0; n < z->_levelData._exits.size();n++)
							for (unsigned i = 0; i < airlocks.size(); i++)
								if (!airlocks[i]->isActive()){
									airlocks[i]->rotate(_currentRotation + z->_levelData._exits[n].rotation);
									airlocks[i]->_rigidBody._position = z->_rigidBody._position + z->_levelData._exits[n]._exitBox._position - airlocks[i]->_levelData._entrance._exitBox._position;
									airlocks[i]->setActive(true);
									std::vector<glm::vec3> lightPos;
									for (auto y : z->_levelData._lights)
										lightPos.push_back(y+z->_rigidBody._position);
									for (auto y : airlocks[i]->_levelData._lights)
										lightPos.push_back(y+airlocks[i]->_rigidBody._position);
									_lightManager.resetLights(lightPos);
									break;
								}
				}
			}
		}
	}
		
	
				
}

LightManager::LightManager(Cappuccino::PointLight& light)
{
	_light = &light;
}

void LightManager::update(float dt)
{
}

void LightManager::resetLights(std::vector<glm::vec3>& lightPos)
{
	for (unsigned i = 0; i < _light->getPositions().size(); i++)
		_light->getPositions()[i] = glm::vec3(0,-10000,0);

	for (unsigned i = 0; i < lightPos.size(); i++) {
		glm::vec3 newLightPos = lightPos[i];
		newLightPos.z += 5;
		_light->getPositions()[i] = newLightPos;
	}
		

	_light->resendLights();
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

void EnemyManager::spawnEnemy(glm::vec3 position, int type)
{

	if (type == 0) {

	}
	else if (type == 1)
	{

	}
	else if (type == 2)
	{

	}

}
