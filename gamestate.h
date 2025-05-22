#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "graphics.h"
class Level;
class Enemy;
class GameState
{

	typedef enum { STATUS_START, STATUS_PLAYING, STATUS_END } status_t;
private:

	const std::string m_asset_path = "assets\\";

	const float m_canvas_width = 6.0f;
	const float m_canvas_height = 6.0f;


	class Player* m_player = 0;
	class Enemy* m_enemy = 0;



public:
	graphics::Brush br_enter;
	graphics::Brush br_close;


	GameState();
	~GameState();
	static GameState* getInstance();
	float m_global_offset_x = 0.0f;
	float m_global_offset_y = 0.0f;

	bool m_debugging = false;
	int levelCounter = 1;
	int score;


	bool init();
	void draw();
	void update(float dt);

	std::string getFullAssetPath(const std::string& asset);
	std::string getAssetDir();

	float getCanvasWidth() { return m_canvas_width; }
	float getCanvasHeight() { return m_canvas_height; }

	class Player* getPlayer() { return m_player; }
	class Enemy* getEnemy() { return m_enemy; }

	void reset();
	void createEnemiesForLevel();
	void checkcollision();

	status_t  status = STATUS_START;
	void updateStartScreen();
	void updateLevelScreen(float dt);
	void updateEndScreen();
	void drawStartScreen();
	void drawLevelScreen();
	void drawEndScreen();


private:
	Level* m_current_level;
	static GameState* m_unique_instance;
	std::vector<Enemy*> m_enemies;

};