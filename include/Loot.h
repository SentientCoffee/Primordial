#pragma once
#include "Cappuccino/GameObject.h"
#include "Class.h"
#include "Cappuccino\Random.h"
#include "Cappuccino/ResourceManager.h"


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

class Bullion : public Loot {
public:
	Bullion(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures);
	void pickup(Class* player) override;
	Bullion* spawn(float weight, glm::vec3 pos) override;
};

class Chest : public Cappuccino::GameObject {
public:
	Chest(Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector <Cappuccino::Mesh*>& mesh = { Cappuccino::MeshLibrary::loadMesh("Loot chest closed", "Chest/Chest_closed.obj") });

	void childUpdate(float dt) override;

	Chest* Chest::spawn(glm::vec3 pos);
	std::vector<Loot*> spawn(float weight, glm::vec3 pos, Sednium* sednium, HealthPack* healthpack, AmmoPack* ammopack, Bullion* bullion);
	bool open();

	Cappuccino::HitBox _triggerVolume = Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5.0f, 5.0f, 5.0f));


	bool _opened = false;
private:
	
};
