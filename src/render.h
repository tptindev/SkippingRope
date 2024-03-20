#include "pd_api.h"

extern void load_textures(void* userdata, const char** paths, LCDBitmap** bitmaps, LCDSprite** sprites, unsigned int capacity);
extern void update_sprites(void* userdata, LCDSprite** sprites, LCDBitmap** bitmaps, float x, float y, int16_t z_order, LCDBitmapFlip flip, unsigned int capacity);
