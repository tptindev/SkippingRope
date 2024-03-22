#include "pd_api.h"

void load_textures(PlaydateAPI* pd, const char** paths, LCDSprite** sprites, unsigned int capacity);
void update_sprites(PlaydateAPI* pd, LCDSprite** sprite, float x, float y, int16_t z_order, unsigned int capacity, LCDBitmapFlip flip);
