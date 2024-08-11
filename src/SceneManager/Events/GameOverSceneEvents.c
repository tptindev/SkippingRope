#include "GameOverSceneEvents.h"
#include "SceneIDs.h"
#include "pd_api.h"

void EVT_NEW_GAME_FUNC(SceneManager* manager)
{
    PlaydateAPI* api = manager->pd;
//    api->scoreboards->getScores("Score", NULL);
//    SceneManagerTransition(manager, GAME_SCENE);
}

void EVT_QUIT_GAME_FUNC(SceneManager* manager)
{
    PlaydateAPI* api = manager->pd;
    if (api != NULL)
    {
    }
}
