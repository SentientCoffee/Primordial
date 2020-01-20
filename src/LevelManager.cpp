#include "..\include\LevelManager.h"
#include "Cappuccino/CappMacros.h"
LevelManager::LevelManager(Cappuccino::PointLight& light)
	:_lightManager(light)
{
	
}

void LevelManager::update(float dt, Cappuccino::RigidBody& player)
{
	_lightManager.update(dt);

	if (_start){
		for (auto x : airlocks)
			x->setActive(false);
		for (auto x : rooms)
			x->setActive(false);
		
		airlocks[0]->setActive(true);
		airlocks[0]->_rigidBody._position = (glm::vec3(-30.0f, -2.0f, -5.0f));
		
		rooms[_currentRoom]->setActive(true);
		rooms[_currentRoom]->_rigidBody._position = airlocks[0]->_rigidBody._position+airlocks[0]->_levelData.exits[0]._exitBox._position - rooms[_currentRoom]->_levelData.entrance._exitBox._position;
		_start = false;
	}
	
	//room handling should be dynamic
	for (unsigned x = 0; x < rooms.size(); x++)
		if (rooms[x]->isActive()) {
			for (auto y : rooms[x]->_levelData.exits)
				if (player.checkCollision(y._exitBox, rooms[x]->_rigidBody._position)){
					rooms[x]->setActive(false);
					rooms[x]->reset();
					for (auto z : airlocks){
						if(z->isActive()){
							if (player.checkCollision(z->_levelData.entrance._exitBox, z->_rigidBody._position)) {

								//unsigned temp = std::rand() % rooms.size();
								//rooms[temp]->rotate(_currentRotation);
								//
								//rooms[temp]->setActive(true);
							}
							else {
								z->reset();
								z->setActive(false);
							}
									
	//						rooms[temp]->_rigidBody._position = z->_rigidBody._position + z->_levelData.exits[0]._exitBox._position - rooms[temp]->_levelData.entrance._exitBox._position;
	//						_currentRotation += y.rotation;
	//						break;
						}
					}
					
				}					
		}
	
	////airlock
	for (unsigned x = 0; x < airlocks.size(); x++){
		if (airlocks[x]->isActive()){
			if (player.checkCollision(airlocks[x]->_levelData.exits[0]._exitBox, airlocks[x]->_rigidBody._position)){
				for (unsigned i = 0; i < airlocks.size(); i++){
					airlocks[i]->reset();
					airlocks[i]->setActive(false);
				}
				for (auto z : rooms){
					if (z->isActive())
						for (unsigned n = 0; n < z->_levelData.exits.size();n++)
							for (unsigned i = 0; i < airlocks.size(); i++)
								if (!airlocks[i]->isActive()) {
									airlocks[i]->setActive(true);
									airlocks[i]->rotate(_currentRotation + z->_levelData.exits[n].rotation);
									airlocks[i]->_rigidBody._position = z->_rigidBody._position + z->_levelData.exits[n]._exitBox._position - airlocks[i]->_levelData.entrance._exitBox._position;
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
