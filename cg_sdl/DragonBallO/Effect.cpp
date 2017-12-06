#include "Effect.h"

extern float gDeltaTime;
extern SDLInit sdlInit; //needed for sound effects


Effect::Effect()
{
}


Effect::~Effect()
{
}

void Effect::Update()
{
	if (mTimer < 0) {
		//sdlInit.CleanupSprite();
	}
	if (temporary) {
		mTimer -= gDeltaTime;
	}

	mPos += mVelocity * gDeltaTime;
	Sprite::Update();
}
