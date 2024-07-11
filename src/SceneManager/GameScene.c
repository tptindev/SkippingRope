#include "GameScene.h"
#include "pd_api.h"
#include "../Physics2D/DataStructs/QuadTree.h"
#include "../ecs/system.h"

static struct QuadTree* tree = NULL;

void GameSceneInit(void* api, Scene *scene)
{
    if (scene == NULL || api == NULL) return;
    tree = CreateQuadTreeNode(NULL, scene->world->w, scene->world->h, 0);


    Entity* earth = NULL;
    { // earth
        earth = CreateEntity(scene->world, (Vec2){ 2.5f, 1.5f }, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.0f, 1.0f });
        if (earth != NULL)
        {
            SceneAddGameObject(scene, earth);
            // Add components
            AddSpriteComponent(api, earth, "images/earth", false, 1);
            AddCircleColliderComponent(api, tree, earth, (Vec2) { 0.0f, 0.0f }, 0.3f);
            AddHealthComponent(api, earth, 100);
        }
    }

    { // moon
        Entity* moon = NULL;
        moon = CreateEntity(scene->world, (Vec2){ 2.0f, 1.0f }, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.0f, 1.0f });
        if (moon != NULL)
        {
            SceneAddGameObject(scene, moon);
            // Add components
            if (earth != NULL)
            {
                moon->components.motion->direction = earth->components.transform->position;
            }
            AddKeyInputComponent(api, moon, false, false, false, false, false, false, true);
            AddSpriteComponent(api, moon, "images/moon", false, 1);
            AddCircleColliderComponent(api, tree, moon, (Vec2) { 0.0f, 0.0f }, 0.15f);
            AddHealthComponent(api, moon, 40);
        }
    }
    { // enemy
        Entity* enemy = NULL;
        enemy = CreateEntity(scene->world, (Vec2) { 0.0f, 0.0f }, (Vec2) { 0.0f, 0.0f }, (Vec2) { 1.0f, 1.0f });
        if (enemy != NULL)
        {
            SceneAddGameObject(scene, enemy);
            // Add components
            if (earth != NULL)
            {
                enemy->components.motion->acceleration = scene->world->gravity;
                enemy->components.motion->direction = Vec2Normalize(Vec2Subtract(earth->components.transform->position, enemy->components.transform->position));
            }
            AddAnimatedSpriteComponent(api, enemy, "images/enemy", 12, 12, 8, 1);
            AddCircleColliderComponent(api, tree, enemy, (Vec2) { 0.0f, 0.0f }, (float)(4.0f/80.0f));
            AddHealthComponent(api, enemy, 10);
        }
    }
}

void GameSceneUpdate(void* api, Scene *scene, float dt)
{
    if (scene == NULL || api == NULL) return;
    QuadTreeClear(tree);
    unsigned int tick = ((PlaydateAPI*)api)->system->getCurrentTimeMilliseconds();

    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entites->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entites, i);
        UpdateInput(api, entity);
        UpdateScale(entity, 1);
        UpdateRotation(entity);
        UpdateMovement(entity, dt);
        UpdateCollider(entity, tree);
        UpdateSprite(entity, tick);
        UpdateAnimateSprite(entity, tick);
    }
    for (size_t i = 0; i < scene->entites->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entites, i);
        UpdateCollision(entity, tree, NULL);
    }
    for (size_t i = 0; i < scene->entites->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entites, i);
        UpdateHealth(api, scene->world, &entity, NULL);
    }
}

void GameSceneRender(void* api, Scene *scene)
{
    if (scene == NULL || api == NULL) return;
    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entites->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entites, i);
        UpdateRenderer(api, entity);
    }
}
