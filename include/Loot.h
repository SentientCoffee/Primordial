#pragma once
#include "Cappuccino/GameObject.h"
#include "UIPointLight.h"
#include "Class.h"
#include <random>
#include <functional>


class Loot : public Cappuccino::GameObject {
public:
	Loot(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void childUpdate(float dt) override;

	virtual void pickup(Class* player) = 0;
	virtual Loot* spawn(float weight, glm::vec3 pos) = 0;

protected:
	float weightedLoot();
	glm::vec3 lootSpeed();

};

class Sednium : public Loot {
public:
	Sednium(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures);
	void pickup(Class* player) override;
	Sednium* spawn(float weight, glm::vec3 pos) override;
};

class HealthPack : public Loot {
public:
	HealthPack(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures);
	void pickup(Class* player) override;
	HealthPack* spawn(float weight, glm::vec3 pos) override;

};

class AmmoPack : public Loot {
public:
	AmmoPack(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures);
	void pickup(Class* player) override;
	AmmoPack* spawn(float weight, glm::vec3 pos) override;

};

class Bullion : public Loot {
public:
	Bullion(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures);
	void pickup(Class* player) override;
	Bullion* spawn(float weight, glm::vec3 pos) override;
};

class Chest : public Cappuccino::GameObject {
public:
	Chest(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector <Cappuccino::Mesh*>& mesh = { new Cappuccino::Mesh("lootChest-closed.obj") });

	void childUpdate(float dt) override;

	Chest* Chest::spawn(glm::vec3 pos);
	std::vector<Loot*> spawn(float weight, glm::vec3 pos, Sednium* sednium, HealthPack* healthpack, AmmoPack* ammopack, Bullion* bullion);
	bool open();

	Cappuccino::HitBox _triggerVolume = Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5.0f, 5.0f, 5.0f));

private:
	bool _opened = false;
};
