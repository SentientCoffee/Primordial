#include "Cappuccino/Application.h"
#include "Cappuccino/FontManager.h"
#include "Cappuccino/ShaderProgram.h"
#include "Cappuccino/SoundSystem.h"
#include "GameplayScene711.h"
#include "MenuScene.h"

using Application = Cappuccino::Application;
using SoundSystem = Cappuccino::SoundSystem;
using FontManager = Cappuccino::FontManager;
using Shader      = Cappuccino::Shader;

#pragma region PROGRAM SETTINGS

constexpr GLuint  SCR_WIDTH = 1600;
constexpr GLuint  SCR_HEIGHT = 1200;
constexpr GLchar* SCR_TITLE = "Primordial Alpha 0.0.1";

#pragma endregion

// This piece of code was given to us by Shawn Matthews
// Auto-magically changes OpenGL to use the high performance GPU to render rather than the iGPU
// (for dual GPU systems)
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x01;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x01;
}

int main() {

	if(!Application::isInstantiated()) {
		Application* application = new Application(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE);
		application->init();

		// Create your scenes here
		// This is a test scene that you can run to see the capabilities of the engine
		// Cappuccino::TestScene* e = new Cappuccino::TestScene(true);
		// e->init();

		SoundSystem::setDefaultPath("./Assets/Sounds/");
		FontManager::setDefaultPath("./Assets/Fonts/");
		Shader::setDefaultPath("./Assets/Shaders/");

		FontManager::loadTypeFace("Viper Nora.ttf");

		MenuScene* m = new MenuScene(true);
		m->init();

		GameplayScene* g = new GameplayScene(false);
		//g->init();
		

		application->run();
		delete application;
	}

	return 0;

}
