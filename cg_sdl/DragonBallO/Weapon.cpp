#include "Weapon.h"
#include "LivingThing.h"

LivingThing livingThingTypeHolder;

Weapon::Weapon()
{
}



Weapon::~Weapon()
{
}

void Weapon::SetDamage(int damage)
{
	attackDamage = damage;
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

	if (typeid(*other) != typeid(livingThingTypeHolder)) {
		attacking = false;
		Entity::OnCollision(other);
		return;
	}
	printf("Weapon hit LivingThing.\n");
	LivingThing* lOther = (LivingThing *)other;
	lOther->TakeDamage(attackDamage);

	attacking = false;
	Entity::OnCollision(other);
}




