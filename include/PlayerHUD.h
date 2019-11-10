#pragma once

#include <Cappuccino/UI.h>
#include <Cappuccino/GameObject.h>

enum class PlayerClass : unsigned int {
	ASSAULT = 0,
	COMMANDO,
	DEMOLITION,
	SCOUT
};

class HUD : public Cappuccino::UserInterface {
public:

	HUD(PlayerClass playerClass);
	~HUD() = default;

	void setHealth(unsigned int hp);
	void setShield(unsigned int sp);
	void setAmmoCount(unsigned int ac);
	void setCurrencyCount(unsigned int cp);

	void updateHud(float dt);

private:

	PlayerClass _playerClass;
	std::string _class;

	unsigned int _health, _shield, _ammo, _currency;
	unsigned int _maxHealth, _maxShield, _maxAmmo;
	
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
};