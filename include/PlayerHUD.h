#pragma once

#include <Cappuccino/UI.h>
#include <Cappuccino/GameObject.h>

enum class PlayerClass : unsigned int {
	ASSAULT = 0,
	COMMANDO,
	DEMOLITION,
	SCOUT
};

class enemyHUD : Cappuccino::UserInterface {
public:
	enemyHUD(std::string enemyName);
	~enemyHUD() = default;

	void setHealth(unsigned int hp) { _health = hp; };
	void setShield(unsigned int sp) { _shield = sp; };

	void updateHud(float dt);

	void toggleHud();

private:

	unsigned int _health, _shield, _maxHealth, _maxShield;

	Cappuccino::UIBar* _healthBar;
	Cappuccino::UIBar* _shieldBar;
	Cappuccino::UIBar* _healthLerpBG;
	Cappuccino::UIBar* _shieldLerpBG;
};

class HUD : public Cappuccino::UserInterface {
public:

	HUD(PlayerClass playerClass);
	~HUD() = default;

	void setHealth(unsigned int hp);
	void setShield(unsigned int sp);
	void setAmmoCount(unsigned int ac);
	void setAmmoMax(unsigned int am);
	void setCurrencyCount(unsigned int cp);

	void updateHud(float dt);

	void toggleHud();

private:

	PlayerClass _playerClass;
	std::string _class;

	unsigned int _health, _shield, _ammo, _currency;
	unsigned int   _maxHealth, _maxShield, _maxAmmo;

	glm::vec4 _colour;

	Cappuccino::UIBar* _healthBar;
	Cappuccino::UIText* _healthCount;

	Cappuccino::UIBar* _shieldBar;
	Cappuccino::UIText* _shieldCount;

	Cappuccino::UIText* _classLabel;
	Cappuccino::UIBar* _classBg;

	Cappuccino::UIText* _currencyCount;
	Cappuccino::UIBar* _currencyBg;

	Cappuccino::UIText* _ammoCount;
	Cappuccino::UIBar* _ammoBg;

	Cappuccino::UIBar* _healthLerpBG;
	Cappuccino::UIBar* _shieldLerpBG;
	Cappuccino::UIBar* _ammoLerpBG;
};