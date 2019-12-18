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
	data[10] = 1;
}

void mat4::Translate(float decX, float decY, float decZ)
{
	data[12] = decX;
	data[13] = decY;
	data[14] = decZ;
}

void mat4::Ortho(float near, float far, float left, float right, float top, float bottom)
{
	for (int i = 0; i < 16; i++)
	{
		data[i] = 0;
	}

	data[0] = 2 / (right - left);
	data[5] = 2 / (top - bottom);
	data[10] = -2 / (far - near);
	data[12] = -(right + left) / (right - left);
	data[13] = -(top + bottom) / (top - bottom);
	data[14] = -(far + near) / (far - near);
	data[15] = 1;
}

void mat4::Persp(float near, float far, float width, float height)
{
	for (int i = 0; i < 16; i++)
	{
		data[i] = 0;
	}

	float e = 1 / tan((3.14 * 0.5) / 2);

	data[0] = e / (width / height);
	data[5] = e;
	data[10] = (far + near) / (near - far);
	data[11] = -1;
	data[14] = (2 * far * near) / (near - far);
}