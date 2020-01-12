#include "Shop.h"
#include "Cappuccino/SoundSystem.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/CappMath.h"

std::vector<UIInteractive*> UIInteractive::_all = {};
unsigned UIInteractive::group = 0;
UIInteractive::UIInteractive(const std::string& text, const glm::vec2& windowSize, const glm::vec2& defaultPosition, const glm::vec3& defaultColour, float defaultSize, const Cappuccino::HitBox& textBox, const std::vector<std::string>& tags)
	:Cappuccino::UIText(text, windowSize, defaultPosition, defaultColour, defaultSize), _textBox(new EmptyBox({}, textBox))
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
	_sound = Cappuccino::SoundSystem::load2DSound(path);
}

void UIInteractive::playClickSound()
{
	//check if the sound exists
	if (_sound >= 1000)
		return;
	Cappuccino::SoundSystem::playSound2D(_sound, group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
}

Empty::Empty(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	:Cappuccino::GameObject(SHADER, textures, meshes)
{
	_rigidBody.setGrav(false);
}

void Empty::childUpdate(float dt)
{
}

EmptyBox::EmptyBox(const Cappuccino::Shader& SHADER, const Cappuccino::HitBox& hitbox)
	:Empty(SHADER, {}, {})
{
	_rigidBody._hitBoxes.push_back(hitbox);
}

void EmptyBox::childUpdate(float dt)
{
}

bool ShopTerminal::_cursorLocked = false;
ShopTerminal::ShopTerminal(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, Class* player, Cappuccino::HitBox& cursorBox)
	:Cappuccino::GameObject(SHADER, textures, meshes), _triggerVolume(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 20.0f, 20.0f)), _sadSound("uiSadClick.wav", "groupeee")
{
	_player = player;
	_cursorBoxPtr = &cursorBox;

	_rigidBody._moveable = true;
	_rigidBody.setGrav(false);


	//set up the ui elements of the shop

	_shopUI._uiComponents.push_back(new UIInteractive("0",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(1050.0f, 488.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f,
		Cappuccino::HitBox(glm::vec3(50.0f, 200.0f + 0.0f, 0.0f), glm::vec3(150.0f, 200.0f + 50.0f, 0.0f)),
		{ }));

	_shopUI._uiComponents.push_back(new UIInteractive("Cost: 220",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 2.0f,
		Cappuccino::HitBox(glm::vec3(50.0f, 0.0f, 0.0f), glm::vec3(150.0f, 50.0f, 0.0f)),
		{ "dragable","$" }));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setPrice(220);


	_shopUI._uiComponents.push_back(new UIInteractive("Sednium:",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(844.0f, 490.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f,
		Cappuccino::HitBox(glm::vec3(200.0f + 50.0f, 0.0f, 0.0f), glm::vec3(200.0f + 150.0f, 50.0f, 0.0f)),
		{  }));


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
	_billboardShader.loadOrthoProjectionMatrix(4.0f, 5.0f);
	Cappuccino::Camera camera;
	camera.lookAt(glm::vec3(0.0f, 0.0f, -3.0f));
	_billboardShader.loadViewMatrix(camera);
	_billboardShader.setUniform("image", 0);
	_shopBackground = new Billboard(&_billboardShader, { new Cappuccino::Texture("container2.png",Cappuccino::TextureType::DiffuseMap) });

	_finalTransform = _shopBackground->_transform;
	_finalTransform.scale(glm::vec3(1.5f, 1.5f, 1.0f), 4.0f);



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
		if (_player->_input.keyboard->keyPressed(Events::E)) {
			_shopPrompt._uiComponents.back()->setVisible(false);
			_shopOpen = true;
			_shopBackground->setActive(true);

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
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
			}

			//temporary code for moving around the text so that i can decide where the text should go
			for (unsigned i = 0; i < _shopUI._uiComponents.size(); i++) {
				auto element = static_cast<UIInteractive*>(_shopUI._uiComponents[i]);
				if (_cursorBoxPtr->checkCollision(element->getTextBox(), element->getTextBox()._position, _cursorBoxPtr->_position)) {
					element->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

					static bool hasClicked = false;
					if (_player->_input.clickListener.leftClicked()) {

						if (!hasClicked) {
							for (unsigned j = 0; j < element->_tags.size(); j++) {

								if (element->_tags[j] == "$") {
									if (_player->getCurrency() - (int)element->getPrice() > 0)
										_player->getCurrency() -= element->getPrice();
									else
										Cappuccino::SoundSystem::playSound2D(_sadSound.getSoundHandle(), _sadSound.getGroupHandle(), Cappuccino::SoundSystem::ChannelType::SoundEffect);
									//printf("%d %d\n", _player->getCurrency(),element->getPrice());
									break;
								}
							}


							element->playClickSound();
							hasClicked = true;
						}

						for (unsigned j = 0; j < element->_tags.size(); j++) {

							if (element->_tags[j] == "dragable") {
								element->setTextPosition(2.0f * glm::vec2(_cursorBoxPtr->_position.x, -_cursorBoxPtr->_position.y));
								element->getTextBox()._position = glm::vec3(_cursorBoxPtr->_position.x, _cursorBoxPtr->_position.y, 0.0f);
							}

						}
					}
					else
						hasClicked = false;
				}
				else
					element->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

				auto pos = static_cast<Cappuccino::UIText*>(_shopUI._uiComponents[i])->getPosition();

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
	}


	//printf("\nx: %f\ty: %f\n\n", _cursorBoxPtr->_position.x, _cursorBoxPtr->_position.y);


	_cursorLocked = _shopOpen;
	_shopPrompt.update(dt);
	_shopUI.update(dt);
}
