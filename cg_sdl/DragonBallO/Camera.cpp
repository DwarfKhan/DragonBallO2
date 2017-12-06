#include "Camera.h"

using namespace MyMath;

extern float gDeltaTime;
extern World gWorld;

namespace {
	RectBoundary nextZoneBound;
	RectBoundary lastZoneBound;
	float panTime = 1.0f;
	int zoneIndX = 0;
	int zoneIndY = 0;
}

void Camera::RestrictTargetToWorld(Entity &target) {
	int targetWidth = target.mSize.x - target.mBottomRightCollOffset.x - target.mTopLeftCollOffset.x;
	int targetHeight = target.mSize.y - target.mBottomRightCollOffset.y - target.mTopLeftCollOffset.y;

	RectBoundary targetBound {
		target.mPos.x + target.mTopLeftCollOffset.x,				//left...
		target.mPos.x + target.mTopLeftCollOffset.x + targetWidth,	//right...
		target.mPos.y + target.mTopLeftCollOffset.y,				//top...
		target.mPos.y + target.mTopLeftCollOffset.y + targetHeight,	//bottom...
	};

	//collides left...
	if (targetBound.left < 0.0f) {
		target.mPos.x = -(float)target.mTopLeftCollOffset.x;
		target.mPushbackSides |= 1 << 0;	//can't pushback from right...
	}	//collides right...
	else if (targetBound.right > gWorld.mWidth) {
		target.mPos.x = float(gWorld.mWidth - target.mSize.x + target.mBottomRightCollOffset.x);
		target.mPushbackSides |= 1 << 2;	//can't pushback from left...
	}

	//collides top...
	if (targetBound.top < 0.0f) {
		target.mPos.y = -(float)target.mTopLeftCollOffset.y;
		target.mPushbackSides |= 1 << 3;	//can't pushback from bottom...
	}	//collides bottom...
	else if (targetBound.bottom > gWorld.mHeight) {
		target.mPos.y = float(gWorld.mHeight - target.mSize.y + target.mBottomRightCollOffset.y);
		target.mPushbackSides |= 1 << 1;	//can't pushback from top...
	}
}

void Camera::SetPos(Float2 pos) {
	pos.x = pos.x - (SCREEN_WIDTH / 2.0f);
	pos.y = pos.y - (SCREEN_HEIGHT / 2.0f);

	if (mMode == Mode::FOLLOW) {
		return;
	}

	zoneIndX = gWorld.GetIndX(mPos.x);
	zoneIndY = gWorld.GetIndY(mPos.y);
	nextZoneBound = gWorld.GetZoneBoundary(zoneIndX, zoneIndY);;
	panTime = 1.0f;
}

void Camera::LookAt(Entity &target) {
	mLookAtTarget = &target;

	//Camera follows target...
	if (mMode == Mode::FOLLOW) {
		SetPos({ target.mPos.x + (target.mSize.x / 2.0f), target.mPos.y + (target.mSize.y / 2.0f) });

		//Restricting camera to world coords...
		if (mPos.x < 0.0f) {
			mPos.x = 0.0f;
		}
		if (mPos.y < 0.0f) {
			mPos.y = 0.0f;
		}

		if (mPos.x + SCREEN_WIDTH > gWorld.mWidth) {
			mPos.x = float(gWorld.mWidth - SCREEN_WIDTH);
		}
		if (mPos.y + SCREEN_HEIGHT > gWorld.mHeight) {
			mPos.y = float(gWorld.mHeight - SCREEN_HEIGHT);
		}

		return;
	}

	if (panTime > 1) {
		auto &zoneBound = gWorld.GetZoneBoundary(zoneIndX, zoneIndY);
		int targetWidth = target.mSize.x - target.mBottomRightCollOffset.x - target.mTopLeftCollOffset.x;
		int targetHeight = target.mSize.y - target.mBottomRightCollOffset.y - target.mTopLeftCollOffset.y;

		RectBoundary targetBound{
			target.mPos.x + target.mTopLeftCollOffset.x,				//left...
			target.mPos.x + target.mTopLeftCollOffset.x + targetWidth,	//right...
			target.mPos.y + target.mTopLeftCollOffset.y,				//top...
			target.mPos.y + target.mTopLeftCollOffset.y + targetHeight,	//bottom...
		};

		bool panRight = targetBound.right > zoneBound.right;
		bool panLeft = targetBound.left < zoneBound.left;
		bool panUp = targetBound.top < zoneBound.top;
		bool panDown = targetBound.bottom > zoneBound.bottom;

		if (panRight) {
			nextZoneBound = gWorld.GetZoneBoundary(++zoneIndX, zoneIndY);
			lastZoneBound = zoneBound;
			panTime = 0.0f;
			printf("Pan Right...!\n");
		}
		else if (panLeft) {
			nextZoneBound = gWorld.GetZoneBoundary(--zoneIndX, zoneIndY);
			lastZoneBound = zoneBound;
			panTime = 0.0f;
			printf("Pan Left...!\n");
		}
		
		if (panDown) {
			nextZoneBound = gWorld.GetZoneBoundary(zoneIndX, ++zoneIndY);
			lastZoneBound = zoneBound;
			panTime = 0.0f;
			printf("Pan Left...!\n");
		}
		else if (panUp) {
			nextZoneBound = gWorld.GetZoneBoundary(zoneIndX, --zoneIndY);
			lastZoneBound = zoneBound;
			panTime = 0.0f;
			printf("Pan Left...!\n");
		}
	}

	panTime += gDeltaTime;
	mPos.x = Lerp(lastZoneBound.left, nextZoneBound.left, panTime);
	mPos.y = Lerp(lastZoneBound.top, nextZoneBound.top, panTime);
}

bool Camera::IsPanning() {
	return panTime < 1.0f;
}
