
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

        bool CreateBlock(BlockType type, float x, float y, float z, bool rebuildSegment = false);
        BlockType GetBlock(float x, float y, float z);

        Segment* CreateSegment(float x, float y, float z);
        Segment* GetSegment(float x, float y, float z, bool force = false);

        Sector* GetSector(float x, float z, bool force = false);

        std::unordered_map<SectorIndex, Sector*, std::hash<SectorIndex>> sectors;

    private:
        BlockType GetBlock(Segment* segment, BlockIndex& index);
        Segment* GetSegment(Sector* sector, SegmentIndex& index, bool force = false);
        Sector* GetSector(SectorIndex& index, bool force = false);

	    Scene* scene;
    };

#endif // !WORLD_H
