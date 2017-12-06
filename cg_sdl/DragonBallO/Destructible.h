#pragma once

class Destructible {
public:
	//Base class destructor is ALWAYS implicitly called...
	virtual ~Destructible() {}

public:
	void SetHealth(int health, int maxHealth);
	bool TakeDamage(int damage);

protected:
	int mMaxHealth{100};
	int mHealth{100};

	//TODO: add typedef struct for inventory...
	//TODO: struct for weapons to attach and give damage...
};