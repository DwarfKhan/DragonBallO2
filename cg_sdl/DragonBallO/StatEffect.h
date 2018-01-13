#pragma once
class StatEffect
{
public:
	StatEffect(char * name, float time);
	~StatEffect();
	bool Update(float deltaTime);

	char * name;
	float time;
};

