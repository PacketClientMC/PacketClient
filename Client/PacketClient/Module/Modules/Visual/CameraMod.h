#pragma once
#include "../../ModuleManager.h"
#include "../../Utils/DrawUtils.h"
#include "../Module.h"

class CameraMod : public IModule {
public:
	bool fullbright = false;
	bool nohurtcam = false; 
	bool viewClip = false;

	float* gammaPtr = nullptr;
	int intensity = 25;

	virtual const char* getModuleName();
	virtual void onTick(GameMode* gm);
	virtual void onDisable();
	virtual void onEnable();
	CameraMod();
};