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
static int buttonCbFunc(PDButtons button, int down, uint32_t when, void* userdata);

const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;

// variables 

int w, h; // windows size
unsigned int ticks = 0;
unsigned int frame_speed = 100;

// --------------------------------------

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	w = pd->display->getWidth();
	h = pd->display->getHeight();

	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
		const char* err;
		font = pd->graphics->loadFont(fontpath, &err);
		
		if ( font == NULL )
			pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);

		load_texture(pd, "AB", "images/snow");
		load_texture(pd, "player", "images/bunny");

		player_sprite = pd->sprite->newSprite();
		pd->sprite->addSprite(player_sprite);
		LCDBitmap* player_img = texture("player");

		if (player_img == NULL)
		{
			pd->sprite->freeSprite(player_sprite);
			return;
		}
		pd->sprite->setImage(player_sprite, player_img, kBitmapUnflipped);

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

	pd->system->drawFPS(0,0);

	update_sprite(userdata, "player", w / 2, h / 2, 1, 48, 48, 0, (ticks / frame_speed) % 2, 0, kBitmapUnflipped);

	pd->sprite->updateAndDrawSprites();

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