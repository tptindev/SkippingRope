//
//  main.c
//  Extension
//
//  Created by Dave Hayden on 7/30/14.
//  Copyright (c) 2014 Panic, Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>    

#include "pd_api.h"
#include "logic.h"
#include "render.h"

static int update(void* userdata);
const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
		const char* err;
		font = pd->graphics->loadFont(fontpath, &err);
		
		if ( font == NULL )
			pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);

		load_texture(pd, "snow_image", "images/snow");

		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
		pd->system->setUpdateCallback(update, pd);
	}
	
	return 0;
}

static int update(void* userdata)
{
	PlaydateAPI* pd = userdata;
	
	pd->system->drawFPS(0,0);
	for (int i = 0; i < (int)(sizeof(textures) / sizeof(LCDBitmap*)); i++)
	{
		pd->system->logToConsole("Texture at %d with value: %p\n", i, textures[i]);
		if (textures[i] == NULL) continue;
		pd->graphics->drawBitmap(textures[i], pd->display->getWidth() / 2, pd->display->getHeight() / 2, kBitmapUnflipped);
	}
	return 1;
}

