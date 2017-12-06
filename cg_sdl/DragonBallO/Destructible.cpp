#include "Destructible.h"

void Destructible::SetHealth(int health, int maxHealth) {
	mHealth = health;
	mMaxHealth = maxHealth;
}

bool Destructible::TakeDamage(int damage) {
	mHealth -= damage;
	bool hasDied = mHealth <= 0;

	return hasDied;
}