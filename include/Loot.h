#pragma once
#include "Cappuccino/GameObject.h"
#include "UIPointLight.h"
#include "Class.h"

class Loot : public Cappuccino::GameObject {
public:
	Loot(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void childUpdate(float dt) override;

	virtual void pickup(Class* player) = 0;
	virtual Loot* spawn(float weight, glm::vec3 pos) = 0;

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

class Chest : public Cappuccino::GameObject {
public:
	Chest(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures);

	void childUpdate(float dt) override;

	std::vector<Loot*> spawn(float weight, glm::vec3 pos, Sednium* sednium, HealthPack* healthpack, AmmoPack* ammopack);
	bool open();

	Cappuccino::HitBox _triggerVolume = Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5.0f, 5.0f, 5.0f));

private:
	bool _opened = false;
};