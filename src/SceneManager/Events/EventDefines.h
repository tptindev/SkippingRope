#ifndef EVENTDEFINES_H
#define EVENTDEFINES_H

#include <stdlib.h>
#include "Event.h"
#include "EventIDs.h"
#include "MenuSceneEvents.h"
#include "GameSceneEvents.h"

Event MenuSceneEvents[2] = {
    {EVT_START, EVT_START_FUNC},
    {EVT_EXIT,  EVT_EXIT_FUNC},
};

Event GameSceneEvents[1] = {
    {EVT_BACK_MENU, EVT_BACK_MENU_FUNC}
};

#endif // EVENTDEFINES_H
