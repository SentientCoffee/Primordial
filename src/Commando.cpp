#include "Commando.h"
#include "glfw/glfw3.h"

Commando::Commando(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	:GameObject(*SHADER, textures, meshes, 1.0f), _input(true, 0)//change this field later (mass)
	, _uiLight(glm::vec2(1600.0f, 1200.0f), _rigidBody._position, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f)
{
	_primary = new AR(_uiLight._pointLightShader, std::vector<Cappuccino::Texture*>{new Cappuccino::Texture(std::string("./Assets/Textures/matte.png"), Cappuccino::TextureType::DiffuseMap),
		new Cappuccino::Texture(std::string("./Assets/Textures/matte.png"), Cappuccino::TextureType::SpecularMap)}, std::vector<Cappuccino::Mesh*>{new Cappuccino::Mesh("./Assets/Meshes/autoRifle.obj")},
		"Assault Rifle", 1.0f, 0.1f, 300);

	_secondary = new Pistol(_uiLight._pointLightShader, std::vector<Cappuccino::Texture*>{new Cappuccino::Texture(std::string("./Assets/Textures/Metal_specmap.png"), Cappuccino::TextureType::DiffuseMap),
		new Cappuccino::Texture(std::string("./Assets/Textures/Metal_specmap.png"), Cappuccino::TextureType::SpecularMap)}, std::vector<Cappuccino::Mesh*>{new Cappuccino::Mesh("./Assets/Meshes/pistol.obj")},
		"Energy Pistol", 2.0f, 0.35f, -1);

	_primary->setShootSound("autoRifle.wav", "autoRifleGroup");
	_secondary->setShootSound("SentryLaser.wav", "pistolGroup");

	_primary->setActive(true);

	//user interface
	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;


	_secondary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_secondary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_secondary->_transform._translateMat[3].y += 0.1f;

	//crosshair
	_crosshairShader = new Cappuccino::Shader("screenSpaceModel.vert", "screenSpace.frag");
	_crosshairShader->use();
	_crosshairShader->loadOrthoProjectionMatrix(1600.0f / 20.0f, 1200.0f / 20.0f);
	_crosshairShader->setUniform("colour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	_crosshair = new Crosshair(_crosshairShader, std::vector<Cappuccino::Texture*>{}, std::vector<Cappuccino::Mesh*>{new Cappuccino::Mesh("./Assets/Meshes/Crosshair.obj")});

	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(_rigidBody._position, glm::vec3(1.0f, 4.0f, 1.0f)));
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(_rigidBody._position, glm::vec3(1.0f, 4.0f, 1.0f)));

	_rigidBody.setGrav(true);
}

void Commando::childUpdate(float dt)
{
	getGun()->setDelay(dt);

	if (_input.keyboard->keyPressed(Events::Shift))
		speed = 10.0f;
	else
		speed = 5.0f;

	//movement

	

	if (_input.keyboard->keyPressed(Events::W) || _input.keyboard->keyPressed(Events::A) || _input.keyboard->keyPressed(Events::S) || _input.keyboard->keyPressed(Events::D)
		|| _input.keyboard->keyPressed(Events::Space)) {

		auto moveForce = glm::vec3(0.0f, 0.0f, 0.0f);
		//forward
		if (_input.keyboard->keyPressed(Events::W))
			moveForce += (glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z) * speed);

		//back
		else if (_input.keyboard->keyPressed(Events::S))
			moveForce += (-glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z) * speed);

		//left
		if (_input.keyboard->keyPressed(Events::A))
			moveForce += (-glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z) * speed);

		//right
		else if (_input.keyboard->keyPressed(Events::D))
			moveForce += (glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z) * speed);

		if (_input.keyboard->keyPressed(Events::Space))
			_rigidBody._vel.y += 2.0f*dt;


		_rigidBody.setVelocity(glm::vec3(moveForce.x,_rigidBody._vel.y,moveForce.z));
	}
	else
		_rigidBody.setVelocity(glm::vec3(0.0f, _rigidBody._vel.y, 0.0f));



	if (_input.keyboard->keyPressed(Events::Control))
		_rigidBody.setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

	_playerCamera->setPosition(_rigidBody._position);

	//weapon swap
	if (_input.keyboard->keyPressed(Events::One))
		toggleGun(true);
	if (_input.keyboard->keyPressed(Events::Two))
		toggleGun(false);

	//shooting
	if (_input.clickListener.leftClicked() && getGun()->shoot(_playerCamera->getFront(), _rigidBody._position - _rigidBody._vel * dt + getGun()->getOffset())) {
		if (!(getGun()->_rigidBody._position.z +10.0F*dt >= 0.2f))
			getGun()->_rigidBody._position.z += 10.0f*dt;
	}
	else if (!getGun()->getFire()) {
		if (getGun()->_rigidBody._position.z > 0.0f)
			getGun()->_rigidBody._position.z -= dt;
	}
}

Gun* Commando::getGun()
{
	if (gunToggle)
		return _primary;
	else
		return _secondary;
}

void Commando::addAmmo(Bullet* primary, Bullet* secondary)
{
	_primary->addBullets(primary);
	_secondary->addBullets(secondary);
}

void Commando::toggleGun(const bool gun)
{
	if (gun)
	{
		_primary->setActive(true);
		_secondary->setActive(false);
		gunToggle = true;
	}
	else
	{
		_primary->setActive(false);
		_secondary->setActive(true);
		gunToggle = false;
	}
}

