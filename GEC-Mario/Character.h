﻿#pragma once

#include <SDL.h>
#include <string>

#include "Common.h"

#ifndef _CHARACTER_H
#define _CHARACTER_H

class Texture2D;

class Character
{
protected:
    SDL_Renderer* m_renderer;
    Vector2D m_position;
    Texture2D* m_texture;
    
    FACING m_facing_direction;
    bool m_moving_left;
    bool m_moving_right;
    float m_movement_speed;
    bool m_jumping;
    bool m_can_jump;
    float m_jump_force;

    virtual void Jump();
    virtual void MoveLeft(float deltaTime);
    virtual void MoveRight(float deltaTime);
    void AddGravity(float deltaTime);

public:
    Character(SDL_Renderer* renderer, std::string image_path, Vector2D start_position, float movement_speed);
    ~Character();

    virtual void Render();
    virtual void Update(float deltaTime, SDL_Event e);

    void UpdateMovement(float deltaTime);
    virtual void HandleInput(float deltaTime, SDL_Event e) = 0;

    void SetPosition(Vector2D new_position);
    Vector2D GetPosition();
};

#endif
