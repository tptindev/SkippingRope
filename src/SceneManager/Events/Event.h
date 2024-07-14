#ifndef EVENT_H
#define EVENT_H

typedef struct
{
    int id;
    void (*fn)(void* userdata);
} Event;

#endif // EVENT_H
