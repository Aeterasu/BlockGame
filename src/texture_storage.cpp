#include "texture_storage.h"

#include "texture.h"

namespace blockgame
{
	TextureStorage textureStorage;

	void InitTextureStorage()
	{
		textureStorage.grid = LoadTexture("texture_grid.png");
		textureStorage.border = LoadTexture("texture_border.png");
		textureStorage.block = LoadTexture("texture_block.png");
		textureStorage.blockTelegraph = LoadTexture("texture_block_telegraph.png");
		textureStorage.blockTelegraphWarning = LoadTexture("texture_block_telegraph_warning.png");
		textureStorage.cursor = LoadTexture("texture_cursor.png");
		textureStorage.bomb = LoadTexture("texture_bomb.png");
		textureStorage.letterboxBg = LoadTexture("me.png");
	}
} // namespace blockgame
