#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Jesus : public IModule {
private:
	bool viewBobbingEffective = false;
	bool velocityEffective = false;
	bool wasInWater = false;
	float height = 0.5;
public:
	bool foundWater = false;

	std::string name = "Jesus";
	SettingEnum mode = this;

	virtual void onMove(MoveInputHandler* input);
	virtual const char* getRawModuleName();
	virtual const char* getModuleName();
	virtual void onTick(GameMode* gm);
	virtual void onDisable();
	Jesus();
};