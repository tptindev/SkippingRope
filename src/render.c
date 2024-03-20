#include "render.h"
#include "hash.h"
#include "pd_api/pd_api_sprite.h"

void load_textures(void* userdata, const char** paths, LCDSprite** sprites, unsigned int capacity)
{
	if (userdata == NULL) return;
	if (paths == NULL) return;

	PlaydateAPI* pd = userdata;
	sprites = pd->system->realloc(NULL, (size_t)capacity * sizeof(LCDSprite*));

	for (int i = 0; i < capacity; i++)
	{
		pd->system->logToConsole("Begin: Load image at path: '%s'", *(paths + i));
		const char* outerr = NULL;
		LCDBitmap* bitmap = pd->graphics->loadBitmap(*(paths + i), &outerr);
		*(sprites + i) = pd->sprite->newSprite();
		if (outerr != NULL) {
			pd->system->logToConsole("Error loading image at path '%s': %s", *(paths + i), outerr);
		}
		else
		{
			pd->sprite->setImage(*(sprites + i), bitmap, kBitmapUnflipped);
			pd->sprite->addSprite(*(sprites + i));
		}

		pd->system->logToConsole("End: Load image at path: '%s', bitmap: %p", *(paths + i), *(sprites + i));

	}
}

void update_sprites(void* userdata, LCDSprite** sprites, float x, float y, int16_t z_order, LCDBitmapFlip flip, unsigned int capacity)
{
	PlaydateAPI* pd = userdata;
	if (sprites == NULL) return;
	else
	{
		for (int i = 0; i < capacity; i++)
		{
			LCDSprite* sprite = *(sprites + i);
			if (sprite == NULL) continue;
			pd->sprite->moveTo(sprite, x, y);
			pd->sprite->setZIndex(sprite, z_order);
			pd->sprite->setCenter(sprite, 0,0);
		}
	}
}