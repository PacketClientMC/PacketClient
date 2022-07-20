#include "NoRotate.h"
#include "../../../../SDK/Attribute.h"

using namespace std;
NoRotate::NoRotate() : IModule(0, Category::PLAYER, "Prevents servers from setting your rotation") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Vanilla", 0);
	//mode.addEntry("", 1);
	registerBoolSetting("Body", &body, body);
}

const char* NoRotate::getRawModuleName() {
	return "NoRotate";
}

const char* NoRotate::getModuleName() {
	if (mode.getSelectedValue() == 0) name = string("NoRotate ") + string(GRAY) + string("Vanilla");
	return name.c_str();
}

void NoRotate::onTick(GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
}

void NoRotate::onPlayerTick(Player* plr) {
	auto killaura = moduleMgr->getModule<Killaura>();
	auto autoHive = moduleMgr->getModule<AutoHive>();
	auto breaker = moduleMgr->getModule<Breaker>();

	if (!killaura->targetListEmpty || !autoHive->lootBoxListEmpty || breaker->destroy || breaker->eat) return;
	if (plr == nullptr) return;

	if (body) for (int i = 0; i < 44; i++) plr->bodyYaw = plr->yaw;
}

void NoRotate::onSendPacket(Packet* packet) {
	if (packet->isInstanceOf<MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>()) {
		if (g_Data.getLocalPlayer() != nullptr && g_Data.canUseMoveKeys() && body) {
		}
	}
}