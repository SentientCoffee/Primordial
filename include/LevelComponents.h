#pragma once
#include "Cappuccino/HitBox.h"
#include "Cappuccino/GameObject.h"

struct HurtBox {
public:
	HurtBox() {};
	HurtBox(Cappuccino::HitBox box,float dps);
	Cappuccino::HitBox _hurtBox{ glm::vec3(0),glm::vec3(0) };
	float _damagePerSecond = 0.0f;
};

struct GravLift {
public:
	GravLift() {};
	GravLift(Cappuccino::HitBox box, float lift, glm::vec3 dir = glm::vec3(0.0f,1.0f,0.0f));
	Cappuccino::HitBox _areaOfAffect{ glm::vec3(0),glm::vec3(0) };
	float _liftPower = 110.0f;
	glm::vec3 _direction{ 0.0f,1.0f,0.0f };
};

struct DoorLoc {
public:
	DoorLoc() {};
	DoorLoc(Cappuccino::HitBox box, float rotation);
	Cappuccino::HitBox _exitBox{ glm::vec3(0.0f), glm::vec3(0.0f) };
	float _rotation = 0.0f;
};

class Door : public Cappuccino::GameObject {
public:
	Door(float rotation, Cappuccino::HitBox hitbox, Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs);

	glm::vec3 _originalLoc = glm::vec3(0.0f);
	void setHitbox(Cappuccino::HitBox hitbox);
	void childUpdate(float dt) override;
	float _rotation = 0.0f;
	bool _locked = false;
};

struct TeleporterLoc {
public:
	TeleporterLoc() {};
	TeleporterLoc(glm::vec3 pos) { _position = pos; }
	glm::vec3 _position = glm::vec3();
	
};

class Teleporter : public Cappuccino::GameObject {
public:
	Teleporter(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs);

	void childUpdate(float dt) override;
	float _tpDelay = 5.0f;
	float _currentDelay = 0.0f;
	Cappuccino::HitBox _areaOfAffect{ glm::vec3(0.0f), glm::vec3(2.5f) };
};