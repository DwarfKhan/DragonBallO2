#pragma once
#include "LivingThing.h"
#include "Player.h"
#include "Sprite.h"
#include "MyMath.h"
#include <stdio.h>

#define RECT_ALLIGNMENT_THRESHOLD 1.0f

extern float gDeltaTime;
extern Camera gCamera;
extern SDLInit sdlInit; //needed for sound effects


using namespace MyMath;
using Float2 = MyMath::Float2;
using UInt = unsigned int;



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
	Attack();
	Animate();
	Sprite::Update();
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
		mFollowVector = { 0,0 };
		return;
		break;

	case sRandom:


		mRandomNavTimer -= gDeltaTime;
		//printf("navTimer: %f\n", randomNavTimer);

		if (mRandomNavTimer <= 0)
		{
			//choose a new direction
			if (DiceRoll(0, 3) == 0) {
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
				/ 10.0f);
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
		//printf("distance = %f\n", Mag(difPos));


		if (Mag(difPos) > awareDist)//if target is outside aware distance
		{
			mAlertSoundHasPlayed = false;
			mFollowVector = { 0,0 };
			break;
		}
		else if (Mag(difPos) < attackDist)//if target is within attack range
		{
			mFollowVector = { 0,0 };
			break;
		}

	//once just outside of attack range we want to allign either vertically or horizontally with the target
		else if (Mag(difPos) < attackDist * 1.5f)
		{
			
			if (Abs(difPos.x) < RECT_ALLIGNMENT_THRESHOLD || Abs(difPos.y) < RECT_ALLIGNMENT_THRESHOLD) {
				//once alligned, we go back to direct follow.
			}
			else if (Abs(difPos.x) > Abs(difPos.y))
			{
				difPos.x = 0;
			}
			else if (Abs(difPos.x) <= Abs(difPos.y))
			{
				difPos.y = 0;
			}
		}
	
		mFollowVector = difPos;
		Normalize(mFollowVector);



		if (mAlertSoundHasPlayed == false) { //alert sound plays once when target enters awaredist (instead of spamming the sound effect)
			mAlertSoundHasPlayed = true;
			sdlInit.PlaySFX(mAlertSound, 80);
		}




		break;

	default:
		printf("Invalid movestate, returning to default movestate.\n");
		moveState = defaultMoveState;

		break;

	}



	MyMath::Float2 newPos = mPos; //making some temporary values to be assigned to mPos and animState at the end


		//Display idle if not moving
		if (mFollowVector.x == 0 && mFollowVector.y == 0)
		{
			moveTempState = sIdle;
		}
		else {

			WalkSound();
			moveTempState = sMove;
			newPos += (mFollowVector * mMoveSpeed * gDeltaTime);
		}

		//Decide on facingDirection

		mFacingDirection = MyMath::FindDirectionFromVector(mFollowVector);

		

		mPos = newPos; //assigning the temp values to their destination
}

void LivingThing::Death()
{
	if ( (mHealth <= 0) && (mIsAlive) ) {
		mAnimDeath->active = true;
		mAnimIdle->active = false;
		mIsAlive = false;
		deathTempState = sDeath;
		sdlInit.PlaySFX(mDeathSound);
	}
}

void LivingThing::Animate()
{
	bool finished;

	if (!mIsAlive) {//death animation highest priority
		animState = deathTempState;
	}
	else if (animState == AnimState::sDisplayAll) {
		//display all overrides all but death
	}
	else if (damageTempState != sIdle) {
		animState = damageTempState;
	}
	else if (attackTempState != AnimState::sIdle) {
		animState = attackTempState;
	}
	else {
		animState = moveTempState;
	}

	switch(animState) {

	case sIdle: // if (animState == sIdle)
		mAnimIdle->UpdateSpriteClipIndex(mSpriteClipIndex);
		break;

	case sAttack:
		switch (mFacingDirection) {

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
		if(finished) {
			attackTempState = sIdle;
		}
		//printf("%d\n", mSpriteClipIndex);
		break;

	case sDamage:
		finished = mAnimDamage->UpdateSpriteClipIndex(mSpriteClipIndex);
		if (finished) {
			damageTempState = sIdle;
			mAnimIdle->active = true;
		}
		break;

	case sDeath:
		finished = mAnimDeath->UpdateSpriteClipIndex(mSpriteClipIndex);
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
		mAnimDisplayAll.UpdateSpriteClipIndex(mSpriteClipIndex);
		break;
	}
}

void LivingThing::Attack()
{
	if (!mIsAlive) {
		return;
	}
	if (damageTempState != sIdle) {
		return;
	}
	if (mFollowTarget == nullptr) { //making sure there is a target before attacking
		attackState = AttackState::sNotAttacking;
		return;
	}
	else if (mWeapon == nullptr) {//making sure there is a weapon before attacking
		attackState = AttackState::sNotAttacking;
		return;
	}

	MyMath::Float2 dif = mFollowTarget->GetPos() - mPos;
	float distance = Mag(dif);

	if (distance > attackDist) { //making sure target is in range
		attackState = AttackState::sNotAttacking;
		attackTimer = attackTime/2.0f;
	}
	if(isHostile && distance <= attackDist) {
		attackState = AttackState::sAttack1;
	}

	mFacingDirection = MyMath::FindDirectionFromVector(dif);


	switch (attackState)
	{
	case sNotAttacking:
		attackTempState = sIdle;
		break;
	case sAttack1:
		if (attackTimer < 0.0f) {
				sdlInit.PlaySFX(mAttackSound);
				mWeapon->SetDamage(attackDamage);
				SetCorners();
				mWeapon->SetPosition(FindWeaponPos());
				printf("WeaponPos:%f, %f\n", FindWeaponPos().x, FindWeaponPos().y);
				mWeapon->attacking = true;
			attackTimer = attackTime;
			attackTempState = sAttack;
		}
		else if (attackTimer < warmUpTime) {//attack animation starts before weapon does damage for a warmup frame
			attackTempState = sAttack;
			attackTimer -= gDeltaTime;	//Updates timer...
		}
		else {
			mWeapon->attacking = false;
			attackTimer -= gDeltaTime;	//Updates timer...
				//printf("AttackTimer: %f\n", attackTimer);
		}
		break;
	}

}

bool LivingThing::TakeDamage(int damage)
{
	if (mHealth - damage > 0) {
	mAnimDamage->active = true;
	damageTempState = sDamage;
	sdlInit.PlaySFX(mDamageSound);
	}
	printf("HP: %d \n", (mHealth - damage));
	return Destructible::TakeDamage(damage);
}

void LivingThing::SetFollowTarget(Entity * target)
{

	mFollowTarget = target;

}

MyMath::Float2 LivingThing::FindWeaponPos()
{
		MyMath::Int2 wepSize = mWeapon->GetSize();
		MyMath::Float2 position = { 0,0 };
		if (mFacingDirection == 0) {
			position.x = ((topLeftCornerPos.x + topRightCornerPos.x) / 2) - (wepSize.x / 2);
			position.y = topLeftCornerPos.y - (wepSize.y + (attackRange + 1));
			//													    	  /\
					//								Not sure why this 1 is needed but it was the only way to get it to look right...
		}
		else if (mFacingDirection == 1) {
			position.x = ((topLeftCornerPos.x + topRightCornerPos.x) / 2) - (wepSize.x / 2);
			position.y = bottomLeftCornerPos.y + attackRange;
		}
		else if (mFacingDirection == 2) {
			position.y = ((topLeftCornerPos.y + bottomLeftCornerPos.y) / 2) - (wepSize.y / 2);
			position.x = topLeftCornerPos.x - (wepSize.x + attackRange);
		}
		else if (mFacingDirection == 3) {
			position.y = ((topLeftCornerPos.y + bottomLeftCornerPos.y) / 2) - (wepSize.y / 2);
			position.x = topRightCornerPos.x + attackRange;
		}
		return position;
}

void LivingThing::SetWeapon(Weapon * wep, float range, int damage)
{
	mWeapon = wep;
	attackRange = range;
	attackDamage = damage;
	mWeapon->SetDamage(attackDamage);
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

void LivingThing::SetSpriteClip(int x, int y, unsigned int w, unsigned int h, unsigned int index)
{
	mAnimDisplayAll.AddSpriteClip(index);
	Sprite::SetSpriteClip(x,y,w,h,index);
}

void LivingThing::WalkSound()
{
	if (mWalkSoundTimer > 0) { //making a walk sound effect every so often based on move speed
		mWalkSoundTimer -= gDeltaTime * mMoveSpeed;
		//printf("%f\n", mWalkSoundTimer);
	}
	else {
		sdlInit.PlaySFX(mWalkSound, 10);
		mWalkSoundTimer = mWalkSoundTime;
	}
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