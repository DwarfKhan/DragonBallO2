#include "StatEffect.h"


extern float gDeltaTime;

StatEffect::StatEffect(char * name, float time) : name(name), time(time)
{
}


StatEffect::~StatEffect()
{
}

bool StatEffect::Update()
{
	time -= gDeltaTime;

	if (time < 0) {
	return false;
}
	else{
		return true;
	}
}

bool StatFunctions::StatCheck(std::vector<StatEffect> vec, char * name)
{
	for (int i = 0; i < vec.size(); ++i) {
		if (vec[i].name == name) {
			return true;
		}
	}
	return false;
}

bool StatFunctions::StatCheck(std::vector<StatEffect> vec, char * name, float & timeLeft)
{
	for (int i = 0; i < vec.size(); ++i) {
		if (vec[i].name == name) {
			timeLeft = vec[i].time;
			return true;
		}
	}
	return false;
}

void StatFunctions::UpdateStats(std::vector<StatEffect> &vec, int multiplier)
{
	for (int j = 0; j < multiplier; ++j) {
		for (int i = 0; i < vec.size(); ++i ) {
			if (vec.size() == 0) {
				return;
			}
			printf("%f\n",vec[i].time);
			if (!vec[i].Update()) {
				vec.erase(vec.begin()+(i));//we update each effect in the vector, and remove them if their timer is 0
			}
		}
	}
}
