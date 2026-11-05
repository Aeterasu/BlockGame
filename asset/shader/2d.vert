#version 300 es

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 in_uv;

out vec4 vertex_color;
out vec2 uv;

void main()
{
	uv = in_uv;

	gl_Position = vec4(position, 1.0f);
	vertex_color = vec4(uv.y, 0.0f, uv.x, 1.0f);
}
