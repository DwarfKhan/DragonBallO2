#pragma once
#include "Entity.h"

class Weapon :
	public Entity
{
public:

	bool attacking;
	int attackDamage;



	Weapon();
	~Weapon();
	void SetDamage(int damage);
	void Update() override;
	void OnCollision(Entity *other) override;



};

