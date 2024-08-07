#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "../Physics2D/DataStructs/Array1D.h"
#include "Scene.h"
typedef struct
{
    Scene* current_scene;
    Array1D* scenes;
    void* pd;
} SceneManager;

SceneManager* CreateSceneManager(void* pd);
void SceneManagerActiveScene(SceneManager* manager, Scene* scene);
void SceneManagerAddScene(SceneManager* manager, Scene* scene);
void SceneManagerRemoveScene(SceneManager* manager, int id);
void SceneManagerTransition(SceneManager* manager, int id);
void FreeSceneManager(void *api, SceneManager* manager);

#endif // SCENEMANAGER_H
