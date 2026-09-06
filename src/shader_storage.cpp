#include "shader_storage.h"

#include "shader.h"

namespace blockgame
{
	ShaderStorage shaderStorage;

	void InitShaderStorage()
	{
		shaderStorage.spriteShader =
			LoadShader("quad.vertex.glsl", "texture.fragment.glsl");

		shaderStorage.blockTelegraphShader =
			LoadShader("quad.vertex.glsl", "telegraph.fragment.glsl");
	}
} // namespace blockgame
