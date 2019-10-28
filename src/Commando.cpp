#include "Commando.h"

Commando::Commando(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	:GameObject(SHADER,textures,meshes,1.0f), _input(true, std::nullopt)//change this field later (mass)
{
}

void Commando::childUpdate(float dt)
{
	if (_input.keyboard->keyPressed(Events::Shift))
		speed = 1.0f;
	else
		speed = 0.1f;

	if (_input.keyboard->keyPressed(Events::W))
		_rigidBody.setAccel(glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z) * speed);
	else
		_rigidBody.addAccel(_rigidBody._accel * -1.0f);


	if (_input.keyboard->keyPressed(Events::A))
		_rigidBody.setAccel(-glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z)*speed);
	if (_input.keyboard->keyPressed(Events::D))													  
		_rigidBody.setAccel( glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z)*speed);

	if (_input.keyboard->keyPressed(Events::Control))
		_rigidBody.setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

	_playerCamera->setPosition(_rigidBody._position);
}
