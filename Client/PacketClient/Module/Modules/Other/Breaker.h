#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Breaker : public IModule {
private:
	bool treasures = false;
	bool redstone = false;
	float animYaw = 0.f;
	bool cakes = false;
	bool beds = false;
	bool eggs = false;
	int delay = 0;

	vec3_ti blockPos;
public:
	bool destroy = false;
	bool swing = true;
	bool eat = false;
	int range = 5;

	std::string name = "Breaker";
	SettingEnum rotations = this;

	// Inherited via IModule
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(Packet* packet) override;
	virtual void onPlayerTick(Player* plr) override;
	virtual const char* getRawModuleName() override;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
	Breaker();
};