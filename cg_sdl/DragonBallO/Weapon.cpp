#include "Weapon.h"
#include "LivingThing.h"
#include "Player.h"

LivingThing livingThingTypeHolder;
Player playerTypeHolder;

Weapon::Weapon()
{
}



Weapon::~Weapon()
{
}

void Weapon::SetDamage(int &damage)
{
	attackDamage = &damage;
}

void Weapon::Update()
{
	Entity::Update();
}

void Weapon::OnCollision(Entity * other)
{
	if (!attacking) { 
		return;
	}

	if (typeid(*other) == typeid(livingThingTypeHolder)) {//weapon can collide with non living things (buttons)
	printf("Weapon hit LivingThing.\n");
	LivingThing* lOther = (LivingThing *)other;
	lOther->TakeDamage(*attackDamage);
	//insert special weapon effects here
	}

	if (typeid(*other) == typeid(playerTypeHolder)) {//weapon can collide with non living things (buttons)
		printf("Weapon hit Player.\n");
		Player* pOther = (Player *)other;
		pOther->TakeDamage(*attackDamage);
		//insert special weapon effects here
	}


	attacking = false;
	Entity::OnCollision(other);
}




