#include "Player.h"
#include "Camera.h"
#include "SDLInit.h"



#define ANIM_RIGHT_COUNT 2
#define ANIM_LEFT_COUNT 2
#define ANIM_UP_COUNT 2
#define ANIM_DOWN_COUNT 2

#define ANIM_ATTACK_COUNT 1

//1.4142f = sqrt(sqr(1) + sqr(1))
#define SQRHYPE 1.4142f	

extern float gDeltaTime;
extern Camera gCamera;
extern SDLInit sdlInit; //needed for sound effects

//Keys held down...
extern int gHorizKeysHeld;	//keys a and d
extern int gVertKeysHeld;	//keys w and s

//Keys pressed...
extern bool gFirstKeyDown;	//keys 1
extern bool gSecondKeyDown;	//keys 2
extern bool gThirdKeyDown;	//keys 3
extern bool gFourthKeyDown;	//keys 4

//Keys released...
extern bool gFirstKeyUp;	//keys e
extern bool gSecondKeyUp;	//keys 2
extern bool gThirdKeyUp;	//keys 3
extern bool gFourthKeyUp;	//keys 4





namespace {
	int attackRange = 10;
	int attackDamage = 1;
}

namespace {
	int lastMoveIndex = 4;
	int lastAttackIndex = 0;
}

namespace {
	//Animation times...
	float attackTime = .25f;

	//Animation timers...
	float moveRightTimer = 0.f;
	float moveLeftTimer = 0.f;
	float moveUpTimer = 0.f;
	float moveDownTimer = 0.f;

	float attackTimer = .0f;

	//Animation speeds...
	float animMoveSpeed = 12;
	float animAttackSpeed = 12;

	//Animation indices...
	int animRightIndices[ANIM_RIGHT_COUNT] = { 4, 18};
	int animLeftIndices[ANIM_LEFT_COUNT] = { 2, 16 };
	int animUpIndices[ANIM_UP_COUNT] = { 3, 17 };
	int animDownIndices[ANIM_DOWN_COUNT] = { 1, 15 };

	int animAttackLeftIndices[4][ANIM_ATTACK_COUNT] = {
		{ 5 },	//up attack...
		{ 8 },	//down attack...
		{ 6 },	//left attack...
		{ 7 }	//right attack...
	};
}

void Player::Update() {
	if (gCamera.IsPanning()) {
		return;
	}
	
	
	Move();
	PrintPos();
	Attack();
	Sprite::Update();
}

void Player::SetWeapon(Weapon *weapon, int range, int damage)
{
	playerWeapon = weapon;
	attackRange = range;
	attackDamage = damage;
	playerWeapon->SetDamage(damage);
}



void Player::PrintPos()
{
	if (!gSecondKeyDown) {
		return;
	}
		printf("X: %f\n", mPos.x);
		printf("Y: %f\n", mPos.y);
}

void Player::OnProxCollision(Entity * other)
{
}

MyMath::Float2 Player::FindWeaponPos()
{
	MyMath::Int2 wepSize = playerWeapon->GetSize();
	MyMath::Float2 position = { 0,0 };
	if (Entity::GetFacingDirection() == 0) {
		position.x = ((topLeftCornerPos.x + topRightCornerPos.x) / 2) - (wepSize.x / 2);
		position.y = topLeftCornerPos.y - (wepSize.y + (attackRange + 1));
		//													    	  /\
		//								Not sure why this 1 is needed but it was the only way to get it to look right...
	}
	else if (Entity::GetFacingDirection() == 1) {
		position.x = ((topLeftCornerPos.x + topRightCornerPos.x) / 2) - (wepSize.x/2);
		position.y = bottomLeftCornerPos.y + attackRange;
	}
	else if (Entity::GetFacingDirection() == 2) {
		position.y = ((topLeftCornerPos.y + bottomLeftCornerPos.y) / 2) - (wepSize.y/2);
		position.x = topLeftCornerPos.x - (wepSize.x + attackRange);
	}
	else if (Entity::GetFacingDirection() == 3) {
		position.y = ((topLeftCornerPos.y + bottomLeftCornerPos.y) / 2) - (wepSize.y / 2);
		position.x = topRightCornerPos.x + attackRange;
	}
	return position;
}


void Player::Move() {
	//If we are attacking we want to stop movement...
	if (attackTimer > 0.f) {
		return;
	}

	//Setting velocity...
	float velocity = mMoveSpeed * gDeltaTime;

	//Updates position. SQRHYPE is used so diagnal direction is NOT faster...
	mPos.x += (gVertKeysHeld != 0 ? (gHorizKeysHeld * 
		SQRHYPE)/2.0f : gHorizKeysHeld) * velocity;
	mPos.y += (gHorizKeysHeld != 0 ? (gVertKeysHeld * 
		SQRHYPE)/2.0f : gVertKeysHeld) * velocity;

	//Update animations...
	if (gHorizKeysHeld > 0) {
		moveRightTimer += animMoveSpeed * gDeltaTime;

		int index = (int)moveRightTimer % ANIM_RIGHT_COUNT;
		mSpriteClipIndex = animRightIndices[index];

		
		lastMoveIndex = animRightIndices[0];
		Entity::mFacingDirection = 3;
	}
	else if (gHorizKeysHeld < 0) {
		moveLeftTimer += animMoveSpeed * gDeltaTime;

		int index = (int)moveLeftTimer % ANIM_LEFT_COUNT;
		mSpriteClipIndex = animLeftIndices[index];

		lastMoveIndex = animLeftIndices[0];
		Entity::mFacingDirection = 2;
	}
	else if (gVertKeysHeld > 0) {
		moveDownTimer += animMoveSpeed * gDeltaTime;

		int index = (int)moveDownTimer % ANIM_DOWN_COUNT;
		mSpriteClipIndex = animDownIndices[index];

		lastMoveIndex = animDownIndices[0];
		Entity::mFacingDirection = 1;
	}
	else if (gVertKeysHeld < 0) {
		moveUpTimer += animMoveSpeed * gDeltaTime;

		int index = (int)moveUpTimer % ANIM_UP_COUNT;
		mSpriteClipIndex = animUpIndices[index];

		lastMoveIndex = animUpIndices[0];
		Entity::mFacingDirection = 0;
	}
	else {
		mSpriteClipIndex = lastMoveIndex;
	}
}

void Player::Attack() {
	//Update attack variables
	playerWeapon->SetDamage(attackDamage);
	//Update animation...
	if (attackTimer > 0.f) {

		attackTimer -= gDeltaTime;	//Updates timer...

		//TODO: replace all player animation with Animation class
		float time = 1.f - (attackTimer/attackTime);
		int index = (int)(time * ANIM_ATTACK_COUNT) % ANIM_ATTACK_COUNT;
		mSpriteClipIndex = animAttackLeftIndices[mFacingDirection][index];

	}	//Start animation...
	else if (gFirstKeyDown) {
		sdlInit.PlaySFX(sdlInit.sfxSlash01);
		attackTimer = attackTime;   
		SetCorners();
		weaponPos = FindWeaponPos();
		playerWeapon->SetPosition(weaponPos);
		playerWeapon->attacking = true;
	}
	else {
		playerWeapon->attacking = false;
	}
}