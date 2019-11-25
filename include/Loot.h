#pragma once
#include "Cappuccino/GameObject.h"
#include "UIPointLight.h"
#include "Class.h"

class Loot : public Cappuccino::GameObject {
public:
	Loot(Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void childUpdate(float dt) override;

	virtual void pickup(Class* player) = 0;
	virtual void spawn(float weight, std::vector<Loot*> loot, glm::vec3 pos) = 0;

};

class Sednium : public Loot {
public:
	Sednium(Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures);
	void pickup(Class* player) override;
	void spawn(float weight, std::vector<Loot*> loot, glm::vec3 pos)override;

};

class HealthPack : public Loot {
public:
	HealthPack(Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures);
	void pickup(Class* player)override;
	void spawn(float weight, std::vector<Loot*> loot, glm::vec3 pos)override;

};

class AmmoPack : public Loot {
public:
	AmmoPack(Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures);
	void pickup(Class* player)override;
	void spawn(float weight, std::vector<Loot*> loot, glm::vec3 pos)override;

};