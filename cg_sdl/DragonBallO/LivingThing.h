#pragma once
#include "Sprite.h"
#include"Destructible.h"
#include "Weapon.h"
#include "SDLinit.h"
#include "DisplayAnimation.h"



class LivingThing :
	public Sprite,
	public Destructible
{
public:
	LivingThing();
	~LivingThing();

	void Update() override;
	void OnCollision(Entity *other) override;
	void OnProxCollision(Entity *other);
	void Move();
	void Animate();
	void Attack();
	void Death();
	bool TakeDamage(int damage); // Not sure why override doesnt work here...
	void SetFollowTarget(Entity *target);

	MyMath::Float2 FindWeaponPos();

	void SetWeapon(Weapon *wep, float range, int damage = 1);

	void SetAnimDamage(Animation *anim);
	void SetAnimIdle(Animation *anim);
	void SetAnimDeath(Animation *anim);
	void SetAnimMoveUp(Animation *anim);
	void SetAnimMoveDown(Animation *anim);
	void SetAnimMoveLeft(Animation *anim);
	void SetAnimMoveRight(Animation *anim);
	void SetAnimAttackUp(Animation *anim);
	void SetAnimAttackDown(Animation *anim);
	void SetAnimAttackLeft(Animation *anim);
	void SetAnimAttackRight(Animation *anim);

	void SetDeathSound(Mix_Chunk * sound);
	void SetWalkSound(Mix_Chunk * sound);
	void SetDamageSound(Mix_Chunk * sound);
	void SetAlertSound(Mix_Chunk * sound);
	void SetAttackSound(Mix_Chunk * sound);

	void SetSpriteClip(int x, int y, unsigned int w, unsigned int h, unsigned int index) override;


	enum AnimState {sIdle, sDeath, sDamage, sMove, sAttack, sDisplayAll};
	AnimState animState = sIdle; //determines which animation is displayed
	enum MoveState {sNotMoving, sDirectFollow, sRandom};
	MoveState defaultMoveState = sNotMoving;
	MoveState moveState; // determines how the living thing moves (will be ignored if attacking)
	enum AttackState {sNotAttacking, sAttack1};
	AttackState attackState = sNotAttacking; // determines weather the living thing is attacking (higher priority than moveState)


	int attackDamage;
	float attackDist = 60.0f; //how close LivingThing will get to mFollowTarget before stopping
	float attackRange = 40.0f;
	float awareDist = 250.0f; //how close mFollowTarget must be in order to follow

	bool isHostile = true;

private:
	void WalkSound();




protected:

	float mRandomNavTimer;
	float mRandomNavMaxTime = 18.0f;
	float mRandomNavMinTime = 10.0f;
	float mWalkSoundTimer;
	float mWalkSoundTime = 54.5f;

	float mAttackVelocity;
	float attackTimer;
	float attackTime = 1.8f;


	
	AnimState moveTempState;
	AnimState attackTempState;
	AnimState damageTempState;
	AnimState deathTempState;





	bool mIsAlive = true;
	bool mAlertSoundHasPlayed = false;

	DisplayAnimation mAnimDisplayAll;
	Animation *mAnimIdle;
	Animation *mAnimDamage;
	Animation *mAnimDeath;
	Animation *mAnimMoveUp;
	Animation *mAnimMoveDown;
	Animation *mAnimMoveLeft;
	Animation *mAnimMoveRight;
	Animation *mAnimAttackUp;
	Animation *mAnimAttackDown;
	Animation *mAnimAttackLeft;
	Animation *mAnimAttackRight;

	Mix_Chunk *mDeathSound;
	Mix_Chunk *mWalkSound; //TODO:  make use of these
	Mix_Chunk *mDamageSound;
	Mix_Chunk *mAlertSound;
	Mix_Chunk *mAttackSound;

	Entity * mFollowTarget;
	MyMath::Float2 mFollowVector;

	Weapon *mWeapon;

};





