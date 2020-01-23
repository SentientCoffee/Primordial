#include "Loot.h"

Loot::Loot(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes) : GameObject(SHADER, textures, meshes)
{
	setActive(false);
	_rigidBody.setGrav(true);
	_rigidBody._moveable = true;
	_rigidBody._canTouch = true;
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(glm::vec3(0.0f,1.0f,0.0f), glm::vec3(1.0f)));
	_rigidBody.myType = "Loot";
	this->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), .5f);
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
	temp->_rigidBody.addAccel(glm::vec3(0.0f, 12.0f, 0.0f));
	return temp;
	//}
}

Bullion::Bullion(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures) : Loot(SHADER, textures, { new Cappuccino::Mesh("lootChest-contents.obj") })
{
}

void Bullion::pickup(Class* player)
{
	if (player->checkCollision(Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f)), _rigidBody._position))
	{
		setActive(false);
		for (int i = 0; i < 5; i++)
			player->addCurrency();
	}
}

Bullion* Bullion::spawn(float weight, glm::vec3 pos)
{
	//for (unsigned i = 0; i <= weight * 10.0f; i++)// spawn needs to be reworked for random # to spawn while considering weight
	//{
	Bullion* temp = new Bullion(_shader, _textures);
	temp->setActive(true);
	temp->_rigidBody._position = pos;
	return temp;
	//}
}

Chest::Chest(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector <Cappuccino::Mesh*>& mesh) : GameObject(SHADER, textures, mesh)
{
	setActive(false);
}

void Chest::childUpdate(float dt)
{
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
