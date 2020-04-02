#pragma once
#include <vector>
#include "Cappuccino/CappInput.h"
#include "Cappuccino/SoundSystem.h"

class Options
{
public:
	static bool Scout;
	static bool Demolitionist;
	static bool Commando;
	static bool Assault;
	static void setTutorial(bool yn);
	static bool isTutorial();
private:
	static bool _tutorial;

	static Cappuccino::SoundBank* Music;
};

class MusicManager {
public:

	//get current playing song
	static unsigned getCurrentPlaying();
	
	//play a song at the index
	static void playSong(unsigned index);

	//tell fmod to turn on or off combat trigger
	//index is for whatever index the song is stored
	static void combatTrigger(unsigned index, float yn);

	//turn on or off the level clear event at index	
	static void levelClearTrigger(unsigned index, float yn);

private:
	static unsigned playingIndex;
};

namespace Cappuccino {

	class LUT;
	class Application;
}

//graphics options... goptions
class Goptions {
public:

	static void update(float dt);

	static bool isLookupOn();
	static bool isBloomOn();

	static int getNumBlurPasses();
	static int getExposure();

	static void setNumBlurPasses(int num);
	static void setExposure(unsigned e);

	static void setLookupOn(bool yn);
	static void setBloomOn(bool yn);

	static std::vector<Cappuccino::LUT*>& getLookupTables();

	static void toggleGoptions();

	static bool isActive();

	//LUT* operator[](unsigned index);
private:
	static void sendUniforms();
	static void processKey(Cappuccino::KeyEvent k, bool& boolToChange,bool& controlBool);
	static void processKey(Cappuccino::KeyEvent k, int& intToChange,int incrementBy, bool& controlBool);
	static void processKey(Cappuccino::KeyEvent k, float& floatToChange,float incrementBy, bool& controlBool);
	static bool _bloomOn;
	static bool _lookupTableOn;
	static std::vector<Cappuccino::LUT*> _lookupTables;//must have at least 2 modes, warm, cool, sepia...
	static int _numBlurPasses;
	static float _exposure;

	static Cappuccino::CappInput* _in;
	static bool _active;

};