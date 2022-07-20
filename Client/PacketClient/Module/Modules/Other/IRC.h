#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class IRC : public IModule {
public:
    bool toggled = false;

    virtual void sendMessage(std::string message);
    virtual void onTick(GameMode* gm) override;
    virtual const char* getModuleName() override;
    virtual void onDisable() override;
    virtual void onEnable() override;
    virtual void startConnection();
    virtual void endConnection();
    IRC();
};