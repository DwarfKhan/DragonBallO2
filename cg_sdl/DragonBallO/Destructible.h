#pragma once

class Destructible {
public:
	//Base class destructor is ALWAYS implicitly called...
	virtual ~Destructible() {}

public:
	void SetHealth(int health, int maxHealth);
	virtual bool TakeDamage(int damage);

protected:
	int mMaxHealth{5};
	int mHealth{5};

	//TODO: add typedef struct for inventory...
	//TODO: struct for weapons to attach and give damage...
};