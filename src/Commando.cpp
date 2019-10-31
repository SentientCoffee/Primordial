#include "Commando.h"

Commando::Commando(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	:GameObject(*SHADER, textures, meshes, 1.0f), _input(true, 0)//change this field later (mass)
	, _uiLight(glm::vec2(1600.0f, 1200.0f), _rigidBody._position, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f)
{
	_uiGun = new UIGun(&_uiLight._pointLightShader, std::vector<Cappuccino::Texture*>{new Cappuccino::Texture(std::string("./Assets/Textures/matte.png"),Cappuccino::TextureType::DiffuseMap),
		new Cappuccino::Texture(std::string("./Assets/Textures/matte.png"), Cappuccino::TextureType::SpecularMap)}, std::vector<Cappuccino::Mesh*>{new Cappuccino::Mesh("./Assets/Meshes/autoRifle.obj")});
	_uiGun->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_uiGun->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	//_uiGun->_rigidBody._position = glm::vec3(0.0f, 0.0f, 0.0f);
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(glm::vec3(0, 0, 0), glm::vec3(3.0f)));
	gunToggle = true;
	_primary = new Gun(*SHADER, textures, meshes, "Assault Rifle", 20.0f, 0.5f, 150);
	_secondary = new Gun(*SHADER, textures, meshes, "Energy Pistol", 10.0f, 1.6f, -1);
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
	_primary->setDelay(dt);

	if (_input.keyboard->keyPressed(Events::Shift))
		speed = 10.0f;
	else
		speed = 1.0f;

	if (_input.keyboard->keyPressed(Events::W))
		_rigidBody.setAccel(glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z) * speed);
	else
		_rigidBody.addAccel(_rigidBody._accel * -1.0f);
	
	if (_input.keyboard->keyPressed(Events::F) && _primary->getFire())
	_primary->shoot(_playerCamera->getFront() - _playerCamera->getPosition(), _rigidBody._position);


	if (_input.keyboard->keyPressed(Events::A))
		_rigidBody.setAccel(-glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z) * speed);
	if (_input.keyboard->keyPressed(Events::D))
		_rigidBody.setAccel(glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z) * speed);

	if (_input.keyboard->keyPressed(Events::Control))
		_rigidBody.setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

	_playerCamera->setPosition(_rigidBody._position);
}
