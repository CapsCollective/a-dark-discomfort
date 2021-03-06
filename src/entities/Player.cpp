#include "Player.h"
#include "../engine/collision/CollisionSystem.h"
#include "../engine/scene/SceneSerialiser.h"

// Static member initialisation
const std::string Player::ENTITY_NAME("Player");

void Player::OnUpdate()
{
    // Get move axes as vector
    raylib::Vector3 move = {
            (float)(-IsKeyDown(KEY_LEFT) + IsKeyDown(KEY_RIGHT)),
            0.f,
            (float)(-IsKeyDown(KEY_UP) + IsKeyDown(KEY_DOWN)),
    };

    // Normalise and apply move to velocity
    velocity += move.Normalize() * speed * GetFrameTime();

    // Apply force of gravity
    velocity += raylib::Vector3(0.f, -0.01f, 0.f);

    // Set the resulting attempted move's velocity to the object's position
    velocity = CollisionSystem::MoveAndSlide(GetBoundingBox(), velocity);
    position += velocity;

    // Dampen velocity
    velocity = velocity * 0.9f;
}

void Player::OnDraw()
{
    // Set the model's texture to this entity's texture
    ModelData::SetTexture(ResourceManager::Get<Model>(modelData.GetModelPath()),
                          ResourceManager::Get<Texture2D>(modelData.GetTexturePath()));

    // Draw the model
    DrawModelEx(
            ResourceManager::Get<Model>(modelData.GetModelPath()),
            position,
            raylib::Vector3(0, 1, 0),
            rotation,
            raylib::Vector3::One(),
            WHITE
    );

    // Draw the model wireframe
    DrawModelWiresEx(
            ResourceManager::Get<Model>(modelData.GetModelPath()),
            position,
            raylib::Vector3(0, 1, 0),
            rotation,
            raylib::Vector3::One(),
            PINK
    );
}

BoundingBox Player::GetBoundingBox() const
{
    return BoundingBox {
            raylib::Vector3(position) - raylib::Vector3::One(),
            raylib::Vector3(position) + raylib::Vector3::One(),
    };
}

const ModelData& Player::GetModelData()
{
    return modelData;
}

void Player::SetModelData(const ModelData& data)
{
    modelData = data;
}

Entity* Player::Clone() const
{
    return new Player(position, rotation);
}

static std::string Serialise(Entity* entity)
{
    std::string fileData;
    auto player = dynamic_cast<Player*>(entity);
    fileData += DefineField("MODEL_PATH", player->GetModelData().GetModelPath());
    fileData += DefineField("TEXTURE_PATH", player->GetModelData().GetTexturePath());
    return fileData;
}

static Entity* Deserialise(const EntityData& data, const std::vector<std::string>& args)
{
    return new Player(data.position, data.rotation);
}

REGISTER_SERIALISATION_INTERFACE(Player::ENTITY_NAME, Serialise, Deserialise);
