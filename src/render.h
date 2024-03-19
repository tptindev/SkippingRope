#include "pd_api.h"

extern void load_textures(void* userdata, const char** paths, LCDBitmap** bitmaps, unsigned int capacity);
extern void update_sprites(void* userdata, float x, float y, int16_t z_order, LCDBitmapFlip flip, LCDBitmap** bitmaps, unsigned int capacity);