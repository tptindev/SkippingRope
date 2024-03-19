#include "render.h"
#include "hash.h"
#include "pd_api/pd_api_sprite.h"

void load_textures(void* userdata, const char** paths, LCDBitmap** bitmaps, unsigned int capacity)
{
	if (userdata == NULL) return;
	if (paths == NULL) return;

	PlaydateAPI* pd = userdata;
	bitmaps = pd->system->realloc(bitmaps, (size_t)capacity * sizeof(LCDBitmap*));
	pd->system->logToConsole("%p\n", bitmaps);
	for (int i = 0; i < capacity; i++)
	{
		const char* outerr;
		*(bitmaps + i) = pd->graphics->loadBitmap(*(paths + i), &outerr);
		pd->system->logToConsole("At: %d -> %s, bitmap: %p, error: %s\n", i, *(paths + i), *(bitmaps + i), outerr);
	}
}

void update_sprites(void* userdata, float x, float y, int16_t z_order, LCDBitmapFlip flip, LCDBitmap** bitmaps, unsigned int capacity)
{
	PlaydateAPI* pd = userdata;
	for (int i = 0; i < capacity; i++)
	{
		if ((bitmaps + i) == NULL) return;
		LCDSprite* sprite = pd->sprite->newSprite();
		pd->sprite->setImage(sprite , *(bitmaps + i), flip);
		pd->sprite->moveTo(sprite, x, y);
		pd->sprite->setZIndex(sprite, z_order);
		pd->sprite->addSprite(sprite);
	}
}