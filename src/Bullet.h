#pragma once
#include <SFML/Graphics.hpp>
#include "TextureHolder.h"

using namespace sf;

class Bullet
{
private:
    Vector2f m_Position;
    Sprite m_Sprite;
    float m_Speed = 2000;
    bool m_InFlight = false;

    // what fraction of 1 does the bullet travel in both axis
    float m_BulletDistanceX;
    float m_BulletDistanceY;

    // some boundaries so the bullet dosn't fly for ever
    float m_MaxX;
    float m_MinX;
    float m_MaxY;
    float m_MinY;
public:
    Bullet();
    void stopBullet();
    bool isInFlight();

    // launch a new bullet
    void shoot(float  startX, float startY, float targetX, float targetY);


    // tell the calling code where the bullet is in the world
    FloatRect getPosition();

    Sprite& getShape();
    void update(float elapsedTime);
};

