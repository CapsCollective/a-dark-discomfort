#ifndef A_DARK_DISCOMFORT_GEOMETRY_H
#define A_DARK_DISCOMFORT_GEOMETRY_H

#include "../engine/entity/Entity.h"
#include "../engine/resource/ModelData.h"
#include "../engine/resource/Modelable.h"
#include "../engine/collision/Collidable.h"
#include "../engine/collision/CollisionSystem.h"

class Geometry : public Entity, public Modelable
{
public:

    // Public constants

    static const std::string ENTITY_NAME;

    // 'Structors

    Geometry() :
        Geometry(raylib::Vector3::Zero(), 0.f, raylib::Vector3::One())
    {};

    Geometry(raylib::Vector3 position, float rotation, raylib::Vector3 dimensions) :
        Geometry(position, rotation, dimensions, ModelData(
                "assets/models/cube/cube.obj",
                "assets/models/cube/cube.png"))
    {};

    Geometry(raylib::Vector3 position, float rotation, raylib::Vector3 dimensions, const ModelData& data) :
        Entity(ENTITY_NAME, position, rotation),
        dimensions(dimensions),
        modelData(data)
    {
        // Register the entity as collidable
        CollisionSystem::Add(this);
    };

    // Public overrides

    Entity* Clone() const override;

    void QueueFree() override;

    BoundingBox GetBoundingBox() const override;

    const ModelData& GetModelData() override;

    void SetModelData(const ModelData& modelData) override;

    // Public methods

    const raylib::Vector3& GetDimensions();

protected:

    // Protected overrides

    void OnDraw() override;

private:

    // Private fields

    raylib::Vector3 dimensions;

    ModelData modelData;

};

#endif //A_DARK_DISCOMFORT_GEOMETRY_H
