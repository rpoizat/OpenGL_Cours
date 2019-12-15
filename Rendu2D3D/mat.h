#pragma once
struct mat4
{
	float data[16];

	mat4();
	void Identity();
	void Rotate(float angle);
	void Scale(float factor);
	void Ortho();
};