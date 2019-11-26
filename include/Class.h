#pragma once 
#include "Cappuccino/CappInput.h"
#include "UIPointLight.h"
#include "Gun.h"
#include "PlayerHUD.h"
/*
class Sednium : public Cappuccino::GameObject {
public:
	Sednium(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures);

	void childUpdate(float dt) override;


};
*/
class Class : public Cappuccino::GameObject {
public:
	
	Class(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void childUpdate(float dt) override;

	Cappuccino::CappInput _input;
	Cappuccino::Camera* getCamera() { return _playerCamera; }
	Gun* getGun();
	void addAmmo(Bullet* primary, Bullet* secondary);
	void addCurrency();
	void addAmmo();
	void addHealth();

	float getShield() const { return _shield; }
	void setShield(const float shield) { _shield = shield; }
	float getHealth() const { return _hp; }
	void setHealth(const float hp) { _hp = hp; }

	float getMaxShield() const { return _maxShield; }
	float getMaxHp() const { return _maxHp; }

	void takeDamage(float dmg);

	void toggleGun(bool gun);

	void setActive(bool yn);
	UIPointLight& getUILight() { return _uiLight; }
	
protected:
	
	static Cappuccino::Texture* diffuse;
	static Cappuccino::Texture* spec;
	static Cappuccino::Texture* norm;
	static Cappuccino::Texture* emission;
	static Cappuccino::Texture* height;


	unsigned soundHandle = 0;
	unsigned groupHandle = 0;
	HUD* _hud;
	int _currency = 0;

	Cappuccino::Shader* _crosshairShader;
	Gun* _crosshair;
	Gun* _crosshairPrimary;
	UIPointLight _uiLight;
	Cappuccino::Camera* _playerCamera;
	Gun* _primary;
	Pistol* _secondary;
	bool gunToggle;

	float _shield;
	float _maxShield;
	float _hp;
	float _maxHp;

	float _speed = 3.5f;
};

class Commando : public Class {
public:
	Commando(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);
};

class Assault : public Class {
public:
	Assault(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);
};

class Scout : public Class {
public:
	Scout(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);
};

class Demolitionist : public Class {
public:
	Demolitionist(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);
};