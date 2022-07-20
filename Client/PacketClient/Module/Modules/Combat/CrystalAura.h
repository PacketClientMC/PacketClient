#pragma once
#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class CrystalAura : public IModule {
private:
	int prevSlot;
	int delay = -50;
	int eRange = 3;
	bool autoplace = true;
	bool pEnhanced = false;
	bool dEnhanced = false;
	bool Preview = false;
	bool AutoSelect = true;
	bool FinishSelect = false;
	PlayerInventoryProxy* supplies = nullptr;
	Inventory* inv = nullptr;

public:
	CrystalAura();

	// Inherited via IModule
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx);
	virtual const char* getModuleName();
	virtual void onTick(GameMode* gm);
	virtual void onDisable();
	virtual void onEnable();

	int cRange = 10;
	int pRange = 5;
	int range = 10;

	void CPlace(GameMode* gm, vec3_t* pos);
	void DestroyC(Entity* ent, int range);
};