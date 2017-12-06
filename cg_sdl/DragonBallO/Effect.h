#pragma once
#include "Sprite.h"
class Effect :
	public Sprite
{
public:
	Effect();
	~Effect();
	void Update() override;

	bool temporary = false;
	float mTimer;
	MyMath::Float2 mVelocity = {0,0};
private:
};

