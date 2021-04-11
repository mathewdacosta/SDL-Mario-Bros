﻿#include "Character.h"
#include <iostream>

#include "constants.h"
#include "Texture2D.h"

Character::Character(SDL_Renderer* renderer, std::string image_path, Vector2D start_position, float movement_speed,
                     float jump_force, short max_jumps, float collision_radius, LevelMap* map) :
    m_renderer(renderer),
    m_position(start_position),
    m_current_level_map(map),
    m_movement_speed(movement_speed),
    m_jump_force(jump_force),
    m_max_jumps(max_jumps),
    m_collision_radius(collision_radius),
    m_facing_direction(Facing::RIGHT),
    m_moving_left(false),
    m_moving_right(false),
    m_jump_ascending(false),
    m_remaining_jumps(max_jumps),
    m_velocity_y(0.0f),
    m_alive(true)
{
    m_texture = new Texture2D(m_renderer);
    if (!m_texture->LoadFromFile(image_path))
    {
        std::cout << "Failed to load texture " << image_path << std::endl;
    }
}

Character::~Character()
{
    m_renderer = nullptr;
    delete m_texture;
}

void Character::Jump()
{
    m_velocity_y = m_jump_force;
    m_jump_ascending = true;
    m_remaining_jumps -= 1;
}

void Character::MoveLeft(float deltaTime)
{
    m_facing_direction = Facing::LEFT;
    m_position.x -= deltaTime * m_movement_speed;
}

void Character::MoveRight(float deltaTime)
{
    m_facing_direction = Facing::RIGHT;
    m_position.x += deltaTime * m_movement_speed;
}


void Character::AddGravity(float deltaTime)
{
    m_velocity_y -= deltaTime * GRAVITY;
}

bool Character::CanJump()
{
    return m_remaining_jumps > 0;
}

bool Character::IsJumping()
{
    return m_remaining_jumps < m_max_jumps;
}

void Character::CancelJump(bool force)
{
    // If we're still ascending when we collide with something, don't cancel
    if (force || !m_jump_ascending)
    {
        m_remaining_jumps = m_max_jumps;
        m_velocity_y = 0.0f;
    }
}

void Character::Render()
{
    SDL_RendererFlip flip;
    if (m_facing_direction == Facing::LEFT)
        flip = SDL_FLIP_HORIZONTAL;
    else
        flip = SDL_FLIP_NONE;

    m_texture->Render(m_position, flip);

#ifdef DEBUG_DRAW_CHARACTER_BOXES
    SDL_SetRenderDrawColor(m_renderer, 127, 0, 127, 255);
    Rect2D collision = GetCollisionBox();
    SDL_Rect sdlRect = {
        collision.x,
        collision.y,
        collision.width,
        collision.height
    };

    SDL_RenderDrawRect(m_renderer, &sdlRect);
#endif
}

void Character::Update(float deltaTime, SDL_Event e)
{
    HandleInput(deltaTime, e);
    UpdateMovement(deltaTime);
}

void Character::UpdateMovement(float deltaTime)
{
    // Adjust position for jump/fall
    m_position.y -= m_velocity_y * deltaTime;

    // Apply gravity
    AddGravity(deltaTime);
    if (m_velocity_y <= 0)
    {
        m_jump_ascending = false;
    }

    Rect2D collisionBox = GetCollisionBox();
    int tileXCenter = (int)(collisionBox.x + (collisionBox.width * 0.5)) / TILE_WIDTH;
    int tileYFoot = (int)(collisionBox.y + collisionBox.height) / TILE_HEIGHT;

    if (!m_current_level_map->GetTileAt(tileYFoot, tileXCenter) == 0)
    {
        // Cancel jump
        CancelJump();

        // If we're still ascending during a jump, don't reset velocity
        if (!m_jump_ascending)
        {
            m_velocity_y = 0.0f;
        }
    }

    if (m_moving_left)
    {
        MoveLeft(deltaTime);
    }
    else if (m_moving_right)
    {
        MoveRight(deltaTime);
    }

    // Check if off left/right of screen and wrap around
    if (collisionBox.x < (float)-(collisionBox.width * 0.5f))
    {
        SetPosition(Vector2D(SCREEN_WIDTH - (0.5 * collisionBox.width), collisionBox.y));
    }
    else if (collisionBox.x > SCREEN_WIDTH - (float)(collisionBox.width * 0.5f))
    {
        SetPosition(Vector2D(-0.5f * collisionBox.width, collisionBox.y));
    }
}

void Character::HandleInput(float deltaTime, SDL_Event e)
{
}

void Character::SetPosition(Vector2D new_position)
{
    m_position = new_position;
}

Vector2D Character::GetPosition()
{
    return m_position;
}

float Character::GetCollisionRadius()
{
    return m_collision_radius;
}

Rect2D Character::GetCollisionBox()
{
    return Rect2D(m_position.x, m_position.y, m_texture->GetWidth(), m_texture->GetHeight());
}
