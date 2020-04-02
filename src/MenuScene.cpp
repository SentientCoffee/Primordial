#include "MenuScene.h"
#include "Options.h"

#include <Cappuccino/CappMacros.h>
#include <Cappuccino/ResourceManager.h>
#include "Cappuccino/Application.h"

MenuScene::MenuScene(bool isActive) :
	Scene(isActive),
	_in(true, std::nullopt),
	cursorBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f)),
	startBox(glm::vec3(-635.0f, -120.0f, 0.0f), glm::vec3(175.0f, 20.0f, 200.0f)),
	tutorialBox(glm::vec3(-635.0f, -60.0f, 0.0f), glm::vec3(175.0f, 20.0f, 200.0f)),
	optionsBox(glm::vec3(-635.0f, 0.0f, 0.0f), glm::vec3(175.0f, 20.0f, 200.0f)),
	creditsBox(glm::vec3(-635.0f, 60.0f, 0.0f), glm::vec3(175.0f, 20.0f, 200.0f)),
	exitBox(glm::vec3(-615.0f, 200.0f, 0.0f), glm::vec3(240.0f, 20.0f, 200.0f)),
	backBox(glm::vec3(-645.0f, -550.0f, 0.0f), glm::vec3(150.0f, 20.0f, 200.0f)),
	commandoBox(glm::vec3(-400.0f, -300.0f, 0.0f), glm::vec3(1400.0f, 760.0f, 250.0f)),
	assaultBox(glm::vec3(400.0f, -300.0f, 0.0f), glm::vec3(1400.0f, 760.0f, 200.0f)),
	scoutBox(glm::vec3(-400.0f, 150.0f, 0.0f), glm::vec3(1400.0f, 760.0f, 250.0f)),
	demoBox(glm::vec3(400.0f, 150.0f, 0.0f), glm::vec3(1400.0f, 760.0f, 200.0f))
{
	using namespace Cappuccino;
	menuShader = ShaderLibrary::loadShader("Billboard", "billboardShader.vert", "billboardShader.frag");


	// Main Menu
	ui._uiComponents.push_back(new UIText("Start", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1400.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));
	ui._uiComponents.push_back(new UIText("Tutorial", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1400.0f, -100.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));
	ui._uiComponents.push_back(new UIText("Options", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1400.0f, -200.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));
	ui._uiComponents.push_back(new UIText("Credits", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1400.0f, -300.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));
	ui._uiComponents.push_back(new UIText("Exit Game", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1400.0f, -600.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));

	ui._uiComponents.push_back(new UIText("P R I M O R D I A L", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1400.0f, 600.0f), glm::vec3(1.0f, 0.0f, 0.0f), 2.5f));

	// Selection Boxes
	ui._uiComponents.push_back(new UIBar({ -800.0f, 395.0f }, { 0.0f, 0.0f, 0.0f, 0.4f }, { 1500.0f, 880.0f, 1.0f }, UIBar::OriginPoint::Middle));
	ui._uiComponents.back()->setVisible(false);
	ui._uiComponents.push_back(new UIBar({ 800.0f, 395.0f }, { 0.0f, 0.0f, 0.0f, 0.4f }, { 1500.0f, 880.0f, 1.0f }, UIBar::OriginPoint::Middle));
	ui._uiComponents.back()->setVisible(false);
	ui._uiComponents.push_back(new UIBar({ -800.0f, -510.0f }, { 0.0f, 0.0f, 0.0f, 0.4f }, { 1500.0f, 880.0f, 1.0f }, UIBar::OriginPoint::Middle));
	ui._uiComponents.back()->setVisible(false);
	ui._uiComponents.push_back(new UIBar({ 800.0f, -510.0f }, { 0.0f, 0.0f, 0.0f, 0.4f }, { 1500.0f, 880.0f, 1.0f }, UIBar::OriginPoint::Middle));
	ui._uiComponents.back()->setVisible(false);

	ui._uiComponents.push_back(new UIText("Back", glm::vec2(1600.0f, 1000.0f), glm::vec2(-1400.0f, 900.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
	ui._uiComponents.back()->setVisible(false);

	
	ui._uiComponents.push_back(new UIScreenQuad({
		new Texture("cs",  "char-select.png", TextureType::DiffuseMap),
		new Texture("csd", "char-select.png", TextureType::DiffuseMap)
	}));
	ui._uiComponents.back()->setVisible(false);
	
	ui._uiComponents.push_back(new UIScreenQuad({
		new Texture("bg",  "primordial-title-screen-1600x1000.png", TextureType::DiffuseMap),
		new Texture("bgd", "primordial-title-screen-1600x1000.png", TextureType::DiffuseMap)
	}));
	ui._uiComponents.back()->setVisible(true);

	_closeTrigger = new Billboard(menuShader, { new Texture("close", "defaultEmission.png", TextureType::DiffuseMap) });
	_closeTrigger->setActive(false);

	//menuShader->use();
	//menuShader->loadOrthoProjectionMatrix(4.0f, 3.0f);
	camera.lookAt(glm::vec3(0.0f, 0.0f, -3.0f));
	//menuShader->loadViewMatrix(camera);
	//menuShader->setUniform("image", (int)Cappuccino::TextureType::PBRAlbedo);
}

bool MenuScene::init()
{
	_initialized = true;
	_shouldExit = false;
	Options::setTutorial(false);
	for (int i = 0; i < 6; i++)
		ui._uiComponents[i]->setVisible(true);
	ui._uiComponents.back()->setVisible(true);

	MusicManager::playSong(0);
	MusicManager::levelClearTrigger(0, 0);
	return true;
}

bool MenuScene::exit()
{
	_initialized = false;
	_shouldExit = true;

	for (auto x : ui._uiComponents)
		x->setVisible(false);

	for (auto& x : _icons)
		x->setActive(false);

	MusicManager::playSong(1);
	Options::Music->stopEvent(0);

	return true;
}

void MenuScene::childUpdate(float dt)
{
	static bool characterSelect = false;
	static float clickDelay = 0.5f;
	cursorBox._position = glm::vec3(cursorPos.x, cursorPos.y, 0.0f);
	//CAPP_PRINT("%f %f\n",cursorBox._position.x, cursorBox._position.y);
	//CAPP_PRINT("box %f %f\n",startBox._position.x, startBox._position.y);

		clickDelay -= dt;

	static bool change = false;

	//commando
	if (cursorBox.checkCollision(commandoBox, commandoBox._position, cursorBox._position) && characterSelect && clickDelay <= 0.0f) {
		dynamic_cast<Cappuccino::UIBar*>(ui._uiComponents[6])->setColour(glm::vec4(1.0f, 1.0f, 1.0f, 0.4f));
		if (_in.clickListener.leftClicked()) {
			Options::Commando = true;
			change = true;
		}
	}
	else if (!cursorBox.checkCollision(commandoBox, commandoBox._position, cursorBox._position) && characterSelect)
		dynamic_cast<Cappuccino::UIBar*>(ui._uiComponents[6])->setColour(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

	//assault
	if (cursorBox.checkCollision(assaultBox, assaultBox._position, cursorBox._position) && characterSelect && clickDelay <= 0.0f) {
		dynamic_cast<Cappuccino::UIBar*>(ui._uiComponents[7])->setColour(glm::vec4(1.0f, 1.0f, 1.0f, 0.4f));
		if (_in.clickListener.leftClicked()) {
			Options::Assault = true;
			change = true;
		}
	}
	else if (!cursorBox.checkCollision(assaultBox, assaultBox._position, cursorBox._position) && characterSelect)
		dynamic_cast<Cappuccino::UIBar*>(ui._uiComponents[7])->setColour(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

	//scout
	if (cursorBox.checkCollision(scoutBox, scoutBox._position, cursorBox._position) && characterSelect && clickDelay <= 0.0f) {
		dynamic_cast<Cappuccino::UIBar*>(ui._uiComponents[8])->setColour(glm::vec4(1.0f, 1.0f, 1.0f, 0.4f));
		if (_in.clickListener.leftClicked()) {
			Options::Scout = true;
			change = true;
		}
	}
	else if (!cursorBox.checkCollision(scoutBox, scoutBox._position, cursorBox._position) && characterSelect)
		dynamic_cast<Cappuccino::UIBar*>(ui._uiComponents[8])->setColour(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

	//demolitionist
	if (cursorBox.checkCollision(demoBox, demoBox._position, cursorBox._position) && characterSelect && clickDelay <= 0.0f) {
		dynamic_cast<Cappuccino::UIBar*>(ui._uiComponents[9])->setColour(glm::vec4(1.0f, 1.0f, 1.0f, 0.4f));
		if (_in.clickListener.leftClicked()) {
			Options::Demolitionist = true;
			change = true;
		}
	}
	else if (!cursorBox.checkCollision(demoBox, demoBox._position, cursorBox._position) && characterSelect)
		dynamic_cast<Cappuccino::UIBar*>(ui._uiComponents[9])->setColour(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));


	//start button
	if (cursorBox.checkCollision(startBox, startBox._position, cursorBox._position) && !characterSelect && clickDelay <= 0.0f) {
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[0])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));
		

		if (_in.clickListener.leftClicked()) {
			characterSelect = true;
			for (unsigned i = 0; i < 6; i++)
				ui._uiComponents[i]->setVisible(false);
			for (int i = 6; i < ui._uiComponents.size(); i++)
				ui._uiComponents[i]->setVisible(true);
			ui._uiComponents.back()->setVisible(false);

		}
	}
	else if (!cursorBox.checkCollision(startBox, startBox._position, cursorBox._position) && !characterSelect)
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[0])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

	// tutorial button
	if (cursorBox.checkCollision(tutorialBox, tutorialBox._position, cursorBox._position) && !characterSelect && clickDelay <= 0.0f) {
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[1])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

		if (_in.clickListener.leftClicked()) {
			Options::Commando = true;
			change = true;
			Options::setTutorial(true);
		}
	}
	else if (!cursorBox.checkCollision(tutorialBox, tutorialBox._position, cursorBox._position) && !characterSelect)
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[1])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

	// options button
	if (cursorBox.checkCollision(optionsBox, optionsBox._position, cursorBox._position) && !characterSelect && clickDelay <= 0.0f) {
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[2])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

		if (_in.clickListener.leftClicked()) {
			//option stuff
			//Goptions::toggleGoptions();
		}
	}
	else if (!cursorBox.checkCollision(optionsBox, optionsBox._position, cursorBox._position) && !characterSelect)
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[2])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

	// credits button
	if (cursorBox.checkCollision(creditsBox, creditsBox._position, cursorBox._position) && !characterSelect && clickDelay <= 0.0f) {
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[3])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

		if (_in.clickListener.leftClicked()) {
			//credits stuff
		}
	}
	else if (!cursorBox.checkCollision(creditsBox, creditsBox._position, cursorBox._position) && !characterSelect)
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[3])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

	if (cursorBox.checkCollision(exitBox, exitBox._position, cursorBox._position) && !characterSelect && clickDelay <= 0.0f) {
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[4])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

		//exit stuff
		if (_in.clickListener.leftClicked())
		{
			_closeTrigger->setClose(true);
			exit();
		}
	}
	else if (!cursorBox.checkCollision(exitBox, exitBox._position, cursorBox._position) && !characterSelect)
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[4])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

	//back button
	if (cursorBox.checkCollision(backBox, backBox._position, cursorBox._position) && characterSelect && clickDelay <= 0.0f) {
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[10])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

		if (_in.clickListener.leftClicked()) {
			characterSelect = false;
			for (int i = 0; i < 6; i++)
				ui._uiComponents[i]->setVisible(true);
			for (int i = 6; i < ui._uiComponents.size(); i++)
				ui._uiComponents[i]->setVisible(false);
			ui._uiComponents.back()->setVisible(true);
			for (auto x : _icons)
				x->setActive(false);
			clickDelay = 0.5f;

		}
	}
	else if (!cursorBox.checkCollision(backBox, backBox._position, cursorBox._position) && characterSelect)
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[10])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));

	ui.update(dt);

	Goptions::update(dt);

	if (change)
	{
		characterSelect = false;
		change = false;
		clickDelay = 0.5f;
		Cappuccino::SceneManager::changeScene(1);
	}

}

void MenuScene::mouseFunction(double xpos, double ypos)
{
	cursorPos = glm::vec2(xpos, ypos);
	cursorPos -= glm::vec2(800.0f, 600.0f);
}

void MenuScene::clickFunction(int button, int action, int mods)
{
	_in.clickListener.checkClick(button, action, mods);
}
