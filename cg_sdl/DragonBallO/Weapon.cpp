#include "Weapon.h"
#include "LivingThing.h"

LivingThing livingThingTypeHolder;

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

	if (typeid(*other) != typeid(livingThingTypeHolder)) {//weapon can collide with non living things (buttons)
		attacking = false;
		Entity::OnCollision(other);
		return;
	}

	printf("Weapon hit LivingThing.\n");
	LivingThing* lOther = (LivingThing *)other;
	lOther->TakeDamage(*attackDamage);
	//insert special weapon effects here


	attacking = false;
	Entity::OnCollision(other);
}




