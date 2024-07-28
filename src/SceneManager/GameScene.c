#include "GameScene.h"
#include "pd_api.h"
#include <math.h>
#include "../Physics2D/DataStructs/QuadTree.h"
#include "../ecs/system.h"
#include "SceneManager.h"
#include "Events/SceneIDs.h"
#include "Events/EventIDs.h"
#include "../ecs/EntityIDs.h"

static struct QuadTree* tree = NULL;

static void UpdateArrowDirection(void* scene_ptr, Entity* enemy)
{
    Scene* scene = scene_ptr;
    SceneManager* manager = scene->manager;
    PlaydateAPI* api = manager->pd;
    if (scene == NULL) return;

    for (size_t i = 0; i < scene->entities_active->size; ++i)
    {
        Entity* entity = Array1DItemAtIndex(scene->entities_active, i);
        if (entity->id == ENTITY_ARROW_MINI_MAP)
        {
            if (entity->components.sprite != NULL)
            {
                if (entity->components.sprite->bitmap != NULL)
                {
                    double radian = atan(enemy->components.motion->direction.y / enemy->components.motion->direction.x);
                    float angle = (float)radian * (180.0f / 3.14f) * -1;
                    api->graphics->freeBitmap(entity->components.sprite->bitmap);
                    const char* outerr = NULL;
                    LCDBitmap* bitmap_ptr = api->graphics->loadBitmap("images/widgets/arrow/arrow", &outerr);
                    if (outerr != NULL)
                    {
                        api->system->logToConsole("Error: %s", outerr);
                        api->graphics->freeBitmap(bitmap_ptr);
                        return;
                    }
                    bitmap_ptr = api->graphics->rotatedBitmap(entity->components.sprite->bitmap, angle, 1, 1, NULL);
                    entity->components.sprite->bitmap = bitmap_ptr;
                }
            }
            break;
        }
    }
}

void GameSceneInit(void* pd_ptr, Scene *scene)
{
    if (scene == NULL || pd_ptr == NULL) return;
    tree = CreateQuadTreeNode(NULL, scene->world->w, scene->world->h, 0);

    Entity* score_board = NULL;
    Entity* earth_blood = NULL;
    { // widgets
        score_board = CreateEntity(ENTITY_SCORE_BOARD, scene->world, (Vec2){ 0.5f, 1.3f }, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.0f, 1.0f });
        if (score_board != NULL)
        {
            SceneAddGameObject(scene, score_board);
            AddAnimatedSpriteComponent(pd_ptr, score_board, "images/numbers/black", 9, 9, 10, 0.0f, false, 3);
        }
        earth_blood = CreateEntity(ENTITY_EARTH_BLOOD, scene->world, (Vec2){ 0.1f, 0.05f }, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.0f, 1.0f });
        if (earth_blood != NULL)
        {
            SceneAddGameObject(scene, earth_blood);
            AddAnimatedSpriteComponent(pd_ptr, earth_blood, "images/widgets/blood", 72, 12, 5, 0.0f, false, 2);
        }
    }
    { // earth
        Entity* earth = NULL;
        earth = CreateEntity(ENTITY_EARTH, scene->world, (Vec2){ 3.0f, 1.5f }, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.0f, 1.0f });
        if (earth != NULL)
        {
            SceneAddGameObject(scene, earth);
            // Add components
            AddSpriteComponent(pd_ptr, earth, "images/earth", false, 0.5f, 1);
            AddCircleColliderComponent(pd_ptr, tree, earth, (Vec2) { 0.0f, 0.0f }, 0.3f, EVT_GAME_EARTH_COLLIDED);
            AddHealthComponent(pd_ptr, earth, 100, EVT_GAME_EARTH_DEAD);
            AddBindingComponent(pd_ptr, earth, earth_blood, EVT_GAME_EARTH_HIT);
        }

        { // moon
            Entity* moon = CreateEntity(ENTITY_MOON, scene->world, (Vec2) { 2.0f, 1.0f }, (Vec2) { 0.0f, 0.0f }, (Vec2) { 1.0f, 1.0f });
            if (moon != NULL)
            {
                SceneAddGameObject(scene, moon);
                // Add components
                if (earth != NULL)
                {
                    moon->components.motion->direction = earth->components.transform->position;
                }
                AddKeyInputComponent(pd_ptr, moon, false, false, false, false, false, false, true);
                AddSpriteComponent(pd_ptr, moon, "images/moon", false, 0.5f, 1);
                AddCircleColliderComponent(pd_ptr, tree, moon, (Vec2) { 0.0f, 0.0f }, 0.15f, EVT_GAME_MOON_COLLIDED);
                AddBindingComponent(pd_ptr, moon, score_board, EVT_GAME_MOON_HIT);
            }
        }
        { // enemies
            for (int i = 0; i < (int)ENTITY_ENEMY_MAX; ++i)
            {
                Entity* enemy = CreateEntity(ENTITY_ENEMY + i, scene->world, (Vec2){0.0f, 0.0f}, (Vec2) { 0.0f, 0.0f }, (Vec2) { 1.0f, 1.0f });
                if (enemy != NULL)
                {
                    enemy->active = false;
                    SceneAddGameObject(scene, enemy);
                    // Add components
                    AddAnimatedSpriteComponent(pd_ptr, enemy, "images/enemy", 12, 12, 8, 0.5f, true, 1);
                    AddCircleColliderComponent(pd_ptr, tree, enemy, (Vec2) { 0.0f, 0.0f }, (float)(4.0f / 80.0f), EVT_GAME_ENEMY_COLLIDED);
                    AddHealthComponent(pd_ptr, enemy, 10, EVT_GAME_ENEMY_DEAD);
                    AddStrengthComponent(pd_ptr, enemy, 12.0f);
                }
            }
        }
    }
    {
        Entity* side_bar = CreateEntity(ENTITY_SIDE_BAR, scene->world, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
        if (side_bar != NULL)
        {
            SceneAddGameObject(scene, side_bar);
            // Add Components
            AddSpriteComponent(pd_ptr, side_bar, "images/side_bar/side_bar_bg", false, 0.0f, 1);
        }
    }
    {
        Entity* arrow_mini_map = CreateEntity(ENTITY_ARROW_MINI_MAP, scene->world, (Vec2) {0.5625f, 2.3f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
        if (arrow_mini_map != NULL)
        {
            SceneAddGameObject(scene, arrow_mini_map);
            // Add Components
            AddSpriteComponent(pd_ptr, arrow_mini_map, "images/widgets/arrow/arrow", false, 0.5f, 3);
        }
    }
}

void GameSceneUpdate(void* pd_ptr, Scene *scene, float dt)
{
    if (scene == NULL || pd_ptr == NULL) return;
    PlaydateAPI* api = pd_ptr;
    (void)api;
    QuadTreeClear(tree);
    unsigned int tick = ((PlaydateAPI*)pd_ptr)->system->getCurrentTimeMilliseconds();

    UpdateSpawn(pd_ptr, scene, UpdateArrowDirection);
    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entities_active->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entities_active, i);
        UpdateScale(entity, 1);
        UpdateRotation(entity);
        UpdateMovement(entity, dt);
        UpdateSprite(entity, tick);
        UpdateCollider(entity, tree);
        UpdateAnimateSprite(entity, tick);
    }
    for (size_t i = 0; i < scene->entities_active->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entities_active, i);
        UpdateCollisionDetection(scene, entity, tree);
    }
    for (size_t i = 0; i < scene->entities_active->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entities_active, i);
        UpdateHealth(pd_ptr, scene, entity);
        UpdateBinding(scene, entity);
    }
}

void GameSceneRender(void* pd_ptr, Scene *scene)
{
    if (scene == NULL || pd_ptr == NULL) return;
    if (scene->active == false) return;
    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entities_active->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entities_active, i);
        UpdateRenderer(pd_ptr, entity);
    }
}

void GameSceneEvent(void *pd_ptr, Scene *scene, void *manager)
{
    if (scene == NULL || pd_ptr == NULL) return;
    PlaydateAPI* api = pd_ptr;
    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entities_active->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entities_active, i);
        if (entity != NULL)
        {
            if (entity->active == false) continue;
            if (entity->components.transform != NULL)
            {
                if (entity->components.motion != NULL)
                {
                    entity->components.motion->last_position.x = entity->components.transform->position.x;
                    entity->components.motion->last_position.y = entity->components.transform->position.y;
                }
                if (entity->components.input != NULL)
                {
                    if (entity->components.input->crank == true)
                    {
                        float angle_rad = api->system->getCrankAngle() * (3.14159265358979323846f / 180.0f);
                        entity->components.transform->rotation.x = 0.5f * (float)(cos((double)angle_rad));
                        entity->components.transform->rotation.y = 0.5f * (float)(sin((double)angle_rad));
                    }
                    // ...
                }
            }
        }
    }
    { // system
        PDButtons current;
        PDButtons pushed;
        PDButtons released;
        api->system->getButtonState(&current, &pushed, &released);
        switch (current) {
        case kButtonB:
            if (current == pushed)
            {
                SceneManagerTransition(manager, MENU_SCENE);
            }
            break;
        default:
            break;
        }
    }
}
