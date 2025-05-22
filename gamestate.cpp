#include "gamestate.h"
#include "level.h"
#include "player.h"
#include <thread>
#include "timer.h"
#include "enemy.h"
#include <iostream>
#include "box.h"
#include "util.h"

GameState::GameState()
    : m_current_level(nullptr), m_enemy(nullptr), m_debugging(false), m_asset_path("assets/")
{
}

GameState::~GameState()
{
    if (m_current_level)
        delete m_current_level;

    if (m_enemy)
        delete m_enemy;
}

GameState* GameState::getInstance()
{
    if (!m_unique_instance)
    {
        m_unique_instance = new GameState();
    }
    return m_unique_instance;
}

bool GameState::init()
{
    m_current_level = new Level("1.lvl", this);
    m_current_level->init();

    m_player = new Player("Player");
    m_player->init();



    createEnemiesForLevel();

    graphics::preloadBitmaps(getAssetDir());
    graphics::setFont(m_asset_path + "OpenSans-Regular.ttf");

    graphics::playMusic(getFullAssetPath("intro.wav"), 0.3f);



    return true;
}

void GameState::draw()
{

    if (status == STATUS_START) {
        drawStartScreen();

    }

    if (status == STATUS_PLAYING) {
        drawLevelScreen();
    }

    if (status == STATUS_END) {
        drawEndScreen();
    }
}

void GameState::update(float dt)
{


    if (status == STATUS_START) {

        updateStartScreen();
    }

    if (status == STATUS_PLAYING) {
        updateLevelScreen(dt);
    }

    if (status == STATUS_END) {
        updateEndScreen();
    }






}

void GameState::createEnemiesForLevel()
{
    int numEnemies = levelCounter;

    const int maxEnemies = 5;

    std::cout << "Creating " << numEnemies << " enemies for level " << levelCounter << std::endl;

    // Δημιουργία των εχθρών και προσθήκη τους στο vector
    for (int i = 0; i < numEnemies; ++i)
    {
        std::string enemyName = "Enemy_Type_" + std::to_string(i + 1);
        Enemy* newEnemy = new Enemy(enemyName, m_current_level, this);
        newEnemy->init();
        m_enemies.push_back(newEnemy);
    }
}

std::string GameState::getFullAssetPath(const std::string& asset)
{
    return m_asset_path + asset;
}

std::string GameState::getAssetDir()
{
    return m_asset_path;
}


void GameState::checkcollision() {
    auto it = m_enemies.begin();
    while (it != m_enemies.end()) {
        bool collided = false;

        if (!collided && getPlayer()->intersectSideways(**it)) {
            printf("Collision sideways with an ENEMY at address %p!!!!!\n", static_cast<void*>(*it));

            if (!m_current_level->isTimerRunning()) {
                getPlayer()->drainLife(0.2f);
                graphics::playSound(getFullAssetPath("damage.wav"), 1.0f);
            }
            if (m_current_level->isTimerRunning()) {
                score = score + 10;
                printf("Score: %d\n", score);
            }

            printf("Life: %f\n", getPlayer()->getRemainingLife());
            delete* it;
            it = m_enemies.erase(it);
            collided = true;
        }

        if (!collided && getPlayer()->intersectDown(**it)) {
            printf("Collision down and delete with an ENEMY at address %p!!!!!\n", static_cast<void*>(*it));
            printf("Life: %f\n", getPlayer()->getRemainingLife());
            graphics::playSound(getFullAssetPath("confirmation.wav"), 1.0f);
            printf("Score: %d\n", score);
            score = score + 20;
            delete* it;
            it = m_enemies.erase(it);
            collided = true;
        }

        if (!collided && getPlayer()->intersectUp(**it)) {
            printf("Collision up with an ENEMY at address %p!!!!!\n", static_cast<void*>(*it));

            if (!m_current_level->isTimerRunning()) {
                getPlayer()->drainLife(0.2f);
                graphics::playSound(getFullAssetPath("damage.wav"), 1.0f);
            }
            if (m_current_level->isTimerRunning()) {
                score = score + 10;
                printf("Score: %d\n", score);
            }
            printf("Life: %f\n", getPlayer()->getRemainingLife());
            delete* it;
            it = m_enemies.erase(it);
            collided = true;
        }

        if (!collided) {
            ++it;
        }
    }
}

void GameState::updateStartScreen()
{




    if (graphics::getKeyState(graphics::SCANCODE_RETURN)) {

        graphics::stopMusic();

        status = STATUS_PLAYING;
    }


}




void GameState::updateLevelScreen(float dt)
{

    // Skip an update if a long delay is detected 
    // to avoid messing up the collision simulation
    if (dt > 500) // ms
        return;

    // Avoid too quick updates
    float sleep_time = std::max(17.0f - dt, 0.0f);
    if (sleep_time > 0.0f)
    {
        std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_time));
    }

    if (!m_current_level)
        return;



    m_current_level->update(dt);
    for (auto& enemy : m_enemies)
    {
        enemy->update(dt);
    }

    if (m_player->life <= 0) {
        graphics::playSound(getFullAssetPath("death.wav"), 1.0f);
        status = STATUS_END;
    }

    m_debugging = graphics::getKeyState(graphics::SCANCODE_0);



    checkcollision();

}

void GameState::reset()

{
    score = score + 100;
    printf("Score: %d\n", score);
    if (!m_player)
    {
        m_player = new Player("Player");
        m_player->init();
    }

    getPlayer()->m_pos_x = -5;
    getPlayer()->m_pos_y = 20;

    std::string levelFileName = std::to_string(levelCounter) + ".lvl";
    std::cout << "Creating new level: " << levelFileName << std::endl;

    if (m_current_level)
    {
        delete m_current_level;
        m_current_level = nullptr;
    }

    if (m_enemy)
    {
        delete m_enemy;
        m_enemy = nullptr;
    }







    m_current_level = new Level(levelFileName, this);
    m_current_level->init();

    std::cout << "Current levelCounter value: " << levelCounter << std::endl;

    levelCounter++;
    createEnemiesForLevel();



    m_enemy = new Enemy("Enemy", m_current_level, this);
    m_enemy->init();

    graphics::preloadBitmaps(getAssetDir());
    graphics::setFont(m_asset_path + "OpenSans-Regular.ttf");
}

void GameState::updateEndScreen()
{


    graphics::playSound(getFullAssetPath("gameover.wav"), 1.0f);

}


void GameState::drawStartScreen()
{


    br_enter.texture = getFullAssetPath("enter.png");

    graphics::drawRect(3, 3, 6, 6, br_enter);


    graphics::drawText(0.5, 5, 0.29, "Watch out for the birds! Search for the apple!", br_close);



}

void GameState::drawLevelScreen()
{

    if (!m_current_level)
        return;

    m_current_level->draw();


    for (auto& enemy : m_enemies)
    {
        if (!enemy)
            continue;

        enemy->draw();
    }

    if (!m_player)
        return;

    m_player->draw();

}

void GameState::drawEndScreen()
{
    br_enter.texture = getFullAssetPath("end.png");

    graphics::drawRect(3, 3, 6, 6, br_enter);

    std::string scoreString = "Your score: " + std::to_string(score);

    br_close.fill_color[0] = 0.2;

    graphics::drawText(0, 5, 0.6, scoreString, br_close);
}


GameState* GameState::m_unique_instance = nullptr;
