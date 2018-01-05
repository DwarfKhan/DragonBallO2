#pragma once
#include "Sprite.h"
class Item :
	public Sprite
{
public:
	Item();
	~Item();
	void OnCollision(Entity *other) override;

};

