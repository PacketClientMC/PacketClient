#pragma once
#include "../../ModuleManager.h"
#include "../../Utils/DrawUtils.h"
#include "../Module.h"

class Radar : public IModule {
private:
	bool grid = true;
public:
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx);
	virtual const char* getModuleName();
	Radar();
};