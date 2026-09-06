#pragma once

#include "texture.h"

namespace blockgame
{
	struct TextureStorage
	{
		Texture grid;
		Texture border;
		Texture block;
		Texture blockTelegraph;
		Texture blockTelegraphWarning;
		Texture cursor;
		Texture bomb;
		Texture letterboxBg;
	};

	extern TextureStorage textureStorage;

	void InitTextureStorage();
} // namespace blockgame
