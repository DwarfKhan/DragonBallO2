#pragma once
#include "LivingThing.h"
#include "Player.h"
class ProxTrigger :
	public Entity
{
public:
	ProxTrigger();
	~ProxTrigger();
	void OnCollision(Entity* other) override;
	void Update() override;
	void Follow();
	void SetFollowingEntity(Entity &ent);


protected:
	Entity * mFollowingEntity;
};

