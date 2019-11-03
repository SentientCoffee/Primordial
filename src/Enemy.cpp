#include "..\include\Enemy.h"
#include "Cappuccino/CappMath.h"
#include "Cappuccino/HitBoxLoader.h"

Enemy::Enemy(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass)
	:Cappuccino::GameObject(*SHADER, textures, meshs, mass)
{
	_rigidBody._position = glm::vec3(0.0f, 0.0f, 10.0f);
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/SentryBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	enemyGun = new Pistol(*SHADER, std::vector<Cappuccino::Texture*>{}, meshs, std::string("testWeapon"), 1.0f, 0.5f, 200);

	enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");
}

void Enemy::childUpdate(float dt)
{
	enemyGun->setDelay(dt);

	auto norm = glm::normalize(_rigidBody._vel);

	enemyGun->shoot(glm::vec3(norm), _rigidBody._position);
}

void Enemy::trackGO(GameObject* other, float speed)
{
	//lerpSpeed = speed;
	//_rigidBody._position = (Cappuccino::Math::lerp(_rigidBody._position, other->_rigidBody._position, lerpFloat));

	auto newPos = other->_rigidBody._position - _rigidBody._position;
	_transform._transformMat[0].x = newPos.x * -1.0f;
	_transform._transformMat[0].z = newPos.z * -1.0f;

	auto normOther = glm::normalize(newPos);

	_rigidBody.setVelocity(normOther * 3.0f);

}
