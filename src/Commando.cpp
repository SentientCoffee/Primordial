#include "Commando.h"

Commando::Commando(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	:GameObject(SHADER, textures, meshes, 1.0f), _input(true, 0)//change this field later (mass)
{
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(glm::vec3(0, 0, 0), glm::vec3(3.0f)));
}

void Commando::childUpdate(float dt)
{
	//_input.update();

	//if (_input.controller) {
	//	Sedna::XInputManager::update();
	//	_input.controller->updateSticks(_input.sticks);
	//	_input.controller->getTriggers(_input.triggers);
	//	
	//	auto norm = glm::vec3(_input.sticks[0].x, _input.sticks[0].y,0.0f);
	//	norm /= sqrt(norm.x * norm.x + norm.y * norm.y);
	//
	//	_rigidBody.addAccel(norm * 3.0f);
	//}


	if (_input.keyboard->keyPressed(Events::Shift))
		speed = 10.0f;
	else
		speed = 1.0f;

	if (_input.keyboard->keyPressed(Events::W))
		_rigidBody.setAccel(glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z) * speed);
	else
		_rigidBody.addAccel(_rigidBody._accel * -1.0f);


	if (_input.keyboard->keyPressed(Events::A))
		_rigidBody.setAccel(-glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z) * speed);
	if (_input.keyboard->keyPressed(Events::D))
		_rigidBody.setAccel(glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z) * speed);

	if (_input.keyboard->keyPressed(Events::Control))
		_rigidBody.setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

	_playerCamera->setPosition(_rigidBody._position);
}
