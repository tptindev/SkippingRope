#include "GameOverSceneEvents.h"
#include "SceneIDs.h"
#include "pd_api.h"
#include <stdio.h>

void EVT_NEW_GAME_FUNC(SceneManager* manager)
{
    PlaydateAPI* api = manager->pd;
    SceneManagerTransition(manager, GAME_SCENE);
}

void EVT_QUIT_GAME_FUNC(SceneManager* manager)
{
    PlaydateAPI* api = manager->pd;
    if (api != NULL)
    {
    }
}
