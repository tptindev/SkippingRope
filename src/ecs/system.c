#include "system.h"
#include "entity.h"
#include "component.h"
#include "../Physics2D/Collision.h"
#include "pd_api.h"
#include "../SceneManager/EventDefines.h"

void UpdateRotation(Entity* entity)
{
    if (entity == NULL) return;
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
    if (entity->components.transform != NULL)
    {
        entity->components.transform->position.x += buffer.x;
        entity->components.transform->position.y += buffer.y;
    }
}

void UpdateCollider(Entity* entity, struct QuadTree* tree)
{
    if (entity == NULL) return;
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

void UpdateCollision(Entity* entity, struct QuadTree* tree, void (*callback)(Entity* a, Entity* b))
{
    if (entity == NULL || callback == NULL) return;
    Array1D* nodes = CreateArray1D();
    QuadTreeSearch(tree, nodes, &entity->components.collider->shape.box);
    Circle* c0 = entity->components.collider->shape.define;
    for (int i = 0; i < nodes->size; i++)
    {
        struct QuadTree* _node = Array1DItemAtIndex(nodes, i);
        Array1D* _objs = _node->objects;
        for (int j = 0; j < _node->objects->size; ++j)
        {
            Entity* other = Array1DItemAtIndex(_objs, j);
            if (other == NULL) continue;
            if (entity->id != other->id)
            {
                Circle* cx = other->components.collider->shape.define;

                bool collided = IsCollisionCircle(c0, cx);
                if (collided == true)
                {
                    callback(entity, other);
                }
            }
        }
    }
}

void UpdateSprite(Entity* entity, unsigned int tick)
{
    if (entity == NULL) return;
    if (entity->components.sprite != NULL)
    {
    }
}

void UpdateAnimateSprite(Entity* entity, unsigned int tick)
{
    if (entity == NULL) return;
    if (entity->components.animated_sprite != NULL)
    {
        int frame_count = entity->components.animated_sprite->frame_count;
        entity->components.animated_sprite->frame_index = (tick % frame_count);
    }
}

void UpdateHealth(void* pd_ptr, World2D* world, Entity** entity, void (*callback)(void* api, World2D* world, Entity** entity, void* health))
{

    if (entity == NULL || pd_ptr == NULL) return;
    if (*entity == NULL) return;
    if ((*entity)->components.health != NULL && callback != NULL)
    {
        callback(pd_ptr, world, entity, (*entity)->components.health);
    }
}

void UpdateRenderer(void* pd_ptr, Entity* entity)
{
    if (entity == NULL || pd_ptr == NULL) return;
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
        api->sprite->setImage(sprite, bitmap, kBitmapUnflipped);
    }
    else if (entity->components.button_img != NULL)
    {
        sprite = entity->components.button_img->sprite;
        bitmap = entity->components.button_img->bitmaps[entity->components.button_img->status];
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
}

void UpdateButtonImage(Entity* entity, void* userdata)
{
    if (entity == NULL) return;
    if (entity->components.button_img != NULL)
    {
        if (entity->components.button_img->state == PUSHED)
        {
            for (size_t i = 0; i < (sizeof(MenuSceneEvents)/sizeof(Event)); i++)
            {
                if (MenuSceneEvents[i].id == entity->components.button_img->event_id)
                {
                    MenuSceneEvents[i].fn(userdata);
                }
            }
        }
    }
}
