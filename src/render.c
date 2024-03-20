#include "render.h"
#include "hash.h"
#include "pd_api/pd_api_sprite.h"

void load_textures(void* userdata, const char** paths, LCDBitmap** bitmaps, LCDSprite** sprites, unsigned int capacity)
{
	if (userdata == NULL) return;
	if (paths == NULL) return;

	PlaydateAPI* pd = userdata;
	bitmaps = pd->system->realloc(NULL, (size_t)capacity * sizeof(LCDBitmap*));
	sprites = pd->system->realloc(NULL, (size_t)capacity * sizeof(LCDSprite*));

	for (int i = 0; i < capacity; i++)
	{
		pd->system->logToConsole("Load image at path: '%s'", *(paths + i));
		const char* outerr = NULL;
		*(bitmaps + i) = pd->graphics->loadBitmap(*(paths + i), &outerr);
		*(sprites + i) = pd->sprite->newSprite();
		if (outerr != NULL) {
			pd->system->logToConsole("Error loading image at path '%s': %s", *(paths + i), outerr);
		}
		else
		{
		}
	}
}

void update_sprites(void* userdata, LCDSprite** sprites, LCDBitmap** bitmaps, float x, float y, int16_t z_order, LCDBitmapFlip flip, unsigned int capacity)
{
	PlaydateAPI* pd = userdata;
	if (bitmaps == NULL) return;
	else if (sprites == NULL) return;
	else
	{
		for (int i = 0; i < capacity; i++)
		{
			LCDBitmap* bitmap = *(bitmaps + i);
			LCDSprite* sprite = *(sprites + i);
			if (bitmap == NULL || sprite == NULL) continue;

			pd->sprite->setImage(sprite, *(bitmaps + i), flip);
			pd->sprite->moveTo(sprite, x, y);
			pd->sprite->setZIndex(sprite, z_order);
			pd->sprite->addSprite(sprite);
		}
	}
}