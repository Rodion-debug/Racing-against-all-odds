#pragma once
#include "GameObject.h"
#include "PlayerCar.h"
#include <string>

// Abstract item class. Different collectibles have different effects.
class Collectible : public GameObject {
public:
    Collectible(sf::Vector2f startPosition, sf::Color color, const std::string& textureFile);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;

    virtual void apply(PlayerCar& player) = 0;

protected:
    sf::Color color;
    float pulse;

    sf::Texture texture;
    bool hasTexture;
};

class TelemetryData : public Collectible {
public:
    TelemetryData(sf::Vector2f startPosition);
    void apply(PlayerCar& player) override;
};

class RepairBox : public Collectible {
public:
    RepairBox(sf::Vector2f startPosition);
    void apply(PlayerCar& player) override;
};

class BoostBox : public Collectible {
public:
    BoostBox(sf::Vector2f startPosition);
    void apply(PlayerCar& player) override;
};

class ShieldBox : public Collectible {
public:
    ShieldBox(sf::Vector2f startPosition);
    void apply(PlayerCar& player) override;
};
