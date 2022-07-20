#include "AutoTotem.h"

using namespace std;
AutoTotem::AutoTotem() : IModule(0, Category::PLAYER, "Automatically puts totems into your offhand") {
	registerBoolSetting("Force", &force, force);
}

const char* AutoTotem::getModuleName() {
	return ("AutoTotem");
}

void AutoTotem::onTick(GameMode* gm) {
	PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();
	ItemStack* current = g_Data.getLocalPlayer()->getEquippedTotem();
	if (force) {
		InventoryAction* firstAction = nullptr;
		ItemDescriptor* desc = nullptr;
		ItemStack* yot = nullptr;
		int itemId = 0;
		if (itemId == 0) {
			TextHolder tempText("totem");
			unique_ptr<void*> ItemPtr = make_unique<void*>();
			unique_ptr<void*> buffer = make_unique<void*>();
			Item*** cStack = ItemRegistry::lookUpByName(ItemPtr.get(), buffer.get(), tempText);
			yot = new ItemStack(***cStack, 1, 0);
		}
		if (yot != nullptr) yot->count = 2;
		desc = new ItemDescriptor((*yot->item)->itemId, 0);
		firstAction = new InventoryAction(0, desc, nullptr, yot, nullptr, 1, 507, 99999);
		g_Data.getLocalPlayer()->getTransactionManager()->addInventoryAction(*firstAction);
		delete firstAction;
		delete desc;
		g_Data.getLocalPlayer()->setOffhandSlot(yot);
		return;
	}
	if (current->item == NULL) {
		for (int i = 0; i < 36; i++) {
			ItemStack* totem = inv->getItemStack(i);
			if (totem->item != NULL && (*totem->item)->itemId == 568) {
				ItemDescriptor* desc = nullptr;
				desc = new ItemDescriptor((*totem->item)->itemId, 0);
				InventoryAction first(i, desc, nullptr, totem, nullptr, 1);
				InventoryAction second(37, nullptr, desc, nullptr, totem, 1);
				g_Data.getLocalPlayer()->setOffhandSlot(totem);
				manager->addInventoryAction(first);
				manager->addInventoryAction(second);
			}
		}
	}
}