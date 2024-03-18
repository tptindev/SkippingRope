#include "render.h"
#include "hash.h"

LCDBitmap* textures[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
void load_texture(void* userdata, const char* id, const char* path)
{
	PlaydateAPI* pd = userdata;
	LCDBitmap* bitmap = NULL;
	unsigned int index = -1;
	const char* outerr;
	index = hash(id, sizeof(textures) / sizeof(LCDBitmap*));
	if (index < 0) return;
	if (textures[index] != NULL) return;
	bitmap = pd->graphics->loadBitmap(path, &outerr);
	textures[index] = bitmap;
	pd->system->logToConsole("Load texture success with key: %d, value: %p\n", index, bitmap);
}

LCDBitmap* texture(const char* id)
{
	return;
}