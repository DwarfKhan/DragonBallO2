#include "Animation.h"


extern float gDeltaTime;

Animation::Animation()
{
}


Animation::~Animation()
{
}

void Animation::AddSpriteClip(int spriteclip)
{
	mSpriteClips.push_back(spriteclip);
	mFrameCount++;
}

void Animation::AddSpriteClip(vector<int> spriteClips)
{
	for (int i = spriteClips.size(); i > 0; --i) {
		AddSpriteClip(i - 1);
	}
}

void Animation::SetAnimSpeed(float speed)
{
	mSpeed = speed;
}

bool Animation::UpdateSpriteClipIndex(int &spriteClipIndex) //returns true for a finished non-looping animation
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
	int index = (int)mTimer % mFrameCount; 
	mTimer += mSpeed * gDeltaTime;

	if (loops)
	{
		spriteClipIndex = mSpriteClips[index];
		return false;
	}

	if (mCurrentFrame != index) // if the frame needs to be changed on a non looping...
	{
			mCurrentFrame++;
			spriteClipIndex = mSpriteClips[index];
			//printf("Current Frame: %d.\n", mCurrentFrame);
			//printf("Spriteclip: %d.\n", mSpriteClips[index]);

			
		if (mCurrentFrame > mFrameCount) // if the animation is over, end animation and reset timing values
		{
			spriteClipIndex = mSpriteClips[mFrameCount - 1];
			active = false;
			mTimer = 0;
			mCurrentFrame = 0;
			return true;
		}
	}
	

	return false;
}

int Animation::GetFrameCount()
{
	return mFrameCount;
}

/* Ex: what i put in gamemanager
Animation targetDeath; //decleration
targetDeath.loops = false;
targetDeath.SetAnimSpeed(7.5);
targetDeath.AddSpriteClip(1); //adds index to animation
targetDeath.AddSpriteClip(2);
targetDeath.AddSpriteClip(3);
targetDeath.AddSpriteClip(4);
targetDeath.AddSpriteClip(5);
*/

/* ex: in actor class
void SetAnimDeath(Animation *anim); // needs reference to an animation to play
mAnimDeath->UpdateSpriteClipIndex(mSpriteClipIndex); // this changes the mSpriteclipIndex to whatever frame the animation dictates
*/

