#include "Loot.h"

Loot::Loot(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes) : GameObject(SHADER, textures, meshes)
{
}

void Loot::childUpdate(float dt)
{
}

void Loot::pickup(Class* player)
{
}

Loot* Loot::spawn(float weight, glm::vec3 pos)
{
	return nullptr;
}

Sednium::Sednium(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures) : Loot(SHADER, textures, { new Cappuccino::Mesh("sednium.obj") })
{
	setActive(false);
}

void Sednium::pickup(Class* player)
{
	if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f)), _rigidBody._position))
	{
		setActive(false);
		player->addCurrency();
	}
}

Sednium* Sednium::spawn(float weight, glm::vec3 pos)
{
	//for (unsigned i = 0; i <= weight * 10.0f; i++)// spawn needs to be reworked for random # to spawn while considering weight
	//{
	Sednium* temp = new Sednium(_shader, _textures);
	temp->setActive(true);
	temp->_rigidBody._position = pos;
	return temp;
	//}
}

HealthPack::HealthPack(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures) : Loot(SHADER, textures, { new Cappuccino::Mesh("healthPickup.obj") })
{
	setActive(false);
}

void HealthPack::pickup(Class* player)
{
	if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f)), _rigidBody._position))
	{
		setActive(false);
		player->addHealth();
	}
}

HealthPack* HealthPack::spawn(float weight, glm::vec3 pos)
{
	//for (unsigned i = 0; i <= weight * 1.0f; i++)
	//{
	HealthPack* temp = new HealthPack(_shader, _textures);
	temp->setActive(true);
	temp->_rigidBody._position = pos;
	return temp;
	//}
}

AmmoPack::AmmoPack(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures) : Loot(SHADER, textures, { new Cappuccino::Mesh("pickup-ammo.obj") })
{
	setActive(false);
}

void AmmoPack::pickup(Class* player)
{
	if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f)), _rigidBody._position))
	{
		setActive(false);
		player->addAmmo();
	}
}

AmmoPack* AmmoPack::spawn(float weight, const glm::vec3 pos)
{
	//for (unsigned i = 0; i <= weight * 1.0f; i++)
	//{
	AmmoPack* temp = new AmmoPack(_shader, _textures);
	temp->setActive(true);
	temp->_rigidBody._position = pos;
	return temp;
	//}
}

Chest::Chest(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures) : GameObject(SHADER, textures, { new Cappuccino::Mesh("lootChest-closed.obj") })
{
	setActive(false);
}

void Chest::childUpdate(float dt)
{
}

std::vector<Loot*> Chest::spawn(float weight, glm::vec3 pos, Sednium* sednium, HealthPack* healthpack, AmmoPack* ammopack)
{
	std::vector<Loot*> _contents;
	_contents.push_back(sednium->spawn(weight, pos));
	_contents.push_back(healthpack->spawn(weight, pos));
	_contents.push_back(ammopack->spawn(weight, pos));
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