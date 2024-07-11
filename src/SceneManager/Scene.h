#ifndef SCENE_H
#define SCENE_H

#include <stdbool.h>
#include "../Physics2D/DataStructs/Array1D.h"
#include "../Physics2D/World2D.h"
#include "../ecs/entity.h"
typedef struct
{
    int id;
    Array1D* entites;
    World2D* world;
    int16_t z;
    bool active;
} Scene;

Scene* CreateScene(int id, World2D* world);
void SceneAddGameObject(Scene* scene, Entity* entity);
void SceneRemoveGameObject(Scene* scene, unsigned int id);
void FreeScene(void *api, Scene* scene);

#endif // SCENE_H
