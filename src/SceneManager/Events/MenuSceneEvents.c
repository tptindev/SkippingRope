#include "MenuSceneEvents.h"
#include "../SceneManager.h"
#include "SceneIDs.h"

void EVT_START_FUNC(void *userdata)
{
    Scene* scene = userdata;
    SceneManager* manager = scene->manager;
    SceneManagerTransition(manager, GAME_SCENE);
}

void EVT_EXIT_FUNC(void *userdata)
{

}
