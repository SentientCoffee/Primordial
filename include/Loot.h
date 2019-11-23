#pragma once
#include "Cappuccino/GameObject.h"
#include "UIPointLight.h"
#include "Class.h"

class Loot : public Cappuccino::GameObject {
public:
	Loot(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void childUpdate(float dt) override;

	virtual void pickup(Class* player) = 0;
	virtual void spawn(float weight, std::vector<Loot*> loot, glm::vec3 pos) = 0;

protected:
	UIPointLight* _uiLight = new UIPointLight({ 1600.0f, 1200.0f }, _rigidBody._position, { 0.05f, 0.05f, 0.05f }, { 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f, 0.5f }, 32.0f);
	Cappuccino::Texture* diffuse = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::DiffuseMap);
	Cappuccino::Texture* spec = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::SpecularMap);
};

class Sednium : public Loot {
public:
	Sednium();
	void pickup(Class* player) override;
	void spawn(float weight, std::vector<Loot*> loot, glm::vec3 pos) override;
};

class HealthPack : public Loot {
public:
	HealthPack();
	void pickup(Class* player)override;
	void spawn(float weight, std::vector<Loot*> loot, glm::vec3 pos) override;

};

class AmmoPack : public Loot {
public:
	AmmoPack();
	void pickup(Class* player)override;
	void spawn(float weight, std::vector<Loot*> loot, glm::vec3 pos) override;

};