#include "enemy.h"
#include "util.h"
#include <cmath>
#include"player.h"
#include <algorithm>
#include "level.h"
#include "gamestate.h"
#include <iostream>

Enemy::Enemy(const std::string& name, Level* level, GameState* gameState)
    : GameObject(name), m_level(level), m_state(gameState)
{
    // Καθορίζετε τυχαία την ταχύτητα κατά την δημιουργία
    m_speed = (rand() % 85 + 1) / 300.0f; // Παράδειγμα: ταχύτητα από 0.01 έως 1.00
}


Enemy::~Enemy()
{
}

void Enemy::update(float dt)
{
    float delta_time = dt / 1000.0f;

    moveEnemyTowardsPlayer(dt);

    // Check for collisions and adjust position

    checkCollisions();
    GameObject::update(dt);
}

void Enemy::draw()
{
    int sprite = (int)fmod(100.0f - m_pos_x * 9.0f, m_sprites.size());

    m_brush_enemy.texture = m_sprites[sprite];
    graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 1.0f, 1.0f, m_brush_enemy);

}

void Enemy::init()
{

    m_vx = 0;
    m_vy = 0;
    m_pos_x = 4 + 4 * rand0to1();
    m_pos_y = 4 + 4 * rand0to1();


    m_accel_horizontal = 20.0f;
    m_accel_vertical = 300.1f;
    m_max_velocity = 10.0f;
    m_gravity = 2.0f;


    m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
    m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

    m_brush_enemy.fill_opacity = 1.0f;
    m_brush_enemy.outline_opacity = 0.0f;
    m_brush_enemy.texture = m_state->getFullAssetPath("bird.png");

    m_sprites.push_back(m_state->getFullAssetPath("bird.png"));
    m_sprites.push_back(m_state->getFullAssetPath("bird.png"));
    m_sprites.push_back(m_state->getFullAssetPath("bird.png"));
    m_sprites.push_back(m_state->getFullAssetPath("bird.png"));
    m_sprites.push_back(m_state->getFullAssetPath("bird.png"));
    m_sprites.push_back(m_state->getFullAssetPath("bird.png"));
    m_sprites.push_back(m_state->getFullAssetPath("bird.png"));
    m_sprites.push_back(m_state->getFullAssetPath("bird.png"));
    m_sprites.push_back(m_state->getFullAssetPath("bird.png"));
    // Adjust width for finer collision detection
    m_width = 0.5f;
}

float Enemy::rand0to1()
{
    return rand() / (float)RAND_MAX;
}



// ...

void Enemy::moveEnemyTowardsPlayer(float dt)
{
    float delta_time = dt / 1000.0f;  // Convert time to seconds

    // Assume there's a player object with position (m_player_pos_x, m_player_pos_y)
    float player_pos_x = m_state->getPlayer()->getX();
    float player_pos_y = m_state->getPlayer()->getY();

    // Calculate direction towards the player
    float dir_x = player_pos_x - m_pos_x;
    float dir_y = player_pos_y - m_pos_y;

    // Calculate the distance to the player
    float distance = std::sqrt(dir_x * dir_x + dir_y * dir_y);

    // Normalize the direction vector
    if (distance > 0.0f)
    {
        dir_x /= distance;
        dir_y /= distance;
    }

    // Set the enemy's velocity towards the player

    m_vx = dir_x * m_max_velocity * m_speed;
    m_vy = dir_y * m_max_velocity * m_speed;

    // Update position based on velocity
    m_pos_x += m_vx * delta_time;
    m_pos_y += m_vy * delta_time;





    // Check and handle collisions (similar to the player's logic)
    checkCollisions();
}








void Enemy::checkCollisions()
{
    const std::vector<Box>& blocks = m_level->getBlocks();

    for (const auto& box : blocks)
    {
        float offset = 0.0f;

        if (offset = intersectDown(const_cast<Box&>(box)))
        {
            m_pos_y += offset;
            m_vy = 0.0f;
            // printf("Offset from intersectDown: %f\n", offset);
            break;
        }
    }

    for (const auto& box : blocks)
    {
        float offset = 0.0f;

        if (offset = intersectSideways(const_cast<Box&>(box)))
        {
            m_pos_x += offset;
            m_vx = 0;
            //printf("Offset from intersectSides: %f\n", offset);
            break;
        }
    }

    for (const auto& box : blocks)
    {
        float offset = 0.0f;

        if (offset = intersectUp(const_cast<Box&>(box)))
        {
            m_pos_y -= -offset; // Note the subtraction here
            m_vy = 0.0f;
            //printf("Offset from intersectUp: %f\n", offset);
            break;
        }
    }
}



