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

	//_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(glm::vec3(0, 0, 0), glm::vec3(2.0f)));
	//_transform.scale(glm::vec3(0.5f, 1.0f, 0.5f), 1.0f);
}

void Enemy::childUpdate(float dt)
{
	_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180 * dt);
	
	auto norm = glm::normalize(_transform._transformMat[0]);
	_rigidBody.setVelocity(-norm * 2.0f);
}

void Enemy::trackGO(GameObject* other, float speed)
{
	//lerpSpeed = speed;
	//_rigidBody._position = (Cappuccino::Math::lerp(_rigidBody._position, other->_rigidBody._position, lerpFloat));

	auto newPos = other->_rigidBody._position - _rigidBody._position;

	auto normOther = newPos / sqrt(newPos.x * newPos.x + newPos.y * newPos.y
		+ newPos.z * newPos.z);

	_rigidBody.setVelocity(normOther * 3.0f);

}
