#pragma once

namespace MyMath {
	using UByte = unsigned char;
	struct Int2 {
		Int2(){}
		Int2(int x, int y) : x(x), y(y) {}
		int x{0};
		int y{0};
		Int2 operator/(int other) {
			Int2 ans;
			ans.x = x / other;
			ans.y = y / other;
			return ans;
		}
		Int2 operator+(Int2 right) {
			Int2 result(x - right.x, y + right.y);
			return result;
		}
		Int2 operator-(Int2 right) {
			Int2 result(x - right.x, y - right.y);
			return result;
		}
		bool operator==(Int2 right) {
			if (x == right.x && y == right.y) {
				return true;
			}
			else {
				return false;
			}
		}
		bool operator!=(Int2 right) {
			if (x == right.x && y == right.y) {
				return false;
			}
			else {
				return true;
			}
		}
	};
	struct Float2 {
		Float2(){}
		Float2(float x, float y) : x(x), y(y) {}
		float x{0.f};
		float y{0.f};

		void operator=(Float2 other) {
			x = other.x;
			y = other.y;
		}

		void operator+=(Float2 other) {
			x += other.x;
			y += other.y;
		}
		void operator-=(Float2 other) {
			x -= other.x;
			y -= other.y;
		}

		void operator=(float other) {
			x = other;
			y = other;
		}

		Float2 operator+(Float2 other) {
			Float2 ans = { x,y };
			ans.x += other.x;
			ans.y += other.y;
			return ans;
		}

		Float2 operator-(Float2 other) {
			Float2 ans = {x,y};
			ans.x -= other.x;
			ans.y -= other.y;
			return ans;
		}
		Float2 operator*(float other) {
			Float2 ans = { x,y };
			ans.x *= other;
			ans.y *= other;
			return ans;
		}
		Float2 operator/(float other) {
			Float2 ans = { x,y };
			ans.x /= other;
			ans.y /= other;
			return ans;
		}



	};
	struct RectBoundary {
		RectBoundary(){}
		RectBoundary(float l, float r, float t, float b) : left{l}, right{r}, top{t}, bottom{b} {}
		float left{0};
		float right{0};
		float top{0};
		float bottom{0};
	};

	int Abs(int num);
	float Abs(float num);

	int Max(int leftNum, int rightNum);
	float Max(float leftNum, float rightNum);

	int Min(int leftNum, int rightNum);
	float Min(float leftNum, float rightNum);

	int Clamp(int value, int min, int max);
	float Clamp(float value, float min, float max);

	float Lerp(float start, float end, float time);

	int DiceRoll(int min, int max);

	int FindDirectionFromVector(Float2 vec);
	
	void Normalize(Float2 &vector);
	float Mag(Float2 vector); //returns the length of the hypotenuse of x and y components

}