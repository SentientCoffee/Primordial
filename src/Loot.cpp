#include "Loot.h"

Loot::Loot(Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes) : GameObject(SHADER, textures, meshes)
{
}

void Loot::childUpdate(float dt)
{
}

Sednium::Sednium(Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures) : Loot(SHADER, textures, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("sednium.obj") })
{
	setActive(false);
}

void Sednium::pickup(Class* player)
{
	if (player->_rigidBody.checkCollision(Cappuccino::HitBox(_rigidBody._position, 1.0f), _rigidBody._position))
	{
		setActive(false);
		player->addCurrency();
	}
}

void Sednium::spawn(float weight, std::vector<Loot*> loot, glm::vec3 pos)
{
	//for (unsigned i = 0; i <= weight * 10.0f; i++)// spawn needs to be reworked for random # to spawn while considering weight
	//{
		Sednium* temp = new Sednium(_shader, _textures);
		temp->setActive(true);
		temp->setPosition(pos);
		loot.push_back(temp);
	//}
}

HealthPack::HealthPack(Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures) : Loot(SHADER, textures, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("sednium.obj") })
{
	setActive(false);
}

void HealthPack::pickup(Class* player)
{
	if (player->_rigidBody.checkCollision(Cappuccino::HitBox(_rigidBody._position, 1.0f), _rigidBody._position))
	{
		setActive(false);
		player->addCurrency();
	}
}

void HealthPack::spawn(float weight, std::vector<Loot*> loot, glm::vec3 pos)
{
	//for (unsigned i = 0; i <= weight * 1.0f; i++)
	//{
		HealthPack* temp = new HealthPack(_shader, _textures);
		temp->setActive(true);
		temp->setPosition(pos);
		loot.push_back(temp);
	//}
}

AmmoPack::AmmoPack(Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures) : Loot(SHADER, textures, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("sednium.obj") })
{
	setActive(false);
}

void AmmoPack::pickup(Class* player)
{
	if (player->_rigidBody.checkCollision(Cappuccino::HitBox(_rigidBody._position, 1.0f), _rigidBody._position))
	{
		setActive(false);
		player->addCurrency();
	}
}

void AmmoPack::spawn(float weight, std::vector<Loot*> loot, glm::vec3 pos)
{
	//for (unsigned i = 0; i <= weight * 1.0f; i++)
	//{
		AmmoPack* temp = new AmmoPack(_shader, _textures);
		temp->setActive(true);
		temp->setPosition(pos);
		loot.push_back(temp);
	//}
}
