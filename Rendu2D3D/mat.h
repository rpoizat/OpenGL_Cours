#pragma once
struct mat4
{
	float data[16];

	mat4();
	void Identity();
	void Rotate(float angle);
	void Scale(float factor);
	void Translate(float decX, float decY, float decZ);
	void Ortho(float near, float far, float left, float right, float top, float bottom);
	void Persp(float near, float far, float width, float height);
};