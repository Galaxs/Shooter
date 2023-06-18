#include "Bullet.h"
#include "iostream"
#include "cmath"

Bullet::Bullet()
{
    m_Sprite = Sprite(TextureHolder::getTexture("graphics/bullets/02.png"));
    m_Sprite.setOrigin((m_Sprite.getLocalBounds().left + m_Sprite.getLocalBounds().width) / 2,
                        (m_Sprite.getLocalBounds().top + m_Sprite.getLocalBounds().height) / 2);
    m_Sprite.setScale(Vector2f(.4f, .4f));
}

void Bullet::shoot(float startX, float startY, float targetX, float targetY)
{
    m_InFlight = true;
    m_Position.x = startX;
    m_Position.y = startY;

    // calculate the gradient of fight path
    float gradient = (startX - targetX) / (startY - targetY);

    // any gradient less than 0 needs to be negative
    if (gradient < 0)
    {
        gradient *= -1;
    }

    // calculate the ratio between X and Y
    float ratioXY = m_Speed / (gradient + 1);

    // set the speed horizontaly and vertically
    m_BulletDistanceY = ratioXY;
    m_BulletDistanceX = ratioXY * gradient;

    // points the bullet in the right direction
    if (targetX < startX)
    {
        m_BulletDistanceX *= -1;
    }
    if (targetY < startY)
    {
        m_BulletDistanceY *= -1;
    }
  

    // set the max range
    float range = 1000;
    m_MinX = startX - range;
    m_MaxX = startX + range;
    m_MinY = startY - range;
    m_MaxY = startY + range;

    // Position the bullet ready to be drawn
    m_Sprite.setPosition(m_Position);
      float angle = (atan2(targetY - startY, targetX - startX));        // radiants
      float angleDegrees = angle * (180 / 3.141);                   //      from radiants to degrees
    m_Sprite.setRotation(angleDegrees);
}

void Bullet::stopBullet()
{
    m_InFlight = false;
}

bool Bullet::isInFlight()
{
    return m_InFlight;
}

FloatRect Bullet::getPosition()
{
    return m_Sprite.getGlobalBounds();
}

Sprite& Bullet::getShape()
{
    return m_Sprite;
    
}

void Bullet::update(float elapsedTime)
{
    // update the bullet position variable
    m_Position.x += m_BulletDistanceX * elapsedTime;
    m_Position.y += m_BulletDistanceY * elapsedTime;

    // Move the sprite
    m_Sprite.setPosition(m_Position);

    // has passed the arena?
    if (m_Position.x < m_MinX || m_Position.x > m_MaxX || 
        m_Position.y < m_MinY || m_Position.y > m_MaxY)
    {
        m_InFlight = false;
    }

}
