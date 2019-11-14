#include "..\include\LevelManager.h"

LevelManager::LevelManager()
{
	
}

void LevelManager::update(float dt, Cappuccino::RigidBody player)
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
					y->rotateY(180);
					y->_rigidBody.rotateRigid(180);
					y->setActive(true);
					y->_rigidBody._position = x._position-y->_levelData.entrance._position;
					y->_rigidBody._position.y = -2;
					break;
				}
			}

		}
		_start = false;
	}
	

	for (auto x : rooms)
		if(x->isActive())
		for (auto y : x->_levelData.exits)
			if (player.checkCollision(y._exitBox,y._position+x->_rigidBody._position));
			{
				
			}
				
}
