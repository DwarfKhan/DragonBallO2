#include "LivingThing.h"
#include "Player.h"
#include "Sprite.h"
#include "MyMath.h"

extern float gDeltaTime;
extern Camera gCamera;
extern SDLInit sdlInit; //needed for sound effects


using namespace MyMath;
using Float2 = MyMath::Float2;


LivingThing::LivingThing()
{
}


LivingThing::~LivingThing()
{
}

void LivingThing::OnCollision(Entity * other)
{



	Entity::OnCollision(other);
}

void LivingThing::OnProxCollision(Entity * other) //Gets called by a ProxTrigger
{

}

void LivingThing::Update()
{

	Death();
	Move();
	Sprite::Update();
	Animate();
}

void LivingThing::Move()
{
	if (!mIsAlive)
	{
		return;
	}

	MyMath::Float2 difPos;

	switch (moveState) {

	case sNotMoving:
		moveDir = none;
		mFollowVector = { 0,0 };
		return;
		break;

	case sRandom:


		mRandomNavTimer -= gDeltaTime;
		//printf("navTimer: %f\n", randomNavTimer);

		if (mRandomNavTimer <= 0)
		{
		//choose a new direction
			if(DiceRoll(0, 1) == 0) {
				mFollowVector = 0;
			}
			else {
				//set x and y to random values between -1 and 1
				mFollowVector.x = (((float)DiceRoll(0, 200) / 100.0f) - 1.0f);
				mFollowVector.y = (((float)DiceRoll(0, 200) / 100.0f) - 1.0f);

				//Normalize (set length of follow vector to 1)
				MyMath::Normalize(mFollowVector);
			}

			//reset timer
			float newTime = (
				((float)DiceRoll(mRandomNavMinTime, mRandomNavMaxTime))
				/10.0f);
				mRandomNavTimer = newTime;

		}

		break;

	case sDirectFollow:

		//making sure there is a target to follow
		if (mFollowTarget == nullptr) {
			printf("No target selected to follow, returning to default movestate.\n");
			moveState = defaultMoveState;
			break;
		}

		difPos = mFollowTarget->GetPos() - mPos;
		
		if (Mag(difPos) > awareDist)//if target is outside aware distance
		{
			mAlertSoundHasPlayed = false;
			mFollowVector = { 0,0 };
			moveDir = none;
			break;
		}
		else if (Mag(difPos) < attackDist)//if target is within attack range
		{
			mFollowVector = { 0,0 };
			moveDir = none;
			break;
		}


		if (mAlertSoundHasPlayed == false) { //alert sound plays once when target enters awaredist (instead of spamming the sound effect)
			mAlertSoundHasPlayed = true;
			sdlInit.PlaySFX(mAlertSound, 80);
		}

		mFollowVector = difPos;
		Normalize(mFollowVector);

		break;

	default:
		printf("Invalid movestate, returning to default movestate.\n");
		moveState = defaultMoveState;

		break;

	}



	MyMath::Float2 newPos = mPos; //making some temporary values to be assigned to mPos and animState at the end
		AnimState tempState;


		if (animState == AnimState::sDamage && mAnimDamage->active == true) {//damage animation has top priority
			tempState = sDamage;
		}
		else if (attackState != sNotAttacking) { //attcking has priortiy over movement
			tempState = sAttack;
			newPos += (mFollowVector * mAttackVelocity * gDeltaTime);
		}
		else {

		//Display idle if not moving
		if (mFollowVector.x == 0 && mFollowVector.y == 0)
		{
			tempState = sIdle;
		}
		else {

			if (mWalkSoundTimer > 0) { //making a walk sound effect every so often based on move speed
				mWalkSoundTimer -= gDeltaTime * mMoveSpeed;
				//printf("%f\n", mWalkSoundTimer);
			}
			else {
				sdlInit.PlaySFX(mWalkSound,10);
				mWalkSoundTimer = mWalkSoundTime;
			}

			tempState = sMove;
			//printf("newPos before speed/time x: %f, y: %f.\n", newPos.x, newPos.y);
			newPos += (mFollowVector * mMoveSpeed * gDeltaTime);
			//printf("newPos after speed/time x: %f, y: %f.\n", newPos.x, newPos.y);

		}

		//Decide on facingDirection
		if (.1 + Abs(mFollowVector.x) >= Abs(mFollowVector.y)) {//Further in x distance
			if (mFollowVector.x >= 0) {//right
				mFacingDirection = 3;
			}
			else {//left
				mFacingDirection = 2;
			}
		}
		else {
			if (mFollowVector.y >=0) {//Down
				mFacingDirection = 1;
			}
			else {
				mFacingDirection = 0;
			}
		}
		}

		animState = tempState; //assigning the temp values to their destination
		mPos = newPos;
}

void LivingThing::Death()
{
	if ( (mHealth <= 0) && (mIsAlive) ) {
		mAnimDeath->active = true;
		mAnimIdle->active = false;
		mIsAlive = false;
		animState = sDeath;
		sdlInit.PlaySFX(mDeathSound);
	}
}

void LivingThing::Animate()
{
	bool finished;

	switch(animState) {

	case sIdle: // if (animState == sIdle)
		mAnimIdle->UpdateSpriteClipIndex(mSpriteClipIndex);
		break;

	case sDamage:
		finished = mAnimDamage->UpdateSpriteClipIndex(mSpriteClipIndex);
		if (finished) {
			animState = sIdle;
			mAnimIdle->active = true;
		}
		break;

	case sDeath:
		finished = mAnimDeath->UpdateSpriteClipIndex(mSpriteClipIndex);
		//if (finished) {
		//	sdlInit.PlaySFX(mDeathSound);
		//}
		break;

	case sMove:
		switch (mFacingDirection) {

		case 0:

		mAnimMoveUp->active = true;
		finished = mAnimMoveUp->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;

		case 1:

			mAnimMoveDown->active = true;
			finished = mAnimMoveDown->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;

		case 2:
			mAnimMoveLeft->active = true;
			finished = mAnimMoveLeft->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;

		case 3:
			mAnimMoveRight->active = true;
			finished = mAnimMoveRight->UpdateSpriteClipIndex(mSpriteClipIndex);
			break;

		}
		if (finished) {
			animState = sIdle;
			mAnimIdle->active = true;
		}

		break;

	case sDisplayAll:
		mAnimDisplayAll->UpdateSpriteClipIndex(mSpriteClipIndex);
		printf("Displaying: %d\n", mSpriteClipIndex);
		break;
	}
}

void LivingThing::Attack()
{
	if (mFollowTarget == nullptr) { //making sure there is a target before attacking
		attackState = AttackState::sNotAttacking;
		return;
	}

	float distance = Mag(mFollowTarget->GetPos() - mPos);

	if (distance > attackDist) { //making sure target is in range
		attackState = AttackState::sNotAttacking;
	}

	switch (attackState)
	{
	case sNotAttacking:
		break;
	case sAttack1:
		break;
	}

}

bool LivingThing::TakeDamage(int damage)
{
	if (mHealth - damage > 0) {
	mAnimDamage->active = true;
	animState = sDamage;
	sdlInit.PlaySFX(mDamageSound);
	}
	printf("HP: %d \n", (mHealth - damage));
	return Destructible::TakeDamage(damage);
}

void LivingThing::SetFollowTarget(Entity * target)
{

	mFollowTarget = target;

}

void LivingThing::SetAnimDisplayAll(Animation * anim)
{
	mAnimDisplayAll = anim;
}

void LivingThing::SetAnimDamage(Animation * anim)
{
	mAnimDamage = anim;
}

void LivingThing::SetAnimIdle(Animation* anim)
{
	mAnimIdle = anim;
}

void LivingThing::SetAnimDeath(Animation * anim)
{
	mAnimDeath = anim;
}

void LivingThing::SetAnimMoveUp(Animation * anim)
{
	mAnimMoveUp = anim;
}

void LivingThing::SetAnimMoveDown(Animation * anim)
{
	mAnimMoveDown = anim;
}

void LivingThing::SetAnimMoveLeft(Animation * anim)
{
	mAnimMoveLeft = anim;
}

void LivingThing::SetAnimMoveRight(Animation * anim)
{
	mAnimMoveRight = anim;
}

void LivingThing::SetAnimAttackUp(Animation * anim)
{
	mAnimAttackUp = anim;
}

void LivingThing::SetAnimAttackDown(Animation * anim)
{
	mAnimAttackDown = anim;
}

void LivingThing::SetAnimAttackLeft(Animation * anim)
{
	mAnimAttackLeft = anim;
}

void LivingThing::SetAnimAttackRight(Animation * anim)
{
	mAnimAttackRight = anim;
}

void LivingThing::SetDeathSound(Mix_Chunk * sound)
{
	mDeathSound = sound;
}

void LivingThing::SetWalkSound(Mix_Chunk * sound)
{
	mWalkSound = sound;
}

void LivingThing::SetDamageSound(Mix_Chunk * sound)
{
	mDamageSound = sound;
}

void LivingThing::SetAlertSound(Mix_Chunk * sound)
{
	mAlertSound = sound;
}

void LivingThing::SetAttackSound(Mix_Chunk * sound)
{
	mAttackSound = sound;
}



// TEMPLATE
	/*
	1: declare in declerations namespace
	2: Cleanup
	3: Update
	4: render
	*/
// LIVING THING 
	//health

	//position

	//size

	//collision

	//navigation

	//Path name

	//Load Texture

	//Init sprite sheet

	//Spriteclips

	//Anchor offsets

	//Animation

// END LIVING THING 