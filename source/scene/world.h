
#ifndef WORLD_H
    #define WORLD_H

    #include <memory>
    #include <unordered_map>

    #include "defs_world.h"
    #include "defs_blocks.h"

    class SolidBlock;
    class Segment;
    class Sector;
    class Scene;

    namespace std
    {
        template<>
        struct hash<SectorIndex>
        {
            std::size_t operator()(const SectorIndex& index) const noexcept
            {
                uint64_t _combination = 0;
                uintptr_t _add = (uintptr_t)&_combination;
                _add += 4;
                memcpy(&_combination, &index.x, 4);
                memcpy((void*)(((uintptr_t)&_combination) + 4), &index.z, 4);
                std::hash<uint64_t> hash1;
                return hash1(_combination);
            }
        };
    }

    class World
    {
    public:
	    World(Scene* scene);
	    ~World();

	    void SetupDevelopementWorld();
        void Update();

        bool CreateBlockByGridPos(BlockType type, int x, int y, int z, bool rebuildSegment = false);
        bool CreateBlockByWorldPos(BlockType type, float x, float y, float z, bool rebuildSegment = false);
        BlockType GetBlockByGridPos(int x, int y, int z);
        BlockType GetBlockByWorldPos(float x, float y, float z);

        Segment* CreateSegment(int x, int y, int z);
        Segment* GetSegmentByGridPos(int x, int y, int z, bool force = false);
        Segment* GetSegmentByWorldPos(float x, float y, float z, bool force = false);

        Sector* GetSectorByGridPos(int x, int z, int force = false);
        Sector* GetSectorByWorldPos(float x, float z, int force = false);

        std::unordered_map<SectorIndex, Sector*, std::hash<SectorIndex>> sectors;

    private:
        friend class FinishDigEvent;

        BlockType GetBlock(Segment* segment, BlockIndex& index);
        Segment* GetSegment(Sector* sector, SegmentIndex& index, bool force = false);
        Sector* GetSector(SectorIndex& index, bool force = false);

	    Scene* scene;
    };

#endif // !WORLD_H
