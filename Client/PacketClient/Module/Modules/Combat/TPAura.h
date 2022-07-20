#pragma once
#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class TPAura : public IModule {
private:
	bool teleported = false;
	bool multi = false;
	int delay = 10;
	int ticks = 0;
public:
	float range = 250.f;

	std::string name = "TPAura";
	SettingEnum mode = this;

	virtual void onPostRender(MinecraftUIRenderContext* renderCtx);
	virtual void onSendPacket(Packet* packet);
	virtual const char* getModuleName();
	virtual void onTick(GameMode* gm);
	virtual void onLevelRender();
	virtual void onDisable();
	virtual void onEnable();
	TPAura();
};