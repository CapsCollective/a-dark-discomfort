//
// Created by Jonathan Moallem on 30/11/20.
//

#include "Cube.h"

void Player::onUpdate()
{
    // Get move axes as vector
    raylib::Vector3 move = {
            -(float)IsKeyDown(KEY_LEFT) + (float)IsKeyDown(KEY_RIGHT),
            0.f,
            -(float)IsKeyDown(KEY_UP) + (float)IsKeyDown(KEY_DOWN),
    };

    // Normalise and apply move to velocity
    position += velocity += move.Normalize() * speed * GetFrameTime();

    // Dampen velocity
    velocity = velocity * 0.9f;
}

void Player::onDraw()
{
    // Draw player to current position
    DrawCube(position, 2.f, 2.f, 2.f, RED);
    DrawCubeWires(position, 2.f, 2.f, 2.f, MAROON);
}