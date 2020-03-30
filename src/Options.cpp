#include "Options.h"
#include "Cappuccino/LUT.h"
#include "Cappuccino/Application.h"

using namespace Cappuccino;

bool Options::Assault = false;
bool Options::Commando = false;
bool Options::Demolitionist = false;
bool Options::Scout = false;

bool Goptions::_bloomOn = true;
bool Goptions::_lookupTableOn = true;
std::vector<LUT*> Goptions::_lookupTables = {};
int Goptions::_numBlurPasses = 10;
bool Goptions::_active = false;
float Goptions::_exposure = 1.0f;
CappInput* Goptions::_in = new CappInput(true, std::nullopt);

void Goptions::sendUniforms()
{
	Application::_ppShader->use();
	Application::_ppShader->setUniform("useBloom", _bloomOn);
	Application::_ppShader->setUniform("exposure", _exposure);
	Application::_ppShader->setUniform("useLookupTable", _lookupTableOn);

	Application::_numBlurPasses = _numBlurPasses;
}

void Goptions::processKey(KeyEvent k, bool& boolToChange, bool& controlBool) {
	
	if (_in->keyboard->keyPressed(k) && !controlBool) {
		controlBool = true;
		boolToChange ^= true;
	}
	else if (_in->keyboard->keyReleased(k) && controlBool)
		controlBool = false;
}

void Goptions::processKey(Cappuccino::KeyEvent k, int& intToChange, int incrimentBy, bool& controlBool)
{
	
	if (_in->keyboard->keyPressed(k) && !controlBool) {
		controlBool = true;
		intToChange += incrimentBy;
	}
	else if (_in->keyboard->keyReleased(k) && controlBool)
		controlBool = false;
}

void Goptions::processKey(Cappuccino::KeyEvent k, float& floatToChange, float incrimentBy, bool& controlBool)
{
	
	if (_in->keyboard->keyPressed(k) && !controlBool) {
		controlBool = true;
		floatToChange += incrimentBy;
	}
	else if (_in->keyboard->keyReleased(k) && controlBool)
		controlBool = false;
}

void Goptions::update(float dt)
{
	static bool bControl = false, upControl = false, downControl = false,
				rightControl = false, leftControl = false,lutControl = false;

	if (!_active)
		return;

	processKey(KeyEvent::B, _bloomOn, bControl);
	processKey(KeyEvent::UP_ARROW, _numBlurPasses, 10,upControl);
	processKey(KeyEvent::DOWN_ARROW, _numBlurPasses, -10,downControl);

	processKey(KeyEvent::RIGHT_ARROW, _exposure, 1,rightControl);
	processKey(KeyEvent::LEFT_ARROW, _exposure, -1,leftControl);

	processKey(KeyEvent::N, _lookupTableOn, lutControl);
	
	if (_numBlurPasses < 0)
		_numBlurPasses = 1;
	if (_exposure <= 0.0f)
		_exposure = 1.0f;

	sendUniforms();
}

bool Goptions::isLookupOn()
{
	return _lookupTableOn;
}

bool Goptions::isBloomOn()
{
	return _bloomOn;
}

int Goptions::getNumBlurPasses()
{
	return _numBlurPasses;
}

int Goptions::getExposure()
{
	return _exposure;
}

void Goptions::setNumBlurPasses(int num)
{
	_numBlurPasses = num > 0 ? num : 1;
}

void Goptions::setExposure(unsigned e)
{
	_exposure = e > 0.0f ? e : 1.0f;
}

void Goptions::setLookupOn(bool yn)
{
	_lookupTableOn = yn;
}

void Goptions::setBloomOn(bool yn)
{
	_bloomOn = yn;
}

std::vector<LUT*>& Goptions::getLookupTables()
{
	return _lookupTables;
}

void Goptions::toggleGoptions()
{
	_active ^= true;
}

bool Goptions::isActive()
{
	return _active;
}
