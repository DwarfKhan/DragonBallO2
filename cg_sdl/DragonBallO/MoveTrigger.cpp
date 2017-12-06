#include "MoveTrigger.h"

extern Camera gCamera;

void MoveTrigger::OnCollision(Entity *other) {
	other->SetPosition(mMovePos);
	gCamera.SetPos(mMovePos);
	Entity::OnCollision(other);
}

void MoveTrigger::SetMovePos(Float2 movePos) {
	mMovePos = movePos;
}
