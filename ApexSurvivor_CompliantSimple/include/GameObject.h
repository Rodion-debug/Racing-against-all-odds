#pragma once
#include <SFML/Graphics.hpp>

// Abstract base class for every object in the game.
// It demonstrates inheritance and polymorphism: PlayerCar, Enemy and Collectible
// all share this interface, so they can be stored in one vector<GameObject*>.
class GameObject {
public:
    GameObject(sf::Vector2f startPosition, float collisionRadius);
    virtual ~GameObject() = default;

    // Pure virtual functions: every derived object must implement them.
    // Updates object logic using delta time.
    virtual void update(float dt) = 0;
    // Draws the object on the game window.
    virtual void draw(sf::RenderWindow& window) const = 0;

    sf::Vector2f getPosition() const;
    float getRadius() const;
    bool isAlive() const;
    void destroy();

protected:
    // protected: derived classes can use position directly.
    sf::Vector2f position;
    float radius;

private:
    // private: only GameObject controls whether an object is alive.
    bool alive;
};

float distanceBetween(sf::Vector2f a, sf::Vector2f b);
bool circlesCollide(const GameObject& a, const GameObject& b);
