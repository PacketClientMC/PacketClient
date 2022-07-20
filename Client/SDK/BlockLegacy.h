#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "TextHolder.h"
//#include "Tag.h"

class Material {
public:
	int type;  // 5 for water, 6 for lava
	bool isFlammable;
	bool isNeverBuildable;
	bool isAlwaysDestroyable;
	bool isReplaceable;
	bool isLiquid;  // 0x0008
private:
	char pad2[0x3];  // 0x009
public:
	float translucency;  // 0x00C
	bool isBlockingMotion;
	bool isBlockingPrecipitation;
	bool isSolid;
	bool isSuperHot;
	float color[4];
};

class Entity;
class Block;
class BlockSource;

class BlockLegacy {
public:
	uintptr_t** Vtable;         //0x0000
	class TextHolder tileName;  //0x0008
private:
	char pad_0028[8];  //0x0028
public:
	class TextHolder name;  //0x0030
private:
	char pad_0050[136];  //0x0050
public:
	class Material* material;  //0x00D8
private:
	char pad_00E0[108];  //0x00E0
public:
	short blockId;  //0x014C

	int liquidGetDepth(BlockSource*, const vec3_ti* pos);
	void liquidGetFlow(vec3_t* flowOut, BlockSource*, const vec3_ti* pos);
	bool getCollisionShape(AABB* collShapeOut, Block* block, BlockSource* blockSource, const vec3_ti* pos, Entity* actor);
	bool hasWater(BlockSource*, const vec3_ti& pos);
};

class Block {
public:
	uint8_t data;  // 0x8

private:
	char pad_0x7[0x7]; //0x8

public:
	BlockLegacy* blockLegacy;  // 0x10

	inline BlockLegacy* toLegacy() { return blockLegacy; }

	virtual ~Block();
	virtual int getRenderLayer();
};

class CompoundTag;

class BlockActor {
private:
	virtual void destructor();
	virtual __int64 load(__int64&, CompoundTag*, __int64&);

public:
	virtual bool save(CompoundTag*);
};

class BlockSource {
public:
	Block* getBlock(const vec3_ti& block);

	BlockActor* getBlockEntity(const vec3_ti& block);

	Block* getLiquidBlock(const vec3_ti& block);
};
