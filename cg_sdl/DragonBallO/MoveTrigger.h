#pragma once
#include "Trigger.h"
#include "Camera.h"

class MoveTrigger : public Trigger
{
public:
	MoveTrigger() {};
	~MoveTrigger() {};

	void SetMovePos(Float2 movePos);

protected:
	Float2 mMovePos {0, 0};
	void OnCollision(Entity *other) override;
};

