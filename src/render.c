#include "render.h"
#include "hash.h"

void load_textures(PlaydateAPI* pd, const char** paths, LCDSprite** sprites, unsigned int capacity)
{
	if (pd == NULL) return;
	if (paths == NULL) return;
	if (sprites == NULL) return;

	for (int i = 0; i < capacity; i++)
	{
		const char* path = *(paths + i);
		pd->system->logToConsole("Begin: Load image at path: '%s'", path);

		const char* outerr = NULL;
		LCDBitmap* bitmap = pd->graphics->loadBitmap(path, &outerr);

		if (outerr != NULL) {
			pd->system->logToConsole("Error loading image at path '%s': %s", path, outerr);
		}
		else
		{
			LCDSprite* sprite = *(sprites + i);
			pd->sprite->setImage(sprite, bitmap, kBitmapUnflipped);
			pd->sprite->setCenter(sprite, 0, 0);
		}

		pd->system->logToConsole("End: Load image at path: '%s', bitmap: %p", *(paths + i), *(sprites + i));

	}

}

void update_sprites(PlaydateAPI* pd, LCDSprite** sprites, float x, float y, int16_t z_order, unsigned int capacity, LCDBitmapFlip flip)
{
	if (pd == NULL) return;
	if (sprites == NULL) return;
	else
	{
		for (int i = 0; i < capacity; i++)
		{
			if (*(sprites + i) == NULL) continue;
			LCDSprite* sprite = *(sprites + i);
			pd->sprite->moveTo(sprite, x, y);
			pd->sprite->setZIndex(sprite, z_order);
		}
	}
}