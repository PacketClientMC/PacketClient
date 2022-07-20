#include "BlockLegacy.h"

#include "../Memory/GameData.h"
#include "../Utils/Logger.h"
#include "../Utils/Utils.h"

Block* BlockSource::getBlock(const vec3_ti& block) {
	using getBlock_t = Block*(__fastcall*)(BlockSource*, const vec3_ti&);
	static getBlock_t getBlock = reinterpret_cast<getBlock_t>(FindSignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 48 8B DA 8B 4A"));
	return getBlock(this, block);
}

BlockActor* BlockSource::getBlockEntity(const vec3_ti& block) {
	using getBlockEntity_t = BlockActor*(__fastcall*)(BlockSource*, const vec3_ti&);
	static getBlockEntity_t getBlockEntity = reinterpret_cast<getBlockEntity_t>(FindSignature("40 55 56 57 48 83 EC ?? 8B 02"));
	return getBlockEntity(this, block);
}

Block* BlockSource::getLiquidBlock(const vec3_ti& block) {
	using getLiquidBlock_t = Block*(__fastcall*)(BlockSource*, const vec3_ti&);
	static getLiquidBlock_t getLiquidBlock = Utils::FuncFromSigOffset<getLiquidBlock_t>(FindSignature("E8 ?? ?? ?? ?? 65 48 8B 0C 25 ?? ?? ?? ??"), 1);
	return getLiquidBlock(this, block);
}

bool BlockLegacy::getCollisionShape(AABB* collShapeOut, Block* block, BlockSource* blockSource, const vec3_ti* pos, Entity* actor) {
	return Utils::CallVFunc<5, bool, AABB*, Block*, BlockSource*, const vec3_ti*, Entity*>(this, collShapeOut, block, blockSource, pos, 0);
}

void BlockLegacy::liquidGetFlow(vec3_t* flowOut, BlockSource* reg, const vec3_ti* pos) {
	using liquid_getFlow_t = int(__fastcall*)(BlockLegacy*, vec3_t*, BlockSource*, const vec3_ti*);
	static liquid_getFlow_t liquid_getDepth = Utils::FuncFromSigOffset<liquid_getFlow_t>(FindSignature("E8 ? ? ? ? 48 8B 4C 24 ? 41 BF"), 1);

	liquid_getDepth(this, flowOut, reg, pos);
}

bool BlockLegacy::hasWater(BlockSource* reg, const vec3_ti& pos) {
	if (this->material->isLiquid) {
		return !this->material->isSuperHot;
	}

	auto liquidBlock = reg->getLiquidBlock(pos)->toLegacy();
	return this != liquidBlock && liquidBlock->material->isLiquid && !liquidBlock->material->isSuperHot;
}
