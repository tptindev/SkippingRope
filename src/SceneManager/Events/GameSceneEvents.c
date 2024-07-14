#include "GameSceneEvents.h"
#include "../SceneManager.h"
#include "SceneIDs.h"


void EVT_BACK_MENU_FUNC(void *userdata)
{
    Scene* scene = userdata;
    SceneManager* manager = scene->manager;
    SceneManagerTransition(manager, MENU);
}
