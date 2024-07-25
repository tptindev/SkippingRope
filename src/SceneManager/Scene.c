#include "Scene.h"
#include <stdlib.h>

Scene *CreateScene(int id, World2D* world)
{
    Scene* scene = malloc(sizeof(Scene));
    if (scene != NULL)
    {
        scene->id = id;
        scene->entities = CreateArray1D();
        scene->entities_active = CreateArray1D();
        scene->world = world;
        scene->z = 0;
        scene->active = false;
        scene->manager = NULL;
    }
    return scene;
}

void SceneAddGameObject(Scene* scene, Entity *entity)
{
    if (scene != NULL)
    {
        if (entity != NULL)
        {
            if (entity->active == true)
            {
                Array1DPush(scene->entities_active, entity);
            }
            Array1DPush(scene->entities, entity);
        }
    }
}

void SceneRemoveGameObject(Scene *scene, unsigned int id)
{
    if (scene != NULL)
    {
        for (size_t i = 0; i < scene->entities->size; i++)
        {
            Entity* entity = Array1DItemAtIndex(scene->entities, i);
            if (entity != NULL)
            {
                if (entity->id == id)
                {
                    Array1DDelete(scene->entities_active, i);
                    Array1DDelete(scene->entities, i);
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
        for (size_t i = 0; i < scene->entities->size; i++)
        {
            entity = Array1DItemAtIndex(scene->entities, i);
            if (entity != NULL)
            {
                FreeEntity(api, entity);
            }
        }
        FreeArray1D(scene->entities);
        FreeArray1D(scene->entities_active);
        free(scene);
        scene = NULL;
    }
}
