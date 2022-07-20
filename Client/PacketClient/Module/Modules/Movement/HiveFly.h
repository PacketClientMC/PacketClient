#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class HiveFly : public IModule {
private:
	float clipHeight = 2.f;
	bool clip = false;
	int counter69 = 0;
	int counter = 0;
public:
	virtual void onMove(MoveInputHandler* input);
	virtual const char* getModuleName();
	virtual void onTick(GameMode* gm);
	virtual void onDisable();
	virtual void onEnable();
	HiveFly();
};