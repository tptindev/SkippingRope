#include "system.h"
#include "entity.h"
#include "component.h"
#include "../Physics2D/Collision.h"
#include "pd_api.h"
#include "../SceneManager/Events/EventDefines.h"
#include "../Physics2D/Utils.h"
#include "../ecs/EntityIDs.h"
void UpdateRotation(Entity* entity)
{
    if (entity == NULL) return;
    if (entity->active == false) return;
    if (entity->components.transform != NULL && entity->components.motion != NULL)
    {
        if (entity->components.transform->rotation.x == 0.0f && entity->components.transform->rotation.y == 0.0f) return;
        entity->components.transform->position.x = entity->components.motion->direction.x + entity->components.transform->rotation.x;
        entity->components.transform->position.y = entity->components.motion->direction.y + entity->components.transform->rotation.y;
    }
}

void UpdateMovement(Entity* entity, float dt)
{
    if (entity == NULL) return;
    if (entity->active == false) return;
    if (entity->components.transform != NULL && entity->components.motion != NULL)
    {
        if (entity->components.motion->acceleration.x != 0.0f || entity->components.motion->acceleration.y != 0.0f)
        {
            entity->components.motion->acceleration.x += 0.1f;
            entity->components.motion->acceleration.y += 0.1f;

            Vec2 velocity = Vec2Subtract(entity->components.transform->position, entity->components.motion->last_position);
            entity->components.transform->position.x += velocity.x + (entity->components.motion->direction.x * entity->components.motion->acceleration.x) * dt * dt;
            entity->components.transform->position.y += velocity.y + (entity->components.motion->direction.y * entity->components.motion->acceleration.y) * dt * dt;
        }
    }
}

void UpdatePosition(Entity* entity, Vec2 buffer, float dt)
{
    if (entity == NULL) return;
    if (entity->active == false) return;
    if (entity->components.transform != NULL)
    {
        entity->components.transform->position.x += buffer.x;
        entity->components.transform->position.y += buffer.y;
    }
}

void UpdateCollider(Entity* entity, struct QuadTree* tree)
{
    if (entity == NULL || tree == NULL) return;
    if (entity->active == false) return;
    if (entity->components.collider != NULL && entity->components.transform != NULL)
    {
        // Update Collider
        if (entity->components.collider->shape.type == CIRCLE)
        {
            Circle* circle = entity->components.collider->shape.define;
            circle->center = Vec2Add(entity->components.transform->position, entity->components.collider->offset);
            entity->components.collider->shape.box.x = entity->components.collider->offset.x + entity->components.transform->position.x - circle->radius;
            entity->components.collider->shape.box.y = entity->components.collider->offset.y + entity->components.transform->position.y - circle->radius;
        }
        QuadtreeInsert(tree, entity, &entity->components.collider->shape.box);
    }
}

void UpdateCollisionDetection(void *scene_ptr, Entity* entity, struct QuadTree* tree)
{
    if (entity == NULL || tree == NULL || scene_ptr == NULL) return;
    if (entity->active == false) return;
    if (entity->components.collider == NULL) return;
    Scene* scene = scene_ptr;
    SceneManager* manager = scene->manager;
    PlaydateAPI* api = manager->pd;
    (void)scene;
    (void)api;
    Array1D* nodes = CreateArray1D();
    QuadTreeSearch(tree, nodes, &entity->components.collider->shape.box);
    Circle* c0 = entity->components.collider->shape.define;
    float y = entity->components.collider->shape.box.y;

    for (int i = 0; i < nodes->size; i++)
    {
        struct QuadTree* node = Array1DItemAtIndex(nodes, i);
        Array1D* objs = node->objects;
        for (int j = 0; j < node->objects->size; j++)
        {
            Entity* other = Array1DItemAtIndex(objs, j);
            if (other == NULL) continue;
            if (other->active == false) continue;
            if (entity->id != other->id)
            {
                Circle* cx = other->components.collider->shape.define;
                float y1 = other->components.collider->shape.box.y;
                int16_t* z_order = NULL;
                if (entity->components.sprite != NULL)
                {
                    z_order = &entity->components.sprite->order_in_layer;
                }
                else if (entity->components.animated_sprite != NULL)
                {
                    z_order = &entity->components.animated_sprite->order_in_layer;
                }
                else if (entity->components.button_img != NULL)
                {
                    z_order = &entity->components.button_img->order_in_layer;
                }
                if (z_order != NULL)
                {
                    if (y < y1)
                    {
                        (*z_order)--;
                    }
                    else
                    {
                        (*z_order)++;
                    }
                }

                bool collided = IsCollisionCircle(c0, cx);
                if (collided == true)
                {
                    entity->components.collider->collided = collided;
                    other->components.collider->collided = collided;
                    for (size_t i = 0; i < (sizeof(GameSceneEvents)/sizeof(Event)); i++)
                    {
                        if (GameSceneEvents[i].id == entity->components.collider->event_id)
                        {
                            if (GameSceneEvents[i].collision != NULL)
                            {
                                GameSceneEvents[i].collision(entity, other);
                            }
                        }
                        if (GameSceneEvents[i].id == other->components.collider->event_id)
                        {
                            if (GameSceneEvents[i].collision != NULL)
                            {
                                GameSceneEvents[i].collision(other, entity);
                            }
                        }
                    }
                }
            }
        }
    }
    FreeArray1D(nodes);
}

void UpdateSprite(Entity* entity, unsigned int tick)
{
    if (entity == NULL) return;
    if (entity->active == false) return;
    if (entity->components.sprite != NULL)
    {
    }
}

void UpdateAnimateSprite(Entity* entity, unsigned int tick)
{
    if (entity == NULL) return;
    if (entity->active == false) return;
    if (entity->components.animated_sprite != NULL)
    {
        if (entity->components.animated_sprite->running != true) return;
        int frame_count = entity->components.animated_sprite->frame_count;
        entity->components.animated_sprite->frame_index = (tick % frame_count);
    }
}

void UpdateHealth(void* pd_ptr, void* scene_ptr, Entity* entity)
{
    if (entity == NULL || pd_ptr == NULL || scene_ptr == NULL) return;
    if (entity->active == false) return;
    PlaydateAPI* api = pd_ptr;
    (void)api;
    Scene* scene = scene_ptr;
    if (entity->components.health != NULL)
    {
        if (entity->components.health->current <= 0)
        {
            for (size_t i = 0; i < (sizeof(GameSceneEvents)/sizeof(Event)); i++)
            {
                if (GameSceneEvents[i].id == entity->components.health->event_id)
                {
                    if (GameSceneEvents[i].dead != NULL)
                    {
                        GameSceneEvents[i].dead(scene->manager, entity);
                    }
                    break;
                }
            }
        }
        else
        {
            if (entity->id >= ENTITY_ENEMY && entity->id <= ENTITY_ENEMY_MAX)
            {
                bool validX = (entity->components.transform->position.x < 1.0f || entity->components.transform->position.x > 5.0f);
                bool validY = (entity->components.transform->position.y < 0.0f || entity->components.transform->position.y > 3.0f);
                if (validX  && validY)
                {
                    for (size_t i = 0; i < (sizeof(GameSceneEvents)/sizeof(Event)); i++)
                    {
                        if (GameSceneEvents[i].id == entity->components.health->event_id)
                        {
                            if (GameSceneEvents[i].dead != NULL)
                            {
                                GameSceneEvents[i].dead(scene->manager, entity);
                            }
                            return;
                        }
                    }
                }
            }
        }
    }
}

void UpdateRenderer(void* pd_ptr, Entity* entity)
{
    if (entity == NULL || pd_ptr == NULL) return;
    if (entity->active == false) return;
    PlaydateAPI* api = pd_ptr;
    LCDSprite* sprite = NULL;
    LCDBitmap* bitmap = NULL;
    int16_t z_order = -1;
    if (entity->components.sprite != NULL)
    {
        sprite = entity->components.sprite->_ptr;
        z_order = entity->components.sprite->order_in_layer;
        bitmap = entity->components.sprite->bitmap;
    }
    else if (entity->components.animated_sprite != NULL)
    {
        sprite = entity->components.animated_sprite->_ptr;
        z_order = entity->components.animated_sprite->order_in_layer;
        bitmap = entity->components.animated_sprite->bitmaps[entity->components.animated_sprite->frame_index];
        if (sprite != NULL)
        {
            api->sprite->setImage(sprite, bitmap, kBitmapUnflipped);
        }
    }
    else if (entity->components.button_img != NULL)
    {
        sprite = entity->components.button_img->sprite;
        if (entity->components.button_img->bitmaps_cnt <= entity->components.button_img->state) return;
        bitmap = entity->components.button_img->bitmaps[entity->components.button_img->state];
        z_order = entity->components.button_img->order_in_layer;
        if (sprite != NULL && bitmap != NULL)
        {
            api->sprite->setImage(sprite, bitmap, kBitmapUnflipped);
        }
    }

    if (sprite != NULL && bitmap != NULL && entity->components.transform != NULL)
    {
        api->sprite->setZIndex(sprite, z_order);
        api->sprite->moveTo(sprite, entity->components.transform->position.x * 80.0f, entity->components.transform->position.y * 80.0f);
        api->sprite->addSprite(sprite);
    }
}

void UpdateScale(Entity* entity, float scale)
{
    if (entity == NULL) return;
    if (entity->active == false) return;
}

void UpdateButtonImage(void* pd_ptr, Entity* entity, void* userdata)
{
    if (entity == NULL || pd_ptr == NULL) return;
    if (entity->active == false) return;
    PlaydateAPI* api = pd_ptr;
    if (entity->components.button_img != NULL)
    {
        if (entity->components.button_img->state == RELEASE)
        {
            for (size_t i = 0; i < (sizeof(MenuSceneEvents)/sizeof(Event)); i++)
            {
                if (MenuSceneEvents[i].id == entity->components.button_img->event_id)
                {
                    if (MenuSceneEvents[i].transition != NULL)
                    {
                        for(int i = 0; i <= 50; i++) {
                            api->system->logToConsole("%d", i);
                        }
                        MenuSceneEvents[i].transition(userdata);
                    }
                    break;
                }
            }
        }
    }
}

void UpdateBinding(void* scene_ptr, Entity *entity)
{
    if (entity == NULL || scene_ptr == NULL) return;
    if (entity->active == false) return;
    Scene* scene = scene_ptr;
    if (entity->components.binding != NULL)
    {
        for (size_t k = 0; k < Array1DTotalSize(entity->components.binding->others); k++)
        {
            Entity* other = Array1DItemAtIndex(entity->components.binding->others, k);
            for (size_t i = 0; i < (sizeof(GameSceneEvents)/sizeof(Event)); i++)
            {
                if (GameSceneEvents[i].id == entity->components.binding->event_id)
                {
                    if (GameSceneEvents[i].trigger != NULL)
                    {
                        GameSceneEvents[i].trigger(scene->manager, entity, other);
                    }
                    break;
                }
            }
        }
    }
}

void UpdateSpawn(void* pd_ptr, void* scene_ptr, void (*cb)(void* scene_ptr, Vec2 position))
{
    if (scene_ptr != NULL && pd_ptr != NULL)
    {
        PlaydateAPI* api = pd_ptr;
        unsigned int ms = api->system->getCurrentTimeMilliseconds();
        if (((ms % 1000) % 1000) < 30)
        {
            int id = randIntIn(ENTITY_ENEMY, ENTITY_ENEMY_MAX);
            Vec2 positions[2] = {
                {randRealIn(1, 5), 0},
                {1.1f, randRealIn(0, 3)}
            };

            int idx = randIntIn(0, 1);

            cb(scene_ptr, positions[idx]);

            Scene* scene = scene_ptr;
            if (Array1DTotalSize(scene->entities_active) > 50) return;
            Entity* earth = NULL;
            for (size_t i = 0; i < Array1DTotalSize(scene->entities_active); i++)
            {
                Entity* entity = Array1DItemAtIndex(scene->entities_active, i);
                if (entity->id == ENTITY_EARTH)
                {
                    earth = entity;
                    break;
                }
            }
            for (size_t i = 0; i < Array1DTotalSize(scene->entities); i++)
            {
                Entity* entity = Array1DItemAtIndex(scene->entities, i);
                if (entity->active == true) continue;
                if (entity->id == id)
                {
                    entity->active = true;
                    entity->components.transform->position.x = positions[idx].x;
                    entity->components.transform->position.y = positions[idx].y;
                    entity->components.motion->last_position.x = entity->components.transform->position.x;
                    entity->components.motion->last_position.y = entity->components.transform->position.y;
                    if (earth != NULL)
                    {
                        entity->components.motion->acceleration = scene->world->gravity;
                        entity->components.motion->direction = Vec2Normalize(Vec2Subtract(earth->components.transform->position, entity->components.transform->position));
                    }
                    Array1DPush(scene->entities_active, entity);
                    return;
                }
            }
        }
    }
}
