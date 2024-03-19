#include "render.h"
#include "hash.h"

LCDBitmap* textures[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
unsigned int textures_size = sizeof(textures) / sizeof(LCDBitmap*); 
LCDSprite* player_sprite = NULL;
void load_texture(void* userdata, const char* id, const char* path)
{
	if (userdata == NULL) return;
	PlaydateAPI* pd = userdata;
	LCDBitmap* bitmap = NULL;
	unsigned int index = -1;
	const char* outerr;
	index = hash(id, textures_size);
	if (textures[index] != NULL) return;
	bitmap = pd->graphics->loadBitmap(path, &outerr);
	textures[index] = bitmap;
	pd->system->logToConsole("Load texture success with key: %d, value: %p\n", index, bitmap);
}

LCDBitmap* texture(const char* id)
{
	return textures[hash(id, textures_size)];
}

void update_sprite(void* userdata, const char* id, int x, int y, int z_order, int w, int h, int row, int col, int angle, LCDBitmapFlip flip)
{
	if (userdata == NULL) return;
	PlaydateAPI* pd = userdata;
	LCDBitmap* image = texture(id);
	
	if (image == NULL)
	{
		pd->sprite->freeSprite(player_sprite);
		return;
	}

	int img_width, img_height;
	pd->graphics->getBitmapData(image, &img_width, &img_height, NULL, NULL, NULL);
	pd->sprite->setImage(player_sprite, image, flip);
	pd->sprite->moveTo(player_sprite, x - w, y - h);
	pd->sprite->setZIndex(player_sprite, z_order);
	pd->sprite->setCenter(player_sprite, 0, 0);
	LCDRect rect = LCDMakeRect(w * col + x, h * row + y, w, h);
	pd->sprite->setClipRect(player_sprite, rect);
}
