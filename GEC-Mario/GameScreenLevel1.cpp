﻿#include "GameScreenLevel1.h"

#include <iostream>

#include "Collisions.h"
#include "PowBlock.h"
#include "SoundEffect.h"

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer, AudioManager* audio_manager, GameSession* session) :
	GameScreenLevelBase(renderer, audio_manager, session, "Images/BackgroundMB.png", "Audio/Music/Mario.mp3", "Levels/01.txt")
{
}

GameScreenLevel1::~GameScreenLevel1()
{
    delete m_pow_block;
}

bool GameScreenLevel1::SetUpLevel()
{
	if (!GameScreenLevelBase::SetUpLevel())
	{
		return false;
	}

	// Reset score
	m_session->score = 0;

	// Set up spawner variables
	m_enemy_spawn_side = 0;
	m_enemy_spawn_timer = LEVEL_1_SPAWNER_DELAY;
    
	return true;
}

void GameScreenLevel1::SetUpEntities()
{
	CreateMario(Vector2D(64, 300));
	CreateLuigi(Vector2D(448, 300));

	m_pow_block = new PowBlock(m_renderer, m_level_map);

	CreateKoopa(Vector2D(150, 32), Facing::RIGHT, KOOPA_SPEED);
	CreateKoopa(Vector2D(325, 32), Facing::LEFT, KOOPA_SPEED);
}

void GameScreenLevel1::Render()
{
    GameScreenLevelBase::Render();
	
	m_pow_block->Render();
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
    m_character_mario->Update(deltaTime, e);
    m_character_luigi->Update(deltaTime, e);
    CheckPOWBlockCollisions(m_character_mario);
    CheckPOWBlockCollisions(m_character_luigi);
    UpdateEnemies(deltaTime, e);
	UpdateCoins(deltaTime, e);
	UpdateSpawners(deltaTime);
	UpdateScoreText();

    if (m_screen_shaking)
    {
        m_shake_time -= deltaTime;
        m_wobble++;
        m_background_yPos = sin(m_wobble);
        m_background_yPos *= 3.0f;

        if (m_shake_time <= 0.0f)
        {
            m_screen_shaking = false;
            m_shake_time = 0.0f;
            m_background_yPos = 0.0f;
        }
    }

	// TODO: spawn coins from spawners
	if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
	{
		std::cout << "BLAH " << e.button.x << " , " << e.button.y << std::endl; 
		CreateCoin({ (float) e.button.x, (float) e.button.y }, { 200.0f, 240.0f });
	}
}

void GameScreenLevel1::CheckPOWBlockCollisions(Character* character)
{
    const bool isColliding = Collisions::Instance()->Box(character->GetCollisionBox(), m_pow_block->GetCollisionBox());

    if (isColliding && m_pow_block->IsAvailable() && character->IsJumping())
    {
        DoScreenShake();
        m_pow_block->TakeHit();
        character->CancelJump(true);
    }
}

void GameScreenLevel1::UpdateSpawners(float deltaTime)
{
	m_enemy_spawn_timer -= deltaTime;
	if (m_enemy_spawn_timer <= 0.0f)
	{
		Vector2D position;
		Facing direction;
		if (m_enemy_spawn_side == 0)
		{
			position = LEVEL_1_SPAWNER_POSITION_0;
			direction = Facing::RIGHT;
		}
		else
		{
			position = LEVEL_1_SPAWNER_POSITION_1;
			direction = Facing::LEFT;
		}

		CreateKoopa(position, direction, KOOPA_SPEED);

		m_enemy_spawn_side = (m_enemy_spawn_side + 1) % 2;
		m_enemy_spawn_timer = LEVEL_1_SPAWNER_DELAY;
	}
}
