#pragma once

#include "../Utils/HMath.h"
#include "EntityList.h"
#include "TextHolder.h"

class ChestBlockActor {
public:
	uintptr_t** vTable;  //0x0000
private:
	char pad_0x0008[0x8];  //0x0008
public:
	uintptr_t* block;  //0x0010
private:
	char pad_0x0018[0x14];  //0x0018
public:
	vec3_ti posI;  //0x002C
	AABB aabb;

	bool isPaired() {
		return *reinterpret_cast<__int64*>(reinterpret_cast<__int64>(this) + 0x220) != 0;  //ptr to paired chest block actor
	}

	vec3_ti* getPairedPos() {
		return reinterpret_cast<vec3_ti*>(reinterpret_cast<__int64>(this) + 0x228);
	}

	bool isMainSubchest() {
		return *reinterpret_cast<unsigned char*>(reinterpret_cast<__int64>(this) + 0x1FC) & 1;  //in paired chest, is it the main one
	}

	AABB getFullAABB() {
		if (!isPaired()) {
			if (isBarrelBlock() || isShulkerBlock())
				return AABB(this->posI.toVector3(), this->posI.toVector3().add(1));
			return AABB(this->posI.toVector3().add(0.0625, 0, 0.0625), this->posI.toVector3().add(1 - 0.0625, 1 - 1.f / 8, 1 - 0.0625));
		}
		vec3_ti first = this->posI;
		vec3_ti second = *getPairedPos();

		if (first.x > second.x || first.z > second.z)
			std::swap(first, second);

		return AABB(first.toVector3().add(0.0625, 0, 0.0625), second.toVector3().add(1 - 0.0625, 1 - 1.f / 8, 1 - 0.0625));
	}

	bool isBarrelBlock();
	bool isShulkerBlock();

	AABB getObstructionAABB();
};
