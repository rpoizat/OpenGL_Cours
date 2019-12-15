#include "mat.h"
#include <cmath>

mat4::mat4()
{
	for (int i = 0; i < 16; i++)
	{
		data[i] = 0;
	}
}

void mat4::Identity()
{
	for (int i = 0; i < 16; i++)
	{
		data[i] = 0;
	}

	for (int i = 0; i < 4; i++)
	{
		data[(i) * 5] = 1;
	}
}

void mat4::Rotate(float angle)
{
	data[0] = cos(angle);
	data[1] = sin(angle);
	data[4] = -sin(angle);
	data[5] = cos(angle);
	data[10] = 1;
	data[15] = 1;
}

void mat4::Scale(float factor)
{
	data[0] = factor;
	data[5] = factor;
	data[10] = factor;
}

void mat4::Ortho()
{

}