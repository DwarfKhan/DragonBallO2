#include "Sprite.h"

using namespace MyMath;

Sprite::~Sprite() {
	if (mSpriteCLips == nullptr) {
		delete[] mSpriteCLips;
		mSpriteCLips = nullptr;
	}

	if (mAnimAnchorOffsets == nullptr) {
		delete[] mAnimAnchorOffsets;
		mAnimAnchorOffsets = nullptr;
	}
}

void Sprite::SetTexturePath(const char* texturePath) {
	mTexturePath = texturePath;
}

void Sprite::InitSpriteSheet(UInt startClipIndex, UInt numSpriteCLipsX, UInt numSpriteClipsY) {
	if (numSpriteCLipsX == 0 || numSpriteClipsY == 0) {
		printf("Number of sprite clips must be at least 1.");
		return;
	}

	mNumSpriteClips = numSpriteCLipsX * numSpriteClipsY;
	mNumSpriteClipsX = numSpriteCLipsX;
	mNumSpriteClipsY = numSpriteClipsY;

	mSpriteCLips = new SDL_Rect[mNumSpriteClips];
	mAnimAnchorOffsets = new Int2[mNumSpriteClips];
	mSpriteClipIndex = startClipIndex;
}

/************************************************************************/
/* 	Sprite clip is used to render texture coordinates (for sprite
sheets)
/************************************************************************/
void Sprite::SetSpriteClip(int x, int y, UInt w, UInt h, UInt index) {
	if (mSpriteCLips == NULL) {
		printf("Cannot set sprite clip. Please call InitSpriteSheet first.");
		return;
	}

	int i = index % mNumSpriteClips;
	mSpriteCLips[i].x = x; mSpriteCLips[i].y = y;
	mSpriteCLips[i].w = w; mSpriteCLips[i].h = h;
}

/************************************************************************/
/* 	If the sprite size changes, the sprite will move. This offset
is for anchoring the sprite, so that it doesn't move.
/************************************************************************/
void Sprite::SetAnchorOffset(Int2 anchorOffset, UInt index) {
	if (mAnimAnchorOffsets == NULL) {
		printf("Cannot set clip offset. Please call InitSpriteSheet first.");
		return;
	}

	int i = index % mNumSpriteClips;
	mAnimAnchorOffsets[i] = anchorOffset;
}

/************************************************************************/
/* 	Sprite clip is used to render texture coordinates (for sprite
sheets)
/************************************************************************/
SDL_Rect* Sprite::GetSpriteClip() {
	return mSpriteCLips + mSpriteClipIndex;
}

/************************************************************************/
/* 	If the sprite size changes, the sprite will move. This offset
is for anchoring the sprite, so that it doesn't move.
/************************************************************************/
Int2* Sprite::GetAnchorOffset() {
	return mAnimAnchorOffsets + mSpriteClipIndex;
}