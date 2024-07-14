#ifndef EVENT_H
#define EVENT_H

typedef struct
{
    int id;
    void (*fn)(void* userdata);
} Event;

typedef struct
{
    int id;
    void (*fn)(void* a, void* b);
} CollisionEvent;

#endif // EVENT_H
