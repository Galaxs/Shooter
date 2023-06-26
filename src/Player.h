#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include "TextureHolder.h"

using namespace sf;

class Player
{
private:
    
    const float START_SPEED = 200.f;
    const float START_HEALTH = 300.f;
    Vector2f m_Position;        //  where is the player
    Sprite m_Sprite;        //  visual representation of the player
    Texture m_Texture;
    Vector2f m_ScreenRes;
    IntRect m_ArenaSize;
    float m_DistanceX, m_DistanceY;
    short m_TileSize;
    bool m_UpPressed;
    bool m_DownPressed;
    bool m_LeftPressed;
    bool m_RightPressed;
    int m_Healt;
    int m_MaxHealt;
    Time m_LastHit;
    float m_Speed;

public:
    Player();
    void spawn(IntRect arena, Vector2f resolution, short tileSize);
    void resetPlayerStats();        //  call this at the end of every game
    bool hit(Time timeHit);
    Time getLastTimeHit();          // how long ago was the player last hit
    FloatRect getPosition();        //  where is the player
    Vector2f getCenter();       //  where is the center of the player
    float getRotation();        //  where is the player facing
    Sprite getSprite();     //  send a copy of the sprite to the main function
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void stopMovingUp();
    void stopMovingDown();
    void stopMovingLeft();
    void stopMovingRight();
    void update(float elapsedTime, Vector2i mouseCoords);       // call every frame
    void upgradeSpeed();        // give the player a speed boost
    void upgradeHealth();       // give the player some health
    void healUp(int amount);        //  increase the max HP
    int getHP();                   // how much hp does the player have?
};




#endif