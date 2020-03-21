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
	:State("WanderState")
{
}

void WanderState::update(float dt, Enemy* enemy, Class* player, Building* room)
{
	if (!_walking) {

	}
	else {
		if (true) {

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
