#pragma once
#include "../Module.h"

class ConfigManagerMod : public IModule {
public:
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx);
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx);
	virtual void onLoadConfig(void* conf);
	virtual void onSaveConfig(void* conf);
	virtual const char* getModuleName();
	virtual void onTick(GameMode* gm);
	virtual bool allowAutoStart();
	virtual void onDisable();
	virtual void onEnable();
	ConfigManagerMod();
};