#include "Zombie.h"
#include "TextureHolder.h"
#include "cmath"
#include "iostream"

using namespace sf;

void Zombie::spawn(float xPosition, float yPosition, int type, int seed)
{
    switch (type)
    {
    case 0:
        // bloater
        m_Sprite = Sprite(TextureHolder::getTexture("graphics/soldier1_hold.png"));
        m_Speed = BLOATER_SPEED;
        m_Health = BLOATER_HEALTH;
        break;

    case 1:
        // chaser
        m_Sprite = Sprite(TextureHolder::getTexture("graphics/robot1_hold.png"));
        m_Health = CHASER_HEALTH;
        m_Speed = CHASER_SPEED;
        break;
    
    default:
        // crawler
        m_Sprite = Sprite(TextureHolder::getTexture("graphics/zoimbie1_hold.png"));
        m_Health = CRAWLER_HEALTH;
        m_Speed = CRAWLER_SPEED;
        break;
    }

    //  Modify the speed to make every zombie unique
    srand((int)time(0) + seed);     // seed the srand to create a speed modifier
    float modifier = rand() % MAX_VARRIANCE + OFFSET;       // from (0-30) + 70 == 70-100

    // Express this in a fraction of 1
    modifier /= 100;         // == * .01 CHECKED :)
                            // now its equal to .7 and 1;
    m_Speed *= modifier;    // the speed is now reduced from 0-30%

    // Initialize its position
    m_Position.x = xPosition;
    m_Position.y = yPosition;

    // Set the origin in the center         // FIX IF YOU HAVE A PROBLEM WARNING NOTE
    m_Sprite.setOrigin(m_Sprite.getLocalBounds().width / 2, m_Sprite.getLocalBounds().height / 2);              
    
    // move the graphical representation aka sprite to the position
    m_Sprite.setPosition(m_Position);
}

bool Zombie::hit()
{
    m_Health--;

    if (m_Health < 0)
    {
        // dead
        m_Sprite = Sprite(TextureHolder::getTexture("graphics/splash.png"));
        m_IsAlive = false;
        return true;
    }
    
    return false;
}

bool Zombie::getIsAlive()
{
    return m_IsAlive;
}

FloatRect Zombie::getPosition()     // as Float Rect
{
    return m_Sprite.getGlobalBounds();
}

Sprite& Zombie::getSprite()
{
    return m_Sprite;
}

void Zombie::update(float elapsedTime, Vector2f playerPosition)
{
    //  Move the zombie if the player is futher away from it/him
    if (playerPosition.x > m_Position.x)
    {
        m_Position.x += m_Speed * elapsedTime;
    }
    if (playerPosition.y > m_Position.y)
    {
        m_Position.y += m_Speed * elapsedTime;
    }
    if (playerPosition.x < m_Position.x)
    {
        m_Position.x -= m_Speed * elapsedTime;
    }
    if (playerPosition.y < m_Position.y)
    {
        m_Position.y -= m_Speed * elapsedTime;
    }

    // Move the sprite
    m_Sprite.setPosition(m_Position);

    // Rotate the sprite to face the player location
    float angle = (atan2(playerPosition.y - m_Position.y, playerPosition.x - m_Position.x) * 180) / 3.141;

    // Rotate the sprite
    m_Sprite.setRotation(angle); 
}