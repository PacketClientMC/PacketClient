// MSVC doesn't like pragma once in pch (I think)
#ifndef PCH_H
#define PCH_H

// Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Standard library
#include <unordered_map>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <optional>
#include <Psapi.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>

// Memory
#include "Memory/MinHook.h"
#include "Memory/SlimMem.h"

// Utils
#include "Utils/xorstr.h"
#include "Utils/Json.hpp"
#include "Utils/HMath.h"
#include "Utils/Utils.h"
#include "Utils/Keys.h"

// SDK
#include "SDK/ComplexInventoryTransaction.h"
#include "SDK/ClientInstanceScreenModel.h"
#include "SDK/MinecraftUIRenderContext.h"
#include "SDK/InventoryTransaction.h"
#include "SDK/LoopbackPacketSender.h"
#include "SDK/GameSettingsInput.h"
#include "SDK/MoveInputHandler.h"
#include "SDK/ChestBlockActor.h"
#include "SDK/ClientInstance.h"
#include "SDK/MojangsonToken.h"
#include "SDK/RakNetInstance.h"
#include "SDK/HIDController.h"
#include "SDK/BlockLegacy.h"
#include "SDK/MatrixStack.h"
#include "SDK/Tessellator.h"
#include "SDK/EntityList.h"
#include "SDK/TextHolder.h"
#include "SDK/TextHolder.h"
#include "SDK/Attribute.h"
#include "SDK/Inventory.h"
#include "SDK/GameMode.h"
#include "SDK/UIScene.h"
#include "SDK/Weather.h"
#include "SDK/Camera.h"
#include "SDK/Entity.h"
#include "SDK/Packet.h"
#include "SDK/Item.h"
#include "SDK/Tag.h"
#endif 