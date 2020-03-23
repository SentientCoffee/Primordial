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
	void fade(float dt);
	void toggleHud();
	void toggleHud(bool yn);

private:

	float _alpha;
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
	void toggleHud(bool yn);
	void togglePauseScreen();
	void toggleCrosshair(bool yn);
	void disableCrosshair();

private:

	PlayerClass _playerClass;
	std::string _class;

	unsigned int _health, _shield, _ammo, _currency;
	unsigned int   _maxHealth, _maxShield, _maxAmmo;

	bool _paused;

	glm::vec4 _colour;

	

};