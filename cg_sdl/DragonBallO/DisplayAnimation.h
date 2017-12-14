#pragma once
#include "Animation.h"
class DisplayAnimation :
	public Animation
{
public:
	DisplayAnimation();
	~DisplayAnimation();
	bool UpdateSpriteClipIndex(int &spriteClipIndex) override;
};

