//
// Created by Jonathan Moallem on 11/12/20.
//

#ifndef A_DARK_DISCOMFORT_EDITORCONTROLLER_H
#define A_DARK_DISCOMFORT_EDITORCONTROLLER_H

#include <Camera3D.hpp>
#include <vector>
#include "Entity.h"

class EditorController : public Entity
{
public:

    // Constructors

    explicit EditorController(raylib::Camera3D* camera) :
    selectedEntity(nullptr),
    camera(camera)
    {};

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw() override;

    void OnUIDraw() override;

private:

    // Private fields

    raylib::Camera3D* camera;

    Entity* selectedEntity;
};


#endif //A_DARK_DISCOMFORT_EDITORCONTROLLER_H