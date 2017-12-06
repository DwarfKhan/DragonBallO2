#include "MyMath.h"
#include <random>
#include <time.h>

extern float gRandomizer;
using Float2 = MyMath::Float2;



int MyMath::Abs(int num) {
	return num < 0 ? -num : num;
}

float MyMath::Abs(float num) {
	return num < 0.0f ? -num : num;
}

float MyMath::Lerp(float start, float end, float time) {
	if (time > 1) {
		return end;
	}

	float value = start + ((end - start) * time);
	return value;
}

//returns a random int between min and max
int MyMath::DiceRoll(int min, int max)
{
	if (min > max)
	{
		return 0;
	}
	if (min == max)
	{
		return min;
	}
	gRandomizer++;
	srand(gRandomizer * time(NULL));
	//printf("DiceRoll. Min: %d. Max: %d. Returning: %d.\n", min, max, (rand() % ((max - min) + 1) + min));
	return rand() % ((max - min) + 1) + min;
}

void MyMath::Normalize(Float2 &vector)
{
	float cSq = (vector.x * vector.x) + (vector.y * vector.y);
	float length = Abs((float)pow(cSq, .5));
	MyMath::Float2 ans;
	ans.x = vector.x / length;
	ans.y = vector.y / length;
	//printf("Normalize. Starting x: %f, y: %f. Normalized x: %f, y: %f.\n", vector.x, vector.y, ans.x, ans.y);
	vector = ans;
}

float MyMath::Mag(Float2 vector)
{
	float ans = pow(vector.x, 2) + pow(vector.y, 2); //a^2 + b^2 = c^2
	ans = pow(ans, .5); // this converts c^2 back to c (hypottenuse)
	//printf("Mag. ans: %f.\n",ans);
	return ans;
}

int MyMath::Max(int leftNum, int rightNum) {
	return leftNum > rightNum ? leftNum : rightNum;
}
float MyMath::Max(float leftNum, float rightNum) {
	return leftNum > rightNum ? leftNum : rightNum;
}

int MyMath::Min(int leftNum, int rightNum) {
	return leftNum < rightNum ? leftNum : rightNum;
}
float MyMath::Min(float leftNum, float rightNum) {
	return leftNum < rightNum ? leftNum : rightNum;
}

int MyMath::Clamp(int value, int min, int max) {
	return value > max ? max :
		value < min ? min : value;
}
float MyMath::Clamp(float value, float min, float max) {
	return value > max ? max :
		value < min ? min : value;
}