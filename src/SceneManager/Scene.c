#include "Scene.h"
#include <stdlib.h>

Scene *CreateScene(int id, World2D* world)
{
    Scene* scene = malloc(sizeof(Scene));
    if (scene != NULL)
    {
        scene->id = id;
        scene->entites = CreateArray1D();
        scene->world = world;
        scene->z = 0;
        scene->active = false;
    }
    return scene;
}

void SceneAddGameObject(Scene* scene, Entity *entity)
{
    if (scene != NULL)
    {
        Array1DPush(scene->entites, entity);
    }
}

void SceneRemoveGameObject(Scene *scene, unsigned int id)
{
    if (scene != NULL)
    {
        for (size_t i = 0; i < scene->entites->size; i++)
        {
            Entity* entity = Array1DItemAtIndex(scene->entites, i);
            if (entity != NULL)
            {
                if (entity->id == id)
                {
                    Array1DDelete(scene->entites, i);
                }
            }
        }
    }
}

void FreeScene(void *api, Scene *scene)
{
    if (scene != NULL)
    {
        Entity* entity = NULL;
        for (size_t i = 0; i < scene->entites->size; i++)
        {
            entity = Array1DItemAtIndex(scene->entites, i);
            if (entity != NULL)
            {
                FreeEntity(api, &entity);
            }
        }
        FreeArray1D(scene->entites);
        free(scene);
        scene = NULL;
    }
}
