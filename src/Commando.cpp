#include "Commando.h"

Commando::Commando(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	:GameObject(SHADER,textures,meshes,1.0f), _input(true, std::nullopt)//change this field later (mass)
{
	_state = new Cappuccino::State();
}

void Commando::childUpdate(float dt)
{
	if (_input.keyboard->keyPressed(Events::Shift))
		speed = 7.0f;
	else
		speed = 3.5f;

	if (_input.keyboard->keyPressed(Events::W))
		_rigidBody.addAccel(glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z));
	else
		_rigidBody.addAccel(_rigidBody._accel * -1.0f);


	if (_input.keyboard->keyPressed(Events::A))
		_rigidBody.addAccel(-glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z));
	if (_input.keyboard->keyPressed(Events::D))													  
		_rigidBody.addAccel( glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z));

	_playerCamera->setPosition(_rigidBody._position);
}
