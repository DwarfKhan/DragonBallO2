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
extern bool gSpaceDown;     //key Space
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
	//printf("HP:%d\n", mHealth);
	StatFunctions::UpdateStats(statEffects);
	Death();
	Dodge();
	Move();
	PrintPos();
	Attack();
	Animate();
	Sprite::Update();
}

bool Player::TakeDamage(int damage)
{

	if (StatFunctions::StatCheck(statEffects, "invincible")) {
		return false;
	}
	if (mHealth - damage > 0) {
		sdlInit.PlaySFX(mDamageSound);
		damageTempState = sDamage;
	}
	printf("PLayer HP: %d\n", mHealth - damage);
	return Destructible::TakeDamage(damage);
}

void Player::Death()
{
	if (!isAlive) {
		return;
	}
	if (mHealth > 0) {
		return;
	}
	isAlive = false;
	sdlInit.PlaySFX(mDeathSound);
	deathTempState = AnimState::sDeath;
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
	if (mFacingDirection == 0) {
		position.x = ((topLeftCornerPos.x + topRightCornerPos.x) / 2) - (wepSize.x / 2);
		position.y = topLeftCornerPos.y - (wepSize.y + (attackRange + 1));
		//													    	  /\
		//								Not sure why this 1 is needed but it was the only way to get it to look right...
	}
	else if (mFacingDirection == 1) {
		position.x = ((topLeftCornerPos.x + topRightCornerPos.x) / 2) - (wepSize.x/2);
		position.y = bottomLeftCornerPos.y + attackRange;
	}
	else if (mFacingDirection == 2) {
		position.y = ((topLeftCornerPos.y + bottomLeftCornerPos.y) / 2) - (wepSize.y/2);
		position.x = topLeftCornerPos.x - (wepSize.x + attackRange);
	}
	else if (mFacingDirection == 3) {
		position.y = ((topLeftCornerPos.y + bottomLeftCornerPos.y) / 2) - (wepSize.y / 2);
		position.x = topRightCornerPos.x + attackRange;
	}
	return position;
}

void Player::Move() {
	if (!isAlive) {
		return;
	}
	//If we are attacking we want to stop movement...
	if (attackTimer > 0.f) {
		return;
	}

	//Setting velocity...
	float velocity = mMoveSpeed * gDeltaTime;
	if (StatFunctions::StatCheck(statEffects, "fast")) {
		velocity *= 3.0f;
	}

	//Updates position. SQRHYPE is used so diagnal direction is NOT faster...
	mPos.x += (gVertKeysHeld != 0 ? (gHorizKeysHeld * 
		SQRHYPE)/2.0f : gHorizKeysHeld) * velocity;
	mPos.y += (gHorizKeysHeld != 0 ? (gVertKeysHeld * 
		SQRHYPE)/2.0f : gVertKeysHeld) * velocity;

	//Update animations...
	if (gHorizKeysHeld > 0) {
		moveTempState = sMove;
		Entity::mFacingDirection = 3;
	}
	else if (gHorizKeysHeld < 0) {
		moveTempState = sMove;
		Entity::mFacingDirection = 2;
	}
	else if (gVertKeysHeld > 0) {
		moveTempState = sMove;
		Entity::mFacingDirection = 1;
	}
	else if (gVertKeysHeld < 0) {
		moveTempState = sMove;
		Entity::mFacingDirection = 0;
	}
	else {
		moveTempState = sIdle;
	}
}

void Player::Dodge()
{
	if (dodgeTimer > 0) {
		dodgeTimer -= gDeltaTime;
		return;
	}
	if (!gSpaceDown) {
		return;
}
	dodgeTimer = dodgeTime;
	StatEffect dodgeHP("invincible", 0.3f);
	statEffects.push_back(dodgeHP);

	StatEffect dodgeSP("fast", 0.3f);
	statEffects.push_back(dodgeSP);
}

void Player::Attack() {
	if (!isAlive) {
		return;
	}
	if (damageTempState != sIdle) {
		return;
	}
	//Update attack variables
	playerWeapon->SetDamage(attackDamage);
	//Update animation...
	if (attackTimer > 0.f) {
		attackTempState = sAttack;
		attackTimer -= gDeltaTime;	//Updates timer...
	}	//Start animation...
	else if (gFirstKeyDown) {
		attackTempState = sAttack;
		sdlInit.PlaySFX(sdlInit.sfxSlash01);
		attackTimer = attackTime;   
		SetCorners();
		weaponPos = FindWeaponPos();
		playerWeapon->SetPosition(weaponPos);
		playerWeapon->attacking = true;
	}
	else {
		attackTempState = sIdle;
		playerWeapon->attacking = false;
	}
}

void Player::Animate()
{
	if (deathTempState != sIdle) {
		animState = deathTempState;
	}
	else if (animState == sDisplayAll) {
		//no change needed
	}
	else if (damageTempState != sIdle) {
		animState = damageTempState;
	}
	else if (attackTempState != sIdle) {
		animState = attackTempState;
	}
	else {
		animState = moveTempState;
	}

	bool finished = false;

	switch (animState)
	{
	default:
	case Player::sIdle:
		switch (mFacingDirection)
		{
		case 0:
			mAnimIdleUp->active = true;
			mAnimIdleUp->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;
		case 1:
			mAnimIdleDown->active = true;
			mAnimIdleDown->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;
		case 2:
			mAnimIdleLeft->active = true;
			mAnimIdleLeft->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;
		case 3:
			mAnimIdleRight->active = true;
			mAnimIdleRight->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;
		}
		break;
	case Player::sDeath:
		mAnimDeath->active = true;
		mAnimDeath->UpdateSpriteClipIndex(mSpriteClipIndex);
		break;
	case Player::sDamage:
		mAnimDamage->active = true;
		finished = mAnimDamage->UpdateSpriteClipIndex(mSpriteClipIndex);
		if (finished) {
			damageTempState = sIdle;
		}
		break;
	case Player::sMove:
		switch (mFacingDirection)
		{
		case 0:
			mAnimMoveUp->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;
		case 1:
			mAnimMoveDown->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;
		case 2:
			mAnimMoveLeft->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;
		case 3:
			mAnimMoveRight->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;
		}
		break;
	case Player::sAttack:
		switch (mFacingDirection)
		{
		case 0:
			mAnimAttackUp->active = true;
			finished = mAnimAttackUp->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;
		case 1:
			mAnimAttackDown->active = true;
			finished = mAnimAttackDown->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;
		case 2:
			mAnimAttackLeft->active = true;
			finished = mAnimAttackLeft->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;
		case 3:
			mAnimAttackRight->active = true;
			finished = mAnimAttackRight->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;
		}
		if (finished) {
			attackTempState = sIdle;
		}
		break;

	case Player::sDisplayAll:
		mAnimDisplayAll.UpdateSpriteClipIndex(mSpriteClipIndex);
		break;
	}

}

void Player::SetWeapon(Weapon *weapon, int range, int damage)
{
	playerWeapon = weapon;
	attackRange = range;
	attackDamage = damage;
	playerWeapon->SetDamage(damage);
}

void Player::SetSpriteClip(int x, int y, unsigned int w, unsigned int h, unsigned int index)
{
	mAnimDisplayAll.AddSpriteClip(index);//automatic display animation filling
	Sprite::SetSpriteClip(x, y, w, h, index);
}

void Player::SetAnimDamage(Animation * anim)
{
	mAnimDamage = anim;
}

void Player::SetAnimIdle(Animation * anim, int dir)
{
	switch (dir)
	{
	default:
		mAnimIdleUp = anim;
		break;
	case 1:
		mAnimIdleDown = anim;

		break;
	case 2:
		mAnimIdleLeft = anim;

		break;
	case 3:
		mAnimIdleRight = anim;

		break;
	}
}

void Player::SetAnimDeath(Animation * anim)
{
	mAnimDeath = anim;
}

void Player::SetAnimMove(Animation * anim, int dir)
{
	switch (dir)
	{
	default:
		mAnimMoveUp = anim;
		break;
	case 1:
		mAnimMoveDown = anim;

		break;
	case 2:
		mAnimMoveLeft = anim;

		break;
	case 3:
		mAnimMoveRight = anim;

		break;
	}
}

void Player::SetAnimAttack(Animation * anim, int dir)
{
	switch (dir)
	{
	default:
		mAnimAttackUp = anim;
		break;
	case 1:
		mAnimAttackDown = anim;

		break;
	case 2:
		mAnimAttackLeft = anim;

		break;
	case 3:
		mAnimAttackRight = anim;

		break;
	}
}

void Player::SetDamageSound(Mix_Chunk * sound)
{
	mDamageSound = sound;
}

void Player::SetDeathSound(Mix_Chunk * sound)
{
	mDeathSound = sound;
}
