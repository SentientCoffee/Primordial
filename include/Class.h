#pragma once 
#include "Cappuccino/CappInput.h"
#include "Gun.h"
#include "PlayerHUD.h"
#include "Cappuccino/PointLight.h"

//cannot forward declare sound class for some reason??
#include "Cappuccino/SoundSystem.h"

enum class voiceLine {
	SeeingEnemy,
	KillingEnemy,
	SeeingBoss,
	KillingBoss,
	SeeingHealth,
	CollectHealth,
	SeeingAmmo,
	CollectAmmo,
	SeeingSednium,
	CollectSednium,
	GettingHit,
	GettingKilled,
	Secret,
	Chest,
	BossNewPhase,
	Clear,
	LowHealth,
	LowAmmo
};

class Class : public Cappuccino::GameObject {
public:
	
	Class(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);
	
	void childUpdate(float dt) override;

	
	Cappuccino::CappInput _input;
	Cappuccino::Camera* getCamera() { return _playerCamera; }
	Gun* getGun();
	Gun* getPrimary() { return _primary; }
	void addAmmo(Bullet* primary, Bullet* secondary);
	void addCurrency();
	int& getCurrency() { return _currency; }
	void addAmmo();
	void addHealth();

	void rechargeShields();
	void disableShieldRegen(float disableTime);

	float getShield() const { return _shield; }
	void setShield(const float shield) { _shield = shield; }
	float getHealth() const { return _hp; }
	void setHealth(const float hp) { _hp = hp; }

	float getMaxShield() const { return _maxShield; }
	float getMaxHp() const { return _maxHp; }

	void takeDamage(float dmg);

	void toggleGun(bool gun);

	void setActive(bool yn);
	
	//created for shop
	bool getCrosshairPrimaryActive() { return false; }
	bool getCrosshairActive() { return false; }
	void setCrosshairPrimaryActive(bool yn) { false; }
	void setCrosshairActive(bool yn) { false; }

	void setCanShoot(bool yn) { canShoot = yn; }

	void toggleHud() { _hud->toggleHud(); }
	void toggleHud(bool yn) { _hud->toggleHud(yn); }

	Cappuccino::Ray _testRay = Cappuccino::Ray(glm::vec3(0,-1,0), glm::vec3(0));

	static Cappuccino::Shader* _uiLightShader;
	static std::vector<Cappuccino::PointLight> _uiLights;
	static void resendLights();


	void updateFmodInfo();
	Cappuccino::SoundBank* _voiceLines = nullptr;
protected:

	Cappuccino::Sound _shieldRecharge;
	Cappuccino::Sound _shieldDown;


	bool canShoot = true;

	static Cappuccino::Texture* diffuse;
	static Cappuccino::Texture* metallic;
	static Cappuccino::Texture* norm;
	static Cappuccino::Texture* emission;
	static Cappuccino::Texture* height;
	static Cappuccino::Texture* roughness;
	static Cappuccino::Texture* ao;



	unsigned soundHandle = 0;
	unsigned groupHandle = 0;
	HUD* _hud;
	int _currency = 0;

	Cappuccino::Shader* _crosshairShader;
	Cappuccino::Camera* _playerCamera;
	Gun* _primary;
	Pistol* _secondary;
	bool gunToggle;

	float _shieldTimer = 0.0f;
	float _shield;
	float _maxShield;
	float _hp;
	float _maxHp;

	float _jumpDelay = 2.0f;
	float _speed = 7.0f;

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