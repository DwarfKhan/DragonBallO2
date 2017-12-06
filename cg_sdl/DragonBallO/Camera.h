#pragma once

#include "MyMath.h"
#include "World.h"
#include "Entity.h"

//Screen dimension constants
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class Camera
{

friend class SDLInit;

using Float2 = MyMath::Float2;

public:
	enum Mode { FOLLOW, PAN };

	Camera(Mode mode) : mMode { mode } {};
	~Camera() {};

	void SetPos(Float2 pos);
	void LookAt(Entity &target);

	void RestrictTargetToWorld(Entity &target);

	bool IsPanning();

private:
	Float2 mPos;

	Mode mMode{ Mode::FOLLOW };
	Entity *mLookAtTarget = nullptr;
	SDL_Rect *mBounds = nullptr;
};

