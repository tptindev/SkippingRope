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
#include "playbox.h"
#include "game.h"

PlaydateAPI *pd = NULL;

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInitLua )
	{
		pd = playdate;
		registerPlaybox();
		register_lua_functions(pd);
		register_lua_classes(pd);
	}
	
	return 0;
}