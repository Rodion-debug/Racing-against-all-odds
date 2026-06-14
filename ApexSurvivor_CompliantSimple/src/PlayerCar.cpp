#include "PlayerCar.h"
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
      hasTexture(false) {
    hasTexture = texture.loadFromFile("assets/f1_car.png");
}

void PlayerCar::update(float dt) {
    const float acceleration = 520.0f;
    const float reverseAcceleration = 300.0f;
    const float friction = 0.96f;
    const float turnSpeed = 170.0f;

    const float maxSpeed = boostTimer > 0.0f
        ? Constants::PlayerBoostSpeed
        : Constants::PlayerSpeed;

    bool accelerating = false;

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

    float radians = angle * 3.14159265f / 180.0f;

    sf::Vector2f forward(
        std::sin(radians),
        -std::cos(radians)
    );

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        velocity += forward * acceleration * dt;
        accelerating = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        velocity -= forward * reverseAcceleration * dt;
        accelerating = true;
    }

    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    if (speed > maxSpeed)
    {
        velocity = velocity / speed * maxSpeed;
    }

    if (!accelerating)
    {
        velocity *= friction;
    }

    position += velocity * dt;

    boostTimer = std::max(0.0f, boostTimer - dt);
    shieldTimer = std::max(0.0f, shieldTimer - dt);

    keepInsideWindow();
}

void PlayerCar::draw(sf::RenderWindow& window) const {
    if (hasTexture) {
        sf::Sprite car(texture);
        car.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        car.setPosition(position);
        car.setRotation(angle);
        car.setScale(0.38f, 0.38f);
        window.draw(car);

        // Simple blue circle shows that the shield effect is active.
        if (shieldTimer > 0.0f) {
            sf::CircleShape shield(radius + 8.0f);
            shield.setOrigin(radius + 8.0f, radius + 8.0f);
            shield.setPosition(position);
            shield.setFillColor(sf::Color(80, 180, 255, 80));
            shield.setOutlineColor(sf::Color(80, 180, 255));
            shield.setOutlineThickness(2.0f);
            window.draw(shield);
        }
        return;
    }

    // Fallback drawing: used only if assets/f1_car.png is missing.
    sf::RectangleShape car({64.0f, 34.0f});
    car.setOrigin(32.0f, 17.0f);
    car.setPosition(position);
    car.setRotation(angle);
    car.setFillColor(shieldTimer > 0.0f ? sf::Color(80, 180, 255) : sf::Color(210, 40, 40));
    window.draw(car);

    sf::RectangleShape cockpit({18.0f, 18.0f});
    cockpit.setOrigin(9.0f, 9.0f);
    cockpit.setPosition(position);
    cockpit.setFillColor(sf::Color(30, 30, 30));
    window.draw(cockpit);
}

void PlayerCar::takeDamage(int damage) {
    if (shieldTimer > 0.0f) {
        damage /= 2;
    }
    hp = std::max(0, hp - damage);
}

void PlayerCar::repair(int amount) {
    hp = std::min(Constants::PlayerMaxHp, hp + amount);
}

void PlayerCar::addTelemetry() {
    telemetry++;
}
void PlayerCar::activateBoost() {
    boostTimer = 4.0f;
}
void PlayerCar::activateShield() {
    shieldTimer = 5.0f;
}

int PlayerCar::getHp() const { return hp; }
int PlayerCar::getTelemetry() const {
 return telemetry; }
bool PlayerCar::hasWon() const {
    return telemetry >= Constants::TelemetryToWin; }

void PlayerCar::keepInsideWindow() {
    position.x = std::clamp(position.x, radius, Constants::WindowWidth - radius);
    position.y = std::clamp(position.y, radius, Constants::WindowHeight - radius);
}
