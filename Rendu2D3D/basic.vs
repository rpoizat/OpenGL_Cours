attribute vec2 a_position;
attribute vec3 a_color;

uniform mat4 u_mat;
uniform mat4 u_matScale;
varying vec4 v_color;

void main(void)
{
	gl_Position = u_mat * u_matScale * vec4(a_position, 0.0, 1.0);
	v_color = vec4(a_color, 1.0);
}