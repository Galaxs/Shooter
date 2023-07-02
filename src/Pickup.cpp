#include "Pickup.h"
#include "TextureHolder.h"

Pickup::Pickup(int type) : m_Type(type)
{
    if (type == 1)
    {
        m_Sprite = Sprite(TextureHolder::getTexture("graphics/healthPickup.png"));

        //  value
        m_Value = HEALTH_START_VALUE;

    }
    else if (type == 2)
    {
        m_Sprite = Sprite(TextureHolder::getTexture("graphics/ammoPickup.png"));

        m_Value = AMMO_START_VALUE;
    }

    m_Sprite.setOrigin((m_Sprite.getLocalBounds().left + m_Sprite.getLocalBounds().width) / 2, 
                        (m_Sprite.getLocalBounds().height + m_Sprite.getLocalBounds().top) / 2);
    
    m_SecondsToLive = SECONDS_TO_LIVE_START_VALUE;
    m_SecondsToWait = WAIT_START_VALUE;
}

void Pickup::setArena(IntRect arena)
{
    // copy the arena detalil to the member variable
    m_Arena.left = arena.left + 128;
    m_Arena.width = arena.width - 128;
    m_Arena.height = arena.height - 128;
    m_Arena.top = arena.top + 128;

    spawn();
}

void Pickup::spawn()
{
    // random width and height of the arena to spawn
    srand((int)time(0) / m_Type * m_SecondsSinceSpawned);
    int x = (rand() % m_Arena.width);
    srand((int)time(0) * m_Type / m_SecondsSinceSpawned);
    int y = (rand() % m_Arena.height);

    m_SecondsSinceSpawned = 0;
    m_Spawned = true;

    m_Sprite.setPosition(x, y);
}

FloatRect Pickup::getPosition()
{
    return m_Sprite.getGlobalBounds();
}

Sprite& Pickup::getSprite()
{
    return m_Sprite;
}

bool Pickup::isSpawned()
{
    return m_Spawned;
}

int Pickup::getValue()
{
    m_Spawned = false;
    m_SecondsSinceDeSpawned = 0;
    return m_Value;
}

void Pickup::update(float elapsedTime)
{
    if (m_Spawned)
    {
        m_SecondsSinceSpawned += elapsedTime;
    }
    else
    {
        m_SecondsSinceDeSpawned += elapsedTime;
    }

    //  DESPAWN do we need to hide the pickup? 
    if (m_SecondsSinceSpawned > m_SecondsToLive && m_Spawned)
    {
        m_Spawned = false;
        m_SecondsSinceDeSpawned = 0;
    }

    // SPAWN do we need to spawn a pickup?
    if (m_SecondsSinceDeSpawned > m_SecondsToWait && !m_Spawned)
    {
        spawn();    // spawn the pickup and reset the timer
    }
}

void Pickup::upgrade()
{
    if (m_Type == 1)
    {
        m_Value += HEALTH_START_VALUE * 0.5f;
    }
    else if (m_Type == 2)
    {
        m_Value += AMMO_START_VALUE * .5f;
    }

    // make them more frequently and last longer
    m_SecondsToLive += (SECONDS_TO_LIVE_START_VALUE / 10.0f);
    m_SecondsToWait -= (WAIT_START_VALUE / 10.0f);
    
}

void Pickup::resetStats()
{
    if (m_Type == 1)
    {
        m_Value = HEALTH_START_VALUE;
    }
    else if (m_Type == 2)
    {
        m_Value = AMMO_START_VALUE;
    }
    
    m_SecondsToLive = SECONDS_TO_LIVE_START_VALUE;
    m_SecondsToWait = WAIT_START_VALUE;
}