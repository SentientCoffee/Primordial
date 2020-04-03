#pragma once 
#include "Cappuccino/CappInput.h"
#include "Gun.h"
#include "PlayerHUD.h"
#include "Cappuccino/PointLight.h"

//cannot forward declare sound class for some reason??
#include "Cappuccino/SoundSystem.h"

enum class VoiceLine {
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
	Gun* getGun() const;
	Gun* getPrimary() const { return _primary; }
	void addAmmo(Bullet* primary, Bullet* secondary) const;
	void addCurrency(const int currency) { _currency += currency; }
	void setCurrency(const int currency) { _currency = currency; }
	int& getCurrency() { return _currency; }
	void addAmmo() const;
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
	void toggleCrosshair() { if (_primary->isVisible()) _hud->toggleCrosshair(true); else _hud->toggleCrosshair(false); }

	void setActive(bool yn);
	
	//created for shop

	void setCanShoot(const bool yn) { canShoot = yn; }

	void toggleHud() const { _hud->toggleHud(); }
	void toggleHud(const bool yn) const { _hud->toggleHud(yn); }

	void togglePauseScreen() const { _hud->togglePauseScreen(); }
	void disableCrosshair() const { _hud->disableCrosshair(); }

	void toggleShopping() const { if (_shopping) disableCrosshair(); }
	void setShopping(const bool yn) { _shopping = yn; }

	Cappuccino::Ray _testRay = Cappuccino::Ray(glm::vec3(0, -1, 0), glm::vec3(0));

	static Cappuccino::Shader* _uiLightShader;
	static std::vector<Cappuccino::PointLight> _uiLights;
	static void resendLights();


	void updateFmodInfo();
	Cappuccino::SoundBank* _voiceLines = nullptr;

protected:

	bool _shopping = false;
	bool canShoot  = true;

	static Cappuccino::Texture* diffuse;
	static Cappuccino::Texture* metallic;
	static Cappuccino::Texture* norm;
	static Cappuccino::Texture* emission;
	static Cappuccino::Texture* height;
	static Cappuccino::Texture* roughness;
	static Cappuccino::Texture* ao;

	//hands
	static Cappuccino::Texture* hAlbedo;
	static Cappuccino::Texture* hMetallic;
	static Cappuccino::Texture* hNorm;
	static Cappuccino::Texture* hEmissive;
	static Cappuccino::Texture* hRoughness;
	static Cappuccino::Texture* hAO;


	unsigned soundHandle = 0;
	unsigned groupHandle = 0;

	HUD* _hud                            = nullptr;
	Cappuccino::Shader* _crosshairShader = nullptr;
	Cappuccino::Camera* _playerCamera    = nullptr;
	Gun* _primary                        = nullptr;
	Pistol* _secondary                   = nullptr;

	bool gunToggle     = true;
	
	float _shieldTimer = 0.0f;
	float _shield      = 0.0f;
	float _maxShield   = 0.0f;

	float _hp          = 0.0f;
	float _maxHp       = 0.0f;

	int _currency      = 0;

	float _jumpDelay   = 2.0f;
	float _speed       = 7.0f;

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