#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class TestModule : public IModule {
private:
	bool notification = false;
	bool autoDisable = false;
	bool debugInfo = true;
	int testCounter = 0;
	bool blink = false;
	bool test2 = false;
	bool test = false;

	vec3_t initPos;
	int tick = 0;

	bool kowBool = false;
	float value = -0.0f;
	float kowFloat = 2;
	int kowInt1 = 0;
	int kowInt2 = 0;

public:
	bool copyChatMessages = false;
	int testModuledefaultX = 0;
	int testModuledefaultY = 0;
	float sliderX = 0.f;
	float sliderY = 0.f;
	float sliderZ = 0.f;
	float sliderX2 = 0.f;
	float sliderY2 = 0.f;
	float sliderZ2 = 0.f;

	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onMove(MoveInputHandler* input) override;
	virtual void onTick(GameMode* gm) override;
	virtual void onSendPacket(Packet* packet);
	virtual void onPlayerTick(Player* plr);
	virtual void onImGuiRender() override;
	virtual const char* getModuleName();
	virtual void onDisable() override;
	virtual void onEnable() override;
	TestModule();
};