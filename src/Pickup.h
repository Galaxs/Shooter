#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Pickup
{
private:
    const int HEALTH_START_VALUE = 50;
    const int AMMO_START_VALUE = 12;
    const int WAIT_START_VALUE = 10;
    const int SECONDS_TO_LIVE_START_VALUE = 5;

    Sprite m_Sprite;
    IntRect m_Arena;
    int m_Value;        // how much it worth
    int m_Type;         // 1 health, 2 ammo

    bool m_Spawned;
    float m_SecondsSinceSpawned;
    float m_SecondsSinceDeSpawned;
    float m_SecondsToLive;
    float m_SecondsToWait;

public:
    Pickup(int type);

    //  Prepare the new Pickup
    void setArena(IntRect arena);

    void spawn();

    //  check the possition of the pickup
    FloatRect getPosition();
    
    // get the sprite for drawing
    Sprite& getSprite();

    // Let the pickup update itself each frame
    void update(float elapsedTime);

    // is this pickup currently spawned?
    bool isSpawned();

    // get the value of this pickup
    int getValue();

    // Upgrade the pickup
    void upgrade();

};