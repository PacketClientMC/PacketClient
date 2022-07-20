#include "AutoPot.h"

using namespace std;
AutoPot::AutoPot() : IModule(0, Category::COMBAT, "Throws potions for you") {
    registerIntSetting("Health", &health, health, 1, 19);
    registerIntSetting("Delay", &delay, delay, 0, 20);
}

const char* AutoPot::getRawModuleName() {
    return "AutoPot";
}

const char* AutoPot::getModuleName() {
    name = string("AutoPot ") + string(GRAY) + to_string((int)health);
    return name.c_str();
}

void AutoPot::onTick(GameMode* gm) {
    auto player = g_Data.getLocalPlayer();
    if (player == nullptr) return;

    shouldThrow = false;
    if (g_Data.canUseMoveKeys()) {
        int playerHealth = player->getHealth();
        auto inv = player->getSupplies()->inventory;

        if (playerHealth < health) {
            for (int i = 0; i < 9; i++) {
                ItemStack* stack = inv->getItemStack(i);
                if (stack->item != nullptr) {
                    if (stack->getItem()->itemId == 561) shouldThrow = true;

                    if (shouldThrow) {
                        g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = i;
                        g_Data.getGameMode()->useItem(*stack);
                        return;

                    }
                }
            }
        }
    }
}

void AutoPot::onPlayerTick(Player* plr) {
    if (plr == nullptr) return;

    if (g_Data.canUseMoveKeys() && shouldThrow) {
        plr->pitch = 80;
    }
}

void AutoPot::onSendPacket(Packet* packet) {
    auto player = g_Data.getLocalPlayer();
    if (player == nullptr) return;

    if (g_Data.canUseMoveKeys() && shouldThrow) {
        if (packet->isInstanceOf<MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>()) {
            auto* inputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
            auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);

            inputPacket->pitch = 80;
            movePacket->pitch = 80;
        }
    }
}