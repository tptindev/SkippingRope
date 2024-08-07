#include "entity.h"
#include "component.h"
#include <stdlib.h>
#include <stdio.h>
#include "pd_api.h"
#include "../XUtils.h"

Entity* CreateEntity(unsigned int id, World2D* world, Vec2 position, Vec2 rotation, Vec2 scale)
{
	if (world == NULL) return NULL;
	Entity* entity = malloc(sizeof(Entity));
	if (entity != NULL) {
        entity->id = id;
        entity->world = world;
        entity->active = true;
		entity->components.transform = malloc(sizeof(Transform));
		if (entity->components.transform != NULL)
		{
			// Position
			entity->components.transform->position.x = position.x;
			entity->components.transform->position.y = position.y;
			
			// Rotation
			entity->components.transform->rotation.x = rotation.x;
			entity->components.transform->rotation.y = rotation.y;

			// Scale
			entity->components.transform->scale.x = scale.x;
			entity->components.transform->scale.y = scale.y;
		}

		entity->components.motion = malloc(sizeof(Motion));
		if (entity->components.motion != NULL)
		{
			entity->components.motion->acceleration.x = 0.0f;
			entity->components.motion->acceleration.y = 0.0f;

			entity->components.motion->last_position.x = 0.0f;
			entity->components.motion->last_position.y = 0.0f;

			entity->components.motion->direction.x = 0.0f;
			entity->components.motion->direction.y = 0.0f;
		}

		entity->components.collider = NULL;
		entity->components.input = NULL;
		entity->components.sprite = NULL;
		entity->components.animated_sprite = NULL;
		entity->components.health = NULL;
        entity->components.button_img = NULL;
        entity->components.strength = NULL;
        entity->components.binding = NULL;
        entity->components.score_board = NULL;
        entity->components.score_board_visual = NULL;
	}
	return entity;
}

void DestroyEntity(void* api, Entity* entity)
{
    if (entity != NULL && api != NULL)
    {
        FreeEntity(api, entity);
    }
}

void FreeEntity(void *api, Entity* entity)
{
    if (entity == NULL || api == NULL) return;
    if (entity->components.transform != NULL) freeObjPtr((void*)entity->components.transform);
    if (entity->components.motion != NULL) freeObjPtr((void*)entity->components.motion);
    if (entity->components.collider != NULL)
    {
        freeObjPtr((void*)entity->components.collider->shape.define);
        freeObjPtr((void*)entity->components.collider);
    }
    if (entity->components.input != NULL) freeObjPtr((void*)entity->components.input);
    if (entity->components.sprite != NULL)
    {
        freeSprite(api, entity->components.sprite->_ptr);
        freeBitmap(api, entity->components.sprite->bitmap);
        freeObjPtr((void*)entity->components.sprite);
    }
    if (entity->components.animated_sprite != NULL)
    {
        freeSprite(api, entity->components.animated_sprite->_ptr);
        void** bitmaps = entity->components.animated_sprite->bitmaps;
        if (bitmaps != NULL)
        {
            for (int i = 0; i < entity->components.animated_sprite->frame_count; i++)
            {
                freeBitmap(api, entity->components.animated_sprite->bitmaps[i]);
            }
            freeObjPtr(bitmaps);
        }
        freeObjPtr((void*)entity->components.animated_sprite);
    }
    if (entity->components.health != NULL)
    {
        freeObjPtr((void*)entity->components.health);
    }
    if (entity->components.button_img != NULL)
    {
        if (entity->components.button_img->bitmaps != NULL)
        {
            for (size_t i = 0; i < 3; i++)
            {
                freeBitmap(api, entity->components.button_img->bitmaps[i]);
            }
            free(entity->components.button_img->bitmaps);
            entity->components.button_img->bitmaps = NULL;
        }
        if (entity->components.button_img->sprite != NULL)
        {
            freeSprite(api, entity->components.button_img->sprite);
        }
        freeObjPtr((void*)entity->components.button_img);
    }
    if (entity->components.strength != NULL)
    {
        freeObjPtr(entity->components.strength);
    }
    if (entity->components.binding != NULL)
    {
        FreeArray1D(entity->components.binding->others);
        freeObjPtr(entity->components.binding);
    }
    if (entity->components.score_board != NULL)
    {
        freeObjPtr(entity->components.score_board);
    }
    if (entity->components.score_board_visual != NULL)
    {
        for (size_t i = 0; i < (sizeof(entity->components.score_board_visual->bitmaps)/sizeof(void*)); i++)
        {
            if (i < 6)
            {
                freeSprite(api, entity->components.score_board_visual->sprites[i]);
            }
            freeBitmap(api, entity->components.score_board_visual->bitmaps[i]);
        }
        freeObjPtr(entity->components.score_board_visual);
    }
    freeObjPtr(entity);
}

void AddAnimatedSpriteComponent(void* pd_ptr, Entity* entity, const char* source, int frame_width, int frame_height, int frame_count, float offset, bool running, int16_t z_order)
{
    PlaydateAPI* api = pd_ptr;
	entity->components.animated_sprite = malloc(sizeof(AnimatedSprite));
	if (entity->components.animated_sprite != NULL && entity->components.transform != NULL)
	{
		entity->components.animated_sprite->source = source;
		entity->components.animated_sprite->frame_index = 0;
		entity->components.animated_sprite->frame_width = frame_width;
		entity->components.animated_sprite->frame_height = frame_height;
		entity->components.animated_sprite->frame_count = frame_count;
		entity->components.animated_sprite->order_in_layer = z_order;
		entity->components.animated_sprite->bitmaps = NULL;
		entity->components.animated_sprite->_ptr = NULL;
        entity->components.animated_sprite->running = running;

		void* (*bitmaps) = malloc(sizeof(void*) * frame_count);
		if (bitmaps != NULL)
		{
			for (int i = 0; i < frame_count; i++)
			{
				char path_buffer[32];
				snprintf(path_buffer, sizeof(path_buffer), "%s/%d.png", source, i);

				const char* outerr = NULL;
				bitmaps[i] = api->graphics->loadBitmap(path_buffer, &outerr); // bitmaps[i] <=> *(bitmaps + i)

                if (outerr != NULL)
				{
					api->system->logToConsole("Error: %s", outerr);
                    if (bitmaps[i] != NULL)
                    {
                        freeBitmap(api, bitmaps[i]);
                    }
                    freeObjPtr(bitmaps);
					return;
				}
			}
		}

		entity->components.animated_sprite->bitmaps = bitmaps;
		LCDSprite* sprite_ptr = api->sprite->newSprite();
		if (sprite_ptr != NULL && bitmaps != NULL)
        {
            api->sprite->setCenter(sprite_ptr, offset, offset);
			entity->components.animated_sprite->_ptr = sprite_ptr;
		}
		else
		{
			freeSprite(api, sprite_ptr);
		}
	}
}

void AddSpriteComponent(void* pd_ptr, Entity* entity, const char* source, bool flip, float offset, int16_t z_order)
{
    PlaydateAPI* api = pd_ptr;
	entity->components.sprite = malloc(sizeof(Sprite));
	if (entity->components.sprite != NULL && entity->components.transform != NULL)
	{
		entity->components.sprite->source = source;
		entity->components.sprite->flip = flip;
		entity->components.sprite->order_in_layer = z_order;
		entity->components.sprite->bitmap = NULL;
		entity->components.sprite->_ptr = NULL;

		const char* outerr = NULL;
		LCDBitmap* bitmap_ptr = api->graphics->loadBitmap(source, &outerr);
		if (outerr != NULL)
		{
			api->system->logToConsole("Error: %s", outerr);
			api->graphics->freeBitmap(bitmap_ptr);
			return;
        }
        entity->components.sprite->bitmap = bitmap_ptr;
		LCDSprite* sprite_ptr = api->sprite->newSprite();
		if (sprite_ptr != NULL)
		{
			entity->components.sprite->_ptr = sprite_ptr;
			api->sprite->setImage(sprite_ptr, bitmap_ptr, kBitmapUnflipped);
            api->sprite->setZIndex(sprite_ptr, z_order);
            api->sprite->setCenter(sprite_ptr, offset, offset);
            api->sprite->moveTo(
				sprite_ptr,
				entity->components.transform->position.x * 80.0f,
				entity->components.transform->position.y * 80.0f
			);
		}
		else
		{
			api->sprite->freeSprite(sprite_ptr);
		}
	}
}

void AddKeyInputComponent(void* pd_ptr, Entity* entity, bool left, bool right, bool up, bool down, bool a, bool b, bool crank)
{
	entity->components.input = malloc(sizeof(KeyInput));
	if (entity->components.input != NULL)
	{
		entity->components.input->left = left;
		entity->components.input->right = right;
		entity->components.input->up = up;
		entity->components.input->down = down;
		entity->components.input->a = a;
		entity->components.input->b = b;
		entity->components.input->crank = crank;
	}
}

void AddCircleColliderComponent(void* pd_ptr, struct QuadTree* tree, Entity* entity, Vec2 offset, float radius, int event_id)
{
    entity->components.collider = malloc(sizeof(Collider));
	if (entity->components.collider != NULL && entity->components.transform != NULL)
	{
        entity->components.collider->event_id = event_id;
        entity->components.collider->collided = false;
		entity->components.collider->offset.x = offset.x;
		entity->components.collider->offset.y = offset.y;
		entity->components.collider->shape.type = CIRCLE;
		entity->components.collider->shape.define = CreateCircle(
			Vec2Add(entity->components.transform->position, offset),
			radius
        );
        if (entity->active == false) return;
        GetCircleBoundary(&entity->components.collider->shape.box, entity->components.collider->shape.define);
    }
}

void AddHealthComponent(void* pd_ptr, Entity* entity, float max, int event_id)
{
	entity->components.health = malloc(sizeof(Health));
	if (entity->components.health != NULL)
	{
        entity->components.health->event_id = event_id;
		entity->components.health->current = max;
		entity->components.health->max = max;
	}
}


void AddButtonImageComponent(void *pd_ptr, Entity *entity, BtnState state, int event_id, const char* imgdir, float offset, int16_t z_order)
{
    if (pd_ptr == NULL || entity == NULL) return;
    PlaydateAPI* api = pd_ptr;
    entity->components.button_img = malloc(sizeof(ButtonImage));
    if (entity->components.button_img != NULL)
    {
		entity->components.button_img->order_in_layer = z_order;
        entity->components.button_img->state = state;
        entity->components.button_img->imgdir = imgdir;
		entity->components.button_img->bitmaps = malloc(sizeof(void*) * 3);
        entity->components.button_img->bitmaps_cnt = 3;
        entity->components.button_img->event_id = event_id;
        for (int i = 0; i < entity->components.button_img->bitmaps_cnt; i++)
        {
			entity->components.button_img->bitmaps[i] = NULL;
            char path_buffer[128];
            snprintf(path_buffer, sizeof(path_buffer), "%s/%d", imgdir, i);
			api->system->logToConsole("%s", path_buffer);
			const char* outerr = NULL;
            LCDBitmap* bitmap_ptr = api->graphics->loadBitmap(path_buffer, &outerr);
            if (outerr != NULL)
            {
                api->system->logToConsole("Error: %s", outerr);
                api->graphics->freeBitmap(bitmap_ptr);
                continue;
            }
            entity->components.button_img->bitmaps[i] = bitmap_ptr;
        }

        LCDSprite* sprite_ptr = api->sprite->newSprite();
        if (sprite_ptr != NULL)
        {
            entity->components.button_img->sprite = sprite_ptr;
            api->sprite->setImage(sprite_ptr, entity->components.button_img->bitmaps[(int)state], kBitmapUnflipped);
            api->sprite->setZIndex(sprite_ptr, z_order);
            api->sprite->setCenter(sprite_ptr, offset, offset);
            api->sprite->moveTo(
                sprite_ptr,
                entity->components.transform->position.x * 80.0f,
                entity->components.transform->position.y * 80.0f
                );
        }
        else
        {
            api->sprite->freeSprite(sprite_ptr);
        }
    }
}

void AddStrengthComponent(void *pd_ptr, Entity *entity, float damage)
{
    entity->components.strength = malloc(sizeof(Strength));
    if (entity->components.strength != NULL)
    {
        entity->components.strength->damage = damage;
    }
}

void AddBindingComponent(void *pd_ptr, Entity *entity, void *other, int event_id)
{
    if (entity->components.binding == NULL)
    {
        entity->components.binding = malloc(sizeof(Binding));
        entity->components.binding->event_id = event_id;
        entity->components.binding->others = CreateArray1D();
    }

    Array1DPush(entity->components.binding->others, other);
}

void RemoveComponent(Entity *entity, void *component)
{

}

void AddScoreBoardComponent(void *pd_ptr, Entity *entity)
{
    entity->components.score_board = malloc(sizeof(ScoreBoard));
    if (entity->components.score_board != NULL)
    {
        entity->components.score_board->current = 0;
        entity->components.score_board->high = 0;
    }
}

void AddScoreBoardVisualComponent(void *pd_ptr, Entity *entity, const char *source, int16_t z_order)
{
    if (pd_ptr == NULL || entity == NULL) return;
    PlaydateAPI* api = pd_ptr;
    entity->components.score_board_visual = malloc(sizeof(ScoreBoardVisual));
    if (entity->components.score_board_visual != NULL)
    {
        for (size_t i = 0; i < (sizeof(entity->components.score_board_visual->bitmaps)/sizeof(void*)); i++)
        {
            char path_buffer[128];
            snprintf(path_buffer, sizeof(path_buffer), "%s/%d", source, (int)i);
            api->system->logToConsole("%s", path_buffer);
            const char* outerr = NULL;
            LCDBitmap* bitmap_ptr = api->graphics->loadBitmap(path_buffer, &outerr);
            if (outerr != NULL)
            {
                api->system->logToConsole("Error: %s", outerr);
                bitmap_ptr = NULL;
            }
            entity->components.score_board_visual->bitmaps[i] = bitmap_ptr;
            if (i < 6)
            {
                LCDSprite* sprite_ptr = api->sprite->newSprite();
                if (sprite_ptr != NULL)
                {
                    api->sprite->setImage(sprite_ptr, entity->components.score_board_visual->bitmaps[0], kBitmapUnflipped);
                    api->sprite->setZIndex(sprite_ptr, z_order);
                    api->sprite->setCenter(sprite_ptr, 0.5f, 0.5f);
                    api->sprite->moveTo(
                        sprite_ptr,
                        entity->components.transform->position.x * 80.0f + (i * 9),
                        entity->components.transform->position.y * 80.0f
                        );
                    entity->components.score_board_visual->sprites[i] = sprite_ptr;
                }
            }
        }
    }
}
