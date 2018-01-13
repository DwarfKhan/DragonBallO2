#pragma once
class StatEffect
{
public:
	StatEffect(char * name, float time);
	~StatEffect();

	char * name;
	float time;
};

