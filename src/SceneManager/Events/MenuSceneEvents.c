#include "MenuSceneEvents.h"
#include "SceneIDs.h"

void EVT_MENU_START_FUNC(SceneManager* manager)
{
    SceneManagerTransition(manager, GAME_SCENE);
}

void EVT_MENU_EXIT_FUNC(SceneManager* manager)
{

}
