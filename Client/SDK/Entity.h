#pragma once

#include <bitset>

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "BlockLegacy.h"
#include "EntityList.h"
#include "Inventory.h"
#include "InventoryTransaction.h"
#include "TextHolder.h"

class GameMode;
struct AutomaticID;

class Player;
class Dimension;
struct MobEffectInstance;

#pragma pack(push, 4)
class Entity {
	uint64_t ptrToEntityList; //0x8
private:
	char pad_0x10[0x128]; //0x10
public:
	union {
		struct {
			float pitch; //0x138
			float yaw; //0x13C
		};
		vec2_t viewAngles;
	};
	union {
		struct {
			float previousPitch; //0x140
			float previousYaw; //0x144
		};
		vec2_t viewAngles2;
	};
private:
	char pad_0x148[0x10]; //0x148
public:
	vec3_t eyePos0; //0x158
private:
	char pad_0x164[0x70]; //0x164
public:
	float fallDistance; //0x1D4
	bool onGround; //0x1D8
	bool onGround2; //0x1D9
	bool walkingIntoSomething; //0x1DA
	bool actuallyOnGround; //0x1DB
	bool touchingSomething; //0x1DC
private:
	char pad_0x1DD[0x5B]; //0x1DD
public:
	float stepHeight; //0x238
private:
	char pad_0x23C[0x4]; //0x23C
public:
	float travelDistanceGroundOld; //0x240
	float travelDistanceGround; //0x244
	float travelDistance; //0x248
	vec3_t slowdownFactor; //0x24C
	float weirdMoveKeyTick; //0x258
	bool IsInWater; //0x25C
	bool didEnterWaterBool; //0x25D
	bool hasEnterWaterBool; //0x25E
	bool isSwimming; //0x25F
private:
	char pad_0x260[0x4C]; //0x260
public:
	int32_t ticksAlive; //0x2AC
private:
	char pad_0x2B0[0xA8]; //0x2B0
public:
	class BlockSource* region; //0x358
	class Dimension* dimension; //0x360
	class Level* level; //0x368
private:
	char pad_0x370[0x148]; //0x370
public:
	AABB aabb; //0x4B8
	float width; //0x4D0
	float height; //0x4D4
	vec3_t currentPos; //0x4D8
	vec3_t oldPos; //0x4E4
	vec3_t velocity; //0x4F0
	vec3_t velocity2; //0x4FC
private:
	char pad_0x508[0x40]; //0x508
public:
	int64_t entityRuntimeId; //0x548
private:
	char pad_0x550[0x198]; //0x550
public:
	float bodyYaw; //0x6E8
	float oldBodyYaw; //0x6EC
	float yawUnused1; //0x6F0
	float yawUnused2; //0x6F4
	int32_t damageTime; //0x6F8
	int32_t damageAnimationDuration; //0x6FC
private:
	char pad_0x700[0x19C]; //0x700
public:
	int32_t timeSinceDeath; //0x89C
private:
	char pad_0x8A0[0x108]; //0x8A0
public:
	class TextHolder playerName; //0x9A8
private:
	char pad_0x9C8[0x80]; //0x9C8
public:
	bool isFlying; //0xA48
private:
	char pad_0xA49[0xB]; //0xA49
public:
	bool canFly; //0xA54
private:
	char pad_0xA55[0x713]; //0xA55
public:
	int32_t ticksUsingItem; //0x1168
private:
	char pad_0x116C[0xCCC]; //0x116C
public:
	float glidingTicks; //0x1E38
private:
	char pad_0x1E3C[0xB8]; //0x1E3C
public:
	int gamemode; //0x1EF4
private:
	virtual __int64 getLastHurtByMob(void);
	virtual __int64 setLastHurtByMob(Entity*);
	virtual __int64 getLastHurtByPlayer(void);
	virtual __int64 setLastHurtByPlayer(Entity*);
	virtual __int64 getLastHurtMob(void);
	virtual __int64 setLastHurtMob(Entity*);
	virtual __int64 outOfWorld(void);
	virtual __int64 reloadHardcoded(__int64, __int64 const&);
	virtual __int64 reloadHardcodedClient(__int64, __int64 const&);
	virtual __int64 initializeComponents(__int64, __int64 const&);
	virtual __int64 reloadComponents(__int64, __int64 const&);
	virtual __int64 _serverInitItemStackIds();
	virtual __int64 _doInitialMove();
	virtual bool hasComponent(__int64 const&) const;

private:
	virtual void destructor();
	virtual __int64 reset(void);
	virtual __int64 resetUserPos(bool);
	virtual __int64 getOwnerEntityType(void);
	virtual __int64 remove(void);

public:
	virtual void setPos(vec3_t const&);
	virtual bool isRuntimePredictedMovementEnabled(void);
	virtual vec3_t* getPredictedMovementValues(void) const;
	virtual vec3_t* getPos(void) const;  // Actor::getPosition
	virtual vec3_t* getPosPrev(void) const; // Previously getPosOld
	virtual vec3_t* getPosExtrapolated(float) const;

private:
	virtual __int64 getAttachPos(__int64, float) const;
	virtual __int64 getFiringPos(void) const;

public:
	virtual void setRot(vec2_t const&);
	virtual void move(vec3_t const&);
	virtual void move(class IActorMovementProxy&, vec3_t const&);
	virtual __int64 getInterpolatedRidingPosition(float);
	virtual __int64 getInterpolatedBodyRot(float);
	virtual __int64 getInterpolatedHeadRot(float);
	virtual __int64 getInterpolatedBodyYaw(float);
	virtual __int64 getYawSpeedInDegreesPerSecond(void);
	virtual __int64 getInterpolatedWalkAnimSpeed(float);
	virtual __int64 getInterpolatedRidingOffset(float);
	virtual void updateEntityInside(AABB const&);
	virtual void updateEntityInside();
	virtual bool isFireImmune();
	virtual bool breaksFallingBlocks(void) const;
	virtual bool blockedByShield(__int64 const&, Entity&);
	virtual bool canDisableShield(void);
	virtual void teleportTo(vec3_t const&, bool, int, int);
	virtual void tryTeleportTo(vec3_t const&, bool, bool, int, int);
	virtual void chorusFruitTeleport(vec3_t&);
	virtual void lerpMotion(vec3_t const&);

private:
	virtual __int64 tryCreateAddActorPacket(void);
	virtual __int64 normalTick(void);
	virtual __int64 baseTick(void);
	virtual __int64 passengerTick(void);
	virtual __int64 positionPassenger(Entity&, float);
	virtual __int64 startRiding(Entity&);
	virtual __int64 addPassenger(Entity&);
	virtual __int64 flagPassengerToRemove(Entity&);
	virtual __int64 getExitTip(std::string const&, __int64) const;
	virtual __int64 intersects(vec3_t const&, vec3_t const&) const;

public:
	virtual bool isInWall(void) const;
	virtual bool isInvisible(void) const;
	virtual bool canShowNameTag(void) const;
	virtual bool canExistInPeaceful(void) const;
	virtual void setNameTagVisible(bool);
	virtual TextHolder* getNameTag(void) const;

private:
	virtual __int64 getNameTagAsHash(void) const;

public:
	virtual TextHolder* getFormattedNameTag(void) const;

private:
	virtual __int64 filterFormattedNameTag(__int64 const&);

public:
	virtual void setNameTag(std::string const&);

private:
	virtual __int64 getAlwaysShowNameTag(void) const;

public:
	virtual void setScoreTag(std::string const&);
	virtual TextHolder* getScoreTag(void) const;

public:
	virtual bool isInWater(void) const;
	virtual bool hasEnteredWater(void) const;
	virtual bool isInLava(void) const;
	virtual bool isUnderLiquid(__int64) const;
	virtual bool isOverWater(void) const;

private:
	virtual __int64 setBlockMovementSlowdownMultiplier(BlockLegacy const&, vec3_t const&);
	virtual __int64 resetBlockMovementSlowdownMultiplier();
	virtual __int64 getCameraOffset(void) const;
	virtual __int64 getShadowHeightOffs(void);
	virtual __int64 getShadowRadius(void) const;
	virtual __int64 getHeadLookVector(float);

public:
	virtual bool canSeeInvisible(void) const;
	virtual bool canSee(Entity const&) const;
	virtual bool canSee(vec3_t const&) const;
	virtual __int64 canInteractWithOtherEntitiesInGame(void);
	virtual bool isSkyLit(float);

private:
	virtual __int64 getBrightness(float) const;
	virtual __int64 interactPreventDefault(void);
	virtual __int64 playerTouch(Player&);
	virtual __int64 onAboveBubbleColumn(bool);
	virtual __int64 onInsideBubbleColumn(bool);

public:
	virtual bool isImmobile(void);
	virtual bool isSilent(void);
	virtual bool isPickable(void);
	virtual bool isFishable(void);
	virtual bool isSleeping(void);
	virtual bool setSleeping(bool);
	virtual bool isShootable(void);
	virtual bool setSneaking(bool);
	virtual bool isBlocking(void);
	virtual bool isDamageBlocked(__int64 const&);
	virtual bool isAlive(void);
	virtual bool isOnFire(void);
	virtual bool isOnHotBlock(void);
	virtual bool isCreativeModeAllowed(void);
	virtual bool isSurfaceMob(void);
	virtual bool isTargetable(void);
	virtual bool isLocalPlayer(void);
	virtual bool isPlayer(void);
	virtual bool isAffectedByWaterBottle(void) const;
	virtual bool canAttack(Entity*, bool) const;
	virtual void setTarget(Entity*);

private:
	virtual __int64 findAttackTarget(void);

public:
	virtual bool isValidTarget(Entity*) const;

private:
	virtual __int64 attack(Entity&, class ActorDamageCause const&) const;
	virtual void performRangedAttack(Entity&, float);
	virtual __int64 getEquipmentCount(void) const;

public:
	virtual void setOwner(__int64);
	virtual void setSitting(bool);

private:
	virtual __int64 onTame(void);
	virtual __int64 onFailedTame(void);
	virtual __int64 getInventorySize(void) const;
	virtual __int64 getEquipSlots(void) const;
	virtual __int64 getChestSlots(void) const;

public:
	virtual void setStanding(bool);
	virtual bool canPowerJump(void) const;
	virtual void setCanPowerJump(bool);
	virtual bool isJumping(void) const;
	virtual bool isEnchanted(void) const;

private:
	virtual __int64 vehicleLanded(vec3_t const&, vec3_t const&);
	virtual __int64 shouldRender(void) const;
	virtual __int64 playAmbientSound(void);
	virtual __int64 getAmbientSound(void);

public:
	virtual bool isInvulnerableTo(__int64 const&) const;

public:
	virtual __int64 getBlockDamageCause();
	virtual __int64 animateHurt(void);
	virtual __int64 doFireHurt(int);
	virtual __int64 onLightningHit(void);
	virtual __int64 onBounceStarted(vec3_ti const&, __int64 const&);
	virtual __int64 feed(int);
	virtual __int64 handleEntityEvent(__int64, int);
	virtual __int64 getPickRadius(void);
	virtual const class StringHasher* getActorRendererId(void);
	virtual __int64 spawnAtLocation(int, int);
	virtual __int64 spawnAtLocation(int, int, float);
	virtual __int64 spawnAtLocation(__int64 const&, int);
	virtual __int64 spawnAtLocation(__int64 const&, int, float);
	virtual __int64 spawnAtLocation(__int64 const&, float);
	virtual __int64 despawn(void);
	virtual __int64 killed(Entity&);
	virtual __int64 awardKillScore(Entity&, int);

public:
	virtual void setArmor(int, ItemStack const&);
	virtual ItemStack* getArmor(int) const;

private:
	virtual __int64 getAllArmor(void) const;
	virtual __int64 getArmorMaterialTypeInSlot(__int64) const;
	virtual __int64 getArmorMaterialTextureTypeInSlot(__int64) const;
	virtual __int64 getArmorColorInSlot(__int64, int) const;

public:
	virtual void getEquippedSlot(__int64);
	virtual void setEquippedSlot(__int64, __int64 const&);
	virtual void setCarriedItem(__int64 const&);
	virtual __int64 getCarriedItem(void) const;
	virtual void setOffhandSlot(ItemStack*);
	virtual ItemStack* getEquippedTotem(void) const;
	virtual __int64 consumeTotem(void);
	virtual __int64 save(CompoundTag*);
	virtual __int64 saveWithoutId(CompoundTag*);

private:
	virtual __int64 load(__int64 const&, __int64&);
	virtual __int64 loadLinks(__int64 const&, __int64);

public:
	virtual uint8_t getEntityTypeId(void) const;

private:
	virtual __int64 queryEntityRenderer(void) const;
	virtual __int64 getSourceUniqueID(void) const;

public:
	virtual __int64 thawFreezeEffect(void);
	virtual bool canFreeze(void);
	virtual bool isWearingLeatherArmor(void);

private:
	virtual __int64 getLiquidAABB(__int64);
	virtual __int64 handleInsidePortal(vec3_ti const&);
	virtual __int64 getPortalCooldown(void) const;
	virtual __int64 getPortalWaitTime(void) const;

public:
	virtual int* getDimensionId(int* dimOut) const;

	virtual bool canChangeDimensions(void) const;

private:
	virtual __int64 changeDimension(__int64, bool);
	virtual __int64 changeDimension(__int64 const&);
	virtual __int64 getControllingPlayer(void) const;  // getSourceUniqueID2
	virtual __int64 checkFallDamage(float, bool);

public:
	virtual void causeFallDamage(float);
	virtual void handleFallDistanceOnServer(float, bool);

private:
	virtual __int64 playSynchronizedSound(__int64, vec3_t const&, __int64 const&, bool);
	virtual __int64 playSynchronizedSound(__int64, vec3_t const&, int, bool);
	virtual __int64 onSynchedFlagUpdate(int, __int64, __int64);
	virtual __int64 onSynchedDataUpdate(int);

public:
	virtual bool canAddRider(Entity&) const;
	virtual bool canPickupItem(ItemStack const&);
	virtual bool canBePulledIntoVehicle(void) const;

private:
	virtual __int64 inCaravan(void) const;

public:
	virtual bool isLeashableType(void);

private:
	virtual __int64 tickLeash(void);
	virtual __int64 sendMotionPacketIfNeeded(void);

public:
	virtual bool canSynchronizeNewEntity(void) const;

private:
	virtual __int64 stopRiding(bool, bool, bool);

public:
	virtual void startSwimming(void);
	virtual void stopSwimming(void);

private:
	virtual __int64 buildDebugInfo(std::string&) const;
	virtual __int64 getCommandPermissionLevel(void) const;

public:
	virtual bool isClientSide(void);
	virtual class AttributeInstance* getMutableAttribute(class Attribute* Attribute);
	virtual __int64 getAttribute(int*) const;
	virtual __int64 getDeathTime(void) const;
	virtual __int64 heal(int);
	virtual bool isInvertedHealAndHarm(void) const;
	virtual bool canBeAffected(int) const;
	virtual bool canBeAffected(class ActorEffectInstance const&) const;
	virtual bool canBeAffectedByArrow(__int64 const&) const;

private:
	virtual __int64 onEffectAdded(__int64&);
	virtual __int64 onEffectUpdated(__int64 const&);
	virtual __int64 onEffectRemoved(__int64&);
	virtual __int64 getAnimationComponent(void);
	virtual __int64 openContainerComponent(Player&);

public:
	virtual __int64 swing(void);

private:
	virtual __int64 useItem(__int64&, __int64, bool);

public:
	virtual bool hasOutputSignal(unsigned char) const;

private:
	virtual __int64 getOutputSignal(void) const;
	virtual __int64 getDebugText(__int64&);
	virtual __int64 getMapDecorationRotation(void) const;
	virtual __int64 getPassengerYRotation(Entity const&) const;
	virtual __int64 getYHeadRot(void) const;
	virtual __int64 setYHeadRot(float);
	virtual __int64 getYHeadRotO(void);

public:
	virtual bool isWorldBuilder(void);
	virtual bool isCreative(void) const;
	virtual bool isAdventure(void) const;
	virtual bool isSurvival(void);
	virtual bool isSpectator(void);
	virtual bool isAttackableGamemode(void);

private:
	virtual __int64 add(__int64&);
	virtual __int64 drop(__int64 const&, bool);
	virtual __int64 getInteraction(Player&, __int64&, vec3_t const&);

public:
	virtual bool canDestroyBlock(__int64 const&) const;
	virtual void setAuxValue(int);
	virtual void setSize(float, float);

private:
	virtual __int64 onOrphan(void);
	virtual __int64 wobble(void);
	virtual __int64 wasHurt(void);

public:
	virtual __int64 startSpinAttack(void);
	virtual __int64 stopSpinAttack(void);

public:
	virtual void setDamageNearbyMobs(bool);

private:
	virtual __int64 renderDebugServerState(class Option const&);
	virtual __int64 reloadLootTable(void);
	virtual __int64 reloadLootTable(__int64 const*);
	virtual __int64 getDeletionDelayTimeSeconds(void);
	virtual __int64 kill(void);
	virtual __int64 die(__int64 const&);
	virtual __int64 shouldDropDeathLoot(void);
	virtual __int64 shouldTick(void) const;
	virtual __int64 extractSnapshot(__int64&);
	virtual __int64 applySnapshot(__int64 const&, __int64 const&);
	virtual __int64 getNextStep(float) const;
	virtual __int64 getLootTable(void);
	virtual __int64 interpolatorTick(void);
	virtual __int64 onPush(Entity&);
	virtual __int64 getLastDeathPos(void);
	virtual __int64 getLastDeathDimension(void);
	virtual __int64 hasDiedBefore(void);
	virtual __int64 updateEntitySpecificMolangVariables(__int64&);

public:
	virtual bool shouldTryMakeStepSound(void) const;

private:
	virtual bool canMakeStepSound(void) const;
	virtual __int64 _hurt(class ActorDamageSource const&, float, bool, bool);
	virtual __int64 markHurt(void);
	virtual __int64 _getAnimationComponent(std::shared_ptr<__int64>&, __int64);
	virtual __int64 readAdditionalSaveData(__int64 const&, __int64&);
	virtual __int64 addAdditionalSaveData(__int64&);
	virtual __int64 _playStepSound(vec3_ti const&, __int64 const&);
	virtual __int64 _playFlySound(vec3_ti const&, __int64 const&);
	virtual __int64 _makeFlySound(void);
	virtual __int64 checkInsideBlocks(float);
	virtual __int64 pushOutOfBlocks(vec3_t const&);
	virtual __int64 updateWaterState(void);

public:
	virtual __int64 doWaterSplashEffect(void);

private:
	virtual __int64 spawnTrailBubbles(void);
	virtual __int64 updateInsideBlock(void);
	virtual __int64 _removePassenger(uint64_t const&, bool, bool, bool);
	virtual __int64 _onSizeUpdated(void);
	virtual __int64 _doAutoAttackOnTouch(Entity&);
	virtual __int64 knockback(Entity*, int, float, float, float, float, float);
	virtual __int64 spawnAnim(void);

public:
	virtual void setSprinting(bool);

private:
	virtual __int64 getHurtSound(void);
	virtual __int64 getDeathSound(void);
	virtual __int64 getItemInHandIcon(ItemStack const&, int);

public:
	virtual __int64 getSpeed(void);
	virtual __int64 setSpeed(float);

private:
	virtual __int64 getJumpPower(void) const;
	virtual __int64 hurtEffects(ActorDamageSource const&, float, bool, bool);
	virtual __int64 getMeleeWeaponDamageBonus(__int64*);
	virtual __int64 getMeleeKnockbackBonus(void);
	virtual __int64 travel(float, float, float);
	virtual __int64 travel(IActorMovementProxy&, float, float, float);
	virtual __int64 applyFinalFriction(float, bool);
	virtual __int64 aiStep(void);
	virtual __int64 aiStep(IActorMovementProxy&);
	virtual __int64 pushActors(void);
	virtual __int64 lookAt(Entity*, float, float);

public:
	virtual bool isLookingAtAnEntity(void);

private:
	virtual __int64 checkSpawnRules(bool);
	virtual __int64 checkSpawnObstruction(void) const;
	virtual __int64 getAttackAnim(float);
	virtual __int64 getItemUseDuration(void);
	virtual __int64 getItemUseStartupProgress(void);
	virtual __int64 getItemUseIntervalProgress(void);
	virtual __int64 getItemUseIntervalAxis(void);
	virtual __int64 getTimeAlongSwing(void);
	virtual __int64 ate(void);
	virtual __int64 getMaxHeadXRot(void);

public:
	virtual bool isAlliedTo(Entity*);

private:
	virtual __int64 doHurtTarget(Entity*, ActorDamageCause const&);

public:
	virtual bool canBeControlledBPassenger(void);

private:
	virtual __int64 leaveCaravan(void);
	virtual __int64 joinCaravan(__int64*);

public:
	virtual bool hasCaravanTail(void);

private:
	virtual __int64 getCaravanHead(void);
	virtual __int64 getArmorValue(void);
	virtual __int64 getArmorCoverPercentage(void);
	virtual __int64 getToughnessValue(void);
	virtual __int64 hurtArmorSlots(int, __int64);
	virtual __int64 setDamagedArmor(ArmorItem, ItemStack const&);
	virtual __int64 sendArmorDamage(__int64 const&);
	virtual __int64 sendArmor(void);
	virtual __int64 containerChanged(int);
	virtual __int64 updateEquipment(void);
	virtual __int64 clearEquipment(void);
	virtual __int64 getAllArmorID(void) const;
	virtual __int64 getAllHand(void) const;
	virtual __int64 getAllEquipment(void) const;
	virtual __int64 getArmorTypeHash(void);
	virtual __int64 dropEquipmentOnDeath(ActorDamageSource const&, int);
	virtual __int64 dropEquipmentOnDeath(void);
	virtual __int64 clearVanishEnchantedItemsOnDeath(void);
	virtual __int64 sendInventory(bool);
	virtual __int64 getDamageAfterEnchantReduction(ActorDamageSource const&, float);
	virtual __int64 getDamageAfterMagicAbsorb(ActorDamageSource const&, int);
	virtual __int64 getDamageAfterResistanceEffect(ActorDamageSource const&, float);
	virtual __int64 createAIGoals(void);
	virtual __int64 onBorn(Entity&, Entity&);
	virtual void setItemSlot(__int64, ItemStack const&);

public:
	virtual void setTransitioningSitting(bool);

private:
	virtual __int64 attackAnimation(Entity*, float);
	virtual __int64 getAttackTime(void);
	virtual __int64 _getWalkTargetValue(vec3_ti const&);

public:
	virtual bool canExistWhenDisallowEntity(void) const;

private:
	virtual __int64 useNewAi(void);
	virtual __int64 ascendLadder(void);
	virtual __int64 ascendBlockByJumping(void);
	virtual __int64 descendBlockByCrouching(void);
	virtual __int64 dropContainer(void);
	virtual __int64 initBodyControl(void);
	virtual __int64 jumpFromGround0(__int64);

public:
	virtual void jumpFromGround(void);

private:
	virtual __int64 updateAi(void);
	virtual __int64 newServerAiStep(void);
	virtual __int64 _serverAiEntityStep(void);
	virtual __int64 dropBags(void);
	virtual __int64 tickDeath(void);
	virtual __int64 updateGliding(void);
	virtual __int64 _allowAscendingScaffolding(void);
	virtual __int64 _getAdjustedAABBForSpawnCheck(AABB const&, vec3_t const&);

public:
	InventoryTransactionManager* getTransactionManager();

	AABB* getAABB() {
		return &this->aabb;
	}

	void lerpybutworky(vec3_t motion) {
		using lerpMotion2 = void(__thiscall*)(void*, vec3_t);
		static lerpMotion2 lerpy = reinterpret_cast<lerpMotion2>(FindSignature("8B 02 89 81 ? ? ? ? 8B 42 ? 89 81 ? ? ? ? 8B 42 ? 89 81 ? ? ? ? C3"));
		lerpy(this, motion);
	}

	__int64* getUniqueId() {
		__int64* v1;  // rbx
		char v3;      // [rsp+30h] [rbp+8h]

		v1 = (__int64*)((char*)this + 256);
		if (*((__int64*)this + 32) == -1i64)
			*v1 = *(__int64*)(*(__int64(__fastcall**)(__int64, char*))(**((__int64**)this + 110) + 1960i64))(
				*((__int64*)this + 110),
				&v3);
		return (__int64*)v1;
	}

	float getRealSpeed() {
		return *reinterpret_cast<float*>(this->getSpeed() + 0x84);
	}

	float getTicksPerSecond() {
		if (this != nullptr) {
			vec3_t targetPos = *this->getPos();
			vec3_t targetPosOld = *this->getPosPrev();
			float hVel = sqrtf(((targetPos.x - targetPosOld.x) * (targetPos.x - targetPosOld.x)) + ((targetPos.z - targetPosOld.z) * (targetPos.z - targetPosOld.z)));
			return hVel;
		}
	}

	float getBlocksPerSecond();

	int getTicksUsingItem() {
		return *reinterpret_cast<int*>(reinterpret_cast<__int64>(this) + 0xFF8);
	}

	bool isSneaking() {
		return false;  // TODO
	}

	bool isSprinting() {
		return false;  // TODO
	}

	Dimension* getDimension() {
		return *reinterpret_cast<class Dimension**>(reinterpret_cast<__int64>(this) + 0x360);
	}

	void SetFieldOfView(float num) {
		*(float*)((uintptr_t)(this) + 0x1040) = num;
	}

	float GetFieldOfView() {
		return *reinterpret_cast<float*>(this + 0x1040);
	}

	class Level* getPointingStruct() {
		return *reinterpret_cast<class Level**>(reinterpret_cast<__int64>(this) + 0x368);
	}

	void lerpTo(vec3_t const& pos, vec2_t const& joe, int wow) {
		using lerpTo = void(__fastcall*)(Entity*, vec3_t const&, vec2_t const&, int);
		static lerpTo lerp = reinterpret_cast<lerpTo>(Utils::FindSignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 49 8B F8 48 8B 89 ? ? ? ? 48 85 C9 74 ? 48 8B 01 48 8B 5C 24"));
		lerp(this, pos, joe, wow);
	}

	void spawnDustParticles(int intensity) {
		using spawnDustParticles = void(__fastcall*)(Entity*, int);
		static spawnDustParticles spawnParticle = reinterpret_cast<spawnDustParticles>(Utils::FindSignature("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 8B EA 48 8B F1"));
		spawnParticle(this, intensity);
	}

	float getHealth() {
		static int* healthAttribute = 0x0;

		if (healthAttribute == 0x0) {
			uintptr_t sigOffset = FindSignature("48 8D 15 ?? ?? ?? ?? FF 90 ?? ?? ?? ?? F3 ?? ?? 88 ?? ?? ?? ?? 85 C9 7E ??");
			healthAttribute = (int*)(sigOffset + (*(int*)(sigOffset + 3)) + 7);
		}

		return *(float*)(getAttribute(healthAttribute) + 0x84);
	}
};
#pragma pack(pop)

class ServerPlayer;

class Player : public Entity {
public:
	PlayerInventoryProxy* getSupplies();

	ItemStack* getSelectedItem() {
		auto supplies = this->getSupplies();
		return supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
	}

	int getSelectedItemId() {
		auto item = getSelectedItem();
		if (item == nullptr || item->item == nullptr)
			return 0;
		if (item->getItem() == nullptr)
			return 0;
		return item->getItem()->itemId;
	}

	virtual __int64 prepareRegion(__int64&);
	virtual __int64 destroyRegion(void);
	virtual __int64 suspendRegion(void);
	virtual __int64 resendAllChunks(void);
	virtual __int64 _fireWillChangeDimension(void);
	virtual __int64 _fireDimensionChanged(void);
	virtual __int64 changeDimensionWithCredits(__int64);
	virtual __int64 tickWorld(__int64 const&);
	virtual __int64 nullsub_0(void);
	virtual __int64 getTickingOffsets(void);
	virtual __int64 moveView(void);
	virtual void setName(std::string const&);
	virtual __int64 getTravelledMethod(void);
	virtual __int64 checkMovementStats(vec3_t const&);
	virtual __int64 getCurrentStructureFeature(void);
	virtual bool isAutoJumpEnabled(void);
	virtual void respawn(void);
	virtual void resetRot(void);
	virtual bool isInTrialMode(void);
	virtual bool hasResource(int);
	virtual void completeUsingItem(void);
	virtual __int64 startDestroying(void);
	virtual __int64 stopDestroying(void);
	virtual void openPortfolio(void);
	virtual void openBook(int, bool, int, BlockActor*);
	virtual void openTrading(uint64_t const&, bool);
	virtual bool canOpenContainerScreen(void);
	virtual void openChalkboard(BlockActor*, bool);
	virtual void openNpcInteractScreen(__int64);
	virtual void openInventory(void);
	virtual __int64 displayChatMessage(std::string const&, std::string const&);
	virtual __int64 displayClientMessage(std::string const&);
	virtual __int64 displayTextObjectMessage(std::string const&, std::string const&, std::string const&);
	virtual __int64 displayTextObjectWhisperMessage(std::string const&, std::string const&, std::string const&);
	virtual __int64 displayWhisperMessage(std::string const&, std::string const&, std::string const&, std::string const&);
	virtual __int64 startSleepInBed(vec3_ti const&);
	virtual __int64 stopSleepInBed(bool, bool);
	virtual bool canStartSleepInBed(void);
	virtual __int64 getSleepTimer(void);
	virtual __int64 getPreviousTickSleepTimer(void);
	virtual __int64 openSign(vec3_ti const&);
	virtual void playEmote(std::string const&);
	virtual bool isHostingPlayer(void);
	virtual bool isLoading(void);
	virtual bool isPlayerInitialized(void);
	virtual __int64 stopLoading(void);
	virtual __int64 registerTrackedBoss(uint64_t);
	virtual __int64 unRegisterTrackedBoss(uint64_t);
	virtual void setPlayerGameType(int);
	virtual __int64 initHUDContainerManager(void);
	virtual __int64 _crit(Entity&);
	virtual __int64 getEventing(void);
	virtual __int64 getUserId(void);
	virtual __int64 sendEventPacket(class Packet&);
	virtual void addExperience(int);
	virtual void addLevels(int);
	virtual __int64 nullsub_1(void);
	virtual __int64 nullsub_2(void);
	virtual __int64 inventoryChanged(class Container&, int, ItemStack const&, ItemStack const&, bool);
	virtual __int64 nullsub_3(void);
	virtual __int64 deleteContainerManager(void);
	virtual void setFieldOfViewModifier(float);
	virtual __int64 is2DPositionRelevant(__int64, vec3_ti const&);
	virtual __int64 isActorRelevant(Entity const&);
	virtual __int64 nullsub_4(void);
	virtual __int64 onSuspension(void);
	virtual __int64 onLinkedSlotsChanged(void);
	virtual __int64 startCooldown(Item const*, bool);
	virtual __int64 getItemCooldownLeft(HashedString const&);
	virtual __int64 getItemCooldownLeft(__int64);
	virtual __int64 getMaxItemCooldownLeft(void);
	virtual bool isItemOnCooldown(HashedString const&);
	virtual __int64 nullsub_5(void);
	virtual __int64 nullsub_6(void);
	virtual void sendNetworkPacket(class Packet&);
	virtual __int64 nullsub_7(void);
	virtual __int64 nullsub_8(void);
	virtual __int64 nullsub_9(void);
	virtual __int64 nullsub_10(void);
	virtual __int64 reportMovementTelemetry(__int64);
	virtual bool isSimulated(void);
	virtual __int64 getXuid(void);
	virtual __int64 getMovementSettings(void);
	virtual __int64 onMovePlayerPacketNormal(vec3_t const&, vec2_t const&, float);
	virtual __int64 _createChunkSource(__int64&);

	void enchantItem(ItemStack* item, int id, int level) {
		using getEnchantsFromUserData_t = void(__fastcall*)(ItemStack*, void*);
		using addEnchant_t = bool(__fastcall*)(void*, __int64);
		using saveEnchantsToUserData_t = void(__fastcall*)(ItemStack*, void*);
		static getEnchantsFromUserData_t getEnchantsFromUserData = reinterpret_cast<getEnchantsFromUserData_t>(FindSignature("48 89 5C 24 ? 55 56 57 48 81 EC ? ? ? ? 48 8B F2 48 8B D9 48 89 54 24 ? 33 FF 89 7C 24 ? E8 ? ? ? ? 84 C0"));
		static addEnchant_t addEnchant = reinterpret_cast<addEnchant_t>(FindSignature("48 89 5C 24 ?? 48 89 54 24 ?? 57 48 83 EC ?? 45 0F"));
		static saveEnchantsToUserData_t saveEnchantsToUserData = 0x0;

		if (!saveEnchantsToUserData) saveEnchantsToUserData = reinterpret_cast<saveEnchantsToUserData_t>(FindSignature("48 89 5C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B FA 4C 8B C1 48 8B 41 08 48 85 C0"));
		void* EnchantData = malloc(0x60);
		if (EnchantData != nullptr) memset(EnchantData, 0x0, 0x60);
		getEnchantsFromUserData(item, EnchantData);
		__int64 enchantPair = ((__int64)level << 32) | id;
		if (addEnchant(EnchantData, enchantPair))  // Upper 4 bytes = level, lower 4 bytes = enchant type
			saveEnchantsToUserData(item, EnchantData);
		free(EnchantData);
	}
};

class ServerPlayer : public Player {
};

class LocalPlayer : public Player {
public:
	void unlockAchievements();

	void localPlayerTurn(vec2_t* viewAngles) {
		using Turn = void(__thiscall*)(void*, vec2_t*);
		static Turn TurnFunc = reinterpret_cast<Turn>(FindSignature("48 8B 58 68 48 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 89 ?? ?? ?? ?? ?? ?? ?? 48 8B 03 48 8B CB FF 50 08 90 48 85 DB ?? 09 48 8B ?? 48 8B CB ?? ?? ?? ?? ?? ?? ?? 48 8B D3 48 8B CE E8 0D BB 1D FF 90 48 85 DB 74 09 48 8B 03 48 8B ?? ?? ?? ?? 48 83 C7 10"));
		TurnFunc(this, viewAngles);
	}

	void applyTurnDelta(vec2_t* viewAngleDelta);
	void setGameModeType(int gma);

	auto getSwingState() {
		static unsigned int offset = *reinterpret_cast<int*>(FindSignature("80 BB ? ? ? ? 00 74 1A FF 83") + 2);
		return reinterpret_cast<float*>((uintptr_t)(this) + offset);
	};
};