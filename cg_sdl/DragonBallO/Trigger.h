#pragma once
#include "Entity.h"

class Trigger : public Entity
{
public:
	Trigger() {};
	~Trigger() {};

protected:
	void OnCollision(Entity *other) = 0;
};

