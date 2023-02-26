
#pragma once

#include <string>

#include "entity_basic.h"

class SolidBlock : public BasicEntity
{
public:
	SolidBlock();
	~SolidBlock();

	//TODO: index instead of string (map to vector in IManager?)
	void SetTexture(const std::string& what);
	const std::string& GetTextureName();

private:
	std::string texture_name;

};