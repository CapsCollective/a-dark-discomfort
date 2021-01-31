#include <BoundingBox.hpp>
#include "../entity/Entity.h"
#include "CollisionRegister.h"

// Define static members
std::vector<Entity*> CollisionRegister::collidableEntities;

bool CollisionRegister::CheckCollision(const BoundingBox& boundingBox)
{
    for (auto& entity : collidableEntities)
    {
        if (CheckCollisionBoxes(entity->GetBoundingBox(), boundingBox)) return true;
    }
    return false;
}

void CollisionRegister::Add(Entity* entity)
{
    // Add the entity to the register
    collidableEntities.push_back(entity);
}