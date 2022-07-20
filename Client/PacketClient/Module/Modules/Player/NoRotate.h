#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class NoRotate : public IModule {
private:
	bool body = false;
public:
	std::string name = ("NoRotate");
	SettingEnum mode = this;

	virtual void onSendPacket(Packet* packet) override;
	virtual void onPlayerTick(Player* plr) override;
	virtual const char* getRawModuleName() override;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	NoRotate();
};