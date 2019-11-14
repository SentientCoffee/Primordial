#include "Class.h" 

Class::Class(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: GameObject(*SHADER, textures, meshes, 1.0f), _input(true, 0), //change this field later (mass)
	  _uiLight(glm::vec2(1600.0f, 1200.0f), _rigidBody._position, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f)
{

	auto diffuse = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::DiffuseMap);
	auto spec = new Cappuccino::Texture(   std::string("metal.png"), Cappuccino::TextureType::SpecularMap);

	
	_secondary = new Pistol(_uiLight._pointLightShader, std::vector<Cappuccino::Texture*>{ diffuse,spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("pistol.obj") },
							"Energy Pistol", 2.0f, 0.35f, -1);

	_secondary->setShootSound("SentryLaser.wav", "pistolGroup");

	_primary = _secondary;
	_secondary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_secondary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_secondary->_transform._translateMat[3].y += 0.1f;

	//crosshair
	_crosshairShader = new Cappuccino::Shader("screenSpaceModel.vert", "screenSpace.frag");
	_crosshairShader->use();
	_crosshairShader->loadOrthoProjectionMatrix(1600.0f / 20.0f, 1200.0f / 20.0f);
	_crosshairShader->setUniform("colour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	_crosshair = new Gun(_crosshairShader, std::vector<Cappuccino::Texture*>{}, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("Crosshair.obj") });


	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(_rigidBody._position, glm::vec3(1.0f, 4.0f, 1.0f)));
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(_rigidBody._position, glm::vec3(1.0f, 4.0f, 1.0f)));

	_rigidBody.setGrav(false);
}

void Class::childUpdate(float dt)
{

	//_hud->setHealth(_testCommando->getHealth());
	//_hud->setHealth(_testCommando->getShield());
	_hud->setAmmoCount(getGun()->getAmmoCount());
	_hud->updateHud(dt);

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
			_rigidBody._vel.y += 2.0f * dt;


		_rigidBody.setVelocity(glm::vec3(moveForce.x, _rigidBody._vel.y, moveForce.z));
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

	//take rigidBody pos, add normalized camera * speed, set as A. Find muzzle location in world space, sest as B. Do A - B to find new directional vector.
	glm::vec3 temp = _rigidBody._position + (glm::normalize(_playerCamera->getFront()) * 50.0f);
	glm::vec3 muzzlePos = _rigidBody._position + getGun()->getOffset();
	//if (_input.clickListener.leftClicked() && getGun()->shoot(temp - muzzlePos, muzzlePos - _rigidBody._vel * dt) {

	if (_input.clickListener.leftClicked() && getGun()->shoot(_playerCamera->getFront(), _rigidBody._position - _rigidBody._vel * dt + getGun()->getOffset())) {
		if (!(getGun()->_rigidBody._position.z + 10.0F * dt >= 0.2f))
			getGun()->_rigidBody._position.z += 10.0f * dt;
	}
	else if (!getGun()->getFire()) {
		if (getGun()->_rigidBody._position.z > 0.0f)
			getGun()->_rigidBody._position.z -= dt;
	}
}

Gun* Class::getGun()
{
	if (gunToggle)
		return _primary;
	
	return _secondary;
}

void Class::addAmmo(Bullet* primary, Bullet* secondary)
{
	_primary->addBullets(primary);
	_secondary->addBullets(secondary);
}

void Class::toggleGun(const bool gun)
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

void Class::setActive(bool yn)
{
	GameObject::setActive(yn);
	_primary->setActive(yn);
	_crosshair->setActive(yn);
}



Commando::Commando(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes)
{
	_primary = new AR(_uiLight._pointLightShader, std::vector<Cappuccino::Texture*>{ new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap), new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::SpecularMap) },
		std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("autoRifle.obj") }, "Assault Rifle", 1.0f, 0.1f, 300);
	_primary->setShootSound("autoRifle.wav", "autoRifleGroup");
	//user interface
	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;

	_hud = new HUD(PlayerClass::COMMANDO);
}

Assault::Assault(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes)
{
	_primary = new SG(_uiLight._pointLightShader, std::vector<Cappuccino::Texture*>{ new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap), new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::SpecularMap) },
		std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("shotgun.obj") }, "Shotgun",6,1.25f,72,12);
	_primary->setShootSound("autoRifle.wav", "autoRifleGroup");
	//user interface
	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;
	_hud = new HUD(PlayerClass::ASSAULT);
}

Scout::Scout(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes)
{
	_primary = new AR(_uiLight._pointLightShader, std::vector<Cappuccino::Texture*>{ new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap), new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::SpecularMap) },
		std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("autoRifle.obj") }, "Assault Rifle", 1.0f, 0.1f, 300);
	_primary->setShootSound("autoRifle.wav", "autoRifleGroup");
	//user interface
	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;
	_hud = new HUD(PlayerClass::SCOUT);
}

Demolitionist::Demolitionist(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes)
{
	_primary = new AR(_uiLight._pointLightShader, std::vector<Cappuccino::Texture*>{ new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap), new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::SpecularMap) },
		std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("autoRifle.obj") }, "Assault Rifle", 1.0f, 0.1f, 300);
	_primary->setShootSound("autoRifle.wav", "autoRifleGroup");
	//user interface
	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;
	_hud = new HUD(PlayerClass::DEMOLITION);
}