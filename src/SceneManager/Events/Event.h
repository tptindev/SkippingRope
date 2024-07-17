#ifndef EVENT_H
#define EVENT_H

#include "../SceneManager.h"
#include "../../ecs/entity.h"

typedef struct
{
    int id;
    void (*transition)(SceneManager* manager);
    void (*collision)(Entity* entity, Entity* other);
    void (*dead)(SceneManager* manager, Entity* entity);
    void (*trigger)(SceneManager* manager, Entity* entity, Entity* other);
} Event;

#endif // EVENT_H
