#include "render.h"
#include "hash.h"

LCDBitmap* textures[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
unsigned int textures_size = sizeof(textures) / sizeof(LCDBitmap*);
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

void draw_sprite(void* userdata, const char* id, int x, int y, int z_order, int w, int h, int row, int col, int angle, LCDBitmapFlip flip)
{
	if (userdata == NULL) return;
	PlaydateAPI* pd = userdata;
	LCDSprite* sprite = pd->sprite->newSprite();
	LCDBitmap* image = texture(id);

	if (image == NULL)
	{
		pd->sprite->freeSprite(sprite);
		return;
	}

	pd->sprite->moveTo(sprite, x, y);
	pd->sprite->setZIndex(sprite, z_order);
	pd->sprite->setImage(sprite, image, flip);
	pd->sprite->addSprite(sprite);
}
