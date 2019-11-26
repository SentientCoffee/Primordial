#include "PlayerHUD.h"
#include "Cappuccino/CappMath.h"

using namespace Cappuccino;

HUD::HUD(PlayerClass playerClass) {
	_playerClass = playerClass;
	_currency = 0;

	if (playerClass == PlayerClass::ASSAULT) {
		_colour = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
		_health = _maxHealth = 125;
		_shield = _maxShield = 65;
		_ammo = _maxAmmo = 120;
		_class = "A S S A U L T";
	}
	else if (playerClass == PlayerClass::COMMANDO) {
		_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		_health = _maxHealth = 100;
		_shield = _maxShield = 50;
		_ammo = _maxAmmo = 120;
		_class = "C O M M A N D O";
	}
	else if (playerClass == PlayerClass::DEMOLITION) {
		_colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		_health = _maxHealth = 110;
		_shield = _maxShield = 60;
		_ammo = _maxAmmo = 120;
		_class = "D E M O L I T I O N I S T";
	}
	else if (playerClass == PlayerClass::SCOUT) {
		_colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		_health = _maxHealth = 75;
		_shield = _maxShield = 35;
		_ammo = _maxAmmo = 120;
		_class = "S C O U T";
	}

	_healthBar = new UIBar(glm::vec2(-75.0f, -50.0f),
		_colour,
		glm::vec3(25.0f, 4.0f, 1.0f),
		UIBar::OriginPoint::BottomLeft);

	_healthCount = new UIText(std::to_string(_health) + "/" + std::to_string(_maxHealth),
		glm::vec2(1600, 1200),
		glm::vec2(-950.0f, -975.0f),
		glm::vec3(1.0f),
		1.0f);

	_shieldBar = new UIBar(glm::vec2(-75.0f, -45.0f),
		_colour,
		glm::vec3(25.0f, 4.0f, 1.0f),
		UIBar::OriginPoint::BottomLeft);

	_shieldCount = new UIText(std::to_string(_shield) + "/" + std::to_string(_maxShield),
		glm::vec2(1600, 1200),
		glm::vec2(-950.0f, -875.0f),
		glm::vec3(1.0f),
		1.0f);

	_classBg = new UIBar(glm::vec2(0.0f, -48.0f),
		_colour,
		glm::vec3(40.0f, 4.0f, 1.0f),
		UIBar::OriginPoint::Middle);

	_classLabel = new UIText(_class,
		glm::vec2(1600, 1200),
		glm::vec2(-200.0f, -975.0f),
		glm::vec3(1.0f),
		1.0f);

	_currencyBg = new UIBar(glm::vec2(75.0f, 47.0f),
		_colour,
		glm::vec3(5.0f, 4.0f, 1.0f),
		UIBar::OriginPoint::BottomRight);

	_currencyCount = new UIText(std::to_string(_currency),
		glm::vec2(1600, 1200),
		glm::vec2(1350.0f, 950.0f),
		glm::vec3(1.0f),
		1.0f);

	_ammoBg = new UIBar(glm::vec2(75.0f, -50.0f),
		_colour,
		glm::vec3(25.0f, 4.0f, 1.0f),
		UIBar::OriginPoint::BottomRight);

	_ammoCount = new UIText(std::to_string(_ammo),
		glm::vec2(1600, 1200),
		glm::vec2(850.0f, -975.0f),
		glm::vec3(1.0f),
		1.0f);

	_uiComponents.push_back(_healthCount);
	_uiComponents.push_back(_healthBar);

	_uiComponents.push_back(_shieldCount);
	_uiComponents.push_back(_shieldBar);

	_uiComponents.push_back(_classLabel);
	_uiComponents.push_back(_classBg);

	_uiComponents.push_back(_currencyCount);
	_uiComponents.push_back(_currencyBg);

	_uiComponents.push_back(_ammoBg);
	_uiComponents.push_back(_ammoCount);

}

void HUD::setHealth(unsigned int hp) { _health = hp; }
void HUD::setShield(unsigned int sp) { _shield = sp; }
void HUD::setAmmoCount(unsigned ac) { _ammo = ac; }
void HUD::setCurrencyCount(unsigned int cp) { _currency = cp; }

void HUD::updateHud(float dt) {
	_ammoCount->setText(std::to_string(_ammo));

	static float u = 0.0f;
	static bool reverse = false;

	if (!reverse)
		u += dt;
	else
		u -= dt;
	if (u >= 1.0f || u <= 0.0f) {
		reverse ^= 1;
	}

	_classBg->_transform._scaleMat[0].x = Math::lerp(_classBg->getBarDimensions().x, _classBg->getBarDimensions().x + 10.0f, u);



	update(dt);
}
