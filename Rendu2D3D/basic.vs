attribute vec3 a_position;
attribute vec3 a_color;

uniform mat4 u_matRot;
uniform mat4 u_matScale;
uniform mat4 u_matProjOrtho;
uniform mat4 u_matProjPersp;
uniform mat4 u_matTrans;
varying vec4 v_color;

void main(void)
{
	gl_Position = u_matProjPersp * u_matTrans * u_matRot * u_matScale * vec4(a_position, 1.0);
	v_color = vec4(a_color, 1.0);
}