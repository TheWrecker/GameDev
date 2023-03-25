
#ifndef WORLD_H
    #define WORLD_H

    #include <memory>
    #include <unordered_map>

    #include "defs_world.h"

    class SolidBlock;
    class Segment;
    class Pillar;
    class Scene;

    struct BlockIndex
    {
        unsigned int x, y, z;

        BlockIndex(unsigned int x, unsigned int y, unsigned int z);
    };

    bool operator ==(const BlockIndex& left, const BlockIndex& right) noexcept;

    struct SegmentIndex
    {
        int x, y, z;

        SegmentIndex(int x, int y, int z);
    };

    bool operator ==(const SegmentIndex& left, const SegmentIndex& right) noexcept;

    struct PillarIndex
    {
        int x, z;

        PillarIndex(int x, int z);
    };

    bool operator ==(const PillarIndex& left, const PillarIndex& right) noexcept;

    namespace std
    {
        template<>
        struct hash<PillarIndex>
        {
            std::size_t operator()(const PillarIndex& index) const noexcept
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

        SolidBlock* CreateBlock(SolidBlockType type, float x, float y, float z, bool rebuildSegment = false);
        SolidBlock* GetBlock(float x, float y, float z);
	    SolidBlock* GetBlock(Segment* segment, BlockIndex& index);

        Segment* CreateSegment(float x, float y, float z);
        Segment* GetSegment(float x, float y, float z, bool force = false);
        Segment* GetSegment(SegmentIndex& index, bool force = false);

        Pillar* GetPillar(float x, float z, bool force = false);

        BlockIndex GetBlockIndex(float x, float y, float z);
        SegmentIndex GetSegmentIndex(float x, float y, float z);
        PillarIndex GetPillarIndex(float x, float z);

	    bool IsSegmentWithinBounds(SegmentIndex index);
	    bool IsBlockWithinBounds(BlockIndex& index);

        std::unordered_map<PillarIndex, Pillar*, std::hash<PillarIndex>> pillars;
        std::vector<Pillar*> near_pillars;
        Pillar* last_pillar;

    private:
	    Scene* scene;
    };

#endif // !WORLD_H
