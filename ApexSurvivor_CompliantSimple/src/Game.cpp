#include "Game.h"
#include "Constants.h"
#include "Enemy.h"
#include "Collectible.h"
#include <algorithm>
#include <iostream>

Game::Game()
    : window(sf::VideoMode(Constants::WindowWidth, Constants::WindowHeight), "Apex Survivor - Simple SFML"),
      player(nullptr),
      rng(std::random_device{}()),
      enemySpawnTimer(0.0f),
      itemSpawnTimer(0.0f),
      paused(false),
      gameOver(false),
      victory(false) {
    window.setFramerateLimit(60);

    // Loading font is only used for UI text. On Windows this path normally works.
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "Font was not loaded. UI text may not be visible.\n";
    }

    createPlayer();
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        const float dt = clock.restart().asSeconds(); // delta time for smooth movement
        processEvents();

        if (!paused && !gameOver && !victory) {
            update(dt);
        }

        render();
    }
}

void Game::processEvents() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                paused = !paused;
            }
            if (event.key.code == sf::Keyboard::R && (gameOver || victory)) {
                objects.clear();
                createPlayer();
                gameOver = false;
                victory = false;
                enemySpawnTimer = 0.0f;
                itemSpawnTimer = 0.0f;
            }
        }
    }
}

void Game::update(float dt) {
    enemySpawnTimer += dt;
    itemSpawnTimer += dt;

    if (enemySpawnTimer >= Constants::EnemySpawnTime) {
        spawnEnemy();
        enemySpawnTimer = 0.0f;
    }

    if (itemSpawnTimer >= Constants::ItemSpawnTime) {
        spawnItem();
        itemSpawnTimer = 0.0f;
    }

    // Polymorphism: every object updates itself through the same GameObject pointer.
    for (auto& object : objects) {
        object->update(dt);
    }

    checkCollisions();
    removeDestroyedObjects();

    if (player->getHp() <= 0) {
        gameOver = true;
    }
    if (player->hasWon()) {
        victory = true;
    }
}

void Game::render() {
    window.clear(sf::Color(38, 42, 38));

    // simple arena background grid
    for (int x = 0; x < static_cast<int>(Constants::WindowWidth); x += 80) {
        sf::Vertex line[] = {sf::Vertex({static_cast<float>(x), 0.0f}, sf::Color(55, 60, 55)),
                             sf::Vertex({static_cast<float>(x), static_cast<float>(Constants::WindowHeight)}, sf::Color(55, 60, 55))};
        window.draw(line, 2, sf::Lines);
    }
    for (int y = 0; y < static_cast<int>(Constants::WindowHeight); y += 80) {
        sf::Vertex line[] = {sf::Vertex({0.0f, static_cast<float>(y)}, sf::Color(55, 60, 55)),
                             sf::Vertex({static_cast<float>(Constants::WindowWidth), static_cast<float>(y)}, sf::Color(55, 60, 55))};
        window.draw(line, 2, sf::Lines);
    }

    // Polymorphism: every object draws itself through the same GameObject pointer.
    for (const auto& object : objects) {
        object->draw(window);
    }

    drawUserInterface();
    window.display();
}

void Game::createPlayer() {
    auto newPlayer = std::make_unique<PlayerCar>(sf::Vector2f(Constants::WindowWidth / 2.0f, Constants::WindowHeight / 2.0f));
    player = newPlayer.get();
    objects.push_back(std::move(newPlayer));
}

void Game::spawnEnemy() {
    std::uniform_int_distribution<int> type(0, 1);
    if (type(rng) == 0) {
        objects.push_back(std::make_unique<Kart>(randomEdgePosition(), player));
    } else {
        objects.push_back(std::make_unique<HeavyDrone>(randomEdgePosition(), player));
    }
}

void Game::spawnItem() {
    std::uniform_int_distribution<int> type(0, 3);
    const sf::Vector2f position = randomInsidePosition();

    switch (type(rng)) {
        case 0: objects.push_back(std::make_unique<TelemetryData>(position)); break;
        case 1: objects.push_back(std::make_unique<RepairBox>(position)); break;
        case 2: objects.push_back(std::make_unique<BoostBox>(position)); break;
        default: objects.push_back(std::make_unique<ShieldBox>(position)); break;
    }
}

void Game::checkCollisions() {
    for (auto& object : objects) {
        if (object.get() == player || !object->isAlive()) {
            continue;
        }

        // dynamic_cast is used because all objects are kept in one container.
        // We check what real type is behind the base GameObject pointer.
        if (auto* enemy = dynamic_cast<Enemy*>(object.get())) {
            if (circlesCollide(*player, *enemy)) {
                player->takeDamage(enemy->getDamage());
                enemy->destroy();
            }
        }

        if (auto* collectible = dynamic_cast<Collectible*>(object.get())) {
            if (circlesCollide(*player, *collectible)) {
                collectible->apply(*player);
                collectible->destroy();
            }
        }
    }
}

void Game::removeDestroyedObjects() {
    objects.erase(
        std::remove_if(objects.begin(), objects.end(), [](const std::unique_ptr<GameObject>& object) {
            return !object->isAlive();
        }),
        objects.end()
    );
}

void Game::drawUserInterface() {
    sf::RectangleShape hpBack({220.0f, 24.0f});
    hpBack.setPosition(20.0f, 20.0f);
    hpBack.setFillColor(sf::Color(90, 90, 90));
    window.draw(hpBack);

    sf::RectangleShape hpBar({220.0f * player->getHp() / Constants::PlayerMaxHp, 24.0f});
    hpBar.setPosition(20.0f, 20.0f);
    hpBar.setFillColor(sf::Color(210, 50, 50));
    window.draw(hpBar);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(20.0f, 55.0f);
    text.setString("HP: " + std::to_string(player->getHp()) +
                   "   Telemetry: " + std::to_string(player->getTelemetry()) + "/" + std::to_string(Constants::TelemetryToWin) +
                   "   ESC - pause   R - restart after end");
    window.draw(text);

    if (paused || gameOver || victory) {
        sf::Text center;
        center.setFont(font);
        center.setCharacterSize(48);
        center.setFillColor(sf::Color::White);
        center.setPosition(390.0f, 350.0f);

        if (paused) center.setString("PAUSED");
        if (gameOver) center.setString("GAME OVER - press R");
        if (victory) center.setString("VICTORY - press R");

        window.draw(center);
    }
}

sf::Vector2f Game::randomEdgePosition() {
    std::uniform_real_distribution<float> x(0.0f, static_cast<float>(Constants::WindowWidth));
    std::uniform_real_distribution<float> y(0.0f, static_cast<float>(Constants::WindowHeight));
    std::uniform_int_distribution<int> edge(0, 3);

    switch (edge(rng)) {
        case 0: return {
            x(rng), 0.0f
        };
        case 1: return {
            x(rng), static_cast<float>(Constants::WindowHeight)
        };
        case 2: return {0.0f, y(rng)}
        ;
        default: return {static_cast<float>(Constants::WindowWidth), y(rng)};
    }
}

sf::Vector2f Game::randomInsidePosition() {
    std::uniform_real_distribution<float> x(80.0f, Constants::WindowWidth - 80.0f);
    std::uniform_real_distribution<float> y(120.0f, Constants::WindowHeight - 80.0f);
    return {x(rng), y(rng)};
}
