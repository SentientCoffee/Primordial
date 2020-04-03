#include "Shop.h"
#include "Cappuccino/SoundSystem.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/CappMath.h"
#include <Cappuccino/ResourceManager.h>


std::vector<UIInteractive*> UIInteractive::_all = {};
unsigned UIInteractive::group = 0;
UIInteractive::UIInteractive(const std::string& text, const glm::vec2& windowSize, const glm::vec2& defaultPosition, const glm::vec3& defaultColour, float defaultSize, const Cappuccino::HitBox& textBox, const std::vector<std::string>& tags)
	: UIText(text, windowSize, defaultPosition, defaultColour, defaultSize), _textBox(new EmptyBox({}, textBox))
{
	_textBox->setVisible(false);
	_all.push_back(this);
	setVisible(false);

	if (!group)
		group = Cappuccino::SoundSystem::createChannelGroup("main");

	_tags = tags;
}

void UIInteractive::updateComponent(float dt)
{
	if (isVisible())
		_textBox->setActive(true);
	else
		_textBox->setActive(false);
}

void UIInteractive::setClickSound(const std::string& path)
{
	_sound = Cappuccino::Sound(path, "UIGroup");
}

void UIInteractive::playClickSound()
{
	_sound.play();
}

Empty::Empty(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: GameObject(SHADER, textures, meshes)
{
	_rigidBody.setGrav(false);
}

void Empty::childUpdate(float dt)
{
}

EmptyBox::EmptyBox(const Cappuccino::Shader& SHADER, const Cappuccino::HitBox& hitbox)
	: Empty(SHADER, {}, {})
{
	_rigidBody._hitBoxes.push_back(hitbox);
}

void EmptyBox::childUpdate(float dt)
{
}

bool ShopTerminal::_cursorLocked = false;
std::vector<Cappuccino::Sound> ShopTerminal::_greeting = {};
std::vector<Cappuccino::Sound> ShopTerminal::_success = {};
std::vector<Cappuccino::Sound> ShopTerminal::_declined = {};
std::vector<Cappuccino::Sound> ShopTerminal::_farewell = {};
std::vector<Cappuccino::Sound> ShopTerminal::_pain = {};
ShopTerminal::ShopTerminal(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, Class* player, Cappuccino::HitBox& cursorBox) :
	GameObject(SHADER, textures, meshes), _triggerVolume(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 20.0f, 20.0f))
{
	std::string _ = "Shop/";
	typedef Cappuccino::Sound __;

	//im so lazy
	static bool first = true;
	if (first) {
		//push back all the sounds, only once ever
		for (unsigned i = 0; i < 5; i++)
			_greeting.push_back(__(_ + "18 greeting " + std::to_string(i + 1) + ".wav", "UIGroup"));

		for (unsigned i = 0; i < 7; i++)
			_success.push_back(__(_ + "19 success " + std::to_string(i + 1) + ".wav", "UIGroup"));

		for (unsigned i = 0; i < 11; i++)
			_declined.push_back(__(_ + "20 declined " + std::to_string(i + 1) + ".wav", "UIGroup"));

		for (unsigned i = 0; i < 6; i++)
			_farewell.push_back(__(_ + "21 farewell " + std::to_string(i + 1) + ".wav", "UIGroup"));

		for (unsigned i = 0; i < 6; i++)
			_pain.push_back(__(_ + "22 pain " + std::to_string(i + 1) + ".wav", "UIGroup"));
	}
	_player = player;
	_cursorBoxPtr = &cursorBox;

	_rigidBody._moveable = true;
	_rigidBody.setGrav(false);

	_billboardShader = *Cappuccino::ShaderLibrary::loadShader("Billboard", "billboardShader.vert", "billboardShader.frag");

	//set up the ui elements of the shop

	_shopUI._uiComponents.push_back(new UIInteractive("0",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(1050.0f, 418.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f,
		Cappuccino::HitBox(glm::vec3(5000.0f, 200.0f + 0.0f, 0.0f), glm::vec3(15000.0f, 200.0f + 50.0f, 0.0f)),
		{ }));

	_shopUI._uiComponents.push_back(new UIInteractive("BUY HP: 300",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(-816.0f, 232.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.5f,
		Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(350.0f, 50.0f, 0.0f)),
		{ "$","HP" }));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setPrice(300);
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->getTextBox()._position = glm::vec3(-816.0f / 2.0f + 100.0f, -232.0f / 2.0f, 0.0f);

	_shopUI._uiComponents.push_back(new UIInteractive("BUY AMMO: 300",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(-816.0f, -180.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.5f,
		Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(450.0f, 50.0f, 0.0f)),
		{ "$","AMMO" }));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setPrice(300);
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->getTextBox()._position = glm::vec3(-816.0f / 2 + 100.0f, 180.0f / 2, 0.0f);

	_shopUI._uiComponents.push_back(new UIInteractive("UPGRADE DAMAGE: 550",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(350.0f, 226.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.5f,
		Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(450.0f, 50.0f, 0.0f)),
		{ "$","UPDMG" }));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setPrice(300);
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->getTextBox()._position = glm::vec3(350.0f / 2, -226.0f / 2, 0.0f);

	_shopUI._uiComponents.push_back(new UIInteractive("UPGRADE CLIP SIZE: 550",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(358.0f, -178.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.5f,
		Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(450.0f, 50.0f, 0.0f)),
		{ "$","UPAMM" }));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setPrice(300);
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->getTextBox()._position = glm::vec3(358.0f / 2, 178.0f / 2, 0.0f);


	_shopUI._uiComponents.push_back(new UIInteractive("Sednium:",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(844.0f, 420.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f,
		Cappuccino::HitBox(glm::vec3(2000.0f, 0.0f, 0.0f), glm::vec3(2000.0f + 150.0f, 8000.0f, 0.0f)),
		{  }));

	//820.000000, -570.000000
	_shopUI._uiComponents.push_back(new UIInteractive("Exit",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(820.0f, -570.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 2.0f,
		Cappuccino::HitBox(glm::vec3(820.0f / 2 + 50.f, 570.0f / 2, 0.0f), glm::vec3((820.0f / 2) + 150.0f, 50.0f + 570.0f / 2, 0.0f)),
		{ "Exit" }));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");

	for (auto x : _shopUI._uiComponents)
		x->setVisible(false);


	//set up the text prompt for when the player hits the trigger volume
	_shopPrompt._uiComponents.push_back(new Cappuccino::UIText(
		"Press E to Open Shop",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f));
	_shopPrompt._uiComponents.back()->setVisible(false);

	//set up the billboard shader for the background of the shop
	_billboardShader.use();
	_billboardShader.loadOrthoProjectionMatrix(4.0f, 3.0f);
	Cappuccino::Camera camera;
	camera.lookAt(glm::vec3(0.0f, 0.0f, -3.0f));
	_billboardShader.loadViewMatrix(camera);
	_billboardShader.setUniform("image", 0);
	_shopBackground = new Billboard(&_billboardShader, { Cappuccino::TextureLibrary::loadTexture("Shop background", "shop.png",Cappuccino::TextureType::DiffuseMap) });

	_finalTransform = _shopBackground->_transform;
	_finalTransform.scale(glm::vec3(1.5f, 1.0f, 1.0f), 4.0f);



}

void ShopTerminal::childUpdate(float dt)
{
	static bool exit = false;
	static bool first = true;

	//store the state of each crosshair and make them invisible
	static bool cr1State = false, cr2State = false;
	static auto originalScale = _shopBackground->_transform._scaleMat;
	static float u = 0.0f;
	static bool shopHUDOFF = false;


	//check if the player is in range
	if (_player->checkCollision(_triggerVolume, _rigidBody._position) && !_shopOpen) {

		//make the shop prompt visible
		_shopPrompt._uiComponents.back()->setVisible(true);

		//open the shop if the player presses E
		if (_player->_input.keyboard->keyPressed(Cappuccino::KeyEvent::E)) {
			_shopPrompt._uiComponents.back()->setVisible(false);
			_shopOpen = true;
			_shopBackground->setActive(true);
			auto index = rand() % _greeting.size();
			_greeting[index].play();
		}
	}
	//exit if the player is out of range of the shop and the shop is open
	else if (!_player->checkCollision(_triggerVolume, _rigidBody._position) && _shopOpen)
		exit = true;
	//in any other case, just make sure that the prompt isn't visible
	else
		_shopPrompt._uiComponents.back()->setVisible(false);
	if (!_shopOpen) {
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	}
	else {
		//unlock the cursor if we're in the shop

		//does the math to show that coordinates are being converted back to their original frame
		if (_cursorBoxPtr->_position.x <= -595.0f)
			glfwSetCursorPos(glfwGetCurrentContext(), (800.0 - 595.0), _cursorBoxPtr->_position.y + 500.0);
		else if (_cursorBoxPtr->_position.x >= 595.0f)
			glfwSetCursorPos(glfwGetCurrentContext(), (800.0 + 595.0), _cursorBoxPtr->_position.y + 500.0);
		if (_cursorBoxPtr->_position.y <= -290.0f)
			glfwSetCursorPos(glfwGetCurrentContext(), _cursorBoxPtr->_position.x + 800.0, 500.0 - 290.0);
		else if (_cursorBoxPtr->_position.y >= 290.0f)
			glfwSetCursorPos(glfwGetCurrentContext(), _cursorBoxPtr->_position.x + 800.0, 500.0 + 290.0);

		//set the text element to display the proper currency
		static_cast<Cappuccino::UIText*>(_shopUI._uiComponents[0])->setText(std::to_string(_player->getCurrency()));

		static_cast<UIInteractive*>(_shopUI._uiComponents[1])->setText(std::to_string(
			static_cast<UIInteractive*>(_shopUI._uiComponents[1])->getPrice()));

		static_cast<UIInteractive*>(_shopUI._uiComponents[2])->setText(std::to_string(
			static_cast<UIInteractive*>(_shopUI._uiComponents[2])->getPrice()));


		//lerp the scale
		if (!(u >= 1.0f) && !exit) {
			u += dt * 5.0f;

			_shopBackground->_transform._scaleMat = Cappuccino::Math::lerp(originalScale, _finalTransform._scaleMat, u);
			if (u >= 1.0f)
				u = 1.0f;
		}

		if (u >= 1.0f && !exit) {

			if (first) {

				for (unsigned i = 0; i < _shopUI._uiComponents.size(); i++)
					_shopUI._uiComponents[i]->setVisible(true);

				_player->toggleHud();

				///REMOVE AFTER TESTING
				{
					for (unsigned i = 0; i < 5000; i++)
						_player->addCurrency();
				}
				///REMOVE AFTER TESTING

				cr1State = _player->getCrosshairPrimaryActive();
				cr2State = _player->getCrosshairActive();

				_player->setCrosshairPrimaryActive(false);
				_player->setCrosshairActive(false);
				_player->setCanShoot(false);

				first = false;
				glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				glfwSetCursorPos(glfwGetCurrentContext(), 800.0f, 500.0f);

			}

			//if the player presses escape, exit the shop
			if (_player->_input.keyboard->keyPressed(Cappuccino::KeyEvent::ESCAPE))
				exit = true;


			//temporary code for moving around the text so that i can decide where the text should go
			for (unsigned i = 0; i < _shopUI._uiComponents.size(); i++) {
				if (exit)
					break;
				auto element = static_cast<UIInteractive*>(_shopUI._uiComponents[i]);
				if (_cursorBoxPtr->checkCollision(element->getTextBox(), element->getTextBox()._position, _cursorBoxPtr->_position)) {
					element->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

					static bool hasClicked = false;
					if (_player->_input.clickListener.leftClicked()) {
						//buy the item, make the appropriate sound
						if (!hasClicked) {

							for (unsigned j = 0; j < element->_tags.size(); j++) {
								if (element->_tags[j] == "$") {
									static bool correct = false;//correct is true if the player buys an item
									if (_player->getCurrency() - (int)element->getPrice() > 0) {
										for (auto x : element->_tags) {
											if (x == "HP") {
												auto newHp = (_player->getHealth() + _player->getMaxHp() * 0.2f);
												_player->setHealth(newHp >= _player->getMaxHp() ? _player->getMaxHp() : newHp);
												_player->getCurrency() -= element->getPrice();
											}
											else if (x == "AMMO") {
												_player->addAmmo();
												_player->getCurrency() -= element->getPrice();
											}
											else if (x == "UPDMG") {
												static bool upDMGdone = false;
												if (!upDMGdone) {
													_player->getGun()->setDamage(_player->getGun()->getDamage() * 1.1f);
													_player->getCurrency() -= element->getPrice();
												}
												upDMGdone = true;

											}
											else if (x == "UPAMM") {
												static bool upAMMdone = false;
												if (!upAMMdone) {
													_player->getGun()->setMaxAmmo(_player->getGun()->getMaxAmmo() * 1.15f);
													_player->getCurrency() -= element->getPrice();
												}
												upAMMdone = true;

											}

										}
										element->setPrice(element->getPrice() * 1.2f);
										auto index = rand() % _success.size();
										_success[index].play();
										correct = true;

									}
									else {
										static int index = 0;
										if (correct)
											index = 0;
										_declined[index].play();
										correct = false;
										index++;
										if (index > _declined.size() - 1)
											index = 0;
									}
									//printf("%d %d\n", _player->getCurrency(),element->getPrice());
									break;
								}
								if (element->_tags[j] == "Exit") {
									exit = true;
									break;
								}
							}

							////if the player clicked the exit button, exit the shop


							element->playClickSound();
							hasClicked = true;
						}


						///REMOVE AFTER TESTING, USED TO DRAG UI ELEMENTS INTO PLACE
						for (unsigned j = 0; j < element->_tags.size(); j++) {

							if (element->_tags[j] == "dragable") {
								element->setTextPosition(2.0f * glm::vec2(_cursorBoxPtr->_position.x, -_cursorBoxPtr->_position.y));
								element->getTextBox()._position = glm::vec3(_cursorBoxPtr->_position.x, _cursorBoxPtr->_position.y, 0.0f);
								printf("$300: %f, %f\n", element->getPosition().x, element->getPosition().y);
							}

						}
					}
					else
						hasClicked = false;
				}
				else
					element->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

				//auto pos = static_cast<Cappuccino::UIText*>(_shopUI._uiComponents[i])->getPosition();
				//printf("%s:\nx: %f\ty: %f\n\n", element->getText().c_str(), pos.x, pos.y);
			}

		}
	}

	if (exit) {
		if (!shopHUDOFF) {
			u = 0.0f;
			_player->toggleHud();
			for (unsigned i = 0; i < _shopUI._uiComponents.size(); i++)
				_shopUI._uiComponents[i]->setVisible(false);
			shopHUDOFF = true;
		}
	}

	if (!(u >= 1.0f) && exit) {
		u += dt * 5.0f;

		_shopBackground->_transform._scaleMat = Cappuccino::Math::lerp(_finalTransform._scaleMat, originalScale, u);
		if (u >= 1.0f)
			u = 1.0f;
	}

	//exit the shop when exit is true
	if (u >= 1.0f && exit) {

		_shopBackground->setActive(false);

		_shopOpen = false;
		_player->setCanShoot(true);
		_player->setCrosshairPrimaryActive(cr1State);
		_player->setCrosshairActive(cr2State);
		first = true;
		exit = false;
		shopHUDOFF = false;
		u = 0.0f;
		auto index = rand() % _farewell.size();
		_farewell[index].play();
	}


	//printf("\nx: %f\ty: %f\n\n", _cursorBoxPtr->_position.x, _cursorBoxPtr->_position.y);


	_cursorLocked = _shopOpen;
	_shopPrompt.update(dt);
	_shopUI.update(dt);
}