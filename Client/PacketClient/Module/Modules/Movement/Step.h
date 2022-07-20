#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Step : public IModule {
private:
	float height = 1.5f;
public:
	bool reverse = false;

	std::string name = "Step";
	SettingEnum mode = this;

	virtual void onMove(MoveInputHandler* input) override;
	virtual const char* getRawModuleName() override;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
	Step();
};