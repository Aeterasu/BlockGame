#version 300 es

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;

uniform mat3 u_projection;
uniform mat3 u_model;

uniform vec2 u_atlas_size;
uniform vec2 u_texture_offset;
uniform vec2 u_sprite_size;

out vec4 v_color;
out vec2 v_uv;

void main()
{
	vec2 uv_offset = u_texture_offset / u_atlas_size;
	vec2 uv_scale = u_sprite_size / u_atlas_size;

	v_uv = uv_offset + a_uv * uv_scale;

	vec3 pos = u_projection * (u_model * vec3(a_position.xy, 1.0));
	gl_Position = vec4(pos.xy, 0.0, 1.0);
	v_color = vec4(a_uv.y, 0.0f, a_uv.x, 1.0f);
}
