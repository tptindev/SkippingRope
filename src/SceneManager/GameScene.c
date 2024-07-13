#include "GameScene.h"
#include "pd_api.h"
#include "../Physics2D/DataStructs/QuadTree.h"
#include "../ecs/system.h"
#include "SceneManager.h"
#include "SceneIDs.h"

static struct QuadTree* tree = NULL;

void GameSceneInit(void* pd_ptr, Scene *scene)
{
    if (scene == NULL || pd_ptr == NULL) return;
    tree = CreateQuadTreeNode(NULL, scene->world->w, scene->world->h, 0);


    Entity* earth = NULL;
    { // earth
        earth = CreateEntity(scene->world, (Vec2){ 2.5f, 1.5f }, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.0f, 1.0f });
        if (earth != NULL)
        {
            SceneAddGameObject(scene, earth);
            // Add components
            AddSpriteComponent(pd_ptr, earth, "images/earth", false, 0.5f, 1);
            AddCircleColliderComponent(pd_ptr, tree, earth, (Vec2) { 0.0f, 0.0f }, 0.3f);
            AddHealthComponent(pd_ptr, earth, 100);
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
            AddKeyInputComponent(pd_ptr, moon, false, false, false, false, false, false, true);
            AddSpriteComponent(pd_ptr, moon, "images/moon", false, 0.5f, 1);
            AddCircleColliderComponent(pd_ptr, tree, moon, (Vec2) { 0.0f, 0.0f }, 0.15f);
            AddHealthComponent(pd_ptr, moon, 40);
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
            AddAnimatedSpriteComponent(pd_ptr, enemy, "images/enemy", 12, 12, 8, 0.5f, 1);
            AddCircleColliderComponent(pd_ptr, tree, enemy, (Vec2) { 0.0f, 0.0f }, (float)(4.0f/80.0f));
            AddHealthComponent(pd_ptr, enemy, 10);
        }
    }
}

void GameSceneUpdate(void* pd_ptr, Scene *scene, float dt)
{
    if (scene == NULL || pd_ptr == NULL) return;
    QuadTreeClear(tree);
    unsigned int tick = ((PlaydateAPI*)pd_ptr)->system->getCurrentTimeMilliseconds();

    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entites->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entites, i);
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
        UpdateHealth(pd_ptr, scene->world, &entity, NULL);
    }
}

void GameSceneRender(void* pd_ptr, Scene *scene)
{
    if (scene == NULL || pd_ptr == NULL) return;
    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entites->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entites, i);
        UpdateRenderer(pd_ptr, entity);
    }
}

void GameSceneEvent(void *pd_ptr, Scene *scene, void *manager)
{
    if (scene == NULL || pd_ptr == NULL) return;
    PlaydateAPI* api = pd_ptr;
    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entites->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entites, i);
        if (entity != NULL)
        {
            if (entity->components.transform != NULL)
            {
                if (entity->components.motion != NULL)
                {
                    entity->components.motion->last_position.x = entity->components.transform->position.x;
                    entity->components.motion->last_position.y = entity->components.transform->position.y;
                }
                if (entity->components.input != NULL && entity->components.transform != NULL)
                {
                    if (entity->components.input->crank == true)
                    {
                        double angle_rad = api->system->getCrankAngle() * (3.14159265358979323846f / 180.0f);
                        entity->components.transform->rotation.x = (float)(0.5f * cos(angle_rad));
                        entity->components.transform->rotation.y = (float)(0.5f * sin(angle_rad));
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
                SceneManagerTransition(manager, MENU);
            }
            break;
        default:
            break;
        }
    }
}
