#include "pd_api.h"

extern LCDBitmap* textures[10];
extern unsigned int textures_size;
extern LCDSprite* player_sprite;

extern void load_texture(void* userdata, const char* id, const char* path);
extern LCDBitmap* texture(const char* id);
extern void update_sprite(void* userdata, const char* id, int x, int y, int z_order, int w, int h, int row, int col, int angle, LCDBitmapFlip flip);
