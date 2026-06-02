#include "GameObject.h"
#include <cmath>

GameObject::GameObject(sf::Vector2f startPosition, float collisionRadius)
    : position(startPosition), radius(collisionRadius), alive(true) {}

sf::Vector2f GameObject::getPosition() const {
    return position;
}
float GameObject::getRadius() const {
    return radius;
}
bool GameObject::isAlive() const {
    return alive;
}
void GameObject::destroy() {
    alive = false;
}

float distanceBetween(sf::Vector2f a, sf::Vector2f b) {
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool circlesCollide(const GameObject& a, const GameObject& b) {
    return distanceBetween(a.getPosition(), b.getPosition()) <= a.getRadius() + b.getRadius();
}
