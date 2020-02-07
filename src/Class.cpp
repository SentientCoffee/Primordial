#include "Class.h" 
#include "Cappuccino/FrameBuffer.h"

#include <Cappuccino/ResourceManager.h>

Cappuccino::Texture* Class::diffuse = nullptr;
Cappuccino::Texture* Class::spec = nullptr;
Cappuccino::Texture* Class::norm = nullptr;
Cappuccino::Texture* Class::emission = nullptr;
Cappuccino::Texture* Class::height = nullptr;

Class::Class(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes) :
	GameObject(*SHADER, textures, meshes, 1.0f), //change this field later (mass)
	_input(true, 0),
	_shieldRecharge("shieldRecharge.wav"),
	_shieldDown("shieldDown.wav", "Shield"),
	_uiLight(glm::vec2(1600.0f, 1200.0f), { _rigidBody._position },
		glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 16.0f) {
	_shieldRecharge.setGroupHandle(_shieldDown.getGroupHandle());

	static bool init = false;
	if (!init) {

		diffuse = Cappuccino::TextureLibrary::loadTexture("Pistol diffuse", "pistol/pistol-Diffuse.png", Cappuccino::TextureType::DiffuseMap);
		spec = Cappuccino::TextureLibrary::loadTexture("Pistol specular", "pistol/pistol-Diffuse.png", Cappuccino::TextureType::SpecularMap);
		norm = Cappuccino::TextureLibrary::loadTexture("Pistol normal", "pistol/pistol-Norm.png", Cappuccino::TextureType::NormalMap);
		emission = Cappuccino::TextureLibrary::loadTexture("Pistol emission", "pistol/pistol-Emission.png", Cappuccino::TextureType::EmissionMap);
		height = Cappuccino::TextureLibrary::loadTexture("Pistol height", "pistol/pistol-Height.png", Cappuccino::TextureType::HeightMap);
		init = true;
	}


	_playerCamera = new Cappuccino::Camera;
	_playerCamera->lookAt({ 0.0f, 0.0f, 0.0f });

	_secondary = new Pistol(_uiLight._pointLightShader, {
		diffuse, spec, norm, emission, height
		}, {
			Cappuccino::MeshLibrary::loadMesh("Pistol", "pistol.obj")
		}, "Energy Pistol", 2.0f, 0.35f, 1);

	_secondary->setShootSound("SentryLaser.wav", "pistolGroup");

	_primary = _secondary;
	_secondary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_secondary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_secondary->_transform._translateMat[3].y += 0.1f;

	//crosshair
	_crosshairShader = Cappuccino::ShaderLibrary::loadShader("Screen space", "screenSpaceModel.vert", "screenSpace.frag");
	_crosshairShader->use();
	_crosshairShader->loadOrthoProjectionMatrix(1600.0f / 20.0f, 1200.0f / 20.0f);
	_crosshairShader->setUniform("colour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	_crosshair = new Gun(_crosshairShader, {},
	{
		Cappuccino::MeshLibrary::loadMesh("Pistol crosshair", "crosshairPistol.obj")
	});

	_rigidBody._moveable = true;
	_rigidBody._creature = true;
	//	_rigidBody._creature = true;
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(_rigidBody._position, glm::vec3(1.0f, 4.0f, 1.0f)));
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(_rigidBody._position, glm::vec3(1.0f, 4.0f, 1.0f)));
	_rigidBody.setGrav(true);
}

void Class::childUpdate(float dt)
{
	///REMOVE THIS AFTER TESTING IS DONE
	static bool flymode = false;
	{
		//_hp = 1000;

		if (this->_input.keyboard->keyPressed(Events::K))
			_hp = 0;

		static bool pressed = false;
		if (_input.keyboard->keyPressed(Cappuccino::KeyEvent::LEFT_CONTROL) && !pressed) {
			pressed = true;
			flymode ^= 1;
			_rigidBody._canTouch ^= 1;
		}
		else if (!_input.keyboard->keyPressed(Cappuccino::KeyEvent::LEFT_CONTROL))
			pressed = false;

	}
	///REMOVE THIS AFTER TESTING IS DONE

	//send the lerp percentage to the shader for greyscale effect
	Cappuccino::Framebuffer::_framebuffers.back()->_fbShader->use();
	Cappuccino::Framebuffer::_framebuffers.back()->_fbShader->setUniform("greyscalePercentage", _hp / _maxHp);


	//shield logic
	static bool playing = true;
	static float shieldDownTimer = .25f;
	if (_shieldTimer > 0.0f) {
		playing = false;
		_shieldTimer -= dt;

		if (_shield <= 0.0f)
			shieldDownTimer -= dt;

		if (shieldDownTimer <= 0.0f) {
			_shieldDown.play();
			shieldDownTimer = .25f;
		}
	}

	else if (_shieldTimer <= 0.0f) {
		if (!playing && _shield != _maxShield) {
			playing = true;
			_shieldRecharge.play();
		}

		rechargeShields();
		shieldDownTimer = .25f;
	}
	//shield logic


	_hud->setHealth(static_cast<unsigned>(std::ceilf(_hp)));
	_hud->setShield(static_cast<unsigned>(std::ceilf(_shield)));
	_hud->setAmmoCount(getGun()->getAmmoCount());
	_hud->setAmmoMax(getGun()->getMaxAmmo());
	_hud->setCurrencyCount(_currency);
	_hud->updateHud(dt);

	getGun()->setDelay(dt);
	_jumpDelay -= dt;

	if (_input.keyboard->keyPressed(Events::Shift))
		_speed = 50.0f;//set to 20 when not debugging
	else
		_speed = 10.0f;

	//camera bobbing variables
	static float u = 0.0f;
	static float lastU = u;
	static float lerpSpeed = 7.0f;
	static bool reverse = false;

	//movement
	if (flymode) {
		auto moveForce = glm::vec3(0.0f, 0.0f, 0.0f);

		if (_input.keyboard->keyPressed(Cappuccino::KeyEvent::W))
			moveForce += glm::vec3(_playerCamera->getFront().x, _playerCamera->getFront().y, _playerCamera->getFront().z) * _speed;
		else if (_input.keyboard->keyPressed(Cappuccino::KeyEvent::S))
			moveForce -= glm::vec3(_playerCamera->getFront().x, _playerCamera->getFront().y, _playerCamera->getFront().z) * _speed;
		if (_input.keyboard->keyPressed(Cappuccino::KeyEvent::A))
			moveForce += -glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z) * _speed;
		else if (_input.keyboard->keyPressed(Cappuccino::KeyEvent::D))
			moveForce += glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z) * _speed;

		if (_input.keyboard->keyPressed(Cappuccino::KeyEvent::SPACE))
			moveForce += glm::vec3(0.0f,1.0f,0.0f) * _speed;
		if (_input.keyboard->keyPressed(Cappuccino::KeyEvent::LEFT_ALT))
			moveForce -= glm::vec3(0.0f, 1.0f, 0.0f) * _speed;


		_rigidBody.setVelocity(glm::vec3(moveForce.x, moveForce.y, moveForce.z));

	}
	else {

		if (_input.keyboard->keyPressed(Events::W) ||
			_input.keyboard->keyPressed(Events::A) ||
			_input.keyboard->keyPressed(Events::S) ||
			_input.keyboard->keyPressed(Events::D) ||
			_input.keyboard->keyPressed(Events::Space)) {

			auto moveForce = glm::vec3(0.0f, 0.0f, 0.0f);
			if (_input.keyboard->keyPressed(Events::W)) {
				//forward
				moveForce += glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z) * _speed;
				//bobVec += glm::vec3((float)(rand() % 2) / 100.0f, 0.0f, (float)(rand() % 2) / 100.0f);

				//camera bobbing lerp
				if (!reverse)
					u += dt * lerpSpeed;
				else
					u -= dt * lerpSpeed;
			}
			else if (_input.keyboard->keyPressed(Events::S)) {
				//back
				moveForce += -glm::vec3(_playerCamera->getFront().x, 0, _playerCamera->getFront().z) * _speed;
				//bobVec -= glm::vec3((float)(rand() % 2) / 100.0f, 0.0f, (float)(rand() % 2) / 100.0f);

				if (!reverse)
					u += dt * lerpSpeed;
				else
					u -= dt * lerpSpeed;
			}

			if (_input.keyboard->keyPressed(Events::A)) {
				//left
				moveForce += -glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z) * _speed;
			}
			else if (_input.keyboard->keyPressed(Events::D)) {
				//right
				moveForce += glm::vec3(_playerCamera->getRight().x, 0, _playerCamera->getRight().z) * _speed;
			}

			if (_input.keyboard->keyPressed(Events::Space) && _jumpDelay <= 0.0f)
			{
				this->_rigidBody.addVelocity(glm::vec3(0.0f, 20.0f, 0.0f));
				_jumpDelay = 1.0f;
			}

			_rigidBody.setVelocity(glm::vec3(moveForce.x, _rigidBody._vel.y, moveForce.z));
		}
		else
			_rigidBody.setVelocity(glm::vec3(0.0f, _rigidBody._vel.y, 0.0f));
	}



	if (_input.keyboard->keyPressed(Events::Control))
		_rigidBody.setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));


	//camera bobbing
	static float offset = 0.0f;
	if (u >= 1.0f) {
		u = 1.0f;
		reverse = true;
	}
	else if (u <= 0.0f) {
		u = 0.0f;
		reverse = false;
	}

	if (u != lastU) {
		offset = 0.08f * glm::smoothstep(0.0f, 1.0f, u);
	}

	lastU = u;

	_playerCamera->setPosition(_rigidBody._position + glm::vec3(0.0f, offset, 0.0f));
	//camera bobbing

	//lerp param
	{
		static float u = 0.0f;
		static bool reverse = false;

		if (!reverse)
			u += dt;
		else
			u -= dt;

		if (u >= 1.0f) {
			u = 1.0f;
			reverse = true;
		}
		else if (u <= 0.0f) {
			u = 0.0f;
			reverse = false;
		}


		this->getUILight()._pointLightShader.use();
		this->getUILight()._pointLightShader.setUniform("posVarience", 0.05f * glm::smoothstep(0.0f, 1.0f, u));
	}



	//weapon swap
	if (_input.keyboard->keyPressed(Events::One))
		toggleGun(true);
	if (_input.keyboard->keyPressed(Events::Two))
		toggleGun(false);

	//shooting

	//take rigidBody pos, add normalized camera * speed, set as A. Find muzzle location in world space, set as B. Do A - B to find new directional vector.
	glm::vec3 temp = _rigidBody._position + glm::normalize(_playerCamera->getFront()) * 1000.0f;
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
	setHealth(fminf(_maxHp, _hp + 0.2f * _maxHp));
}

void Class::rechargeShields()
{
	if (_shield < _maxShield)
		_shield += 0.25f;
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
	: Class(SHADER, textures, meshes) {

	const auto diffuse = Cappuccino::TextureLibrary::loadTexture("Auto rifle diffuse", "autoRifle/autoRifle-Diffuse.png", Cappuccino::TextureType::DiffuseMap);
	const auto spec = Cappuccino::TextureLibrary::loadTexture("Auto rifle specular", "autoRifle/autoRifle-Diffuse.png", Cappuccino::TextureType::SpecularMap);
	const auto norm = Cappuccino::TextureLibrary::loadTexture("Auto rifle normal", "autoRifle/autoRifle-Normal.png", Cappuccino::TextureType::NormalMap);
	const auto emission = Cappuccino::TextureLibrary::loadTexture("Auto rifle emission", "autoRifle/autoRifle-Emission.png", Cappuccino::TextureType::EmissionMap);
	const auto height = Cappuccino::TextureLibrary::loadTexture("Auto rifle height", "autoRifle/autoRifle-Height.png", Cappuccino::TextureType::HeightMap);


	_primary = new AR(_uiLight._pointLightShader, {
		diffuse, spec, norm, emission, height
		}, {
			Cappuccino::MeshLibrary::loadMesh("Auto rifle", "autoRifle.obj")
		}, "Assault Rifle", 5.0f, 0.1f, 150);

	_primary->setShootSound("autoRifle.wav", "autoRifleGroup");
	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;

	//crosshair
	_crosshairShader = Cappuccino::ShaderLibrary::loadShader("Screen space", "screenSpaceModel.vert", "screenSpace.frag");
	_crosshairShader->use();
	_crosshairShader->loadOrthoProjectionMatrix(1600.0f / 20.0f, 1200.0f / 20.0f);
	_crosshairShader->setUniform("colour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	_crosshairPrimary = new Gun(_crosshairShader, {},
	{
		Cappuccino::MeshLibrary::loadMesh("Auto rifle crosshair", "crosshairAutoRifle.obj")
	});

	_hp = _maxHp = 100;
	_shield = _maxShield = 50;

	//user interface
	_hud = new HUD(PlayerClass::COMMANDO);
}

Assault::Assault(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes) {

	const auto diffuse = Cappuccino::TextureLibrary::loadTexture("Shotgun diffuse", "shotgun/shotgun-Diffuse.png", Cappuccino::TextureType::DiffuseMap);
	const auto spec = Cappuccino::TextureLibrary::loadTexture("Shotgun specular", "shotgun/shotgun-Diffuse.png", Cappuccino::TextureType::SpecularMap);
	const auto norm = Cappuccino::TextureLibrary::loadTexture("Shotgun normal", "shotgun/shotgun-Norm.png", Cappuccino::TextureType::NormalMap);
	const auto emission = Cappuccino::TextureLibrary::loadTexture("Shotgun emission", "shotgun/shotgun-Emission.png", Cappuccino::TextureType::EmissionMap);
	const auto height = Cappuccino::TextureLibrary::loadTexture("Shotgun height", "shotgun/shotgun-Height.png", Cappuccino::TextureType::HeightMap);

	_primary = new SG(_uiLight._pointLightShader, {
		diffuse, spec, norm, emission, height,
		Cappuccino::TextureLibrary::loadTexture("Hands diffuse", "handsDiffuse.png", Cappuccino::TextureType::DiffuseMap, 1)
		}, {
			Cappuccino::MeshLibrary::loadMesh("Shotgun", "shotgun.obj"), Cappuccino::MeshLibrary::loadMesh("Shotgun hands", "shotgunHands.obj")
		}, "Shotgun", 6, 0.66f, 72, 12);

	_primary->setShootSound("shotgun.wav", "shotgun");
	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;

	//crosshair
	_crosshairShader = Cappuccino::ShaderLibrary::loadShader("Screen space", "screenSpaceModel.vert", "screenSpace.frag");
	_crosshairShader->use();
	_crosshairShader->loadOrthoProjectionMatrix(1600.0f / 20.0f, 1200.0f / 20.0f);
	_crosshairShader->setUniform("colour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	_crosshairPrimary = new Gun(_crosshairShader, {},
	{
		Cappuccino::MeshLibrary::loadMesh("Shotgun crosshair", "crosshairShotgun.obj")
	});

	_hp = _maxHp = 125;
	_shield = _maxShield = 65;

	//user interface
	_hud = new HUD(PlayerClass::ASSAULT);
}

Scout::Scout(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes) {

	const auto diffuse = Cappuccino::TextureLibrary::loadTexture("SAR diffuse", "marksmanRifle/marksmanRifle-Diffuse.png", Cappuccino::TextureType::DiffuseMap);
	const auto spec = Cappuccino::TextureLibrary::loadTexture("SAR specular", "marksmanRifle/marksmanRifle-Diffuse.png", Cappuccino::TextureType::SpecularMap);
	const auto norm = Cappuccino::TextureLibrary::loadTexture("SAR normal", "marksmanRifle/marksmanRifle-Normal.png", Cappuccino::TextureType::NormalMap);
	const auto emission = Cappuccino::TextureLibrary::loadTexture("SAR emission", "marksmanRifle/marksmanRifle-Emission.png", Cappuccino::TextureType::EmissionMap);
	const auto height = Cappuccino::TextureLibrary::loadTexture("SAR height", "marksmanRifle/marksmanRifle-Height.png", Cappuccino::TextureType::HeightMap);

	_primary = new AR(_uiLight._pointLightShader, {
		diffuse, spec, norm, emission, height
		}, {
			Cappuccino::MeshLibrary::loadMesh("SAR", "marksmanRifle.obj")
		}, "Semi Auto Rifle", 50.0f, 0.75f, 100);

	_primary->setShootSound("autoRifle.wav", "autoRifleGroup");
	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;

	//crosshair
	_crosshairShader = Cappuccino::ShaderLibrary::loadShader("Screen space", "screenSpaceModel.vert", "screenSpace.frag");
	_crosshairShader->use();
	_crosshairShader->loadOrthoProjectionMatrix(1600.0f / 20.0f, 1200.0f / 20.0f);
	_crosshairShader->setUniform("colour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	_crosshairPrimary = new Gun(_crosshairShader, {},
	{
		Cappuccino::MeshLibrary::loadMesh("SAR crosshair", "crosshairSemiRifle.obj")
	});

	_hp = _maxHp = 75;
	_shield = _maxShield = 35;

	//user interface
	_hud = new HUD(PlayerClass::SCOUT);
}

Demolitionist::Demolitionist(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes)
{
	const auto diffuse = Cappuccino::TextureLibrary::loadTexture("Grenade launcher diffuse", "grenadeLauncher/grenadeLauncher-Diffuse.png", Cappuccino::TextureType::DiffuseMap);
	const auto spec = Cappuccino::TextureLibrary::loadTexture("Grenade launcher specular", "grenadeLauncher/grenadeLauncher-Diffuse.png", Cappuccino::TextureType::SpecularMap);
	const auto norm = Cappuccino::TextureLibrary::loadTexture("Grenade launcher normal", "grenadeLauncher/grenadeLauncher-Normal.png", Cappuccino::TextureType::NormalMap);
	const auto emission = Cappuccino::TextureLibrary::loadTexture("Grenade launcher emission", "grenadeLauncher/grenadeLauncher-Emission.png", Cappuccino::TextureType::EmissionMap);

	_primary = new GL(_uiLight._pointLightShader, {
		diffuse, spec, norm, emission
		}, {
			Cappuccino::MeshLibrary::loadMesh("Grenade launcher", "grenadeLauncher.obj")
		}, "Grenade Launcher", 80.0f, 0.7f, 35);

	_primary->setShootSound("autoRifle.wav", "autoRifleGroup");
	//user interface
	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;

	//crosshair
	_crosshairShader = Cappuccino::ShaderLibrary::loadShader("Screen space", "screenSpaceModel.vert", "screenSpace.frag");
	_crosshairShader->use();
	_crosshairShader->loadOrthoProjectionMatrix(1600.0f / 20.0f, 1200.0f / 20.0f);
	_crosshairShader->setUniform("colour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	_crosshairPrimary = new Gun(_crosshairShader, {},
	{
		Cappuccino::MeshLibrary::loadMesh("Grenade launcher crosshair", "crosshairGrenadeLauncher.obj")
	});

	_hp = _maxHp = 110;
	_shield = _maxShield = 60;

	_hud = new HUD(PlayerClass::DEMOLITION);
}

