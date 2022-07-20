#pragma once

#include <bitset>

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "BlockLegacy.h"
#include "EntityList.h"
#include "Inventory.h"
#include "InventoryTransaction.h"
#include "TextHolder.h"

class Level {
private:
	char firstpad[0x688];  // 0x8
public:
	int levelTicks;  // 0x5F0
private:
	char secondpad[0x41C];  // 0x5F4
public:
	int rayHitType;    // 0x0A28
	int blockSide;     // 0x0A2C
	vec3_ti block;     // 0x0A30
	vec3_t rayHitVec;  // 0x0A3C
private:
	Entity* entityPtr;          // 0x0A48
	Entity* entityPtr2;         // 0x0A50
	uint64_t GamingEntityFinder;  // 0x0A58

public:
	virtual void initialize(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, class LevelSettings const&, class LevelData*, class Experiments const&, std::basic_string<char, std::char_traits<char>, std::allocator<char>> const*);
	virtual void postProcessResources(void);
	virtual void startLeaveGame(void);
	virtual bool isLeaveGameDone(void);
	virtual void createDimension(uintptr_t);
	virtual int getDimension(uintptr_t);
	virtual void forEachDimension(uintptr_t);
	virtual void forEachDimension2(uintptr_t);
	virtual int getChunkTickRange(void);
	virtual int getPortalForcer(void);
	virtual void requestPlayerChangeDimension(Entity&, std::unique_ptr<class ChangeDimensionRequest, std::default_delete<class ChangeDimensionRequest>>);
	virtual void entityChangeDimension(Entity&, uintptr_t, uintptr_t);
	virtual int getSpawner(void);
	virtual int getProjectileFactory(void);
	virtual int getEntityDefinitions(void);
	virtual int getActorAnimationGroup(void);
	virtual int getActorAnimationControllerGroup(void);
	virtual int getBlockDefinitions(void);
	virtual int getBlockComponentFactory(void);
	virtual int getBlockComponentFactory2(void);
	virtual int getActorPropertyGroup(void);
	virtual int getSpawnRules(void);
	virtual int getSpawnGroupRegistry(void);
	virtual int getSpawnRulesMutable(void);
	virtual int getSpawnSettings(void);
	virtual void setSpawnSettings(class SpawnSettings const&);
	virtual int getBehaviorTreeGroup(void);
	virtual int getBehaviorFactory(void);
	virtual int getDifficulty(void);
	virtual int getInternalComponentRegistry(void);
	virtual int getDimensionConversionData(void);
	virtual int getSpecialMultiplier(uintptr_t);
	virtual void hasCommandsEnabled(void);
	virtual void useMsaGamertagsOnly(void);
	virtual void setMsaGamertagsOnly(bool);
	virtual void registerEntity(uintptr_t);
	virtual void addEntity(BlockSource&, uintptr_t);
	virtual void addGlobalEntity(BlockSource&, uintptr_t);
	virtual void addAutonomousEntity(BlockSource&, uintptr_t);
	virtual void addUser(uintptr_t);
	virtual void addDisplayEntity(BlockSource&, uintptr_t);
	virtual void removeDisplayEntity(uintptr_t);
	virtual void suspendPlayer(Entity&);
	virtual void resumePlayer(Entity&);
	virtual bool isPlayerSuspended(Entity&);
	virtual void removeActorAndTakeEntity(class WeakEntityRef);
	virtual void takeEntity(WeakEntityRef, class LevelChunk*);
	virtual void fetchEntity(uint64_t, bool);
	virtual int getRuntimeEntity(class ActorRuntimeID, bool);
	virtual int getMob(uint64_t);
	virtual int getPlayer(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&);
	virtual int getPlayer(uintptr_t);
	virtual int getPlayerByXuid(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&);
	virtual int getPlayerFromUnknownIdentifier(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&);
	virtual int getPlayer2(uint64_t);
	virtual int getPlatformPlayer(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&);
	virtual int getPlayerFromServerId(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&);
	virtual int getRuntimePlayer(ActorRuntimeID);
	virtual int getNumRemotePlayers(void);
	virtual int getPrimaryLocalPlayer(void);
	virtual int getRandomPlayer(void);
	virtual int getNewPlayerId(void);
	virtual int getEventing(void);
	virtual int getPlayerMC_Color(Entity const&);
	virtual int getCurrentTick(void);
	virtual int getCurrentServerTick(void);
	virtual void removeAllNonPlayerEntities(uint64_t);
	virtual int getBiomeRegistry(void);
	virtual int getBiomeRegistry2(void);
	virtual int getBlockPalette(void);
	virtual int getBlockPalette2(void);
	virtual int getFeatureRegistry(void);
	virtual int getFeatureRegistry2(void);
	virtual int getFeatureTypeFactory(void);
	virtual int getFeatureTypeFactory2(void);
	virtual int getJigsawStructureRegistry(void);
	virtual int getJigsawStructureRegistry2(void);
	virtual int getStructureManager(void);
	virtual int getStructureManager2(void);
	virtual int getBiomeComponentFactory(void);
	virtual int getBiomeComponentFactory2(void);
	virtual int getSurfaceBuilderRegistry(void);
	virtual int getSurfaceBuilderRegistry2(void);
	virtual int getDimensionFactory(void);
	virtual int getDimensionFactory2(void);
	virtual int getLightTextureImageBuilderFactory(void);
	virtual int getLightTextureImageBuilderFactory2(void);
	virtual void addListener(class LevelListener&);
	virtual void removeListener(LevelListener&);
	virtual void tickEntities(void);
	virtual void tickEntitySystems(void);
	virtual int getPauseManager(void);
	virtual int getPauseManager2(void);
	virtual void onPlayerDeath(Entity& a1, class ActorDamageSource const& a2);
	virtual void tick(void);
	virtual void directTickEntities(BlockSource&);
	virtual void animateTick(Entity&);
	virtual void explode(BlockSource&, Entity*, vec3_t const&, float, bool, bool, float, bool);
	virtual void explode(class Explosion&);
	virtual void spawnParticleEffect(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, vec3_t const&, class Dimension*);
	virtual void spawnParticleEffect2(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, Entity const&, vec3_t const&);
	virtual void denyEffect(BlockSource&, vec3_t const&);
	virtual void potionSplash(vec3_t const&, struct MC_Color, bool);
	virtual void applyLiquidPhysicsToActor(Entity*, class MaterialType);
	virtual void extinguishFire(BlockSource& BlockSource, vec3_ti const& position, int blockSide);
	virtual void findPath(Entity&, int, int, int, class NavigationComponent&);
	virtual void findPath(Entity&, Entity&, class NavigationComponent&);
	virtual void updateSleepingPlayerList(void);
	virtual int getTime(void);
	virtual void setTime(int);
	virtual __int64 getSeed(void);
	virtual int getSharedSpawnPos(void);
	virtual void setDefaultSpawn(vec3_ti const&);
	virtual vec3_ti* getDefaultSpawn(void);
	virtual void setDefaultGameType(class GameType);
	virtual int getDefaultGameType(void);
	virtual void setDifficulty(class Difficulty);
	virtual void setMultiplayerGameIntent(bool);
	virtual int getMultiplayerGameIntent(void);
	virtual void setMultiplayerGame(bool);
	virtual bool isMultiplayerGame(void);
	virtual void setLANBroadcastIntent(bool);
	virtual int getLANBroadcastIntent(void);
	virtual void setLANBroadcast(bool);
	virtual int getLANBroadcast(void);
	virtual void setXBLBroadcastIntent(uintptr_t);
	virtual int getXBLBroadcastIntent(void);
	virtual void hasXBLBroadcastIntent(void);
	virtual void setXBLBroadcastMode(uintptr_t);
	virtual int getXBLBroadcastMode(void);
	virtual void hasXBLBroadcast(void);
	virtual void setPlatformBroadcastIntent(uintptr_t);
	virtual int getPlatformBroadcastIntent(void);
	virtual void hasPlatformBroadcastIntent(void);
	virtual void setPlatformBroadcastMode(uintptr_t);
	virtual int getPlatformBroadcastMode(void);
	virtual void hasPlatformBroadcast(void);
	virtual void setHasLockedBehaviorPack(bool);
	virtual void setHasLockedResourcePack(bool);
	virtual void setCommandsEnabled(bool);
	virtual void setWorldTemplateOptionsUnlocked(void);
	virtual void hasLevelStorage(void);
	virtual int getLevelStorage(void);
	virtual int getLevelStorage2(void);
	virtual int getLevelData(void);
	virtual int getLevelData2(void);
	virtual int getPhotoStorage(void);
	virtual void createPhotoStorage(void);
	virtual void setEducationLevelSettings(class EducationLevelSettings);
	virtual int getEducationLevelSettings(void);
	virtual void save(void);
	virtual void saveEducationLevelSettings(void);
	virtual void saveLevelData(void);
	virtual void saveGameData(void);
	virtual void saveVillages(void);
	virtual void savePlayers(void);
	virtual void savePlayer(Entity&);
	virtual void saveBiomeData(void);
	virtual void saveDirtyChunks(void);
	virtual void saveAdditionalData(void);
	virtual void requestTimedStorageDeferment(void);
	virtual void _checkUserStorage(void);
	virtual int getTickingAreasMgr(void);
	virtual int getTickingArea(uintptr_t);
	virtual void addParticle(class ParticleType, vec3_t const&, vec3_t const&, int, CompoundTag const*, bool);
	virtual void sendServerLegacyParticle(ParticleType, vec3_t const&, vec3_t const&, int);
	virtual void playSound(class LevelSoundEvent, vec3_t const&, int, class ActorDefinitionIdentifier const&, bool, bool);
	virtual void playSound(LevelSoundEvent, vec3_t const&, float, float);
	virtual void playSound(BlockSource&, LevelSoundEvent, vec3_t const&, int, ActorDefinitionIdentifier const&, bool, bool);
	virtual void playSynchronizedSound(BlockSource&, LevelSoundEvent, vec3_t const&, class Block const&, ActorDefinitionIdentifier const&, bool, bool);
	virtual void playSynchronizedSound(BlockSource&, LevelSoundEvent, vec3_t const&, int, ActorDefinitionIdentifier const&, bool, bool);
	virtual void setRemotePlayerEventCoordinator(std::unique_ptr<class PlayerEventCoordinator, std::default_delete<PlayerEventCoordinator>>&&);
	virtual int getRemotePlayerEventCoordinator(void);
	virtual void setServerPlayerEventCoordinator(std::unique_ptr<class ServerPlayerEventCoordinator, std::default_delete<ServerPlayerEventCoordinator>>&&);
	virtual int getServerPlayerEventCoordinator(void);
	virtual void setClientPlayerEventCoordinator(std::unique_ptr<class ClientPlayerEventCoordinator, std::default_delete<ClientPlayerEventCoordinator>>&&);
	virtual int getClientPlayerEventCoordinator(void);
	virtual void setActorEventCoordinator(std::unique_ptr<class ActorEventCoordinator, std::default_delete<ActorEventCoordinator>>&&);
	virtual int getActorEventCoordinator(void);
	virtual void setBlockEventCoordinator(std::unique_ptr<class BlockEventCoordinator, std::default_delete<BlockEventCoordinator>>&&);
	virtual int getBlockEventCoordinator(void);
	virtual void setItemEventCoordinator(std::unique_ptr<class ItemEventCoordinator, std::default_delete<ItemEventCoordinator>>&&);
	virtual int getItemEventCoordinator(void);
	virtual void setServerNetworkEventCoordinator(std::unique_ptr<class ServerNetworkEventCoordinator, std::default_delete<ServerNetworkEventCoordinator>>&&);
	virtual int getServerNetworkEventCoordinator(void);
	virtual int getLevelEventCoordinator(void);
	virtual void handleLevelEvent(class LevelEvent, vec3_t const&, int);
	virtual void handleLevelEvent(LevelEvent, CompoundTag const&);
	virtual void handleSoundEvent(LevelSoundEvent, vec3_t const&, int, ActorDefinitionIdentifier const&, bool, bool);
	virtual void handleSoundEvent(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, vec3_t const&, float, float);
	virtual void handleStopSoundEvent(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&);
	virtual void handleStopAllSounds(void);
	virtual void broadcastLevelEvent(LevelEvent, vec3_t const&, int, class UserEntityIdentifierComponent const*);
	virtual void broadcastLevelEvent(LevelEvent, CompoundTag const&, UserEntityIdentifierComponent const*);
	virtual void broadcastLocalEvent(BlockSource&, LevelEvent, vec3_t const&, int);
	virtual void broadcastLocalEvent(BlockSource&, LevelEvent, vec3_t const&, Block const&);
	virtual void broadcastSoundEvent(BlockSource&, LevelSoundEvent, vec3_t const&, Block const&, ActorDefinitionIdentifier const&, bool, bool);
	virtual void broadcastSoundEvent(BlockSource&, LevelSoundEvent, vec3_t const&, int, ActorDefinitionIdentifier const&, bool, bool);
	virtual void broadcastActorEvent(Entity&, class ActorEvent, int);
	virtual void addBossEventListener(class BossEventListener*);
	virtual void removeBossEventListener(BossEventListener*);
	virtual void broadcastBossEvent(class BossEventUpdateType, uint64_t const&, class BossEventPacket const&);
	virtual void broadcastBossEvent(BossEventUpdateType);
	virtual void areBossEventListenersReady(void);
	virtual void addChunkViewTracker(std::weak_ptr<class ChunkViewSource>);
	virtual void pruneChunkViewTrackers(void);
	virtual void onChunkReload(class Bounds const&);
	virtual void onChunkReloaded(class ChunkSource&, LevelChunk&);

private:
	virtual void TryroFunc210();

public:
	virtual __int64 getActivePlayerCount(void);
	virtual __int64 getActiveUsersCount(void);
	virtual void forEachPlayer(uintptr_t);   //(std::function<bool()(Actor&)>);
	virtual void forEachPlayer2(uintptr_t);  //(std::function<bool()(Actor const&)>);
	virtual void forEachUser(uintptr_t);     //(std::function<bool()(Actor&)>);
	virtual void findPlayer(uintptr_t);      //(std::function<bool()(Actor const&)>);
	virtual void findPlayer2(uintptr_t);     //(std::function<bool()(Actor const&)>);
	virtual __int64 getUserCount(void);
	virtual void countUsersWithMatchingNetworkId(class NetworkIdentifier const&);
	virtual int getUsers(void);
	virtual int getUsers2(void);
	virtual __int64 getEntities(void);
	virtual void onSubChunkLoaded(ChunkSource&, LevelChunk&, short);
	virtual void onChunkLoaded(ChunkSource&, LevelChunk&);
	virtual void onChunkDiscarded(LevelChunk&);
	virtual void queueEntityDestruction(uintptr_t, bool);
	virtual void removeEntityReferences(Entity&, bool);
	virtual void removeEntity(Entity&);
	virtual void removeEntity(WeakEntityRef);
	virtual void removeEntityIfExists(Entity*);
	virtual void forceRemoveEntity(Entity&);
	virtual void forceFlushRemovedPlayers(void);
	virtual void loadFunctionManager(void);
	virtual void levelCleanupQueueEntityRemoval(uintptr_t, bool);
	virtual void registerTemporaryPointer(class _TickPtr&);
	virtual void unregisterTemporaryPointer(_TickPtr&);
	virtual void destroyBlock(BlockSource&, vec3_ti const&, bool);
	virtual void upgradeStorageVersion(class StorageVersion);
	virtual void suspendAndSave(void);
	virtual void waitAsyncSuspendWork(void);
	virtual void _destroyEffect(vec3_ti const&, Block const&, int);
	virtual void addParticleEffect(class HashedString const&, vec3_t const&, class MolangVariableMap const&);
	virtual void addParticleEffect(HashedString const&, Entity const&, HashedString const&, vec3_t const&, MolangVariableMap const&);
	virtual void addTerrainParticleEffect(vec3_ti const&, Block const&, vec3_t const&, float, float, float);
	virtual void addTerrainSlideEffect(vec3_ti const&, Block const&, vec3_t const&, float, float, float);
	virtual void addBreakingItemParticleEffect(vec3_t const&, ParticleType, class TextureUVCoordinateSet const&, bool);
	virtual int getNewUniqueID(void);
	virtual int getNextRuntimeID(void);
	virtual int getTickingOffsets(void);
	virtual int getClientTickingOffsets(void);
	virtual bool isExporting(void);
	virtual void setIsExporting(bool);
	virtual int getSavedData(void);
	virtual int getMapSavedData(uint64_t);
	virtual int getMapSavedData(CompoundTag const&);
	virtual int getMapSavedData(CompoundTag const*);
	virtual void requestMapInfo(uint64_t, bool);
	virtual void expandMapByID(uint64_t, bool);
	virtual void copyAndLockMap(uint64_t, uint64_t);
	virtual void createMapSavedData(uint64_t const&, vec3_ti const&, uintptr_t, int);
	virtual void createMapSavedData(std::vector<uint64_t, std::allocator<uint64_t>> const&, vec3_ti const&, uintptr_t, int);

private:
	virtual void TryroFunc262();
	virtual void TryroFunc263();

public:
	virtual void setLevelId(std::basic_string<char, std::char_traits<char>, std::allocator<char>>);
	virtual int getSyncTasksGroup(void);
	virtual int getIOTasksGroup(void);
	virtual int getClientResourcePackManager(void);
	virtual int getServerResourcePackManager(void);
	virtual int getTradeTables(void);
	virtual void addEntryToTagCache(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&);
	virtual void addEntriesToTagCache(std::vector<std::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char>>>>);
	virtual void dropEntryFromTagCache(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&);
	virtual void clearTagCache(void);
	virtual void decrementTagCache(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, uintptr_t, uintptr_t);
	virtual void incrementTagCache(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, uintptr_t, uintptr_t);
	virtual bool isEdu(void);
	virtual int getActorFactory(void);
	virtual int getActorFactory2(void);
	virtual int getActorInfoRegistry(void);
	virtual int getEntityRegistryOwner(void);
	virtual int getEntitySystems(void);
	virtual int getLevelEntity(void);
	virtual void _clientHandleAddOwnedEntity(class EntityNetId);
	virtual void _clientHandleAddWeakRefEntity(EntityNetId, uintptr_t);
	virtual void _clientHandleRemoveOwnedEntity(EntityNetId);
	virtual void _clientHandleRemoveWeakRefEntity(EntityNetId);
	virtual void runCommand(HashedString const&, class CommandOrigin&, class CommandOriginSystem, class CurrentCmdVersion);
	virtual void runCommand(class Command&, CommandOrigin&, CommandOriginSystem);
	virtual int getTagRegistry(void);
	virtual int getPlayerMovementSettings(void);
	virtual void setPlayerMovementSettings(class PlayerMovementSettings const&);
	virtual void canUseSkin(class SerializedSkin const&, NetworkIdentifier const&, uintptr_t, std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&);
	virtual int getPositionTrackerDBClient(void);
	virtual int getPositionTrackerDBServer(void);
	virtual void flushRunTimeLighting(void);
	virtual void loadBlockDefinitionGroup(Experiments const&);
	virtual void initializeBlockDefinitionGroup(void);
	virtual int getUnknownBlockTypeRegistry(void);

private:
	virtual void TryroFunc299();

public:
	virtual bool isClientSide(void);
	virtual int getPlayerList(void);
	virtual int getPlayerList2(void);

private:
	virtual void TryroFunc303();
	virtual void TryroFunc304();

public:
	virtual int getActiveUsers(void);
	virtual int getRuntimeActorList(void);
	virtual int getGlobalActors(void);
	virtual int getAutonomousActors(void);
	virtual int getAutonomousLoadedEntities(void);
	virtual void removeAutonomousEntity(Entity&, LevelChunk*);
	virtual void notifySubChunkRequestManager(class SubChunkPacket const&);
	virtual int getSubChunkRequestManager(void);
	virtual void* getPacketSender(void);
	virtual void setPacketSender(class PacketSender*);
	virtual int getNetEventCallback(void);
	virtual void setNetEventCallback(class NetEventCallback*);
	virtual int getRandom(void);
	virtual int getThreadRandom(void);
	virtual int getHitResult(void);
	virtual int getLiquidHitResult(void);

private:
	virtual void TryroFunc321();

public:
	virtual void setImmersiveReaderString(std::basic_string<char, std::char_traits<char>, std::allocator<char>>);
	virtual int getAdventureSettings(void);
	virtual int getGameRules(void);
	virtual int getGameRules2(void);
	virtual void hasStartWithMapEnabled(void);
	virtual int getDefaultAbilities(void);
	virtual int getTearingDown(void);
	virtual void takePicture(uintptr_t, Entity*, Entity*, class ScreenshotOptions&);
	virtual int getSoundPlayer(void);
	virtual void setSimPaused(bool);
	virtual int getSimPaused(void);
	virtual void setFinishedInitializing(void);
	virtual int getLootTables(void);
	virtual void updateWeather(float, int, float, int);
	virtual int getNetherScale(void);
	virtual int getScoreboard(void);
	virtual int getPlayerAbilities(uint64_t const&);
	virtual void setPlayerAbilities(uint64_t const&, class Abilities);
	virtual void sendAllPlayerAbilities(Entity const&);
	virtual void incrementSpawnableTickedMob(void);
	virtual int getSpawnableTickedMobCountPrevious(void);
	virtual int getRecipes(void);
	virtual int getBlockReducer(void);
	virtual void digestServerBlockProperties(class StartGamePacket const&, uintptr_t);
	virtual void digestServerItemComponents(class ItemComponentPacket const&);
	virtual int getRegisteredBorderBlock(void);
	virtual void asLevel(void);
	virtual void asMultiPlayerLevel(void);
	virtual void onSourceCreated(BlockSource&);
	virtual void onSourceDestroyed(BlockSource&);
	virtual void _localPlayerChangedDimension(void);

public:
	bool hasEntity();
	Entity* getEntity();
	int getLevelTicks() {
		return *reinterpret_cast<int*>(reinterpret_cast<__int64>(this) + 0x690);
	}

	class LoopbackPacketSender* getLoopbackPacketSender() {
		return *reinterpret_cast<class LoopbackPacketSender**>(reinterpret_cast<__int64>(this) + 0xB30);
	}

	// 48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 57 48 83 EC 50 48 ?? ?? ?? ?? 00 00 33 FF 48 2B ?? ?? ?? 00 00 49 8B F0 48 C1 F8 03 48 8B EA
	void playSound(std::string sound, vec3_t const& position, float volume, float pitch) {
		using playSound_t = void(__fastcall*)(Level*, TextHolder*, vec3_t const&, float, float);
		static playSound_t func = reinterpret_cast<playSound_t>(Utils::getBase() + 0x2225c50);
		if (func != 0) {
			TextHolder th(sound);
			func(this, &th, position, volume, pitch);
		}
	}

	// Credits to hacker hansen for this
	// 48 89 5C 24 ?? 56 57 41 56 48 83 EC 40 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B F2 4C 8B F1 48 89 54 24 ?? 33 C9
	std::vector<Entity*> getMiscEntityList() {
		using entityList_t = __int64* (__fastcall*)(Level*, void*);
		static entityList_t func = reinterpret_cast<entityList_t>(Utils::getBase() + 0x222fb00);
		if (func != 0) {
			char* alloc = new char[0x18];  // Allocate memory for the list
			__int64* listStart = func(this, alloc);
			int listSize = ((*reinterpret_cast<__int64*>(reinterpret_cast<__int64>(listStart) + 0x8)) - (*listStart)) / 0x8;
			Entity** entityList = reinterpret_cast<Entity**>(*listStart);
			std::vector<Entity*> res;
			if (listSize > 0 && listSize < 5000) {
				for (int i = 0; i < listSize; i++) {
					res.push_back(entityList[i]);
				}
			}
			delete[] alloc;
			return res;
		}
	}
};