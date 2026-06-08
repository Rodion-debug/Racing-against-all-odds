#include "Collectible.h"
#include <cmath>

Collectible::Collectible(sf::Vector2f startPosition, sf::Color itemColor, const std::string& textureFile)
    : GameObject(startPosition, 20.0f), color(itemColor), pulse(0.0f), hasTexture(false) {
    hasTexture = texture.loadFromFile(textureFile);
}

void Collectible::update(float dt) {
    pulse += dt * 4.0f; // small animation: the item breathes/pulses
}

void Collectible::draw(sf::RenderWindow& window) const {
    const float animatedRadius = radius + std::sin(pulse) * 3.0f;

    if (hasTexture) {
        sf::Sprite item(texture);
        item.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        item.setPosition(position);
        item.setScale(animatedRadius / 30.0f, animatedRadius / 30.0f);
        window.draw(item);
        return;
    }

    // Fallback drawing: used only if the PNG file is missing.
    sf::CircleShape item(animatedRadius);
    item.setOrigin(animatedRadius, animatedRadius);
    item.setPosition(position);
    item.setFillColor(color);
    window.draw(item);
}

TelemetryData::TelemetryData(sf::Vector2f startPosition)
    : Collectible(startPosition, sf::Color(80, 170, 255), "assets/telemetry.png") {}
void TelemetryData::apply(PlayerCar& player) { player.addTelemetry(); }

RepairBox::RepairBox(sf::Vector2f startPosition)
    : Collectible(startPosition, sf::Color(70, 220, 90), "assets/repair.png") {}
void RepairBox::apply(PlayerCar& player) { player.repair(25); }

BoostBox::BoostBox(sf::Vector2f startPosition)
    : Collectible(startPosition, sf::Color(255, 220, 70), "assets/drs.png") {}
void BoostBox::apply(PlayerCar& player) { player.activateBoost(); }

ShieldBox::ShieldBox(sf::Vector2f startPosition)
    : Collectible(startPosition, sf::Color(80, 240, 240), "assets/shield.png") {}
void ShieldBox::apply(PlayerCar& player) { player.activateShield(); }
