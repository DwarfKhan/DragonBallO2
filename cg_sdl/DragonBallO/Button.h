#pragma once
#include "Entity.h"
class Button :
	public Entity
{
public:
	Button();
	~Button();
	void Update() override;
	void OnCollision(Entity *other) override;
};

