#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class AutoHive : public IModule {
private:
	bool autoLootBox = true;
	bool autoQueue = true;
	bool autoGG = true;

	float animYaw = 0.f;
public:
	bool lootBoxListEmpty = true;
	bool autoBridgeWin = true;
	bool doLerp = true;

	virtual void onSendPacket(Packet* packet) override;
	virtual void onPlayerTick(Player* plr) override;
	virtual const char* getRawModuleName() override;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	AutoHive();
};