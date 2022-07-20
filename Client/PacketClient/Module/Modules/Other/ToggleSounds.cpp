#include "ToggleSounds.h"

using namespace std;
ToggleSounds::ToggleSounds() : IModule(0, Category::OTHER, "Plays a sound when you toggle a module") {
	registerEnumSetting("Sound", &sound, 0);
	sound.addEntry("Click", 0);
	sound.addEntry("Piston", 1);
	sound.addEntry("Sigma", 2);
	registerFloatSetting("Volume", &volume, volume, 0.05f, 1.f);
}

const char* ToggleSounds::getRawModuleName() {
	return "ToggleSounds";
}

const char* ToggleSounds::getModuleName() {
	name = string("ToggleSounds ") + string(GRAY) + sound.GetEntry(sound.getSelectedValue()).GetName();
	return name.c_str();
}