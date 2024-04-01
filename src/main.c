//
//  main.c
//  Extension
//
//  Created by Dave Hayden on 7/30/14.
//  Copyright (c) 2014 Panic, Inc. All rights reserved.
//
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "game.h"

#include <box2d.h>
#include <assert.h>

#ifdef BOX2D_PROFILE
#include <tracy/Tracy.hpp>
#else
#define FrameMark
#endif

#if defined(_WIN32)
#include <crtdbg.h>

static int MyAllocHook(int allocType, void* userData, size_t size, int blockType, long requestNumber, const unsigned char* filename,
	int lineNumber)
{
	// This hook can help find leaks
	if (size == 143)
	{
		size += 0;
	}

	return 1;
}
#endif


static unsigned int last_time;
static unsigned int current_time;
static int update(void* userdata);
static void* allocFn(uint32_t size, int32_t alignment);
static void freeFn(void* mem);
static int assertFn(const char* condition, const char* fileName, int lineNumber);

inline bool IsPowerOfTwo(int32_t x)
{
	return (x != 0) && ((x & (x - 1)) == 0);
}

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
#if defined(_WIN32)
	// Enable memory-leak reports
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	//_CrtSetAllocHook(MyAllocHook);
#endif

	// Install memory hooks
	b2SetAllocator(allocFn, freeFn);
	b2SetAssertFcn(assertFn);

	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
		last_time = pd->system->getCurrentTimeMilliseconds();
		game_initialize(pd);
		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
		pd->system->setUpdateCallback(update, pd);

	}
	
	return 0;
}


static int update(void* userdata)
{
	PlaydateAPI* api = userdata;
	current_time = api->system->getCurrentTimeMilliseconds();
	float dt = (float)(current_time - last_time)/1000.0f;
	game_update(dt);
	game_draw();
	last_time = current_time;
	return 1;
}

static void* allocFn(uint32_t size, int32_t alignment)
{
	// Allocation must be a multiple of alignment or risk a seg fault
	// https://en.cppreference.com/w/c/memory/aligned_alloc
	assert(IsPowerOfTwo(alignment));
	size_t sizeAligned = ((size - 1) | (alignment - 1)) + 1;
	assert((sizeAligned & (alignment - 1)) == 0);
#if defined(_WIN64)
	void* ptr = _aligned_malloc(sizeAligned, alignment);
#else
	void* ptr = aligned_alloc(alignment, sizeAligned);
#endif
	assert(ptr != NULL);
	return ptr;
}

static void freeFn(void* mem)
{
#if defined(_WIN64)
	_aligned_free(mem);
#else
	free(mem);
#endif
}

static int assertFn(const char* condition, const char* fileName, int lineNumber)
{
	printf("Game ASSERTION: %s, %s, line %d\n", condition, fileName, lineNumber);
	return 1;
}
