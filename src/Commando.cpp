#include "Commando.h"

Commando::Commando(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	:Player(SHADER,textures,meshes,10.0f)//change this field later
{
	
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

	if (_input.keyboard->keyPressed(Events::S))
		setPosition(-glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z) * speed * dt);

	if (_input.keyboard->keyPressed(Events::A))
		setPosition(-_playerCamera->getRight() * speed * dt);
	if (_input.keyboard->keyPressed(Events::D))
		setPosition(_playerCamera->getRight() * speed * dt);

	_playerCamera->setPosition(_rigidBody._position);
}
