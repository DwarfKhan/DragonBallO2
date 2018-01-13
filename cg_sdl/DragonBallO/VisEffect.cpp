#include "VisEffect.h"
#include "SDLInit.h"


extern float gDeltaTime;
extern SDLInit sdlInit; //needed for sound effects


VisEffect::VisEffect()
{
}


VisEffect::~VisEffect()
{
}

void VisEffect::Update()
{
	if (mTimer < 0) {
		sdlInit.CleanupSprite(*this);//UNTESTED
	}
	if (temporary) {
		mTimer -= gDeltaTime;
	}

	mPos += mVelocity * gDeltaTime;
	Sprite::Update();
}
