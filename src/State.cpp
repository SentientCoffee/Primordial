#include "..\include\State.h"

#include "Enemy.h"

BaseState::BaseState()
	:State("BaseState")
{
}

void BaseState::update(float dt, Enemy* enemy, Class* player, Building* room)
{
}

State::State(std::string statename)
{
	_stateName = statename;
}

WanderState::WanderState()
	:State("WanderState")
{
}

void WanderState::update(float dt, Enemy* enemy, Class* player, Building* room)
{
	if (!_walking) {
		createNewDestination(room);
	}
	else {
		if (enemy->checkCollision(box,_point2point.front()->_position+room->_rigidBody._position)) {
			_walking = false;
		}
		else
		{

		}
	}

}

void WanderState::createNewDestination(Building* room)
{
	std::shared_ptr<NavPoint> currentPoint = _point2point.front();//get where we are
	_point2point.clear();//empty the list
	int newDestinationInt = Cappuccino::randomInt(0, room->_navMeshData->navmesh.size() - 1);
	std::shared_ptr<NavPoint> newDestination = room->_navMeshData->navmesh[newDestinationInt];//get a new random point
	_point2point.push_back(newDestination);//add it to the front of the list

	std::vector<std::shared_ptr<NavPoint>> openList;//open and closed list
	std::vector<std::shared_ptr<NavPoint>> closedList;

	{

		currentPoint->Gcost = 0.0f;
		currentPoint->Hcost = glm::length(newDestination->_position - currentPoint->_position);
		currentPoint->Fcost = currentPoint->Gcost + currentPoint->Hcost;
		currentPoint->start = true;
		closedList.push_back(currentPoint);


		for (auto x : currentPoint->_connections) {
			x->Gcost = currentPoint->Gcost + glm::length(currentPoint->_position - x->_position);
			x->Hcost = glm::length(newDestination->_position - x->_position);
			x->Fcost = x->Gcost + x->Hcost;
			x->_lastConnection = currentPoint;
			openList.push_back(x);
		}
	}

		bool complete = false;
		while (!complete) {//until we have a path to the destination
			std::shared_ptr<NavPoint> lowestNode = openList[0];//find the new lowest fcost node in the open list
			unsigned lowestNodePos = 0;//store its position
			for (unsigned i =0;i<openList.size();i++) 
				if (openList[i]->Fcost < lowestNode->Fcost) {
					lowestNode = openList[i];
					lowestNodePos = i;
				}
			openList.erase(openList.begin()+lowestNodePos);//remove from the openlist
			closedList.push_back(lowestNode);//add to closed list

			if (lowestNode == newDestination) {//if we reached our final node
				_point2point.push_back(lowestNode);
				bool connected = false;
				while (!connected) {
					_point2point.push_back(_point2point.back()->_lastConnection);
					if (_point2point.back()->start) {
						connected = true;
					}						
				}
				complete = true;
				continue;
			}
				

			for (auto x : lowestNode->_connections) {//for the lowest node we want to check it's connections
				bool dealtWith = false;//boolean for if we have accomplised what we need to
				for (auto y : closedList)//check the closed list
					if (x == y)//if it's in there
						dealtWith = true;//we are done with the connection

				if(!dealtWith)//if not dealt with
					for (auto y : openList) //check the open list
						if (x == y) {//if in the openlist
							float tempG = x->Gcost + glm::length(x->_position - y->_position);
							float tempH = y->Hcost;
							float tempF = tempG + tempH;
							if (tempF < y->Fcost) {
								y->Fcost = tempF;
								y->_lastConnection = x;
							}
							dealtWith = true;
						}
				if (!dealtWith) {
					x->Gcost = lowestNode->Gcost + glm::length(lowestNode->_position-x->_position);
					x->Hcost = glm::length(newDestination->_position-x->_position);
					x->Fcost = x->Gcost + x->Hcost;
					x->_lastConnection = lowestNode;
					openList.push_back(x);
				}
			}
		}
}

AttackState::AttackState()
:State("AttackState")
{
}

void AttackState::update(float dt, Enemy* enemy, Class* player, Building* room)
{
}
