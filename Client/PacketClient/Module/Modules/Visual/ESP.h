#pragma once
#include "../../../../Utils/TargetUtil.h"
#include "../../../../SDK/Camera.h"
#include "../../ModuleManager.h"
#include "../../Utils/DrawUtils.h"
#include "../Module.h"

class ESP : public IModule {
public:
	bool tracers = false;
	bool mobs = false;
	bool is2D = false;

	SettingEnum mode = this;

	void onLevelRender() override;
	virtual const char* getModuleName() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
	ESP();
};