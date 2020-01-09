#include "..\include\LevelManager.h"
#include "Cappuccino/CappMacros.h"
LevelManager::LevelManager(Cappuccino::PointLight& light)
	:_lightManager(light)
{
	
}

LevelManager::LevelManager()
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
		rooms[_currentRoom]->setActive(true);
		for (auto x : rooms[_currentRoom]->_levelData.exits){
			for (auto y : airlocks){
				if (!y->isActive()){
					y->setActive(true);
					_currentRotation = 180.0f;
					y->rotate(_currentRotation);
					y->_rigidBody._position = x._exitBox._position - y->_levelData.entrance._exitBox._position;
					y->_rigidBody._position.y = -2;
					break;
				}
			}
		}
		_start = false;
	}
	
	//room handling should be dynamic
	for (unsigned x = 0;x<rooms.size();x++)
		if (rooms[x]->isActive()){
			for (auto y : rooms[x]->_levelData.exits)
				if (player.checkCollision(y._exitBox, rooms[x]->_rigidBody._position)){
					
					rooms[x]->reset();
					rooms[x]->setActive(false);
					for (auto z : airlocks)
					{
						if(z->isActive()){//TODO check the airlocks to see which one to attach the room to this is a current problem child
							unsigned temp = std::rand() % rooms.size();
							rooms[temp]->setActive(true);
							rooms[temp]->rotate(_currentRotation);
							rooms[temp]->_rigidBody._position = z->_rigidBody._position + z->_levelData.exits[0]._exitBox._position - rooms[temp]->_levelData.entrance._exitBox._position;
							_currentRotation += y.rotation;
							break;
						}
					}
					
				}					
		}
	
	//airlock
	for (unsigned x = 0; x < airlocks.size(); x++){
		if (airlocks[x]->isActive())
		{
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
									airlocks[i]->rotate(z->_levelData.exits[n].rotation-_currentRotation);//MAY CAUSE FUTURE PROBLEMS -180f
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

LightManager::LightManager()
{
}

void LightManager::update(float dt)
{
}
