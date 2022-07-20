#include "TriggerBot.h"

TriggerBot::TriggerBot() : IModule(0, Category::COMBAT, "Attacks the entity you're looking at") {
	registerIntSetting("APS", &APS, APS, 1, 20);
}

const char* TriggerBot::getModuleName() {
	return ("TriggerBot");
}

void TriggerBot::onTick(GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	auto target = g_Data.getLocalPlayer()->level->getEntity();
	auto pointing = g_Data.getLocalPlayer()->level;

	if (g_Data.canUseMoveKeys()) {
		if (target != 0 && TimeUtil::hasTimedElapsed((1000 / APS), true)) {
			if (!TargetUtil::isValidTarget(target))
				return;

			player->swing();
			gm->attack(target);
		}
	}
}