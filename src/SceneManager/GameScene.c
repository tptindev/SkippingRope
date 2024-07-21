#include "GameScene.h"
#include "pd_api.h"
#include "../Physics2D/DataStructs/QuadTree.h"
#include "../ecs/system.h"
#include "SceneManager.h"
#include "Events/SceneIDs.h"
#include "Events/EventIDs.h"
#include "../ecs/EntityIDs.h"

static struct QuadTree* tree = NULL;

void GameSceneInit(void* pd_ptr, Scene *scene)
{
    if (scene == NULL || pd_ptr == NULL) return;
    tree = CreateQuadTreeNode(NULL, scene->world->w, scene->world->h, 0);

    Entity* earth_blood = NULL;
    { // widgets
        earth_blood = CreateEntity(ENTITY_EARTH_BLOOD, scene->world, (Vec2){ 0.1f, 0.05f }, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.0f, 1.0f });
        if (earth_blood != NULL)
        {
            SceneAddGameObject(scene, earth_blood);
            AddAnimatedSpriteComponent(pd_ptr, earth_blood, "images/widgets/blood", 72, 12, 5, 0.0f, false, 1);
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
            }
        }
        { // enemy
            Entity* enemy = CreateEntity(ENTITY_ENEMY, scene->world, (Vec2) { 0.0f, 0.0f }, (Vec2) { 0.0f, 0.0f }, (Vec2) { 1.0f, 1.0f });
            if (enemy != NULL)
            {
                SceneAddGameObject(scene, enemy);
                // Add components
                if (earth != NULL)
                {
                    enemy->components.motion->acceleration = scene->world->gravity;
                    enemy->components.motion->direction = Vec2Normalize(Vec2Subtract(earth->components.transform->position, enemy->components.transform->position));
                }
                AddAnimatedSpriteComponent(pd_ptr, enemy, "images/enemy", 12, 12, 8, 0.5f, true, 1);
                AddCircleColliderComponent(pd_ptr, tree, enemy, (Vec2) { 0.0f, 0.0f }, (float)(4.0f / 80.0f), EVT_GAME_ENEMY_COLLIDED);
                AddHealthComponent(pd_ptr, enemy, 10, EVT_GAME_ENEMY_DEAD);
                AddStrengthComponent(pd_ptr, enemy, 20.0f);
            }
        }
    }
}

void GameSceneUpdate(void* pd_ptr, Scene *scene, float dt)
{
    if (scene == NULL || pd_ptr == NULL) return;
    QuadTreeClear(tree);
    unsigned int tick = ((PlaydateAPI*)pd_ptr)->system->getCurrentTimeMilliseconds();
    UpdateSpawnEntity(pd_ptr,  scene, NULL, tree, 10);
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
        UpdateBinding(scene, entity);
    }
    for (size_t i = 0; i < scene->entites->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entites, i);
        UpdateCollisionDetection(entity, tree);
    }
    for (size_t i = 0; i < scene->entites->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entites, i);
        UpdateHealth(pd_ptr, scene, entity);
    }
}

void GameSceneRender(void* pd_ptr, Scene *scene)
{
    if (scene == NULL || pd_ptr == NULL) return;
    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entites->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entites, i);
        if (entity != NULL)
        {
            UpdateRenderer(pd_ptr, entity);
        }
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
                if (entity->components.input != NULL)
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
                SceneManagerTransition(manager, MENU_SCENE);
            }
            break;
        default:
            break;
        }
    }
}
