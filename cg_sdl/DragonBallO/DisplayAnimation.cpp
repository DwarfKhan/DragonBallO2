#include "DisplayAnimation.h"

extern float gDeltaTime;



DisplayAnimation::DisplayAnimation()
{

	active = true;
	SetAnimSpeed(1.3);
}


DisplayAnimation::~DisplayAnimation()
{
}

bool DisplayAnimation::UpdateSpriteClipIndex(int &spriteClipIndex) //returns true for a finished non-looping animation
{
	if (this == nullptr) {
		printf("null animation attempted\n");
		return false;
	}
	if (!active)
	{
		return false;
	}

	// one frame animations get buggy, so this doubles one frame animations
	if (mFrameCount == 1)
	{
		AddSpriteClip(mSpriteClips[0]);
		SetAnimSpeed(mSpeed * 2);
	}

	//math for finding which frame needs to be displayed
	mTimer += mSpeed * gDeltaTime;
	int index = (int)mTimer % mFrameCount;

		spriteClipIndex = mSpriteClips[index];
		printf("Displaying: %d\n", mSpriteClips[index]);

		return false;

	return false;
}

