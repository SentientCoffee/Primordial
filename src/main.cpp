#include "Cappuccino/Application.h"

#include "GameplayScene711.h"
#include "MenuScene.h"
#include "Cappuccino/SoundSystem.h"
#include "Cappuccino/FrameBuffer.h"

using Application = Cappuccino::Application;
using SoundSystem = Cappuccino::SoundSystem;
using FontManager = Cappuccino::FontManager;
using Shader = Cappuccino::Shader;
using Texture = Cappuccino::Texture;
using Mesh = Cappuccino::Mesh;


#pragma region PROGRAM SETTINGS

constexpr GLuint  SCR_WIDTH = 1600;
constexpr GLuint  SCR_HEIGHT = 1000;
constexpr GLchar* SCR_TITLE = "Primordial Alpha 11/19";

#pragma endregion

// This piece of code was given to us by Shawn Matthews
// Auto-magically changes OpenGL to use the high performance GPU to render rather than the iGPU
// (for dual GPU systems)
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x01;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x01;
}

int main() {

	if (!Application::isInstantiated()) {
		unsigned border = 4;

		Cappuccino::Viewport view{ glm::vec4(0.0f,0.0f,0.0f,1.0f),glm::vec4(0.0f,0.0f,SCR_WIDTH,SCR_HEIGHT),[]() {}, };

		Application* application = new Application(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE, { view });
		application->init();

		application->_clearColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		//test post proccessing effect
		char* frag = R"(#version 420 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloom;

struct ScreenFlare{
	sampler2D texture;
	float lerpParam;
};

struct LookupTable{
	sampler3D LUT;
	bool active;
};

uniform LookupTable lookup;

uniform ScreenFlare shieldFlare;

uniform float greyscalePercentage = 1;
	const float offset = 1.0 / 300.0; 
void main()
{
    vec3 col = vec3(texture(screenTexture, TexCoords.st));
	
vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1,	2,	1,
		2,	4,	2,
		1,	2,	1
    );
    
    vec3 sampleTex[9];
	   for(int i = 0; i < 9; i++)
		{
			sampleTex[i] = vec3(texture(bloom,TexCoords.st + offsets[i]));
		}
	vec3 fBloom = vec3(0.0f);

	
    for(int i = 0; i < 9; i++)
        fBloom += sampleTex[i] * kernel[i]/16.0f;

	//fBloom = vec3(1.0) - exp(-fBloom*1.0);//1 is exposure

	col += fBloom;
	vec4 fCol = texture(lookup.LUT,col);    

	float average = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
	vec3 grey = vec3(average,average,average).xyz;
	vec3 finalColour = mix(grey,col,greyscalePercentage);
	vec3 sFlare = vec3(texture(shieldFlare.texture,TexCoords.st)).rgb;
	//finalColour += sFlare;

    FragColor = vec4(finalColour, 1.0);
	//FragColor = fCol;
})";

		Cappuccino::Framebuffer test(glm::vec2(1600.0f, 1000.0f), 2,
			[]()
		{
			CAPP_GL_CALL(glEnable(GL_DEPTH_TEST));
			CAPP_GL_CALL(glEnable(GL_CULL_FACE));
			CAPP_GL_CALL(glEnable(GL_BLEND));
			CAPP_GL_CALL(glEnable(GL_SCISSOR_TEST));
			CAPP_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		}, std::nullopt, frag);
		Cappuccino::Framebuffer::_fbShader->use();
		Cappuccino::Framebuffer::_fbShader->setUniform("shieldFlare.texture", 10);
		Cappuccino::Framebuffer::_fbShader->setUniform("shieldFlare.lerpParam",1.0f);

		SoundSystem::setDefaultPath("./Assets/Sounds/");
		FontManager::setDefaultPath("./Assets/Fonts/");
		Shader::setDefaultPath("./Assets/Shaders/");
		Mesh::setDefaultPath("./Assets/Meshes/");
		Texture::setDefaultPath("./Assets/Textures/");

		FontManager::loadTypeFace("Viper Nora.ttf");

		MenuScene* m = new MenuScene(true);
		m->init();
		
		GameplayScene* g = new GameplayScene(false);


		application->run();
		delete application;
	}

	return 0;

}
