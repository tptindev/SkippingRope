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
#include "playbox.h"
#include "platform.h"
#include "model.h"
#include "assets.h"

#define MAX_HUMAN 5

static int update(void* userdata);
static int buttonCbFunc(PDButtons button, int down, uint32_t when, void* userdata);

// variables 

PlaydateAPI* pd = NULL;
int w, h; // windows size
unsigned int ticks = 0;
unsigned int frame_speed = 100;
const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;
Human* humans[MAX_HUMAN];
Human human;
Rope rope;

const char* sprite_paths[5] = { "1", "2", "3", "4", "5" };
// --------------------------------------

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
		pd = playdate;
		registerPlaybox();

		w = pd->display->getWidth();
		h = pd->display->getHeight();

		const char* err;
		font = pd->graphics->loadFont(fontpath, &err);
		
		if (font == NULL)
		{
			pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);
		}

		// load textures: start
		load_textures(pd, sprite_paths, human.bitmaps, 5);
		// load textures: end

		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
		pd->system->setUpdateCallback(update, pd);
		pd->system->setButtonCallback(buttonCbFunc, pd, 2);

	}
	
	return 0;
}

static int update(void* userdata)
{
	PlaydateAPI* pd = userdata;
	ticks = pd->system->getCurrentTimeMilliseconds();

	update_sprites(userdata, 0, 0, 0, kBitmapUnflipped, human.bitmaps, 5);

	pd->sprite->updateAndDrawSprites();

	pd->system->drawFPS(0,0);
	return 1;
}

static int buttonCbFunc(PDButtons button, int down, uint32_t when, void* userdata)
{
	switch (button)
	{
	case kButtonUp:
	{
		break;
	}
	default:
		break;
	}
}