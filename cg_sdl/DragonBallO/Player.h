#pragma once
#include "Sprite.h"
#include "Destructible.h"
#include "Weapon.h"
#include "DisplayAnimation.h"
#include "SDL_mixer.h"
#include "StatEffect.h"

class Player : public Sprite, public Destructible {
public:
	//Base class destructor is ALWAYS implicitly called...
	Player() { mCanBePushedBack = false; }
	virtual ~Player() override {}
	void Update() override;
	bool TakeDamage(int damage) override;
	void Death();
	void SetWeapon(Weapon *weapon, int range = 1, int damage = 1);
	void PrintPos();
	void OnProxCollision(Entity *other);
	MyMath::Float2 FindWeaponPos();

	void SetSpriteClip(int x, int y, unsigned int w, unsigned int h, unsigned int index) override;//used for automatic display animations
	void SetAnimDamage(Animation *anim);
	void SetAnimIdle(Animation *anim, int dir = 0);
	void SetAnimDeath(Animation *anim);
	void SetAnimMove(Animation *anim, int dir = 0);
	void SetAnimAttack(Animation *anim, int dir = 0);

	void SetDamageSound(Mix_Chunk * sound);
	void SetDeathSound(Mix_Chunk * sound);



	int lastMoveIndex;
	int attackRange;
	int attackDamage;
	Weapon *playerWeapon;
	int xDirMultiplier;
	int yDirMultiplier;
	MyMath::Float2 weaponPos;
	bool isAlive = true;

	enum AnimState { sIdle, sDeath, sDamage, sMove, sAttack, sDisplayAll };
	AnimState animState = sIdle; //determines which animation is displayed
	enum MoveState { sNotMoving, sMoving };
	MoveState defaultMoveState = sNotMoving;
	MoveState moveState; // determines how the living thing moves (will be ignored if attacking)
	enum AttackState { sNotAttacking, sAttack1 };
	AttackState attackState = sNotAttacking; // determines weather the living thing is attacking (higher priority than moveState)

private:
	void Move();
	void Dodge();
	void Attack();
	void Animate();

	//TODO: use these instead of doing anim math every time...
	DisplayAnimation mAnimDisplayAll;
	Animation *mAnimIdleUp;
	Animation *mAnimIdleDown;
	Animation *mAnimIdleLeft;
	Animation *mAnimIdleRight;
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

	Mix_Chunk *mDamageSound;
	Mix_Chunk *mDeathSound;
	Mix_Chunk *mWalkSound;
	Mix_Chunk *mAttackSound;

	AnimState moveTempState;
	AnimState attackTempState;
	AnimState damageTempState;
	AnimState deathTempState;
};