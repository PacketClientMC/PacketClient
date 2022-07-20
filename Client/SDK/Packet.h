#pragma once
#include "../Utils/HMath.h"
#include "ComplexInventoryTransaction.h"
#include "Entity.h"
#include "TextHolder.h"

class Packet {
public:
	uintptr_t** vTable; // 0x0000
	TextHolder* getName();
	template <class T>
	bool isInstanceOf() {
		T packet;
		if (packet.vTable == this->vTable)
			return true;
		else
			return false;
	}
};

class LevelSoundEventPacket : public Packet {
private:
	char pad_[0x28]; // 0x8
public:
	int sound; // 0x28
	vec3_t pos; // 0x2C
	int extraData = -1; // 0x38
private:
	int unknown = 0; // 0x3C
public:
	TextHolder entityType; // 0x40
	bool isBabyMod = false; // 0x60
	bool disableRelativeVolume = false; // 0x61
	LevelSoundEventPacket();
};

class PlayerAuthInputPacket : public Packet {
public:
	uint64_t VTable; // 0x0000
	uint64_t num4294967298;
	char padThingy[32];
	// uint64_t entityRuntimeId; //0x0020
	float pitch; // 0x0028
	float yaw;
	vec3_t pos; // 0x0030
	float yawUnused; // 0x0038
	vec3_t velocity;
	float InputAD; // 1 for A, -1 for D, multiply by sqrt(2)/2 if mixed with InputWS
	float InputWS; // 1 for W, -1 for S, multiply by sqrt(2)/2 if mixed with InputAD
	uint8_t epicpad[12];
	uint32_t inputKeys;
	int zero;
	int one;
	int two;
	int counter;

public:
	PlayerAuthInputPacket();
	PlayerAuthInputPacket(vec3_t pos, float pitch, float yaw, float yawUnused);
};

/*class ActorFallPacket : public Packet {
private:
	char pad_0x8[0x28]; //0x8
public:
	ActorFallPacket();
	__int64 runtimeId; // 0x28
	float fallDistance;
	bool isInVoid;
};*/

class PlayerActionPacket : public Packet {
public:
	PlayerActionPacket();
	char pad_0x8[0x28]; // 0x8
	vec3_ti blockPosition; // 0x28
	int face; // 0x34
	int action; // 0x38
	__int64 entityRuntimeId; // 0x40
};

class SubChunkRequestPacket : public Packet {
public:
	SubChunkRequestPacket();
};

class EmotePacket : public Packet {
public:
	EmotePacket();
};

class AnimatePacket : public Packet {
public:
	AnimatePacket();

private:
	char padding[0x28];

public:
	int64_t entityId;
	int64_t action;
	float unknown;
};

class NPCRequestPacket : public Packet {
public:
	NPCRequestPacket();
	long entityRuntimeId; // Test
	__int64 Unknown0; // Test
	std::string Unknown1; // Test
	__int64 Unknown2; // Test
};

class PlayerSkinPacket : public Packet {
public:
	PlayerSkinPacket();

private:
	char padding[0x28];

public:
	__int64 UUID;
	__int64 skin; // meant to be skin class
	std::string skinName;
	std::string oldSkinName;
};

class NetworkLatencyPacket : public Packet {
public:
	NetworkLatencyPacket();
	NetworkLatencyPacket(long timestamp, bool sendback);
	unsigned long timeStamp; // Test
	bool sendBack; // Test
	int pad[0x100]; // 0x0
};

class InteractPacket : public Packet {
public:
	InteractPacket(/**enum InteractPacket::Action, class ActorRuntimeID, vec3_t const&*/);

private:
	char padding[0x28];

public:
	int action;
	long target;
};

class ActorEventPacket : public Packet {
public:
	uint64_t VTable; // 0x0000
	int two; // 0x0008
	int one; // 0x000C
	char pad_0xC[0x20]; // 0x0010
	uint64_t entityRuntimeId; // 0x0030
	char eventId; // 0x0038
	char pad_0x39[0x5]; // 0x0039
	int16_t itemId; // 0x003E
	// ActorEventPacket();
	ActorEventPacket(uint64_t entityRuntimeId, char eventId, int16_t itemId);
};

class MobEquipmentPacket : public Packet {
public:
	MobEquipmentPacket();
	MobEquipmentPacket(__int64 entityRuntimeId, ItemStack& item, int hotbarSlot, int inventorySlot);

private:
	char pad_0x8[0x28]; //0x0
public:
	__int64 entityRuntimeId; //0x28
	ItemStack item;  //0x30
	int inventorySlot; // 0xB8 DONT USE FOR PACKET SENDING
	int hotbarSlot; // 0xBC DONT USE FOR PACKET SENDING
	char windowId; //0xC0 DONT USE FOR PACKET SENDING
	char windowId1; //0xC1 DONT USE FOR PACKET SENDING
	char inventorySlot1; //0xC2
	char hotbarSlot1;  //0xC3
	char windowId2; //0xC4 ALL OF THIS IS PROBABLY BROKEN, DONT USE
private:
	char unknown1;
};
class InventoryTransactionPacket : public Packet {
public:
	InventoryTransactionPacket();
	InventoryTransactionPacket(ComplexInventoryTransaction* transac) : InventoryTransactionPacket() {
		this->complexTransaction = transac;
	}

private:
	char pad_0x8[0x28]; // 0x8
public:
	int unknown = 0; // 0x28
	__int64* unknownStart = 0; // 0x30
	__int64* unknownEnd = 0; // 0x38
	__int64 filler = 0; // 0x40
	ComplexInventoryTransaction* complexTransaction; // 0x48
	unsigned char numTransactions; // 0x50
};

class CommandRequestPacket : public Packet {
public:
	CommandRequestPacket();

	uint64_t VTable; // 0x0000
	int two;
	int one;
	uint64_t zeroes[4];
	TextHolder payload;
	uint64_t thingy[6];
};

class TextPacket : public Packet {
public:
	TextPacket();

	unsigned char gap0[0x28];
	unsigned __int8 messageType; // 0x28

	unsigned char gap[7];

	TextHolder sourceName; // 0x30
	TextHolder message; // 0x50
	unsigned char field_24222[24]; // 0x70
	bool translationNeeded = false; // 0x88

	unsigned char ga2p[7]; // 0x89
	TextHolder xboxUserId;
	TextHolder platformChatId;
};

#pragma pack(push, 8)

__declspec(align(8)) class MovePlayerPacket : public Packet {
public:
	MovePlayerPacket();
	MovePlayerPacket(LocalPlayer* player, vec3_t pos);
	MovePlayerPacket(LocalPlayer* player, vec3_t pos, bool onground);

	// uintptr_t** vTable;		// 0x0
private:
	char filler[0x28]; // 0x8
public:
	__int64 entityRuntimeID; // 0x28
	vec3_t Position; // 0x30
	float pitch; // 0x3c
	float yaw; // 0x40
	float headYaw; // 0x44
	uint8_t mode; // 0x48
	bool onGround;
	__int64 ridingEid;
	int int1;
	int int2;
};

#pragma pack(pop)