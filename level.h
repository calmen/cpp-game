#pragma once

#include "gameobject.h"
#include <vector>
#include <list>
#include <string>
#include <graphics.h>
#include "player.h"


class GameState;
class Level : public GameObject
{
private:
	std::string m_name;



public:

	graphics::Brush m_brush_background;
	graphics::Brush m_block_brush;
	graphics::Brush m_block_brush_debug;
	graphics::Brush m_brush_blackexit;
	graphics::Brush m_brush_apple;

	Timer timer;

	// add some collidable blocks
	Box blackHole;
	Box apple;
	std::vector<Box> m_blocks;
	std::vector<std::string> m_block_names;
	const float m_block_size = 1.0f;


	float m_center_x = 5.0f;
	float m_center_y = 5.0f;

	// dedicated method to draw a block
	void drawBlock(int i);

	void drawBlackExit();

	void drawApple();



	float initialBlackHoleX;
	float initialBlackHoleY;

	float initialAppleX;
	float initialAppleY;

	// detect collisions



	void checkCollisions();
	void update(float dt) override;
	void draw() override;
	void init() override;

	bool isTimerRunning();







	Level(const std::string& name, GameState* gameState);
	~Level();

	const std::vector<Box>& getBlocks() const {
		return m_blocks;
	}



	void clearBlocks();



	void initHorizontalBlocks(int startX, int endX, int yPos, const std::string& blockName);
	void initVerticalBlocks(int startY, int endY, int xPos, const std::string& blockName);

	void initVerticalBlocksWithGapLeft(int startY, int endY, int xPos, const std::string& blockName);

	void initVerticalBlocksWithGapRight(int startY, int endY, int xPos, const std::string& blockName);

	void initCollidableBlocks(int minX, int maxX, int minY, int maxY, float coveragePercentage);

	std::string getName() const
	{
		return m_name;
	}

	std::string getBlockName(size_t i) const {
		if (i < m_block_names.size()) {
			return m_block_names[i];
		}
		else {
			// Επιστροφή κενού string αν ο δείκτης είναι άκυρος
			return "";
		}
	}





protected:
	GameState* m_state;

};