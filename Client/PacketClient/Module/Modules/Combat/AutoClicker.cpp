#include "AutoClicker.h"

AutoClicker::AutoClicker() : IModule(0, Category::COMBAT, "Automatically clicks for you") {
	registerBoolSetting("Weapons", &weapons, weapons);
	registerBoolSetting("Hold", &hold, hold);
	registerIntSetting("MaxCPS", &maxCPS, maxCPS, 1, 20);
	registerIntSetting("MinCPS", &minCPS, minCPS, 1, 20);
}

const char* AutoClicker::getModuleName() {
	return ("AutoClicker");
}

void AutoClicker::onPlayerTick(Player* plr) {
	if (plr == nullptr) return;

	auto selectedItem = plr->getSelectedItem();
	if (weapons && selectedItem->getAttackingDamageWithEnchants() < 1)
		return;

	if (hold && !g_Data.isLeftClickDown())
		return;

	if (maxCPS < minCPS) maxCPS = minCPS;
	CPS = randomFloat(maxCPS, minCPS);


}

void AutoClicker::onTick(GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	auto selectedItem = player->getSelectedItem();
	if (weapons && selectedItem->getAttackingDamageWithEnchants() < 1)
		return;

	if (hold && !g_Data.isLeftClickDown())
		return;

	if (g_Data.canUseMoveKeys() && !moduleMgr->getModule<ClickGUIMod>()->isEnabled()) {
		Level* pointing = player->level;
		bool isDestroyed = false;

		g_Data.leftclickCount++;
		player->swing();
		if (pointing->hasEntity()) { if (TimeUtil::hasTimedElapsed((1000 / (float)CPS), true)) gm->attack(pointing->getEntity()); }
		else {
			gm->startDestroyBlock(pointing->block, pointing->blockSide, isDestroyed);
			gm->stopDestroyBlock(pointing->block);
			if (isDestroyed && player->region->getBlock(pointing->block)->blockLegacy->blockId != 0) gm->destroyBlock(&pointing->block, 0);
		}
	}
}