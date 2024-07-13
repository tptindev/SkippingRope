#ifndef EVENTDEFINES_H
#define EVENTDEFINES_H

#include "Event.h"
#include "EventIDs.h"
#include "MenuSceneEvents.h"

static Event MenuSceneEvents[2] = {
    {EVT_START, EVT_START_FUNC},
    {EVT_EXIT,  EVT_EXIT_FUNC},
};

//static Event GameSceneEvents[0] = {};

#endif // EVENTDEFINES_H
