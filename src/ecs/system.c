#include "system.h"
#include "entity.h"
#include "component.h"
#include "../Physics2D/Collision.h"
#include "pd_api.h"
#include "../SceneManager/Events/EventDefines.h"
#include "../ecs/EntityIDs.h"
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

void UpdateCollisionDetection(Entity* entity, struct QuadTree* tree)
{
    if (entity == NULL || tree == NULL) return;
    if (entity->components.collider == NULL) return;
    Array1D* nodes = CreateArray1D();
    QuadTreeSearch(tree, nodes, &entity->components.collider->shape.box);
    Circle* c0 = entity->components.collider->shape.define;
    float y = entity->components.collider->shape.box.y;

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
                    }
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

void UpdateHealth(void* pd_ptr, void* scene_ptr, Entity* entity)
{
    if (entity == NULL || pd_ptr == NULL || scene_ptr == NULL) return;
    Scene* scene = scene_ptr;
    if (entity->components.health != NULL)
    {
        if (entity->components.health->current <= 0)
        {
            for (size_t i = 0; i < (sizeof(GameSceneEvents)/sizeof(Event)); i++)
            {
                if (GameSceneEvents[i].id == entity->components.health->event_id)
                {
                    if (GameSceneEvents[i].transition != NULL)
                    {
                        GameSceneEvents[i].transition(scene->manager);
                    }
                    if (GameSceneEvents[i].collision != NULL)
                    {
                        GameSceneEvents[i].collision(entity, NULL);
                    }
                    if (GameSceneEvents[i].dead != NULL)
                    {
                        GameSceneEvents[i].dead(scene->manager, entity);
                    }
                    break;
                }
            }
        }
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
}

void UpdateButtonImage(void* pd_ptr, Entity* entity, void* userdata)
{
    if (entity == NULL && pd_ptr == NULL) return;
    PlaydateAPI* api = pd_ptr;
    if (entity->components.button_img != NULL)
    {
        if (entity->components.button_img->state == RELEASE)
        {
            for (size_t i = 0; i < (sizeof(MenuSceneEvents)/sizeof(Event)); i++)
            {
                if (MenuSceneEvents[i].id == entity->components.button_img->event_id)
                {
                    if (GameSceneEvents[i].transition != NULL)
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

void UpdateSpawnEntity(void *pd_ptr, void* scene_ptr, Entity* earth, void* tree,  size_t N)
{
    if (earth == NULL || scene_ptr == NULL || pd_ptr == NULL || tree == NULL) return;
    Scene* scene = scene_ptr;
    Entity* enemy = CreateEntity(ENTITY_ENEMY, scene->world, (Vec2) { 1.1f, 0.0f }, (Vec2) { 0.0f, 0.0f }, (Vec2) { 1.0f, 1.0f });
    if (enemy != NULL)
    {
        SceneAddGameObject(scene, enemy);
        // Add components
        if (earth != NULL)
        {
            enemy->components.motion->acceleration = scene->world->gravity;
            enemy->components.motion->direction = Vec2Normalize(Vec2Subtract(earth->components.transform->position, enemy->components.transform->position));
        }
        AddAnimatedSpriteComponent(pd_ptr, enemy, "images/enemy", 12, 12, 8, 0.5f, 1);
        AddCircleColliderComponent(pd_ptr, tree, enemy, (Vec2) { 0.0f, 0.0f }, (float)(4.0f / 80.0f), EVT_GAME_ENEMY_COLLIDED);
        AddHealthComponent(pd_ptr, enemy, 10, EVT_GAME_ENEMY_DEAD);
        AddStrengthComponent(pd_ptr, enemy, 20.0f);
    }
}
