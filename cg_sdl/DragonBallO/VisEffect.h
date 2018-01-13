#pragma once
#include "Sprite.h"
class VisEffect :
	public Sprite
{
public:
	VisEffect();
	~VisEffect();
	void Update() override;

	bool temporary = false;
	float mTimer;
	MyMath::Float2 mVelocity = {0,0};
private:
};

