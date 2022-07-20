#include "DupeCommand.h"

DupeCommand::DupeCommand() : IMCCommand("dupe", "Duplicates the item in hand", "<count> <mode: give / offhand : 1/0>") {
	registerAlias("d");
}

bool DupeCommand::execute(std::vector<std::string>* args) {
	PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	auto transactionManager = g_Data.getLocalPlayer()->getTransactionManager();
	Inventory* inv = supplies->inventory;
	int selectedSlot = supplies->selectedHotbarSlot;
	ItemStack* item = inv->getItemStack(selectedSlot);
	int count = item->count;
	bool isGive = true;
	if (args->size() > 1)
		item->count = assertInt(args->at(1));
	if (args->size() > 2)
		isGive = static_cast<bool>(assertInt(args->at(2)));
	if (isGive) {
		InventoryAction* firstAction = nullptr;
		InventoryAction* secondAction = nullptr;
		firstAction = new InventoryAction(0, item, nullptr, 507, 99999);
		transactionManager->addInventoryAction(*firstAction);
		inv->addItemToFirstEmptySlot(item);
	} else {
		InventoryAction* no = nullptr;
		no = new InventoryAction(0, item, nullptr, 507, 99999);
		transactionManager->addInventoryAction(*no);
		g_Data.getLocalPlayer()->setOffhandSlot(item);
	}
	if (args->size() > 1)
		item->count = count;
	clientMessageF("[Packet] %sSuccessfully duplicated the item!", GREEN);
	return true;
}