#pragma once
#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie
{
    private:
    const float BLOATER_SPEED = 40;
    const float CHASER_SPEED = 80;
    const float CRAWLER_SPEED = 20;

    //  healts
    const float BLOATER_HEALTH = 5;
    const float CHASER_HEALTH = 1;
    const float CRAWLER_HEALTH = 3;

    // to make slightly variation of the speed
    const int MAX_VARRIANCE = 30;
    const int OFFSET = 101 - MAX_VARRIANCE;

    // Variables for gradient / XY ratio
    float m_DistanceY, m_DistanceX;

    // Position of the enemy
    Vector2f m_Position;

    // Sprite for the enemy
    Sprite m_Sprite;

    // Speed
    float m_Speed;

    // Health
    float m_Health;

    // alive?
    bool m_IsAlive = true;

    public:
    // Handle when a bullet hit the zombie
    bool hit();

    // is this instance of the zombie is alive?
    bool getIsAlive();

    // spawn a new zombie
    void spawn(float xPosition, float yPosition, int type, int seed);

    // Return a rectangle that is the position in the world
    FloatRect getPosition();

    // return the sprite
    Sprite& getSprite();

    // update the zombie
    void update(float elapsedTime, Vector2f playerPosition);

};
#endif