#pragma once
#include "../Module.h"

class Criticals : public IModule {
private:
	bool hurttime = true;
	int tick = 0;

public:
	Criticals();

	std::string name = "Criticals";
	SettingEnum mode = this;

	// Inherited via IModule
	virtual void onSendPacket(Packet* packet);
	virtual const char* getRawModuleName();
	virtual const char* getModuleName();
	virtual void onTick(GameMode* gm);
};
