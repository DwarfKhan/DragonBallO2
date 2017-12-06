#include "ProxTrigger.h"



ProxTrigger::ProxTrigger()
{
}


ProxTrigger::~ProxTrigger()
{
}

void ProxTrigger::OnCollision(Entity * other)
{
	LivingThing ltHolder;
	Player pHolder;
	if (other == nullptr)
	{
		return;
	}

	//If the prox trigger is following a LivingThing
	else if (typeid(*mFollowingEntity) == typeid(ltHolder)) {
		LivingThing * lFollowingEntity = (LivingThing *)mFollowingEntity;
		lFollowingEntity->OnProxCollision(other);
	}

	//If the prox trigger is following the Player
	else if (typeid(*mFollowingEntity) == typeid(pHolder)) {
		Player * pFollowingEntity = (Player *)mFollowingEntity;
		pFollowingEntity->OnProxCollision(other);
	}
	Entity::OnCollision(other);
}

void ProxTrigger::Update()
{
	Follow();
	Entity::Update();
}

void ProxTrigger::Follow() //Sets position to be centered on mFollowingEntity
{
	mPos.x = (mFollowingEntity->GetCenterPos().x) - (mSize.x/2);
	mPos.y = (mFollowingEntity->GetCenterPos().y) - (mSize.y / 2);
}

void ProxTrigger::SetFollowingEntity(Entity &ent)
{
	mFollowingEntity = &ent;
}
