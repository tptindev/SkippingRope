#ifndef EVENT_H
#define EVENT_H

#include "../SceneManager.h"
#include "../../ecs/entity.h"

typedef struct
{
    int id;
    void (*transition)(SceneManager* manager);
    void (*collision)(Entity* entity);
} Event;

#endif // EVENT_H
