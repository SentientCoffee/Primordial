#include "Loot.h"
#include <Cappuccino/ResourceManager.h>

Loot::Loot(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes) : GameObject(SHADER, textures, meshes)
{
	setActive(false);
	_rigidBody.setGrav(true);
	_rigidBody._moveable = true;
	_rigidBody._canTouch = true;
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(0.5f)));
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(0.5f)));
	_rigidBody.myType = "Loot";
	this->_transform.scale(glm::vec3(1.0f), 0.5f);
}

void Loot::childUpdate(float dt)
{
	if (_rigidBody._vel.y == 0.0f)
		_rigidBody.setVelocity(glm::vec3(0.0f));
}

void Loot::pickup(Class* player)
{
}

Loot* Loot::spawn(float weight, glm::vec3 pos)
{
	return nullptr;
}

Sednium::Sednium(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures) : Loot(SHADER, textures, { new Cappuccino::Mesh("Sednium","sednium.obj") })
{
}

void Sednium::pickup(Class* player)
{
	if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(5.0f)), _rigidBody._position))
		this->_rigidBody.addVelocity(glm::normalize(player->_rigidBody._position - _rigidBody._position));
	if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(1.0f)), _rigidBody._position))
	{
		setActive(false);
		player->addCurrency();
	}
}

Sednium* Sednium::spawn(float weight, glm::vec3 pos)
{
	if (float(Cappuccino::randomInt()) <= weight)
	{
		Sednium* temp = new Sednium(_shader, _textures);
		temp->setActive(true);
		temp->_rigidBody._position = pos;

		glm::vec3 speed = glm::vec3(Cappuccino::randomFloat() * 15.0f, Cappuccino::randomFloat() * 25.0f, Cappuccino::randomFloat() * 15.0f);
		temp->_rigidBody.addVelocity(speed);
		return temp;
	}
	else
		return new Sednium(_shader, _textures);
}

HealthPack::HealthPack(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures) :
	Loot(SHADER, textures, { Cappuccino::MeshLibrary::loadMesh("Health pack", "healthPickup.obj") }) {
	setActive(false);
	this->_transform.scale(glm::vec3(1.0f), 0.5f);
}

void HealthPack::pickup(Class* player)
{
	if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(5.0f)), _rigidBody._position))
		this->_rigidBody.addVelocity(glm::normalize(player->_rigidBody._position - _rigidBody._position));
	if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(1.0f)), _rigidBody._position))
	{
		setActive(false);
		player->addHealth();
	}
}

HealthPack* HealthPack::spawn(float weight, glm::vec3 pos)
{
	if (float(Cappuccino::randomInt()) <= weight)
	{
		HealthPack* temp = new HealthPack(_shader, _textures);
		temp->setActive(true);
		temp->_rigidBody._position = pos;

		glm::vec3 speed = glm::vec3(Cappuccino::randomFloat() * 15.0f, Cappuccino::randomFloat() * 25.0f, Cappuccino::randomFloat() * 15.0f);
		temp->_rigidBody.addVelocity(speed);
		return temp;
	}
	else
		return new HealthPack(_shader, _textures);
}

AmmoPack::AmmoPack(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures) : Loot(SHADER, textures, { Cappuccino::MeshLibrary::loadMesh("Ammo pack", "pickup-ammo.obj") })
{
	setActive(false);
}

void AmmoPack::pickup(Class* player)
{
	if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(5.0f)), _rigidBody._position))
		this->_rigidBody.addVelocity(glm::normalize(player->_rigidBody._position - _rigidBody._position));
	if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(1.0f)), _rigidBody._position))
	{
		setActive(false);
		player->addAmmo();
	}
}

AmmoPack* AmmoPack::spawn(float weight, const glm::vec3 pos)
{
	if (float(Cappuccino::randomInt()) <= weight)
	{
		AmmoPack* temp = new AmmoPack(_shader, _textures);
		temp->setActive(true);
		temp->_rigidBody._position = pos;

		glm::vec3 speed = glm::vec3(Cappuccino::randomFloat() * 15.0f, Cappuccino::randomFloat() * 25.0f, Cappuccino::randomFloat() * 15.0f);
		temp->_rigidBody.addVelocity(speed);
		return temp;
	}
	else
		return new AmmoPack(_shader, _textures);
}

Bullion::Bullion(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures) : Loot(SHADER, textures, { Cappuccino::MeshLibrary::loadMesh("Bullion", "lootChestContents.obj") })
{
}

void Bullion::pickup(Class* player)
{
	if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(5.0f)), _rigidBody._position))
		this->_rigidBody.addVelocity(glm::normalize(player->_rigidBody._position - _rigidBody._position));
	if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 1.0f)), _rigidBody._position))
	{
		setActive(false);
		for (int i = 0; i < 5; i++)
			player->addCurrency();
	}
}

Bullion* Bullion::spawn(float weight, glm::vec3 pos)
{
	if (float(Cappuccino::randomInt()) <= weight)
	{
		Bullion* temp = new Bullion(_shader, _textures);
		temp->setActive(true);
		temp->_rigidBody._position = pos;

		glm::vec3 speed = glm::vec3(Cappuccino::randomFloat() * 15.0f, Cappuccino::randomFloat() * 25.0f, Cappuccino::randomFloat() * 15.0f);
		temp->_rigidBody.addVelocity(speed);
		return temp;
	}
	else
		return new Bullion(_shader, _textures);
}

Chest::Chest(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector <Cappuccino::Mesh*>& mesh) : GameObject(SHADER, textures, mesh)
{
	setActive(false);
	
	auto temp = new Cappuccino::Mesh ("opened","Chest/Chest_open.obj");
	auto temp2 = new Cappuccino::Mesh("middle","Chest/Chest_halfway.obj");
	temp->loadMesh();
	temp2->loadMesh();
	_animator.addAnimation(new Cappuccino::Animation({ _meshes.back(),temp2,temp }, AnimationType::Interact));
	_animator.setLoop(AnimationType::Interact, true);
	_animator.setSpeed(AnimationType::Interact, 2.0f);
}

void Chest::childUpdate(float dt)
{
	if (_opened)
		_animator.playAnimation(AnimationType::Interact, dt);
}

Chest* Chest::spawn(glm::vec3 pos)
{
	Chest* temp = new Chest(_shader, _textures);
	temp->setActive(true);
	temp->_rigidBody._position = pos;
	return temp;
}

std::vector<Loot*> Chest::spawn(float weight, glm::vec3 pos, Sednium* sednium, HealthPack* healthpack, AmmoPack* ammopack, Bullion* bullion)
{
	std::vector<Loot*> _contents;
	_contents.push_back(sednium->spawn(weight, pos));
	_contents.push_back(healthpack->spawn(weight, pos));
	_contents.push_back(ammopack->spawn(weight, pos));
	_contents.push_back(bullion->spawn(weight, pos));
	return _contents;
}


bool Chest::open()
{
	if (!_opened)
	{
		_opened = true;
		return false;
	}
	else
		return _opened;
}
