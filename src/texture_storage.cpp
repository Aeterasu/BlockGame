#include "texture_storage.h"

#include "texture.h"

namespace blockgame
{
	TextureStorage textureStorage;

	void InitTextureStorage()
	{
		textureStorage.grid = LoadTexture("assets/textures/texture_grid.png");
		textureStorage.border = LoadTexture("assets/textures/texture_border.png");
		textureStorage.block = LoadTexture("assets/textures/texture_block.png");
		textureStorage.cursor = LoadTexture("assets/textures/texture_cursor.png");
		textureStorage.bomb = LoadTexture("assets/textures/texture_bomb.png");
		textureStorage.letterboxBg = LoadTexture("assets/textures/me.png");
	}
} // namespace blockgame
