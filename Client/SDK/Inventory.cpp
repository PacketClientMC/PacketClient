#include "Inventory.h"
#include "../Utils/Utils.h"
#include "../Memory/GameData.h"

void Inventory::dropSlot(int slot) {
	using drop_t = void(__fastcall*)(Inventory*, int, char);
	static drop_t func = reinterpret_cast<drop_t>(FindSignature("85 D2 0F 88 ?? ?? ?? ?? 48 89 5C 24 ?? 55 56 57 41 54"));
	if (func != 0)
		func(this, slot, 0);
}
void Inventory::dropAll() {
	// FillingContainer::dropAll will redo when needed
	//using dropAll_t = void(__fastcall*)(Inventory*, int, int, char);
	//static dropAll_t func = reinterpret_cast<dropAll_t>(FindSignature("48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 48 89 7C 24 ?? 41 56 48 83 EC ?? 48 8B 01 41 0F"));
	//if (func != 0)
	//func(this, slot, 0, 0);
	for (int i = 0; i < 36; i++) {
		this->dropSlot(i);
	}
}
bool Inventory::isFull() {
	int fullslots = 0;
	for (int i = 0; i < 36; i++) {
		if (this->getItemStack(i)->item != nullptr)
			fullslots++;
	}
	if (fullslots == 36) return true;
	return false;
}

void ContainerScreenController::handleAutoPlace(uintptr_t a1, std::string name, int slot) {
	using ContainerScreenController__autoPlace = __int64(__fastcall*)(ContainerScreenController*, uintptr_t, TextHolder, int);
	static ContainerScreenController__autoPlace autoPlaceFunc = reinterpret_cast<ContainerScreenController__autoPlace>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 45 8B E1 4D 8B F0"));

	TextHolder txt = TextHolder(name);

	if (autoPlaceFunc != 0x0) autoPlaceFunc(this, a1, txt, slot);
}

void Inventory::moveItem(int from, int to = -1) {
	InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();

	uintptr_t sigOffset = FindSignature("48 8D 3D ? ? ? ? 80 B8 ? ? ? ? ? 75 19 48 8B 88 ? ? ? ? 48 8B 11 4C 8B 42 28 8B 50 10");
	int offset = *reinterpret_cast<int*>(sigOffset + 3);
	ItemStack* emptyItemStack = reinterpret_cast<ItemStack*>(sigOffset + offset + /*length of instruction*/ 7);

	if (to < 0) to = getFirstEmptySlot();
	ItemStack* item1 = getItemStack(from);
	ItemStack* item2 = getItemStack(to);

	if (item2->item == NULL) {
		InventoryAction first(from, item1, nullptr);
		InventoryAction second(to, nullptr, item1);
		manager->addInventoryAction(first);
		manager->addInventoryAction(second);
		*item1 = *emptyItemStack;
	}
	else {
		InventoryAction first(from, item1, nullptr);
		InventoryAction second(to, item2, item1);
		InventoryAction third(from, nullptr, item2);
		manager->addInventoryAction(first);
		manager->addInventoryAction(second);
		manager->addInventoryAction(third);
	}
}

void Inventory::swapSlots(int from, int to) {
	InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();
	ItemStack* i1 = getItemStack(from);
	ItemStack* i2 = getItemStack(to);
	InventoryAction first(from, i1, nullptr);
	InventoryAction second(to, i2, i1);
	InventoryAction third(from, nullptr, i2);
	manager->addInventoryAction(first);
	manager->addInventoryAction(second);
	manager->addInventoryAction(third);
}
