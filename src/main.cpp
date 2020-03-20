#include "Cappuccino/Application.h"

#include "GameplayScene711.h"
#include "MenuScene.h"
#include "Cappuccino/SoundSystem.h"
#include "Cappuccino/FrameBuffer.h"
#include <ctime>

using Application = Cappuccino::Application;
using SoundSystem = Cappuccino::SoundSystem;
using FontManager = Cappuccino::FontManager;
using Shader = Cappuccino::Shader;
using Texture = Cappuccino::Texture;
using Mesh = Cappuccino::Mesh;


#pragma region PROGRAM SETTINGS

constexpr GLuint  SCR_WIDTH = 1600;
constexpr GLuint  SCR_HEIGHT = 1000;
constexpr GLchar* SCR_TITLE = "Primordial 03/10/20";

#pragma endregion

// This piece of code was given to us by Shawn Matthews
// Auto-magically changes OpenGL to use the high performance GPU to render rather than the iGPU
// (for dual GPU systems)
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x01;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x01;
}

int main() {
	srand(time(0));
	if (!Application::isInstantiated()) {
		unsigned border = 4;

		Cappuccino::Viewport view{ glm::vec4(0.0f,0.0f,0.0f,1.0f),glm::vec4(0.0f,0.0f,SCR_WIDTH,SCR_HEIGHT),[]() {}, };

		Application* application = new Application(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE, { view });
		application->init();

		application->_clearColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		SoundSystem::setDefaultPath("./Assets/Sounds/");
		FontManager::setDefaultPath("./Assets/Fonts/");
		Shader::setDefaultPath("./Assets/Shaders/");
		Mesh::setDefaultPath("./Assets/Meshes/");
		Texture::setDefaultPath("./Assets/Textures/");

		FontManager::loadTypeFace("Viper Nora.ttf");

		#pragma region Shadow Mapping Shader

		char* shadowVert = R"(
			#version 420 core

			layout(location = 0) in vec3 aPos;

			uniform mat4 model;

			void main() {
				gl_Position = model * vec4(aPos, 1.0);
			}
		)";

		char* shadowGeom = R"(
			#version 420 core

			layout(triangles) in;
			layout(triangle_strip, max_vertices = 18) out;

			layout(location = 0) out vec4 outPosition;

			uniform mat4 shadowMatrices[6];

			void main() {
				for(int face = 0; face < 6; ++face) {
					gl_Layer = face;
					for(int i = 0; i < 3; ++i) {
						outPosition = gl_in[i].gl_Position;
						gl_Position = shadowMatrices[face] * outPosition;
						EmitVertex();
					}
					EndPrimitive();
				}
			}
		)";

		char* shadowFrag = R"(
			#version 460

			layout(location = 0) in vec4 inPosition;

			uniform vec3 lightPosition;
			uniform float farPlane;

			void main() {
				float distanceToLight = length(inPosition.xyz - lightPosition);
				distanceToLight /= farPlane;

				gl_FragDepth = distanceToLight;
			}
			
		)";

		Cappuccino::Application::_shadowMappingShader = new Shader(true, shadowVert, shadowFrag, shadowGeom);

		#pragma endregion

		#pragma region GBuffer Shader
		
		char* gVert = R"(
		#version 420 core
		
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTexCoords;
		layout (location = 2) in vec3 aNormal;
		layout (location = 3) in vec3 aTangs;

		layout (location = 4) in vec3 bPos;
		layout (location = 5) in vec3 bNormal;
		layout (location = 6) in vec3 bTangs;


		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		uniform float dt;

		uniform int useViewMat;

		uniform int isGun;
		uniform float posVarience;
		uniform vec3 PlayerPosition;

		out vec3 FragPos;
		out vec2 TexCoords;
		out mat3 TBN;

		void main(){
		    vec3 apos =      aPos;
			vec3 anormal =   aNormal;
			vec3 atangs =    aTangs;

			vec3 bpos =      bPos;
			vec3 bnormal =   bNormal;
			vec3 btangs =    bTangs;

			
			apos =		mix(apos,bpos,dt);
			anormal =	mix(anormal,bnormal,dt);
			atangs =	mix(atangs,btangs,dt);
			

			TexCoords = aTexCoords;

			//https://learnopengl.com/Advanced-Lighting/Normal-Mapping
			vec3 T = normalize(vec3(model* vec4(atangs,0.0)));
			vec3 N = normalize(vec3(model* vec4(anormal,0.0)));
			vec3 B = normalize(cross(T,N));
			TBN = mat3(T,B,N);

			FragPos = vec3(model * vec4(apos, 1.0));

			if(isGun == 1){
				apos.y += posVarience;
				FragPos -= PlayerPosition;
				FragPos *= -1.0f;
				FragPos = vec3(vec4(FragPos,1.0f)*view).xyz;
			}

			if(useViewMat == 1)
				gl_Position = projection * view * model * vec4(apos, 1.0);
			else
				gl_Position = projection * model * vec4(apos,1.0);

		}

)";

		char* gFrag = R"(
		#version 420 core
		
		layout (location = 0) out vec3 gPos;
		layout (location = 1) out vec3 gNormal;
		layout (location = 2) out vec3 gAlbedo;
		layout (location = 3) out vec3 gMetalRoughnessAO;
		layout (location = 4) out vec3 gEmissive;

		struct Material {
		    sampler2D albedo;
		    sampler2D normalMap;
		    sampler2D metallic;
		    sampler2D roughness;
		    sampler2D ambientOcc;
		    sampler2D emission;
		};  

		uniform Material material;
		in vec3 FragPos;
		in vec2 TexCoords;
		in mat3 TBN;

		void main(){
		    gPos = FragPos;
			vec3 norm = texture(material.normalMap,TexCoords).rgb;
			norm = normalize(norm*2.0 - 1.0);
			norm = normalize(TBN*norm);
			gNormal = norm;
			gAlbedo = texture(material.albedo,TexCoords).rgb;
			
			vec3 temp;
			temp.r = texture(material.metallic,TexCoords).r;
			temp.g = texture(material.roughness,TexCoords).r;
			temp.b = texture(material.ambientOcc,TexCoords).r;
			gMetalRoughnessAO = temp;
			gEmissive = texture(material.emission,TexCoords).rgb;
		}

)";
		Cappuccino::Application::_gBufferShader = new Cappuccino::Shader(true, gVert, gFrag);

		#pragma endregion

		#pragma region Blur Shader
		
		char* blurVert = R"(
		#version 420 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTexCoords;
		
		out vec2 TexCoords;
		
		void main()
		{
		    TexCoords = aTexCoords;
		    gl_Position = vec4(aPos, 1.0);
		} 
)";

		char* blurFrag = R"(
		#version 420 core
		
		out vec4 FragColour;

		in vec2 TexCoords;

		uniform sampler2D image;

		//https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/7.bloom/7.blur.fs
		//https://uoit.blackboard.com/bbcswebdav/pid-1297335-dt-content-rid-23287362_1/courses/20200170442.202001/Week%204%20-%20Bloom%20and%20Motion%20Blur.pdf
		uniform bool horizontal;
		uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
		void main(){
			 vec2 offset = 1.0 / textureSize(image, 0); // gets size of single texel
			
			 if(horizontal)
				offset = offset * vec2(1.0f,0.0f);
			 else
				offset = offset * vec2(0.0f,1.0f);

			 vec4 result = texture(image, TexCoords) * weight[0];//blur without offset
			 for(int i = 1; i < 5;i++){
				 result += texture(image,TexCoords + offset * i) * weight[i];
				 result += texture(image,TexCoords - offset * i) * weight[i];
			 }

			 FragColour = result;
		}

)";
		Application::_blurPassShader = new Cappuccino::Shader(true, blurVert, blurFrag);

		#pragma endregion

		#pragma region PP Shader
		
		char* bloomFrag = R"(
		#version 420 core
		out vec4 FragColour;

		uniform sampler2D screenTexture;
		uniform sampler2D bloomTexture;

		struct LookupTable{
			sampler3D LUT;
			int active;
		};
		uniform LookupTable lookup;
		uniform int useLookupTable;
		

		in vec2 TexCoords;
		void main(){
			vec3 hdr = texture(screenTexture,TexCoords).rgb;
			vec3 bloom = texture(bloomTexture,TexCoords).rgb;
			
			hdr += bloom;

			
			//now apply HDR
			vec3 finalCol = vec3(1.0f) - exp(-hdr);

			vec4 fCol;
			if(useLookupTable == 1)
				fCol = texture(lookup.LUT,finalCol);			
			else
				fCol = vec4(finalCol,1.0f);			

			FragColour = fCol;
		
		}
)";

		Application::_ppShader = new Cappuccino::Shader(true, blurVert, bloomFrag);

		#pragma endregion

		Cappuccino::LUT lut("Custom.CUBE");
		lut.loadLUT();
		Cappuccino::Application::_activeLUT = &lut;


		auto mBank = Cappuccino::SoundBank("Master.bank");
		auto mStringsBank = Cappuccino::SoundBank("Master.strings.bank");
		

		MenuScene* m = new MenuScene(true);
		m->init();

		GameplayScene* g = new GameplayScene(false);
		GameplayScene::sendGBufferShaderUniforms();


		application->run();
		delete application;
	}

	return 0;

}
