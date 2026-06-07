#pragma once
#include "GameObject.h"
#include "PlayerCar.h"
#include <string>

// Abstract enemy class. It receives a pointer to the player and follows him.
class Enemy : public GameObject {
public:
    Enemy(sf::Vector2f startPosition, float radius, PlayerCar* playerTarget, float enemySpeed, int enemyDamage, const std::string& textureFile);

    void update(float dt) override;
    int getDamage() const;

protected:
    PlayerCar* target;
    float speed;
    int damage;
    float animationAngle;

    sf::Texture texture;
    bool hasTexture;

    void drawTextureOrFallback(sf::RenderWindow& window, sf::Color fallbackColor, bool droneStyle) const;
};

class Kart : public Enemy {
public:
    Kart(sf::Vector2f startPosition, PlayerCar* playerTarget);
    void draw(sf::RenderWindow& window) const override;
};

class HeavyDrone : public Enemy {
public:
    HeavyDrone(sf::Vector2f startPosition, PlayerCar* playerTarget);
    void draw(sf::RenderWindow& window) const override;
};
