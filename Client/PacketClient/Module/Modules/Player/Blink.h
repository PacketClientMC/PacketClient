#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Blink : public IModule {
private:
	std::vector<PlayerAuthInputPacket*> PlayerAuthInputPacketHolder;
	std::vector<MovePlayerPacket*> MovePlayerPacketHolder;
public:
	inline std::vector<PlayerAuthInputPacket*>* getPlayerAuthInputPacketHolder() { return &PlayerAuthInputPacketHolder; };
	inline std::vector<MovePlayerPacket*>* getMovePlayerPacketHolder() { return &MovePlayerPacketHolder; };
	virtual void onTick(GameMode* gm);
	virtual const char* getModuleName();
	Blink();
	~Blink();
};