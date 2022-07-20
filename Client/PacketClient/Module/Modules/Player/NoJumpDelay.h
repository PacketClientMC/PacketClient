#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class NoJumpDelay : public IModule {
private:
	bool timerwasenabled = false;
	bool bypass = false;
	bool moving = false;
	int tickTimer = 0;
	int timer = 20;
	int tick = 0;
public:
	virtual void onMove(MoveInputHandler* input) override;
	virtual void onTick(GameMode* gm) override;
	const char* getModuleName() override;
	virtual void onDisable() override;
	NoJumpDelay();
};