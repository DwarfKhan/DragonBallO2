#pragma once
#include <vector>

class StatEffect
{
public:
	StatEffect(char * name, float time);
	~StatEffect();
	bool Update();





	char * name;
	float time;
};

	namespace StatFunctions {
	bool StatCheck(std::vector<StatEffect> vec, char * name);
	bool StatCheck(std::vector<StatEffect> vec, char * name, float &timeLeft);
	void UpdateStats(std::vector<StatEffect> &vec,int multiplier = 1);
	}
