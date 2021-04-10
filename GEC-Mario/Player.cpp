﻿#include "Player.h"

Player::Player(SDL_Renderer* renderer, const std::string& image_path, const Vector2D& start_position,
    float movement_speed, float jump_force, short max_jumps, float collision_radius, LevelMap* map,
    SoundEffect* jump_sound) :
        Character(renderer, image_path, start_position, movement_speed, jump_force, max_jumps, collision_radius, map),
		m_jump_held(false),
		m_jump_sound(jump_sound)
{
}

void Player::HandleInput(float deltaTime, SDL_Event e)
{
    switch (e.type)
    {
    case SDL_KEYDOWN:
        if (e.key.keysym.sym == GetLeftKey())
        {
            m_moving_left = true;
        }
        else if (e.key.keysym.sym == GetRightKey())
        {
            m_moving_right = true;
        }
        else if (e.key.keysym.sym == GetJumpKey())
        {
            if (CanJump() && !m_jump_held)
            {
                m_jump_held = true;
                Jump();
            }
        }
        break;
    case SDL_KEYUP:
        if (e.key.keysym.sym == GetLeftKey())
        {
            m_moving_left = false;
        }
        else if (e.key.keysym.sym == GetRightKey())
        {
            m_moving_right = false;
        }
        else if (e.key.keysym.sym == GetJumpKey())
        {
            m_jump_held = false;
        }
        break;
    }
}

void Player::Jump()
{
    Character::Jump();
    m_jump_sound->Play(0);
}
