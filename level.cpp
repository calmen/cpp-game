#include "level.h"
#include <graphics.h>
#include "player.h"
#include "util.h"
#include <ctime>
#include "enemy.h"
#include "gamestate.h"
#include "timer.h"




void Level::drawBlock(int i)
{
	Box& box = m_blocks[i];
	std::string& name = m_block_names[i];

	float x = box.m_pos_x + m_state->m_global_offset_x;
	float y = box.m_pos_y + m_state->m_global_offset_y;

	m_block_brush.texture = m_state->getFullAssetPath(name);

	graphics::drawRect(x, y, 1.5f * m_block_size, 1.5f * m_block_size, m_block_brush);

	if (m_state->m_debugging)
		graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush_debug);

}

void Level::drawBlackExit()
{
	float x = blackHole.m_pos_x + m_state->m_global_offset_x;
	float y = blackHole.m_pos_y + m_state->m_global_offset_y;

	//std::cout << "Drawing blackHole at (" << x << ", " << y << ")\n";

	m_brush_blackexit.texture = m_state->getFullAssetPath("blackhole.png");
	m_brush_blackexit.outline_opacity = 0.0f;

	graphics::drawRect(x, y, 1.6 * m_block_size, 1.6 * m_block_size, m_brush_blackexit);
}

void Level::drawApple()
{
	float x = apple.m_pos_x + m_state->m_global_offset_x;
	float y = apple.m_pos_y + m_state->m_global_offset_y;



	m_brush_apple.texture = m_state->getFullAssetPath("apple.png");
	m_brush_apple.outline_opacity = 0.0f;

	graphics::drawRect(x, y, 0.8 * m_block_size, 0.8 * m_block_size, m_brush_apple);
}







void Level::checkCollisions()
{
	for (size_t i = 0; i < m_blocks.size(); ++i)
	{
		Box& currentBlock = m_blocks[i];
		const std::string& blockName = m_block_names[i];

		if (blockName == "blackHole")
		{
			// Ελέγχουμε μόνο για το blackHole
			if (m_state->getPlayer()->intersect(currentBlock) or m_state->getPlayer()->intersectUp(currentBlock) || m_state->getPlayer()->intersectSideways(currentBlock) || m_state->getPlayer()->intersectDown(currentBlock))
			{
				// Εφαρμογή επιπτώσεων για τη σύγκρουση με το blackhole
				printf("Collision with the Black Hole!\n");
				printf("Player Position: (%f, %f)\n", m_state->getPlayer()->m_pos_x, m_state->getPlayer()->m_pos_y);
				printf("Black Hole Position: (%f, %f)\n", currentBlock.m_pos_x, currentBlock.m_pos_y);
				graphics::playSound(m_state->getFullAssetPath("letsgo.wav"), 1.0f);
				m_state->reset();
			}
		}

		else if (blockName == "apple")
		{
			// check for the apple
			if (m_state->getPlayer()->intersect(currentBlock) or m_state->getPlayer()->intersectUp(currentBlock) || m_state->getPlayer()->intersectSideways(currentBlock) || m_state->getPlayer()->intersectDown(currentBlock))
			{

				printf("Collision with the Apple!\n");
				//  Delete the apple
				m_blocks.erase(m_blocks.begin() + i);
				m_block_names.erase(m_block_names.begin() + i);

				// Decrease i to account for the erased element
				--i;
				graphics::playSound(m_state->getFullAssetPath("great.wav"), 1.0f);

				//set timer
				timer = Timer(10, Timer::TIMER_ONCE);
				timer.start();

				printf("time is running!\n");


			}
		}
		else
		{
			for (auto& playerBlock : m_blocks)
			{
				float offset = 0.0f;
				if (offset = m_state->getPlayer()->intersectDown(playerBlock))
				{
					m_state->getPlayer()->m_pos_y += offset;

					if (m_state->getPlayer()->m_vy > 1.0f)


						m_state->getPlayer()->m_vy = 0.0f;
					break;
				}
			}

			for (auto& playerBlock : m_blocks)
			{
				float offset = 0.0f;
				if (offset = m_state->getPlayer()->intersectSideways(playerBlock))
				{
					m_state->getPlayer()->m_pos_x += offset;

					m_state->getPlayer()->m_vx = 0.0f;
					break;
				}
			}

			for (auto& playerBlock : m_blocks)
			{
				float offset = 0.0f;
				if (offset = m_state->getPlayer()->intersectUp(playerBlock))
				{
					m_state->getPlayer()->m_pos_y -= -offset;


					if (m_state->getPlayer()->m_vy > 1.0f)


						m_state->getPlayer()->m_vy = 0.0f;
					break;
				}
			}
		}
	}
}



void Level::update(float dt)
{
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt);

	checkCollisions();
	if (timer.isRunning())
	{



		// time stop
		if (static_cast<float>(timer) >= 10.0f)
		{
			printf("Time is up!\n");
			timer.stop();
			printf("Time is stop!!!!!!!!! in update!\n");
		}
	}
	GameObject::update(dt);
}

void Level::draw()
{
	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();

	float offset_x = m_state->m_global_offset_x / 2.0f + w / 2.0f;
	float offset_y = m_state->m_global_offset_y / 2.0f + h / 2.0f;

	//draw background
	graphics::drawRect(offset_x, offset_y, 2.0f * w, 4.0f * w, m_brush_background);

	// draw player
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->draw();

	if (isTimerRunning()) {
		m_state->getPlayer()->drawImmortalDisk();
	}


	// draw blocks,blackhole,apple
	// draw blocks
	for (int i = 0; i < m_blocks.size(); i++)
	{
		if (m_block_names[i] != "blackHole" && m_block_names[i] != "apple")
		{
			drawBlock(i);
		}
	}

	// draw black hole
	for (int i = 0; i < m_blocks.size(); i++)
	{
		if (m_block_names[i] == "blackHole")
		{
			drawBlackExit();
		}
	}

	// draw apples
	for (int i = 0; i < m_blocks.size(); i++)
	{
		if (m_block_names[i] == "apple")
		{
			drawApple();
		}
	}






}

void Level::init()
{
	// Initialize horizontal blocks
	initHorizontalBlocks(-8, 14, 0, "rock.png");
	initHorizontalBlocks(-8, 14, 25, "rock.png");

	// Initialize vertical blocks
	initVerticalBlocks(0, 25, -7, "rock.png");
	initVerticalBlocks(0, 25, 10, "rock.png");

	// Initialize vertical blocks with a gap
	initVerticalBlocksWithGapLeft(0, 25, -7, "rock.png");
	initVerticalBlocksWithGapRight(0, 25, 10, "rock.png");

	// Level exit
	const float minX = 2.0f;
	const float maxX = 6.0f;
	const float minY = 2.0f;
	const float maxY = 9.0f;

	// Set initial positions for the black hole
	initialBlackHoleX = minX + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (maxX - minX)));
	initialBlackHoleY = minY + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (maxY - minY)));

	// Black Hole
	blackHole = Box(initialBlackHoleX, initialBlackHoleY, 1.5 * m_block_size, 1.5 * m_block_size);
	m_blocks.push_back(blackHole);
	m_block_names.push_back("blackHole");

	// Set initial positions for the apple
	const float appleMinX = -5.0f;
	const float appleMaxX = 10.0f;
	const float appleMinY = 2.0f;
	const float appleMaxY = 22.0f;

	initialAppleX = appleMinX + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (appleMaxX - appleMinX)));
	initialAppleY = appleMinY + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (appleMaxY - appleMinY)));

	// Apple
	apple = Box(initialAppleX, initialAppleY, 1.5 * m_block_size, 1.5 * m_block_size);

	// Add the black hole and apple to the list of blocks
	m_blocks.push_back(blackHole);
	m_blocks.push_back(apple);

	m_block_names.push_back("blackHole");
	m_block_names.push_back("apple");

	//set timer
	//Timer timer(10, Timer::TIMER_ONCE);

	// Random blocks with limits and "coverage percentage"
	initCollidableBlocks(-8, 14, 0, 25, 0.43);
	printf("Initial Black Hole Position: (%f, %f)\n", initialBlackHoleX, initialBlackHoleY);
	printf("Initial Apple Position: (%f, %f)\n", initialAppleX, initialAppleY);

	// Set brush properties
	m_block_brush.outline_opacity = 0.0f;
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.1f, 1.0f, 0.1f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);
}


Level::Level(const std::string& name, GameState* gameState)
	: GameObject("Level"), m_name(name), m_state(gameState)
{

	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("background.png");





}



Level::~Level()
{
	clearBlocks();
}

bool Level::isTimerRunning() {
	return timer.isRunning();
}

void Level::clearBlocks()
{
	m_blocks.clear();
	m_block_names.clear();
}

void Level::initHorizontalBlocks(int startX, int endX, int yPos, const std::string& blockName)
{
	int countx = 0;

	for (int i = startX; i < endX; i++) {
		m_blocks.push_back(Box(i, yPos, 1, 1));
		countx++;
	}

	for (int i = 0; i < countx; i++) {
		m_block_names.push_back(blockName);
	}
}

void Level::initVerticalBlocks(int startY, int endY, int xPos, const std::string& blockName)
{
	int county = 0;

	for (int i = startY; i < endY; i++) {
		m_blocks.push_back(Box(xPos, i, 1, 1));
		county++;

	}


	for (int i = 0; i < county; i++) {
		m_block_names.push_back(blockName);

	}
}

void Level::initVerticalBlocksWithGapLeft(int startY, int endY, int xPos, const std::string& blockName)
{
	int county = 0;

	for (int i = startY; i < endY; i) {
		m_blocks.push_back(Box(xPos + 1, i, 1, 1));
		i = i + 3;
		county++;

	}


	for (int i = 0; i < county; i++) {
		m_block_names.push_back(blockName);

	}
}
void Level::initVerticalBlocksWithGapRight(int startY, int endY, int xPos, const std::string& blockName)
{
	int county = 0;

	for (int i = startY; i < endY; i) {
		m_blocks.push_back(Box(xPos - 1, i, 1, 1));
		i = i + 3;
		county++;

	}


	for (int i = 0; i < county; i++) {
		m_block_names.push_back(blockName);

	}
}


void Level::initCollidableBlocks(int minX, int maxX, int minY, int maxY, float coveragePercentage)
{
	// start block here
	m_blocks.push_back(Box(5, 6, 1, 1));
	m_block_names.push_back("rock.png");

	std::srand(static_cast<unsigned int>(std::time(0)));

	int totalBlocks = static_cast<int>(m_blocks.size() * coveragePercentage);

	for (int i = 0; i < totalBlocks; ++i)
	{
		int x, y;

		// Find an empty position that is not too close to the blackHole
		do
		{
			x = rand() % (maxX - minX) + minX;
			y = rand() % (maxY - minY) + minY;

			// Check if the position is empty and not too close to blackHole
			bool positionIsEmpty = true;

			for (const auto& block : m_blocks)
			{
				if (x >= block.m_pos_x && x < (block.m_pos_x + block.m_width) &&
					y >= block.m_pos_y && y < (block.m_pos_y + block.m_height))
				{
					positionIsEmpty = false;
					break;
				}

				// Check distance from blackHole
				float distance = std::sqrt((x - blackHole.m_pos_x) * (x - blackHole.m_pos_x) +
					(y - blackHole.m_pos_y) * (y - blackHole.m_pos_y));

				if (distance < 2.0f) // Adjust the distance threshold as needed
				{
					positionIsEmpty = false;
					break;
				}
			}

			if (positionIsEmpty)
				break;

		} while (true);

		// Add the new block and block name
		m_blocks.push_back(Box(x, y, 1, 1));
		m_block_names.push_back("rock.png");
	}
}


