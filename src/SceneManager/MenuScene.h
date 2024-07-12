#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "Scene.h"

void MenuSceneInit(void* pd_ptr, Scene* scene);
void MenuSceneEvent(void* pd_ptr, Scene* scene, void* manager);
void MenuSceneUpdate(void* pd_ptr, Scene* scene, float dt);
void MenuSceneRender(void* pd_ptr, Scene* scene);

#endif // MENUSCENE_H
