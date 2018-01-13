#include "StatEffect.h"



StatEffect::StatEffect(char * name, float time) : name(name), time(time)
{
}


StatEffect::~StatEffect()
{
}

bool StatEffect::Update(float deltaTime)
{
	time -= deltaTime;

	if (time < 0) {
	return false;
}
	else{
		return true;
	}
}
