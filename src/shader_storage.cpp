#include "shader_storage.h"

#include "shader.h"

namespace blockgame
{
	ShaderStorage shaderStorage;

	void InitShaderStorage()
	{
		shaderStorage.spriteShader =
			LoadShader("assets/shaders/quad.vertex.glsl", "assets/shaders/texture.fragment.glsl");

		shaderStorage.gradientTestShader =
			LoadShader("assets/shaders/quad.vertex.glsl", "assets/shaders/gradient.fragment.glsl");
	}
} // namespace blockgame
