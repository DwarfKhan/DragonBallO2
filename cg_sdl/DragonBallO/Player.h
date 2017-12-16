#pragma once
#include "Sprite.h"
#include "Destructible.h"
#include "Weapon.h"
#include "DisplayAnimation.h"

class Player : public Sprite, public Destructible {
public:
	//Base class destructor is ALWAYS implicitly called...
	Player() { mCanBePushedBack = false; }
	virtual ~Player() override {}
	void Update() override;
	void SetWeapon(Weapon *weapon, int range = 1, int damage = 1);
	void PrintPos();
	void OnProxCollision(Entity *other);
	MyMath::Float2 FindWeaponPos();

	void SetSpriteClip(int x, int y, unsigned int w, unsigned int h, unsigned int index) override;//used for automatic display animations
	void SetAnimDamage(Animation *anim);
	void SetAnimIdle(Animation *anim);
	void SetAnimDeath(Animation *anim);
	void SetAnimMove(Animation *anim, int dir = 0);
	void SetAnimAttack(Animation *anim, int dir = 0);


	int lastMoveIndex;
	int attackRange;
	int attackDamage;
	Weapon *playerWeapon;
	int xDirMultiplier;
	int yDirMultiplier;
	MyMath::Float2 weaponPos;


private:
	void Move();
	void Attack();


	//TODO: use these instead of doing anim math every time...
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
};