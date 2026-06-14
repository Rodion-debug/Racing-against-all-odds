#include "Enemy.h"
#include "Constants.h"
#include <cmath>

Enemy::Enemy(sf::Vector2f startPosition, float collisionRadius, PlayerCar* playerTarget, float enemySpeed, int enemyDamage, const std::string& textureFile)
    : GameObject(startPosition, collisionRadius),
      target(playerTarget),
      speed(enemySpeed),
      damage(enemyDamage),
      animationAngle(0.0f),
      hasTexture(false) {
    hasTexture = texture.loadFromFile(textureFile);
}
// Moves the enemy toward the player position using delta time,
 // so movement speed does not depend on frame rate.
 // Different enemy types use different movement speeds and damage values.
void Enemy::update(float dt) {
    if (!target) return;

    sf::Vector2f direction = target->getPosition() - position;
    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0.01f) {
        direction /= length;
        position += direction * speed * dt;
    }

    animationAngle += 180.0f * dt;
}

int Enemy::getDamage() const { return damage; }

void Enemy::drawTextureOrFallback(sf::RenderWindow& window, sf::Color fallbackColor, bool droneStyle) const {
    if (hasTexture) {
        sf::Sprite sprite(texture);
        sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        sprite.setPosition(position);
        sprite.setRotation(animationAngle);
        const float scale = droneStyle ? 0.45f : 0.55f;
        sprite.setScale(scale, scale);
        window.draw(sprite);
        return;
    }

    // Fallback drawing: used only if the PNG file is missing.
    if (droneStyle) {
        sf::CircleShape drone(radius);
        drone.setOrigin(radius, radius);
        drone.setPosition(position);
        drone.setFillColor(fallbackColor);
        window.draw(drone);

        sf::RectangleShape rotor({52.0f, 6.0f});
        rotor.setOrigin(26.0f, 3.0f);
        rotor.setPosition(position);
        rotor.setRotation(animationAngle);
        rotor.setFillColor(sf::Color(30, 30, 30));
        window.draw(rotor);
    } else {
        sf::RectangleShape body({46.0f, 28.0f});
        body.setOrigin(23.0f, 14.0f);
        body.setPosition(position);
        body.setRotation(animationAngle);
        body.setFillColor(fallbackColor);
        window.draw(body);
    }
}

Kart::Kart(sf::Vector2f startPosition, PlayerCar* playerTarget)
    : Enemy(startPosition, 24.0f, playerTarget, Constants::KartSpeed, Constants::KartDamage, "assets/kart_enemy.png") {}

void Kart::draw(sf::RenderWindow& window) const {
    drawTextureOrFallback(window, sf::Color(240, 150, 40), false);
}

HeavyDrone::HeavyDrone(sf::Vector2f startPosition, PlayerCar* playerTarget)
    : Enemy(startPosition, 22.0f, playerTarget, Constants::DroneSpeed, Constants::DroneDamage, "assets/drone_enemy.png") {}

void HeavyDrone::draw(sf::RenderWindow& window) const {
    drawTextureOrFallback(window, sf::Color(120, 120, 230), true);
}
