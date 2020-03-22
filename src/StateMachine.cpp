#include "..\include\StateMachine.h"

void StateMachine::update(float dt, Enemy* enemy, Class* player, Building* room)
{
	if (_currentState == NULL)
		_currentState = _states[0];
			
	if(_currentState->_changeState)
		for(auto x: _states)
			if (x->_stateName == _currentState->_newState) {
				_currentState->_newState = "";
				_currentState->_changeState = false;
				_currentState = x;
				break;
			}
				
	_currentState->update(dt,enemy,player,room);
}
