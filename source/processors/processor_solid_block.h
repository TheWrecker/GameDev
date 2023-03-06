
#pragma once

#include <vector>

#include "defs_pipeline.h"
#include "../entities/segment.h"

typedef const std::vector<SolidBlockVertex> Face;

enum class FaceName
{
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	FRONT,
	BACK
};

class TextureAtlas;

class SolidBlockProcessor
{
public:
	static void Setup(TextureAtlas* atlas);
	static void Rebuild(Segment* target);

private:
	static bool CheckBlockFace(Segment* target, FaceName face);
	static void AddFaceVertices(Segment* target, Face& face);
	//static void IterateSide(Segment* target, Face& face, unsigned int maxX, unsigned int maxY, unsigned int maxZ, unsigned int& fixed);
	static unsigned int
		index_x,
		index_y,
		index_z,
		solids,
		current_index;
	static TextureAtlas* texture_atlas;
};