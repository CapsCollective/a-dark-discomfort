#ifndef A_DARK_DISCOMFORT_RESOURCEMANAGER_H
#define A_DARK_DISCOMFORT_RESOURCEMANAGER_H

#include <raylib-cpp.hpp>
#include <map>
#include <variant>

#define RESOURCE_MAP std::map<std::string, std::variant<raylib::Model*, raylib::Texture2D*>>

class ResourceManager
{
public:

    // Public functions

    template<typename T>
    static void Register(const std::string& path)
    {
        if (resources.find(path) != resources.end()) return;

        resources.insert({path, new T(path)});
    };

    template<typename T>
    static T& Get(const std::string& path)
    {
        return *std::get<T*>(resources[path]);
    };

    template<typename T>
    static T* GetRef(const std::string& path)
    {
        return std::get<T*>(resources[path]);
    };

    // TODO add model removal function

    // TODO add model and texture data to stack

private:

    // Private fields

    static RESOURCE_MAP resources;
};


#endif //A_DARK_DISCOMFORT_RESOURCEMANAGER_H
