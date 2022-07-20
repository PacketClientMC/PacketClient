#pragma once
#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class PlayerList : public IModule {
private:
	float range = 255;
	bool hold = true;
public:
	float positionX, positionY = 0;
	vec4_t rectPos2;

	SettingEnum theme = this;

	virtual void onPostRender(MinecraftUIRenderContext* renderCtx);
	virtual void onTick(GameMode* gm);
	virtual const char* getModuleName();
	virtual bool isHoldMode();
	virtual void onDisable();
	virtual void onEnable();
	PlayerList();
};