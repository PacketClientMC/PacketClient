#pragma once
#include "../PacketClient/Module/Modules/Module.h"
#include "../PacketClient/Module/ModuleManager.h"
#include "../../Utils/DrawUtils.h"
#include <vcruntime_string.h>
#include "TextHolder.h"

class Attribute {
private:
	__int64 getHealth() {
		static int* HealthAttribute = 0x0;

		if (HealthAttribute == 0x0) {
			uintptr_t offset = FindSignature("48 8D 15 ?? ?? ?? ?? FF 90 ?? ?? ?? ?? F3 ?? ?? 88 ?? ?? ?? ?? 85 C9 7E ??");
			HealthAttribute = (int*)(offset + *(int*)(offset + 3) + 7);
		}
		return g_Data.getLocalPlayer()->getAttribute(HealthAttribute);
	}
public:
	float getMinHealth() {
		return *(float*)(getHealth() + 0x88);
	}
	float getCurrentHealth() {
		return *(float*)(getHealth() + 0x80);
	}
	float getMaxHealth() {
		return *(float*)(getHealth() + 0x84);
	}
};