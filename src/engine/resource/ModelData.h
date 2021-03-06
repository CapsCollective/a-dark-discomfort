#ifndef A_DARK_DISCOMFORT_MODELDATA_H
#define A_DARK_DISCOMFORT_MODELDATA_H

#include <raylib-cpp.hpp>
#include "ResourceManager.h"

class ModelData
{
public:

    // 'Structors

    ModelData(std::string  modelPath, std::string  texturePath) :
    modelPath(std::move(modelPath)),
    texturePath(std::move(texturePath))
    {
        ResourceManager::Register<Model>(this->modelPath);
        ResourceManager::Register<Texture>(this->texturePath);
    };

    ModelData(const ModelData& other)
    {
        this->modelPath = other.modelPath;
        this->texturePath = other.texturePath;

        ResourceManager::Register<Model>(modelPath);
        ResourceManager::Register<Texture>(texturePath);
    }

    // Public methods

    const std::string& GetModelPath() const;

    const std::string& GetTexturePath() const;

    static void SetTexture(const Model& model, const Texture2D& texture);

private:

    // Private fields

    std::string modelPath;

    std::string texturePath;
};

#endif //A_DARK_DISCOMFORT_MODELDATA_H
