#pragma once
#include "../../ModuleManager.h"
#include "../../Utils/DrawUtils.h"
#include "../Module.h"

class ChestESP : public IModule {
private:
	std::vector<std::shared_ptr<AABB>> bufferedChestList;
	int tickTimeout = 0;
	std::mutex listLock;
public:
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx);
	virtual const char* getModuleName();
	virtual void onTick(GameMode* gm);
	ChestESP();
};