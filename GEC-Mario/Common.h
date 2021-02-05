#pragma once

struct Vector2D
{
	float x;
	float y;

	Vector2D()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2D(float initial_x, float initial_y)
	{
		x = initial_x;
		y = initial_y;
	}
};

enum SCREENS
{
	SCREEN_INTRO,
	SCREEN_MENU,
	SCREEN_LEVEL_1,
	SCREEN_LEVEL_2,
	SCREEN_GAME_OVER,
	SCREEN_HIGH_SCORES
};

enum class FACING
{
	LEFT,
	RIGHT
};