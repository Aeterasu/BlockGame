#pragma once

#include "shader.h"

namespace blockgame
{
	struct ShaderStorage
	{
		Shader spriteShader;
		Shader blockTelegraphShader;
	};

	extern ShaderStorage shaderStorage;

	void InitShaderStorage();
} // namespace blockgame
