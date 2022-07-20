#pragma once
#include "../Module.h"

class Xray : public IModule {
public:
	bool* smoothLightningSetting = nullptr;
	bool gotSmoothInfo = false;
	bool wasSmooth = false;

	virtual const char* getModuleName();
	virtual void onTick(GameMode* gm);
	virtual void onDisable();
	virtual void onEnable();
	Xray();
};