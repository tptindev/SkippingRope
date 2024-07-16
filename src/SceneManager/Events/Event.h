#ifndef EVENT_H
#define EVENT_H

#include "../SceneManager.h"
#include "../../ecs/entity.h"

typedef struct
{
    int id;
    void (*transition)(SceneManager* manager);
    void (*collision)(Entity* entity);
    void (*dead)(void* pd_ptr, SceneManager* manager, Entity* entity);
} Event;

#endif // EVENT_H
