#pragma once
#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class HitBoxes : public IModule {
public:
	float height = 3.f;
	float width = 4.f;
	float range = 8.f;

	std::string name = "HitBoxes";

	virtual void onSendPacket(Packet* packet);
	virtual const char* getRawModuleName();
	virtual const char* getModuleName();
	virtual void onTick(GameMode* gm);
	HitBoxes();
};