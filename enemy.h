#pragma once
#include "gameobject.h"
#include "box.h"
#include <graphics.h>
#include "enemy.h"


class Level;

class Enemy : public Box, public GameObject
{
	// animated player
	std::vector<std::string> m_sprites;
	graphics::Brush m_brush_enemy;







	// detect collisions
	void checkCollisions();

public:

	float m_accel_horizontal;
	float m_accel_vertical;
	float m_max_velocity;
	float m_gravity;
	Enemy(const std::string& name, Level* level, GameState* gameState);
	~Enemy();
	float m_vx;
	float m_vy;

	void update(float dt) override;
	void draw() override;
	void init() override;
	float rand0to1();



protected:
	// dynamic motion control
	void moveEnemyTowardsPlayer(float dt);
	GameState* m_state;

private:
	Level* m_level;
	std::vector<Enemy> m_enemies;
	GameState* m_gameState;
	float m_speed;

};