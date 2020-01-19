#include "Class.h" 

Cappuccino::Texture* Class::diffuse = nullptr;
Cappuccino::Texture* Class::spec = nullptr;
Cappuccino::Texture* Class::norm = nullptr;
Cappuccino::Texture* Class::emission = nullptr;
Cappuccino::Texture* Class::height = nullptr;

Class::Class(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes) :
	GameObject(*SHADER, textures, meshes, 1.0f), _input(true, 0), //change this field later (mass)
	_uiLight(glm::vec2(1600.0f, 1200.0f), { _rigidBody._position }, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 16.0f)
	, _shieldDown("shieldDown.wav", "Shield")
{


	static bool init = false;
	if (!init) {

		diffuse = new Cappuccino::Texture(std::string("pistol.png"), Cappuccino::TextureType::DiffuseMap);
		spec = new Cappuccino::Texture(std::string("pistol.png"), Cappuccino::TextureType::SpecularMap);
		norm = new Cappuccino::Texture(std::string("pistolNorm.png"), Cappuccino::TextureType::NormalMap);
		emission = new Cappuccino::Texture(std::string("pistolEmission.png"), Cappuccino::TextureType::EmissionMap);
		height = new Cappuccino::Texture(std::string("pistolHeight.png"), Cappuccino::TextureType::HeightMap);
		init = true;
	}


	_playerCamera = new Cappuccino::Camera;
	_playerCamera->lookAt({ 0.0f, 0.0f, 0.0f });

	_secondary = new Pistol(_uiLight._pointLightShader, { diffuse, spec, norm, emission, height }, { new Cappuccino::Mesh("pistol.obj") }, "Energy Pistol", 2.0f, 0.35f, -1);

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

	_crosshair = new Gun(_crosshairShader, {}, { new Cappuccino::Mesh("crosshairPistol.obj") });

	_rigidBody._moveable = true;
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(_rigidBody._position, glm::vec3(1.0f, 4.0f, 1.0f)));
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(_rigidBody._position, glm::vec3(1.0f, 4.0f, 1.0f)));
	_rigidBody.setGrav(true);
}

void Class::childUpdate(float dt)
{
	///REMOVE THIS AFTER TESTING IS DONE
	{
		//_hp = 1000;
		if (this->_input.keyboard->keyPressed(Events::K))
			_hp = 0;

	}
	///REMOVE THIS AFTER TESTING IS DONE

	static float shieldDownTimer = .25f;
	if (_shieldTimer > 0.0f) {
		_shieldTimer -= dt;

		if (_shield <= 0.0f)
			shieldDownTimer -= dt;

		if (shieldDownTimer <= 0.0f) {
			_shieldDown.play();
			shieldDownTimer = .25f;
		}
	}
	else if (_shieldTimer <= 0.0f) {
		rechargeShields();
		shieldDownTimer = .25f;
	}



	_hud->setHealth(static_cast<unsigned>(std::ceilf(_hp)));
	_hud->setShield(static_cast<unsigned>(std::ceilf(_shield)));
	_hud->setAmmoCount(getGun()->getAmmoCount());
	_hud->setCurrencyCount(_currency);
	_hud->updateHud(dt);

	getGun()->setDelay(dt);

	if (_input.keyboard->keyPressed(Events::Shift))
		_speed = 20.0f;
	else
		_speed = 10.0f;

	//movement
	if (_input.keyboard->keyPressed(Events::W) ||
		_input.keyboard->keyPressed(Events::A) ||
		_input.keyboard->keyPressed(Events::S) ||
		_input.keyboard->keyPressed(Events::D) ||
		_input.keyboard->keyPressed(Events::Space)) {

		auto moveForce = glm::vec3(0.0f, 0.0f, 0.0f);
		if (_input.keyboard->keyPressed(Events::W)) {
			//forward
			moveForce += (glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z) * _speed);
		}
		else if (_input.keyboard->keyPressed(Events::S)) {
			//back
			moveForce += (-glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z) * _speed);
		}

		if (_input.keyboard->keyPressed(Events::A)) {
			//left
			moveForce += (-glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z) * _speed);
		}
		else if (_input.keyboard->keyPressed(Events::D)) {
			//right
			moveForce += (glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z) * _speed);
		}

		if (_input.keyboard->keyPressed(Events::Space))
			this->_rigidBody.addVelocity(glm::vec3(0.0f, 2.0f, 0.0f));


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

	//take rigidBody pos, add normalized camera * speed, set as A. Find muzzle location in world space, set as B. Do A - B to find new directional vector.
	glm::vec3 temp = _rigidBody._position + (glm::normalize(_playerCamera->getFront()) * 1000.0f);
	glm::vec3 muzzlePos = _playerCamera->getPosition() + _playerCamera->getFront() + _playerCamera->getRight() + _playerCamera->getUp() + glm::vec3(0.0f, 0.0f, 0.0f);// getGun()->getOffset();
	//if (_input.clickListener.leftClicked() && getGun()->shoot(temp - muzzlePos, muzzlePos - _rigidBody._vel * dt)) {

	if (canShoot) {
		if (_input.clickListener.leftClicked() && getGun()->shoot(_playerCamera->getFront(), _rigidBody._position - _rigidBody._vel * dt + getGun()->getOffset())) {
			if (!(getGun()->_rigidBody._position.z + 0.1f >= 0.2f))
				getGun()->_rigidBody._position.z += 0.05f;
		}
		else if (!getGun()->getFire()) {
			if (getGun()->_rigidBody._position.z > 0.0f)
				getGun()->_rigidBody._position.z -= 0.006f;
		}
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

void Class::addCurrency()
{
	_currency++;
}

void Class::addAmmo()
{
	_primary->setAmmoCount();
}

void Class::addHealth()
{
	setHealth(fminf(_maxHp, (_hp + (0.2f * _maxHp))));
}

void Class::rechargeShields()
{
	if (_shield < _maxShield) {
		_shield += 0.5f;
	}
}

void Class::disableShieldRegen(float disableTime)
{
	_shieldTimer = disableTime;
}

void Class::takeDamage(const float dmg) {
	if (_shield > 0) {
		_shield -= dmg;
		if (_shield < 0)
		{
			//invert sign if shield goes into the negatives
			_hp -= _shield < 0 ? _shield *= -1 : _shield;
			_shield = 0;
		}
	}
	else {
		_hp -= dmg;
	}
	disableShieldRegen(2.0f);

}

void Class::toggleGun(const bool gun)
{
	if (gun)
	{
		_primary->setActive(true);
		_crosshairPrimary->setActive(true);
		_secondary->setActive(false);
		_crosshair->setActive(false);
		gunToggle = true;
	}
	else
	{
		_primary->setActive(false);
		_crosshairPrimary->setActive(false);
		_secondary->setActive(true);
		_crosshair->setActive(true);
		gunToggle = false;
	}
}

void Class::setActive(const bool yn)
{
	GameObject::setActive(yn);
	_primary->setActive(yn);
	_crosshairPrimary->setActive(yn);
	gunToggle = true;
}



Commando::Commando(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes)
{
	auto diffuse = new Cappuccino::Texture(std::string("autoRifleDiffuse.png"), Cappuccino::TextureType::DiffuseMap);
	auto spec = new Cappuccino::Texture(std::string("autoRifleDiffuse.png"), Cappuccino::TextureType::SpecularMap);
	auto norm = new Cappuccino::Texture(std::string("autoRifleNormal.png"), Cappuccino::TextureType::NormalMap);
	auto emission = new Cappuccino::Texture(std::string("autoRifleEmission.png"), Cappuccino::TextureType::EmissionMap);
	auto height = new Cappuccino::Texture(std::string("autoRifleHeight.png"), Cappuccino::TextureType::HeightMap);


	_primary = new AR(_uiLight._pointLightShader, { diffuse, spec, norm, emission, height },
		{ new Cappuccino::Mesh("autoRifle.obj") }, "Assault Rifle", 5.0f, 0.1f, 300);
	_primary->setShootSound("autoRifle.wav", "autoRifleGroup");
	//user interface
	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;

	//crosshair
	_crosshairShader = new Cappuccino::Shader("screenSpaceModel.vert", "screenSpace.frag");
	_crosshairShader->use();
	_crosshairShader->loadOrthoProjectionMatrix(1600.0f / 20.0f, 1200.0f / 20.0f);
	_crosshairShader->setUniform("colour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	_crosshairPrimary = new Gun(_crosshairShader, {}, { new Cappuccino::Mesh("crosshairAutoRifle.obj") });

	_hp = _maxHp = 100;
	_shield = _maxShield = 50;

	_hud = new HUD(PlayerClass::COMMANDO);
}

Assault::Assault(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes)
{
	auto diffuse = new Cappuccino::Texture(std::string("shotDiffuse.png"), Cappuccino::TextureType::DiffuseMap);
	auto spec = new Cappuccino::Texture(std::string("shotDiffuse.png"), Cappuccino::TextureType::SpecularMap);
	auto norm = new Cappuccino::Texture(std::string("shotNorm.png"), Cappuccino::TextureType::NormalMap);
	auto emission = new Cappuccino::Texture(std::string("shotEmission.png"), Cappuccino::TextureType::EmissionMap);
	auto height = new Cappuccino::Texture(std::string("shotHeight.png"), Cappuccino::TextureType::HeightMap);
	_primary = new SG(_uiLight._pointLightShader, std::vector<Cappuccino::Texture*>{diffuse, spec, norm, emission, height, new Cappuccino::Texture("handsDiffuse.png", Cappuccino::TextureType::DiffuseMap, 1)},
		std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("shotgun.obj"), new Cappuccino::Mesh("shotgunHands.obj") }, "Shotgun", 6, 1.0f, 72, 12);
	_primary->setShootSound("shotgun.wav", "shotgun");
	//user interface
	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;

	//crosshair
	_crosshairShader = new Cappuccino::Shader("screenSpaceModel.vert", "screenSpace.frag");
	_crosshairShader->use();
	_crosshairShader->loadOrthoProjectionMatrix(1600.0f / 20.0f, 1200.0f / 20.0f);
	_crosshairShader->setUniform("colour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	_crosshairPrimary = new Gun(_crosshairShader, {}, { new Cappuccino::Mesh("crosshairShotgun.obj") });

	_hp = _maxHp = 125;
	_shield = _maxShield = 65;

	_hud = new HUD(PlayerClass::ASSAULT);
}

Scout::Scout(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes)
{
	_primary = new AR(_uiLight._pointLightShader, { new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap), new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::SpecularMap) },
		{ new Cappuccino::Mesh("autoRifle.obj") }, "Assault Rifle", 1.0f, 0.1f, 300);
	_primary->setShootSound("autoRifle.wav", "autoRifleGroup");
	//user interface
	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;

	//crosshair
	_crosshairShader = new Cappuccino::Shader("screenSpaceModel.vert", "screenSpace.frag");
	_crosshairShader->use();
	_crosshairShader->loadOrthoProjectionMatrix(1600.0f / 20.0f, 1200.0f / 20.0f);
	_crosshairShader->setUniform("colour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	_crosshairPrimary = new Gun(_crosshairShader, {}, { new Cappuccino::Mesh("crosshairSemiRifle.obj") });

	_hp = _maxHp = 75;
	_shield = _maxShield = 35;

	_hud = new HUD(PlayerClass::SCOUT);
}

Demolitionist::Demolitionist(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes)
{
	auto diffuse = new Cappuccino::Texture(std::string("grenadeLauncherDiffuse.png"), Cappuccino::TextureType::DiffuseMap);
	auto spec = new Cappuccino::Texture(std::string("grenadeLauncherDiffuse.png"), Cappuccino::TextureType::SpecularMap);
	auto norm = new Cappuccino::Texture(std::string("grenadeLauncherNormal.png"), Cappuccino::TextureType::NormalMap);
	auto emission = new Cappuccino::Texture(std::string("grenadeLauncherEmission.png"), Cappuccino::TextureType::EmissionMap);

	_primary = new GL(_uiLight._pointLightShader, { diffuse,spec,norm,emission },
		{ new Cappuccino::Mesh("grenadeLauncher.obj") }, "Grenade Launcher", 80.0f, 0.7f, 35);
	_primary->setShootSound("autoRifle.wav", "autoRifleGroup");
	//user interface
	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;

	//crosshair
	_crosshairShader = new Cappuccino::Shader("screenSpaceModel.vert", "screenSpace.frag");
	_crosshairShader->use();
	_crosshairShader->loadOrthoProjectionMatrix(1600.0f / 20.0f, 1200.0f / 20.0f);
	_crosshairShader->setUniform("colour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	_crosshairPrimary = new Gun(_crosshairShader, {}, { new Cappuccino::Mesh("crosshairGrenadeLauncher.obj") });

	_hp = _maxHp = 110;
	_shield = _maxShield = 60;

	_hud = new HUD(PlayerClass::DEMOLITION);
}

