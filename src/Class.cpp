#include "Class.h" 
#include "Cappuccino/Application.h"

#include <Cappuccino/ResourceManager.h>

Cappuccino::Texture* Class::diffuse = nullptr;
Cappuccino::Texture* Class::metallic = nullptr;
Cappuccino::Texture* Class::norm = nullptr;
Cappuccino::Texture* Class::emission = nullptr;
Cappuccino::Texture* Class::height = nullptr;
Cappuccino::Texture* Class::roughness = nullptr;
Cappuccino::Texture* Class::ao = nullptr;
Cappuccino::Shader* Class::_uiLightShader = nullptr;
std::vector<Cappuccino::PointLight> Class::_uiLights = {};
Class::Class(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes) :
	GameObject(*SHADER, textures, meshes, 1.0f), //change this field later (mass)
	_input(true, 0),
	_shieldRecharge("shieldRecharge.wav"),
	_shieldDown("shieldDown.wav", "Shield")
{
	_shieldRecharge.setGroupHandle(_shieldDown.getGroupHandle());


	static bool init = false;
	if (!init) {
		//_uiLightShader = new Cappuccino::Shader(std::string("class shader"), "pointLightUI.vert", "PBRUI.frag");
		_uiLightShader = Cappuccino::Application::_gBufferShader;
		diffuse = Cappuccino::TextureLibrary::loadTexture("Pistol diffuse", "pistol/pistol-Diffuse.png", Cappuccino::TextureType::PBRAlbedo);
		metallic = Cappuccino::TextureLibrary::loadTexture("Pistol specular", "pistol/pistol-Metallic.png", Cappuccino::TextureType::PBRMetallic);
		norm = Cappuccino::TextureLibrary::loadTexture("Pistol normal", "pistol/pistol-Norm.png", Cappuccino::TextureType::PBRNormal);
		emission = Cappuccino::TextureLibrary::loadTexture("Pistol emission", "pistol/pistol-Emission.png", Cappuccino::TextureType::PBREmission);
		height = Cappuccino::TextureLibrary::loadTexture("Pistol height", "pistol/pistol-Height.png", Cappuccino::TextureType::HeightMap);
		roughness = Cappuccino::TextureLibrary::loadTexture("Pistol roughness", "pistol/pistol-Roughness.png", Cappuccino::TextureType::PBRRoughness);
		ao = Cappuccino::TextureLibrary::loadTexture("Pistol AO", "pistol/pistol-AO.png", Cappuccino::TextureType::PBRAmbientOcc);
		init = true;
	}


	_playerCamera = new Cappuccino::Camera;
	_playerCamera->lookAt({ 0.0f, 0.0f, 0.0f });

	_secondary = new Pistol(*_uiLightShader, {
		diffuse, metallic, norm, emission, height, ao
		}, {
			Cappuccino::MeshLibrary::loadMesh("Pistol", "pistol.obj")
		}, "Energy Pistol", 30.0f, 0.35f, 1);


	_primary = _secondary;
	_secondary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_secondary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_secondary->_transform._translateMat[3].y += 0.1f;

	_rigidBody._moveable = true;
	_rigidBody._creature = true;
	//	_rigidBody._creature = true;
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(_rigidBody._position, glm::vec3(1.0f, 4.0f, 1.0f)));
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(_rigidBody._position, glm::vec3(1.0f, 4.0f, 1.0f)));
	_rigidBody.setGrav(true);
	id = "Player";

}

void Class::childUpdate(float dt)
{
	///REMOVE THIS AFTER TESTING IS DONE
	static bool flymode = false;
	{
		//_hp = 1000;

		if (this->_input.keyboard->keyPressed(Events::K)) {
			_hp = 0;
			toggleHud();
		}

		static bool pressed = false;
		if (_input.keyboard->keyPressed(Cappuccino::KeyEvent::LEFT_CONTROL) && !pressed) {
			pressed = true;
			flymode ^= 1;
			_rigidBody._canTouch ^= 1;
		}
		else if (!_input.keyboard->keyPressed(Cappuccino::KeyEvent::LEFT_CONTROL))
			pressed = false;

		if (this->_input.keyboard->keyPressed(Cappuccino::KeyEvent::V))
			_voiceLines->playEvent(0);

	}
	///REMOVE THIS AFTER TESTING IS DONE



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

	static float deltaHP = 0;
	static float lastHP = 0;

	deltaHP = _hp - lastHP;
	if (deltaHP < 0.0f && !_voiceLines->isEventPlaying((int)voiceLine::GettingHit))
		_voiceLines->playEvent((int)voiceLine::GettingHit);
	lastHP = _hp;

	if (_hp < _maxHp / 2) {
		static float delay = 0.0f;
		if (!_voiceLines->isEventPlaying((int)voiceLine::LowHealth) && delay < 0.0f) {
			delay = Cappuccino::randomFloat(5.0f, 10.0f);
			_voiceLines->playEvent((int)voiceLine::LowHealth);
		}
		delay -= dt;
	}

	_hud->setHealth(static_cast<unsigned>(std::ceilf(_hp)));
	_hud->setShield(static_cast<unsigned>(std::ceilf(_shield)));
	_hud->setAmmoCount(getGun()->getAmmoCount());
	_hud->setAmmoMax(getGun()->getMaxAmmo());
	_hud->setCurrencyCount(_currency);
	if (!_shopping)
		_hud->toggleCrosshair(_primary->isActive());
	else
		toggleShopping();
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
			moveForce += glm::vec3(0.0f, 1.0f, 0.0f) * _speed;
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
				this->_rigidBody.addVelocity(glm::vec3(0.0f, 12.0f, 0.0f));
				_jumpDelay = 0.65f;
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


		_uiLightShader->use();
		_uiLightShader->setUniform("posVarience", 0.05f * glm::smoothstep(0.0f, 1.0f, u));
		_uiLightShader->setUniform("PlayerPosition", _rigidBody._position);
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

	_testRay._rayPos = _rigidBody._position;
	_testRay._rayDir = _playerCamera->getFront();

	updateFmodInfo();
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

void Class::setActive(const bool yn)
{
	GameObject::setActive(yn);
	_primary->setActive(yn);
	gunToggle = true;
}

void Class::resendLights()
{
	//static bool first = true;
	//_uiLightShader->use();
	//if (first) {
	//	first = false;
	//	_uiLightShader->setUniform("material.albedo", (int)Cappuccino::TextureType::PBRAlbedo);
	//	_uiLightShader->setUniform("material.normalMap", (int)Cappuccino::TextureType::PBRNormal);
	//	_uiLightShader->setUniform("material.metallic", (int)Cappuccino::TextureType::PBRMetallic);
	//	_uiLightShader->setUniform("material.roughness", (int)Cappuccino::TextureType::PBRRoughness);
	//	_uiLightShader->setUniform("material.ambientOcc", (int)Cappuccino::TextureType::PBRAmbientOcc);
	//	_uiLightShader->setUniform("material.emission", (int)Cappuccino::TextureType::PBREmission);
	//	_uiLightShader->loadProjectionMatrix(1600.0f, 1200.0f);
	//}
	//_uiLightShader->setUniform("numLights", (int)_uiLights.size());
	//
	//for (unsigned i = 0; i < _uiLights.size(); i++) {
	//	_uiLightShader->setUniform("lights[" + std::to_string(i) + "].position", _uiLights[i]._pos);
	//	_uiLightShader->setUniform("lights[" + std::to_string(i) + "].colour", _uiLights[i]._col);
	//	_uiLightShader->setUniform("lights[" + std::to_string(i) + "].active", (int)_uiLights[i]._isActive);
	//
	//}
}

void Class::updateFmodInfo()
{
	//using namespace Cappuccino;
	//FMOD_3D_ATTRIBUTES f = { {0} };
	//StudioSound::_system->getListenerAttributes(0,&f);
	//f.forward = glmToFmod(glm::vec3(0.0f, 0.0f, 1.0f));
	//f.up = glmToFmod(glm::vec3(0.0f, 1.0f, 0.0f));
	//f.velocity = glmToFmod(glm::vec3(0.0f, 0.0f, 0.0f));
	//f.position = Cappuccino::glmToFmod(_rigidBody._position);
	//f.position.z *= -1.0f;
	//Cappuccino::StudioSound::_system->setListenerAttributes(0, &f);
}



Commando::Commando(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes) {

	const auto diffuse = Cappuccino::TextureLibrary::loadTexture("Auto rifle diffuse", "autoRifle/autoRifle-Diffuse.png", Cappuccino::TextureType::PBRAlbedo);
	const auto metallic = Cappuccino::TextureLibrary::loadTexture("Auto rifle specular", "autoRifle/autoRifle-Metallic.png", Cappuccino::TextureType::PBRMetallic);
	const auto norm = Cappuccino::TextureLibrary::loadTexture("Auto rifle normal", "autoRifle/autoRifle-Norm.png", Cappuccino::TextureType::PBRNormal);
	const auto emission = Cappuccino::TextureLibrary::loadTexture("Auto rifle emission", "autoRifle/autoRifle-Emission.png", Cappuccino::TextureType::PBREmission);
	const auto roughness = Cappuccino::TextureLibrary::loadTexture("Auto rifle roughness", "autoRifle/autoRifle-Roughness.png", Cappuccino::TextureType::PBRRoughness);
	const auto AO = Cappuccino::TextureLibrary::loadTexture("Auto rifle AO", "autoRifle/autoRifle-AO.png", Cappuccino::TextureType::PBRAmbientOcc);


	_voiceLines = new Cappuccino::SoundBank("Commando.bank");
	_voiceLines->addEvent("{f925802b-e40c-4df6-af1b-f09937da9752}");
	_voiceLines->addEvent("{e3101099-40f0-4bf9-b4c8-8d3777b4ee86}");
	_voiceLines->addEvent("{52f5ebde-c0ff-416a-978a-6aaeac6d3d96}");
	_voiceLines->addEvent("{7fce5a2d-595c-4958-a829-de8735d0d54e}");
	_voiceLines->addEvent("{640f1da2-2ffe-406b-b9f5-d110b45c64d9}");
	_voiceLines->addEvent("{f8ec2b8d-e026-4fd0-b418-bea63079cb59}");
	_voiceLines->addEvent("{a226ede7-204f-43e7-ab03-d2b4cf7177bb}");
	_voiceLines->addEvent("{91389cc6-5523-4805-aea6-be0f32ede7e7}");
	_voiceLines->addEvent("{cf14df37-a1c0-423c-9ea1-2587748a5f09}");
	_voiceLines->addEvent("{9f4a4b25-76a9-47ec-88c0-bbee3ebe667b}");
	_voiceLines->addEvent("{d8244b35-66ed-4993-8dea-dc2b1c7a5a6e}");
	_voiceLines->addEvent("{2d984d79-9ba8-4b50-acb7-dc099c33e047}");
	_voiceLines->addEvent("{e67cd013-4673-4d82-9b66-77f3871507f3}");
	_voiceLines->addEvent("{2f05901f-df29-47d7-8564-6bdb93fab379}");
	_voiceLines->addEvent("{1ad530ed-a2eb-4007-8b63-d16deddb4f43}");
	_voiceLines->addEvent("{13d6a4b5-1cf2-41c4-8a27-24ebdcbccf8c}");
	_voiceLines->addEvent("{e1214ec6-c6c3-4c2a-b1e4-83320be9179e}");
	_voiceLines->addEvent("{7dfb2c9d-e9ec-4da5-a87e-a331a98a5b69}");

	_primary = new HSAR(*_uiLightShader, {
		diffuse, metallic, norm, emission, roughness,AO
		}, {
			Cappuccino::MeshLibrary::loadMesh("Auto rifle", "autoRifle.obj")
		}, "Assault Rifle", 20.0f, 0.15f, 150);

	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;

	_hp = _maxHp = 100;
	_shield = _maxShield = 50;

	//user interface
	_hud = new HUD(PlayerClass::COMMANDO);
}

Assault::Assault(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes) {

	_voiceLines = new Cappuccino::SoundBank("Assault.bank");


	_voiceLines->addEvent("{d979cdd7-d2d6-417b-af7a-fa7fa04f4ca0}");
	_voiceLines->addEvent("{e3fcfe97-6788-4377-8b3f-6f762487af74}");
	_voiceLines->addEvent("{453b2147-b94a-4d7a-8561-7203e8960e38}");
	_voiceLines->addEvent("{f83905d6-6b92-40a2-b1b9-7da9f3b54469}");
	_voiceLines->addEvent("{6e918a88-bf62-4bd1-a64f-4aec545217ed}");
	_voiceLines->addEvent("{a1e9a8ee-393e-4290-a81e-50fc51a3949e}");
	_voiceLines->addEvent("{88d6521d-328a-4c03-bb3e-42dc3061d8a8}");
	_voiceLines->addEvent("{ed84753b-e035-4643-95bd-20da3f24254a}");
	_voiceLines->addEvent("{a9dbc034-2e3a-4375-896a-2dc5f08ceadd}");
	_voiceLines->addEvent("{244a40f4-0bcc-440b-a643-35787f0fd97d}");
	_voiceLines->addEvent("{7a779d12-4e48-45c3-8a24-5585a8ae5a1a}");
	_voiceLines->addEvent("{e09d8d52-7341-490d-b697-6095e6917012}");
	_voiceLines->addEvent("{c37851f1-9979-46d1-bf37-741ca9df8ef6}");
	_voiceLines->addEvent("{309c7cc7-791c-48c5-9af5-cae12aef3e17}");
	_voiceLines->addEvent("{02379e8d-fad0-4fe9-b5c3-e53e27534f4e}");
	_voiceLines->addEvent("{8eed76dd-b16a-439e-bc58-24ff6722bd4b}");
	_voiceLines->addEvent("{10e82eed-b0cd-443c-ab3d-56317d7736b4}");
	_voiceLines->addEvent("{73c432f8-4ce7-4c91-a88a-26a6b887457d}");

	const auto diffuse = Cappuccino::TextureLibrary::loadTexture("Shotgun diffuse", "shotgun/shotgun-Diffuse.png", Cappuccino::TextureType::PBRAlbedo);
	const auto metallic = Cappuccino::TextureLibrary::loadTexture("Shotgun specular", "shotgun/shotgun-Metallic.png", Cappuccino::TextureType::PBRMetallic);
	const auto norm = Cappuccino::TextureLibrary::loadTexture("Shotgun normal", "shotgun/shotgun-Norm.png", Cappuccino::TextureType::PBRNormal);
	const auto emission = Cappuccino::TextureLibrary::loadTexture("Shotgun emission", "shotgun/shotgun-Emission.png", Cappuccino::TextureType::PBREmission);
	const auto roughness = Cappuccino::TextureLibrary::loadTexture("Shotgun roughness", "shotgun/shotgun-Roughness.png", Cappuccino::TextureType::PBRRoughness);


	_primary = new SG(*_uiLightShader, {
		diffuse, metallic, norm, emission, roughness,
		Cappuccino::TextureLibrary::loadTexture("Hands diffuse", "handsDiffuse.png", Cappuccino::TextureType::PBRAlbedo, 1)
		}, {
			Cappuccino::MeshLibrary::loadMesh("Shotgun", "shotgun.obj"), Cappuccino::MeshLibrary::loadMesh("Shotgun hands", "shotgunHands.obj")
		}, "Shotgun", 9, 0.66f, 32, 9);

	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;

	_hp = _maxHp = 125;
	_shield = _maxShield = 65;

	//user interface
	_hud = new HUD(PlayerClass::ASSAULT);
}

Scout::Scout(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes) {

	_voiceLines = new Cappuccino::SoundBank("Scout.bank");
	_voiceLines->addEvent("{c130b293-ae57-4022-87a7-91d5d9e6ba6f}");
	_voiceLines->addEvent("{978a80f4-6945-4beb-a816-1352902e11ad}");
	_voiceLines->addEvent("{d855b1d0-7279-4729-9a67-c404c49d17c1}");
	_voiceLines->addEvent("{6a758b1e-30b5-4dcc-bf94-b84c7fdc9c52}");
	_voiceLines->addEvent("{3661a975-25a1-4fe6-a75e-b85838eca1fc}");
	_voiceLines->addEvent("{2b62232d-2d5f-4cf6-a880-b42ff1eb996e}");
	_voiceLines->addEvent("{b180fe80-90d3-4c20-9779-cecbb3672e43}");
	_voiceLines->addEvent("{ad073a82-6c8a-4042-b0b2-dc6e0dfb54ab}");
	_voiceLines->addEvent("{42d23885-3dce-421b-9703-e44739ed896e}");
	_voiceLines->addEvent("{310d2573-5569-4547-8535-00d429c901e8}");
	_voiceLines->addEvent("{675d96e9-7337-454d-a2e8-59c97ff3a369}");
	_voiceLines->addEvent("{6b9e449d-4ed0-457d-b367-10d4c4e2ed22}");
	_voiceLines->addEvent("{5109c5ed-d853-487b-b3b4-d14f83163c1e}");
	_voiceLines->addEvent("{89cf83b5-df97-45da-9aa9-d1504d30d5b1}");
	_voiceLines->addEvent("{d800a8dd-4d74-4331-b423-8294ef4a39e1}");
	_voiceLines->addEvent("{8e7591fb-4d50-4eae-beda-1ee054cca521}");
	_voiceLines->addEvent("{15c0070a-3d59-4505-a458-5914e7a0c1e4}");
	_voiceLines->addEvent("{c10f08f0-b3de-4872-a521-651433988fca}");

	const auto diffuse = Cappuccino::TextureLibrary::loadTexture("SAR diffuse", "marksmanRifle/marksmanRifle-Diffuse.png", Cappuccino::TextureType::PBRAlbedo);
	const auto metallic = Cappuccino::TextureLibrary::loadTexture("SAR specular", "marksmanRifle/marksmanRifle-Metallic.png", Cappuccino::TextureType::PBRMetallic);
	const auto norm = Cappuccino::TextureLibrary::loadTexture("SAR normal", "marksmanRifle/marksmanRifle-Normal.png", Cappuccino::TextureType::PBRNormal);
	const auto emission = Cappuccino::TextureLibrary::loadTexture("SAR emission", "marksmanRifle/marksmanRifle-Emission.png", Cappuccino::TextureType::PBREmission);
	const auto roughness = Cappuccino::TextureLibrary::loadTexture("SAR roughess", "marksmanRifle/marksmanRifle-Roughness.png", Cappuccino::TextureType::PBRRoughness);
	const auto aOcc = Cappuccino::TextureLibrary::loadTexture("SAR aOcc", "marksmanRifle/marksmanRifle-AO.png", Cappuccino::TextureType::PBRAmbientOcc);

	_primary = new AR(*_uiLightShader, {
		diffuse, metallic, norm, emission, roughness,aOcc
		}, {
			Cappuccino::MeshLibrary::loadMesh("SAR", "marksmanRifle.obj")
		}, "Semi Auto Rifle", 75.0f, 0.75f, 50);

	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;

	_hp = _maxHp = 75;
	_shield = _maxShield = 35;

	//user interface
	_hud = new HUD(PlayerClass::SCOUT);
}

Demolitionist::Demolitionist(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Class(SHADER, textures, meshes)
{
	_voiceLines = new Cappuccino::SoundBank("Demolitionist.bank");

	_voiceLines->addEvent("{a8aea502-b263-4332-866a-93603120ca17}");
	_voiceLines->addEvent("{355d109a-a93b-4ee9-9c35-3862e83ff658}");
	_voiceLines->addEvent("{d514af8f-e109-4884-ab6c-7b54b9f820d5}");
	_voiceLines->addEvent("{090188c3-8fcc-4e71-80f5-9dba5d833674}");
	_voiceLines->addEvent("{65d1aa54-01f1-4d5d-8331-9585b4728216}");
	_voiceLines->addEvent("{a9728512-6714-4dc4-b3d2-b86c60c22c32}");
	_voiceLines->addEvent("{e1c2c589-4fe1-436c-b2a7-07911743321a}");
	_voiceLines->addEvent("{02a460d2-0d02-49ee-9097-db1398a5db54}");
	_voiceLines->addEvent("{c2fd18f6-9815-4d40-8e5f-e10c3f87ca9a}");
	_voiceLines->addEvent("{0c989155-0584-4dfb-a439-15be9acb2fa5}");
	_voiceLines->addEvent("{e4fd2c72-b126-47cf-80bd-4c80bbf64317}");
	_voiceLines->addEvent("{6581edec-15a4-41e3-8c60-aca9f0d26264}");
	_voiceLines->addEvent("{e9e71294-e2e6-4161-99e8-1e79a097f8c0}");
	_voiceLines->addEvent("{cf7f21d1-95c5-4f5f-8858-02989f021840}");
	_voiceLines->addEvent("{fbdeb194-17d2-45da-8bf5-47ef1c1d81b6}");
	_voiceLines->addEvent("{11823bda-0d7b-45ad-96f5-11cb54d44162}");
	_voiceLines->addEvent("{65ed5944-d205-4067-9fc2-b36f64bbb659}");
	_voiceLines->addEvent("{5a433ce2-191a-4491-ba0f-9323d9847fb8}");


	const auto diffuse = Cappuccino::TextureLibrary::loadTexture("Grenade launcher diffuse", "grenadeLauncher/grenadeLauncher-Diffuse.png", Cappuccino::TextureType::PBRAlbedo);
	const auto metallic = Cappuccino::TextureLibrary::loadTexture("Grenade launcher specular", "grenadeLauncher/grenadeLauncher-Metallic.png", Cappuccino::TextureType::PBRMetallic);
	const auto norm = Cappuccino::TextureLibrary::loadTexture("Grenade launcher normal", "grenadeLauncher/grenadeLauncher-Normal.png", Cappuccino::TextureType::PBRNormal);
	const auto emission = Cappuccino::TextureLibrary::loadTexture("Grenade launcher emission", "grenadeLauncher/grenadeLauncher-Emission.png", Cappuccino::TextureType::PBREmission);
	const auto roughness = Cappuccino::TextureLibrary::loadTexture("Grenade launcher roughness", "grenadeLauncher/grenadeLauncher-Roughness.png", Cappuccino::TextureType::PBRRoughness);

	_primary = new GL(*_uiLightShader, {
		diffuse, metallic, norm, emission,roughness
		}, {
			Cappuccino::MeshLibrary::loadMesh("Grenade launcher", "grenadeLauncher.obj")
		}, "Grenade Launcher", 80.0f, 0.7f, 35);

	//user interface
	_primary->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
	_primary->_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f);
	_primary->_transform._translateMat[3].y += 0.1f;

	_hp = _maxHp = 110;
	_shield = _maxShield = 60;

	_hud = new HUD(PlayerClass::DEMOLITION);
}

