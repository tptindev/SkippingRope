#include "SceneManager.h"
#include <stdlib.h>

SceneManager *CreateSceneManager()
{
    SceneManager* manager = malloc(sizeof(SceneManager));
    if (manager != NULL)
    {
        manager->current_scene = NULL;
        manager->scenes = CreateArray1D();
    }
    return manager;
}

void FreeSceneManager(void *api, SceneManager *manager)
{
    if (manager != NULL)
    {
        Scene* scene = NULL;
        for (size_t i = 0; i < manager->scenes->size; ++i) {
            scene = Array1DItemAtIndex(manager->scenes, i);
            FreeScene(api, scene);
        }
        FreeArray1D(manager->scenes);
        free(manager);
        manager = NULL;
    }
}

void SceneManagerAddScene(SceneManager *manager, Scene *scene)
{
    if (manager != NULL)
    {
        Array1DPush(manager->scenes, scene);
    }
}

void SceneManagerRemoveScene(SceneManager *manager, int id)
{
    if (manager != NULL)
    {
        Scene* scene = NULL;
        for (size_t i = 0; i < manager->scenes->size; ++i) {
            scene = Array1DItemAtIndex(manager->scenes, i);
            if (id == scene->id)
            {
                Array1DDelete(manager->scenes, i);
            }
        }
    }
}

void SceneManagerActiveScene(SceneManager *manager, Scene *scene)
{
    if (manager != NULL)
    {
        manager->current_scene = scene;
        scene->active = true;
    }
}

void SceneManagerTransition(SceneManager *manager, int id)
{
    if (manager != NULL)
    {
        manager->current_scene->active = false;

        Scene* scene = NULL;
        for (size_t i = 0; i < manager->scenes->size; ++i) {
            scene = Array1DItemAtIndex(manager->scenes, i);
            if (id == scene->id)
            {
                manager->current_scene = scene;
                manager->current_scene->active = true;
            }
        }
    }
}
