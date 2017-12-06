#pragma once

#include "Entity.h"
#include "Animation.h"

class Sprite : public Entity
{
friend class SDLInit;

using UInt = unsigned int;
using Int2 = MyMath::Int2;
using Float2 = MyMath::Float2;

public:
	virtual ~Sprite() override;

	void SetTexturePath(const char* texturePath);

	//Call InitSPriteSheet before calling SetSpriteClip...
	void InitSpriteSheet(UInt startClipIndex, UInt numSpriteCLipsX, UInt numSpriteClipsY);
	void SetSpriteClip(int x, int y, UInt w, UInt h, UInt index);
	void SetAnchorOffset(Int2 anchorOffset, UInt index);

	SDL_Rect* GetSpriteClip();
	Int2* GetAnchorOffset();

	int mSpriteClipIndex{ 0 };
protected:
	//The actual hardware texture
	SDL_Texture* mTexture;
	const char* mTexturePath;

	int mNumSpriteClips{ 0 };
	int mNumSpriteClipsX{ 0 };
	int mNumSpriteClipsY{ 0 };

	//nullptr is better for ambiguity purposes...
	SDL_Rect *mSpriteCLips = nullptr;
	Int2 *mAnimAnchorOffsets = nullptr;
};

