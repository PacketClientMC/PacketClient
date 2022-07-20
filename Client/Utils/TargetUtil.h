#pragma once
#include "../PacketClient/Module/ModuleManager.h"
#include "../SDK/Entity.h"
#include <vector>

class TargetUtil {
private:
	static bool containsOnlyASCII(const std::string& string);
public:
	static void init(LocalPlayer** lc);
	static bool isValidTarget(Entity* ent);
};
