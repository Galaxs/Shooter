#include "Player.h"
#include "cmath"
#include <iostream>

using namespace sf;

Player::Player() : m_Speed (START_SPEED), m_Healt (START_HEALTH), m_MaxHealt (START_HEALTH)
{
    // m_Speed = START_SPEED;
    // m_Healt = START_HEALTH;
    // m_MaxHealt = START_HEALTH;

    //  load and associate the texture to the sprite
    // m_Texture.loadFromFile("graphics/hitman1_gun.png");     
    // m_Texture = 
    m_Sprite = Sprite(TextureHolder::getTexture("graphics/hitman1_gun.png"));

    m_Sprite.setOrigin(24,21);          // could be done in another way TODO
}

void Player::spawn(IntRect arena, Vector2f resolution, short tileSize)
{
    //  place the player in the middle of the arena
    m_Position.x = arena.width / 2;
    m_Position.y = arena.height / 2;

    //  copy the details of the arena to the player's m_Arena
    m_ArenaSize = arena;

    //  remember the tile size
    m_TileSize = tileSize;

    //  store the resolution for future use
    m_ScreenRes = resolution;
}

void Player::resetPlayerStats()
{
    m_Speed = START_SPEED;
    m_Healt = START_HEALTH;
    m_MaxHealt = START_HEALTH;
}

Time Player::getLastTimeHit()
{
    return m_LastHit;
}

bool Player::hit(Time timeHit)
{
    if (timeHit.asMicroseconds() - m_LastHit.asMicroseconds() > 200)
    {
        m_LastHit = timeHit;
        m_Healt -= 10;
        return true;
    }

    return false;
}

FloatRect Player::getPosition()
{
    return m_Sprite.getGlobalBounds();
}

Vector2f Player::getCenter()
{
    return m_Position;
}

float Player::getRotation()
{
    return m_Sprite.getRotation();
}

Sprite Player::getSprite()
{
    return m_Sprite;
}

int Player::getHP()
{
    return m_Healt;
}

void Player::moveUp()
{
    m_UpPressed = true;
}

void Player::moveDown()
{
    m_DownPressed = true;
}

void Player::moveLeft()
{
    m_LeftPressed = true;
}

void Player::moveRight()
{
    m_RightPressed = true;
}

void Player::stopMovingUp()
{
    m_UpPressed = false;
}

void Player::stopMovingDown()
{
    m_DownPressed = false;
}

void Player::stopMovingLeft()
{
    m_LeftPressed = false;
}

void Player::stopMovingRight()
{
    m_RightPressed = false;
}

void Player::update(float elapsedTime, Vector2i mouseCoords)
{
    if (m_UpPressed)
    {
        m_Position.y -= m_Speed * elapsedTime;
    }

    if (m_DownPressed)
    {
        m_Position.y += m_Speed * elapsedTime;
    }

    if (m_LeftPressed)
    {
        m_Position.x -= m_Speed * elapsedTime;
    }

    if (m_RightPressed)
    {
        m_Position.x += m_Speed * elapsedTime;
    }

    m_Sprite.setPosition(m_Position);

    //  keep the player in the arena
    if (m_Position.y < m_ArenaSize.top + m_TileSize)
    {
        m_Position.y = m_ArenaSize.top + m_TileSize;
    }

    if (m_Position.y > m_ArenaSize.height - m_TileSize)
    {
        m_Position.y = m_ArenaSize.height - m_TileSize;
    }

    if (m_Position.x < m_ArenaSize.left + m_TileSize)
    {
        m_Position.x = m_ArenaSize.left + m_TileSize;
    }

    if (m_Position.x > m_ArenaSize.width - m_TileSize)
    {
        m_Position.x = m_ArenaSize.width - m_TileSize;
    }

    //  calculate the angle of the player is facing
  float angle = (atan2(mouseCoords.y - m_ScreenRes.y / 2, mouseCoords.x - m_ScreenRes.x / 2) * 180) / 3.141;
    m_Sprite.setRotation(angle);
}

void Player::upgradeSpeed()
{
    // upgrade speed by 2o%
    m_Speed += START_SPEED * .2;
}

void Player::upgradeHealth()
{
    m_MaxHealt += START_HEALTH * .2;
}

void Player::healUp(int amount)
{
    if (m_Healt + amount < m_MaxHealt)      // missing health (examp: 85/100)
    {
        m_Healt += amount;
    }
    else if (m_Healt < m_MaxHealt)          // missing health but the healing ammount would surpass the max health
    {
        m_Healt = m_MaxHealt;
    }
    else if (m_Healt == m_MaxHealt)         //  is in full health
    {
        return;
    }


}