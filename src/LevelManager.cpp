#include "..\include\LevelManager.h"
#include "Cappuccino/CappMacros.h"
LevelManager::LevelManager()
{
	
}

void LevelManager::update(float dt, Cappuccino::RigidBody& player)
{
	if (_start)
	{
		for (auto x : airlocks)
			x->setActive(false);
		for (auto x : rooms[0]->_levelData.exits)
		{
			for (auto y : airlocks)
			{
				if (!y->isActive())
				{
					y->rotate(180);
					y->setActive(true);
					y->_rigidBody._position = x._exitBox._position - y->_levelData.entrance._exitBox._position;
					y->_rigidBody._position.y = -2;
					break;
				}
			}

		}
		_start = false;
	}
	

	for (unsigned x = 0;x<rooms.size();x++)
		if (rooms[x]->isActive())
		{
			for (auto y : rooms[x]->_levelData.exits)
				if (player.checkCollision(y._exitBox, rooms[x]->_rigidBody._position))
				{
					rooms[x]->rotate(180.0f);
					for (auto z : airlocks)
					{
						if(z->isActive()){
							rooms[x]->_rigidBody._position = z->_rigidBody._position + z->_levelData.exits[0]._exitBox._position - rooms[x]->_levelData.entrance._exitBox._position;
						}
					}
				}					
		}
	for (unsigned x = 0; x < airlocks.size(); x++)
	{
		if (airlocks[x]->isActive())
		{
			if (player.checkCollision(airlocks[x]->_levelData.exits[0]._exitBox, airlocks[x]->_rigidBody._position))
			{
				airlocks[x]->rotate(180);
				for (auto z : rooms)
				{
					if (z->isActive())
					{
						airlocks[x]->_rigidBody._position = z->_rigidBody._position + z->_levelData.exits[0]._exitBox._position - airlocks[x]->_levelData.entrance._exitBox._position;
					}
				}
			}
		}
	}
			
				
}
