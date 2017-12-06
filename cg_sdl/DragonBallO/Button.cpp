#include "Button.h"



Button::Button()
{
}


Button::~Button()
{
}

void Button::Update()
{

	Entity::Update();
}

void Button::OnCollision(Entity * other)
{

	Entity::OnCollision(other);
}
