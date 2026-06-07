#pragma once
#include "GameObject.h"
#include "Constants.h"

class PlayerCar : public GameObject {
public:
    PlayerCar(sf::Vector2f startPosition);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;

    void takeDamage(int damage);
    void repair(int amount);
    void addTelemetry();
    void activateBoost();
    void activateShield();

    int getHp() const;
    int getTelemetry() const;
    bool hasWon() const;

private:
    int hp;
    int telemetry;
    float boostTimer;
    float shieldTimer;
    float angle;

    // Texture is loaded once in the constructor. If loading fails, the old
    // rectangle drawing is used, so the game still works without PNG files.
    sf::Texture texture;
    bool hasTexture;

    void keepInsideWindow();
};
