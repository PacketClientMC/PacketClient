#pragma once

#include "Module.h"

class DebugMenu : public IModule {
public:
    DebugMenu();

    std::string name = "DebugMenu";
    SettingEnum mode = this;

    virtual void onPostRender(MinecraftUIRenderContext* renderCtx);
    virtual const char* getRawModuleName();
    virtual void onTick(GameMode* gm);
    virtual const char* getModuleName();
};