
#pragma once

class IDrawable
{
public:
	virtual void Draw() = 0;
	virtual void Update() {}
};