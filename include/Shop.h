#pragma once
#include "Cappuccino/UI.h"
#include "Cappuccino/GameObject.h"
#include "Billboard.h"
#include "Class.h"
#include "Cappuccino/SoundSystem.h"

class Empty : public Cappuccino::GameObject {
public:
	Empty(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void childUpdate(float dt) override;
};
class EmptyBox : public Empty {
public:
	EmptyBox(const Cappuccino::Shader& SHADER, const Cappuccino::HitBox& hitbox);

	void childUpdate(float dt) override;

};

class UIInteractive : public Cappuccino::UIText {
public:
	UIInteractive(const std::string& text, const glm::vec2& windowSize, const glm::vec2& defaultPosition, const glm::vec3& defaultColour, float defaultSize, const Cappuccino::HitBox& textBox, const std::vector<std::string>& tags);

	void updateComponent(float dt) override;

	static std::vector<UIInteractive*> _all;
	Cappuccino::HitBox& getTextBox() { return _textBox->_rigidBody._hitBoxes[0]; }

	static unsigned group;
	void setClickSound(const std::string& path);
	void playClickSound();

	std::vector<std::string> _tags; 
	
	void setPrice(unsigned p) { _price = p; }
	unsigned getPrice() const { return _price; }

private:
	//shop item price, where applicable
	unsigned _price = 0;
	unsigned _sound = 1000;
	EmptyBox* _textBox;

};

//should have a trigger volume that puts a text prompt on screen when the player collides with it
//needs a pointer to the player since collision checking will be handled internally
class ShopTerminal : public Cappuccino::GameObject {
public:
	ShopTerminal(const Cappuccino::Shader& SHADER,const std::vector<Cappuccino::Texture*>& textures,const std::vector<Cappuccino::Mesh*>& meshes,Class* player,Cappuccino::HitBox& cursorBox);

	void childUpdate(float dt) override;


	Class* _player;
	static bool _cursorLocked;
private:
	Cappuccino::HitBox* _cursorBoxPtr = nullptr;

	bool _shopOpen = false;

	Cappuccino::Sound _sadSound;

	Cappuccino::UserInterface _shopPrompt;
	Cappuccino::UserInterface _shopUI;
	Cappuccino::HitBox _triggerVolume;
	
	Billboard* _shopBackground;
	Cappuccino::Transform _finalTransform;
	
	Cappuccino::Shader _billboardShader;
};