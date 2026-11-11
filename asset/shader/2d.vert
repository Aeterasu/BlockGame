#version 300 es

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;

uniform mat3 u_projection;
uniform mat3 u_model;

out vec4 v_color;
out vec2 v_uv;

void main()
{
	v_uv = a_uv;

	gl_Position = vec4(u_projection * u_model * a_position, 1.0);
	v_color = vec4(v_uv.y, 0.0f, v_uv.x, 1.0f);
}
