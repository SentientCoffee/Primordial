#include "..\include\State.h"

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
	:State("Wander")
{
}

void WanderState::update(float dt, Enemy* enemy, Class* player, Building* room)
{


}
