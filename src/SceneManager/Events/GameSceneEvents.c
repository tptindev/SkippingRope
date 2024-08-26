#include "GameSceneEvents.h"
#include "SceneIDs.h"
#include "../../ecs/EntityIDs.h"
#include "pd_api.h"
#include <stdio.h>
#include <fcntl.h>
#include <cJSON.h>
#include "../../XUtils.h"

void EVT_GAME_BACK_MENU_FUNC(SceneManager* manager)
{
    SceneManagerTransition(manager, MENU_SCENE);
}

void EVT_GAME_EARTH_COLLIDED_FUNC(Entity* entity, Entity* other)
{
    if (entity != NULL && other != NULL)
    {
        if (entity->components.health != NULL && other->components.strength != NULL)
        {
            entity->components.health->current -= other->components.strength->damage;
        }
    }
}

void EVT_GAME_MOON_COLLIDED_FUNC(Entity* entity, Entity* other)
{
}

void EVT_GAME_ENEMY_COLLIDED_FUNC(Entity* entity, Entity* other)
{
    if (entity != NULL)
    {
        if (entity->components.health != NULL)
        {
            entity->components.health->current = 0;
        }
    }
}

void EVT_GAME_EARTH_DEAD_FUNC(SceneManager* manager, Entity* enemy)
{
    PlaydateAPI* api = manager->pd;
    if (manager->current_scene != NULL)
    {
        for (size_t i = 0; i < manager->current_scene->entities_active->size; i++)
        {
            Entity* entity = Array1DItemAtIndex(manager->current_scene->entities_active, i);
            if (entity != NULL)
            {
                if (entity->id == ENTITY_SCORE_BOARD)
                {
                    if (entity->components.score_board == NULL) return;
                }
            }
        }
    }
    SceneManagerTransition(manager, GAME_OVER_SCENE);
}

void EVT_GAME_ENEMY_DEAD_FUNC(SceneManager* manager, Entity* enemy)
{
    PlaydateAPI* api = manager->pd;
    (void)api;
    enemy->active = false;
    if (manager->current_scene != NULL)
    {
        for (size_t i = 0; i < manager->current_scene->entities_active->size; i++)
        {
            Entity* entity = Array1DItemAtIndex(manager->current_scene->entities_active, i);
            if (entity != NULL)
            {
                if (entity->id == enemy->id)
                {
                    Array1DDelete(manager->current_scene->entities_active, i);
                }
            }
        }
    }
}

void EVT_GAME_EARTH_HIT_FUNC(SceneManager* manager, Entity *entity, Entity* other)
{
    if (entity != NULL && manager != NULL && other != NULL)
    {
        PlaydateAPI* api = manager->pd;
        (void)api;

        switch (other->id) {
        case ENTITY_EARTH_BLOOD:
        {
            if (entity->components.health != NULL)
            {
                float current = entity->components.health->current;
                float max = entity->components.health->max;
                if (other->components.animated_sprite != NULL)
                {
                    if (other->components.animated_sprite->_ptr != NULL)
                    {
                        int frame_count = other->components.animated_sprite->frame_count - 1;
                        other->components.animated_sprite->frame_index = (int)((current / max) * frame_count);
                    }
                }
            }
            break;
        }
        default:
            break;
        }
    }
}

void EVT_GAME_MOON_HIT_FUNC(SceneManager* manager, Entity *entity, Entity* other)
{
    if (other != NULL && manager != NULL && other != NULL)
    {
        if (entity->components.collider == NULL) return;
        if (entity->components.collider->collided)
        {
            PlaydateAPI* api = manager->pd;
            (void)api;
            switch (other->id) {
            case ENTITY_SCORE_BOARD:
            {
                if (other->components.score_board != NULL)
                {
                    other->components.score_board->current++;

                    int file_size = 0;
                    char *buffer = NULL;
                    SDFile* file = NULL;
                    file = api->file->open("data/userdata", kFileRead | kFileReadData | kFileWrite | kFileAppend);
                    if (file != NULL)
                    {
                        api->file->seek(file, 0, SEEK_END);
                        file_size = api->file->tell(file);
                        api->file->seek(file, 0, SEEK_SET);

                        buffer = (char *)malloc(sizeof(char) * (file_size + 1));
                        if (api->file->read(file, buffer, file_size) == -1)
                        {
                            api->system->logToConsole("Cant Read File");
                        }
                        else
                        {
                            buffer[file_size] = '\0';
//                            api->system->logToConsole("Buffer: %s", (char*)buffer);
                            cJSON* jsonObj = NULL;
                            if (*buffer == '\0')
                            {
                                jsonObj = cJSON_CreateObject();
                            }
                            else
                            {
                                jsonObj = cJSON_Parse(buffer);
                            }

                            if (jsonObj != NULL)
                            {
                                cJSON_AddBoolToObject(jsonObj, "game_over", false);
                                cJSON_AddNumberToObject(jsonObj, "score", other->components.score_board->current);
                            }

                            char *str = cJSON_Print(jsonObj);

                            if (api->file->write(file, str, file_size) == -1)
                            {
                                api->system->logToConsole("Cant Write File");
                            }

                            cJSON_Delete(jsonObj);
                        }
                        free(buffer);
                        buffer = NULL;
                        api->file->close(file);
                    }
                }
                break;
            }
            default:
                break;
            }
        }
    }
}
