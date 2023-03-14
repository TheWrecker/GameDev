
#ifndef WORLD_H
    #define WORLD_H

    #include <memory>
    #include <unordered_map>

    #include "defs_world.h"

    class SolidBlock;
    class Segment;
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

    namespace std
    {
        template<>
        struct hash<SegmentIndex>
        {
            std::size_t operator()(const SegmentIndex& index) const noexcept
            {
                std::size_t _combination = 
                    (((std::size_t)index.x << 23) * 53) +
                    (((std::size_t)index.y << 17) * 67) + 
                    ((std::size_t)index.z * 83);
                return _combination;
            }
        };
    }

    class World
    {
    public:
	    World(Scene* scene);
	    ~World();

	    void SetupDevelopementWorld();

	    Segment* GetSegment(float x, float y, float z);
	    Segment* GetSegment(SegmentIndex& index);
	    SolidBlock* GetBlock(float x, float y, float z);
	    SolidBlock* GetBlock(Segment* segment, BlockIndex& index);

        BlockIndex GetBlockIndex(float x, float y, float z);
        SegmentIndex GetSegmentIndex(float x, float y, float z);

	    bool IsSegmentWithinBounds(SegmentIndex index);
	    bool IsBlockWithinBounds(BlockIndex& index);

    private:
	    friend class Aggregator;
	    friend class SolidBlockRender;
	    friend class Overlay;

	    Scene* scene;
	    std::unordered_map<SegmentIndex, Segment*, std::hash<SegmentIndex>> segments;
    };

#endif // !WORLD_H
