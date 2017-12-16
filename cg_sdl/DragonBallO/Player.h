#pragma once
#include "Sprite.h"
#include "Destructible.h"
#include "Weapon.h"

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
};