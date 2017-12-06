#pragma once
#include <string>
#include <SDL_image.h>
#include "MyMath.h"
#include <vector>

class Entity {

friend class SDLInit;
friend class Camera;
friend class Grid;

public:
	using UInt = unsigned int;
	using Int2 = MyMath::Int2;
	using Float2 = MyMath::Float2;
	using UByte = MyMath::UByte;

public:
	//Needs to be virtual so that derived destructor gets called...
	virtual ~Entity() {};

public:
	virtual void Update();

	void SetPosition(Float2 pos);
	Float2 GetPos();
	Float2 GetCenterPos();
	void SetSize(int width, int height);
	void SetSize(Int2 size);
	MyMath::Int2 GetSize();
	void SetMoveSpeed(float moveSpeed);

	void SetFacingDirection(int direction);// 0 is up, 1 is down, 2 is left, 3 is right
	int GetFacingDirection();


	void ConfigureCollision(bool canPushBack, bool canBePushedBack,
		Int2 topLeftCollOffset = { 0, 0 }, Int2 bottomRightCollOffset = { 0, 0 });

	void AddCollidableEntity(Entity &entity);

	
	void CheckCollision();

private:

protected:
	virtual void OnCollision(Entity *other);


protected:
	Float2 mPos;
	Int2 mSize;

	Int2 mTopLeftCollOffset;
	Int2 mBottomRightCollOffset;

	UByte mPushbackSides{ 0 };
	int mFacingDirection; // 0 is up, 1 is down, 2 is left, 3 is right

	//If this entity can move, it needs a move speed...
	float mMoveSpeed{ 140.f };

	//For collision...
	std::vector<Entity*> mCollidableEntities;
	bool mCanPushBack{false};
	bool mCanBePushedBack{false};
	int mHasCollided{false};
};