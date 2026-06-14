#include "PlayerCar.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cmath>

PlayerCar::PlayerCar(sf::Vector2f startPosition)
    : GameObject(startPosition, 28.0f),
      hp(Constants::PlayerMaxHp),
      telemetry(0),
      boostTimer(0.0f),
      shieldTimer(0.0f),
      angle(0.0f),
      velocity(0.0f, 0.0f),
      hasTexture(false)
{
    hasTexture = texture.loadFromFile("assets/f1_car.png");
}
// Handles arcade-style car movement.
// The car rotates with A/D and moves forward or backward
// in the direction it is currently facing.
void PlayerCar::update(float dt)
{
    const float turnSpeed = 180.0f;

    const float speed = boostTimer > 0.0f
        ? Constants::PlayerBoostSpeed
        : Constants::PlayerSpeed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        angle -= turnSpeed * dt;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        angle += turnSpeed * dt;
    }

    const float radians = angle * 3.14159265f / 180.0f;

    sf::Vector2f forward(
        std::sin(radians),
        -std::cos(radians)
    );

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        position += forward * speed * dt;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        position -= forward * speed * 0.65f * dt;
    }

    boostTimer = std::max(0.0f, boostTimer - dt);
    shieldTimer = std::max(0.0f, shieldTimer - dt);

    keepInsideWindow();
}
// Draws the player car sprite and the shield effect if it is active.
void PlayerCar::draw(sf::RenderWindow& window) const
{
    if (hasTexture)
    {
        sf::Sprite car(texture);
    car.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    car.setPosition(position);
    car.setRotation(angle - 90.0f);
    car.setScale(0.30f, 0.30f);
    window.draw(car);
    }
    else
    {
        sf::CircleShape car(radius);
        car.setOrigin(radius, radius);
        car.setPosition(position);
        car.setRotation(angle);
        car.setFillColor(sf::Color::Red);
        window.draw(car);
    }

    if (shieldTimer > 0.0f)
    {
        sf::CircleShape shield(radius + 8.0f);
        shield.setOrigin(radius + 8.0f, radius + 8.0f);
        shield.setPosition(position);
        shield.setFillColor(sf::Color::Transparent);
        shield.setOutlineThickness(3.0f);
        shield.setOutlineColor(sf::Color::Cyan);
        window.draw(shield);
    }
}
// Reduces player HP unless the shield is currently active.
void PlayerCar::takeDamage(int damage)
{
    if (shieldTimer > 0.0f)
    {
        return;
    }

    hp -= damage;

    if (hp < 0)
    {
        hp = 0;
    }
}

void PlayerCar::repair(int amount)
{
    hp += amount;

    if (hp > Constants::PlayerMaxHp)
    {
        hp = Constants::PlayerMaxHp;
    }
}

void PlayerCar::addTelemetry()
{
    telemetry++;
}
// Temporarily increases the player's movement speed.
void PlayerCar::activateBoost()
{
    boostTimer = 4.0f;
}
// Activates temporary protection from enemy damage.
void PlayerCar::activateShield()
{
    shieldTimer = 5.0f;
}

int PlayerCar::getHp() const
{
    return hp;
}

int PlayerCar::getTelemetry() const
{
    return telemetry;
}

bool PlayerCar::hasWon() const
{
    return telemetry >= Constants::TelemetryToWin;
}

void PlayerCar::keepInsideWindow()
{
    position.x = std::max(radius, std::min(position.x, Constants::WindowWidth - radius));
    position.y = std::max(radius, std::min(position.y, Constants::WindowHeight - radius));
}