#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <vector>
#include "GameObject.h"
#include "PlayerCar.h"

class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    sf::Font font;

    // Single container for all game objects.
    std::vector<std::unique_ptr<GameObject>> objects;
    PlayerCar* player;

    std::mt19937 rng;
    float enemySpawnTimer;
    float itemSpawnTimer;
    bool paused;
    bool gameOver;
    bool victory;

    void processEvents();
    void update(float dt);
    void render();

    void createPlayer();
    void spawnEnemy();
    void spawnItem();
    void checkCollisions();
    void removeDestroyedObjects();
    void drawUserInterface();

    sf::Vector2f randomEdgePosition();
    sf::Vector2f randomInsidePosition();
};
