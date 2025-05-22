#pragma once
#include "gameobject.h"
#include "box.h"
#include <graphics.h>
#include "timer.h"

class Player : public Box, public GameObject
{
	// animated player
	std::vector<std::string> m_sprites;

	graphics::Brush m_brush_player;
	graphics::Brush m_brush_life;
	graphics::Brush m_brush_disk_immortal;

public:
	const float m_accel_horizontal = 30.0f;
	const float m_accel_vertical = 300.1f;
	const float m_max_velocity = 5.0f;
	const float m_gravity = 10.0f;
	float m_vx = 0.0f;
	float m_vy = 0.0f;

	float getX() const { return m_pos_x; }
	float getY() const { return m_pos_y; }
	float life = 1.0f;




	void update(float dt) override;
	void draw() override;
	void init() override;
	Player(std::string name) : GameObject(name) {}
	float getRemainingLife() const { return life; }
	void drainLife(float amount);
	void drawImmortalDisk();




protected:
	void debugDraw();

	// dynamic motion control
	void movePlayer(float dt);
private:



};
