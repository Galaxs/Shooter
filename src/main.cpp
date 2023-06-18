#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Player.h"
#include "Bullet.h"
#include "Arena.h"
#include "TextureHolder.h"

using namespace sf;
int main()
{
    // Instaces of Classes
    TextureHolder textureHolder;
    Player player;

    //  the game will always be in one of the following states
    enum class State
    {
        PAUSED,
        LEVELING_UP,
        GAME_OVER,
        PLAYING
    };

    State state = State::GAME_OVER; // the engine start at game over state

    Vector2f resolution;
    resolution.x = VideoMode::getDesktopMode().width;
    resolution.y = VideoMode::getDesktopMode().height;

    RenderWindow window(VideoMode(resolution.x, resolution.y), "Shooter", Style::Fullscreen);
    window.setFramerateLimit(60);

    //  Create a SFML View for the main action
    View mainView(FloatRect(0, 0, resolution.x, resolution.y)); // Note: it take a FLOAT RECT for the size of the view

    //  Timing
    Clock clock;
    Time timeTotal;

    //  Mouse
    Vector2f mouseWorldCoords;  // in relation to world coordinates
    Vector2i mouseScreenCoords; // in relation to the screen

    // The boundaires of th arena
    IntRect arena;
    VertexArray background; // background
    // Texture textureBackground;
    // textureBackground.loadFromFile("graphics/Untitled.png");'
    Texture backgroundTexture = TextureHolder::getTexture("graphics/Untitled.png");


    int numZombies;
    int zombiesAlive;
    Zombie* zombies = nullptr;

    // 100 bullet spool
    Bullet bullets[100];
    int currentBullet = 0;
    int bulletsRemaining = 24;
    int bulletsInClip = 6;
    int clipSize = 6;
    float fireRate = 3;
    Time lastFired;             // when was the last bullet fired

    // GAME LOOP
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event)) // retrives events from the main windwo and saves it in the event var.
        {
            //  Handle the window being closed
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
            {
                window.close();
            }
            if (event.type == Event::KeyPressed)
            {
                //  Pause the game while playing
                if (event.key.code == Keyboard::Return && state == State::PLAYING)
                {
                    state = State::PAUSED;
                }

                //  Resume while paused
                else if (event.key.code == Keyboard::Return && state == State::PAUSED)
                {
                    state = State::PLAYING;
                    clock.restart(); // restart the clock so there's not a jump in frame
                }

                //  Start a new game while in Game Over state
                else if (event.key.code == Keyboard::Return && state == State::GAME_OVER)
                {
                    state = State::LEVELING_UP;
                }

                // RELOAD GUN
                if (event.key.code == Keyboard::R)
                {
                    if (bulletsRemaining >= clipSize)           
                    {
                        // TODO - MAKE IT SO DONT WASTE ANY BULLET, perhaps by allowin the reload
                        // ven if have 2 bullets in the inventory 
                        // also fill the clip with the bullet in the inventory so dont waste any bullet by reloading while 
                        //still have some bullets in the clip
                        // Player have plenty bullets in the inventory - allow reload
                        bulletsInClip = clipSize;
                        bulletsRemaining -= clipSize;
                    }
                    else if (bulletsRemaining > 0)
                    {
                        // few bullets left
                        bulletsInClip = bulletsRemaining;
                        bulletsRemaining = 0;
                    }
                    // else
                    // {
                    //     // 2DO
                    // }
                    
                }

              
            }
        } // end of event pooling

        //  WASD MOVEMENT
        if (state == State::PLAYING)
        {
            //  Handle the WSAD Keys (movements) being pressed (held down) individualy per frame to update the player object
            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                player.moveUp();
            }
            else
            {
                player.stopMovingUp();
            }

            if (Keyboard::isKeyPressed(Keyboard::S))
            {
                player.moveDown();
            }
            else
            {
                player.stopMovingDown();
            }

            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                player.moveLeft();
            }
            else
            {
                player.stopMovingLeft();
            }

            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                player.moveRight();
            }
            else
            {
                player.stopMovingRight();
            }

            // SHOOT A BULLET
            if (Mouse::isButtonPressed(sf::Mouse::Left))
            {

                if (timeTotal.asMilliseconds() - lastFired.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
                {
                    // Pass the centre of the player and the center of the cross-hair
                    // to the shoot funciton
                    std::cout << "current bullet: " << currentBullet << std::endl;
                    std::cout << "bullets in clip " << bulletsInClip << endl;
                    bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldCoords.x, mouseWorldCoords.y);
                    currentBullet++;   
                    if (currentBullet > 99)
                    {
                        currentBullet = 0;
                    }   
                    lastFired = timeTotal;
                    
                    bulletsInClip--;
                }
            }
        } // end of wasd while plying


        //  Handle the LEVELING UP state
        if (state == State::LEVELING_UP)
        {
            if (event.key.code == Keyboard::T)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Y)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::U)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::I)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::O)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::P)
            {
                state = State::PLAYING;
            }

            if (state == State::PLAYING) //  prepare the level
            {
                arena.width = 20 * 64;
                arena.height = 20 * 64;
                arena.left = 0;
                arena.top = 0;

                int tileSize = createBackground(background, arena);

                // spawn the player in the middle of the arena
                player.spawn(arena, resolution, tileSize);

                numZombies = 300;
                delete[] zombies;       // delete the previous allocate memory if exists
                zombies = createHorde(numZombies, arena);
                zombiesAlive = numZombies;

                clock.restart(); // so there isn't a frame jump.
            }
        }

        //  UPDATE PART OF THE FRAME / GAME LOOP
        if (state == State::PLAYING)
        {
            //  Update the delta time
            Time dt = clock.restart();
            timeTotal += dt;                                                            // update the total game time
            float dtAsSeconds = dt.asSeconds();                                         // a fraction of 1 from the delta time
            mouseScreenCoords = Mouse::getPosition();                                   // where is the mouse pointer
            mouseWorldCoords = window.mapPixelToCoords(Mouse::getPosition(), mainView); //  convert mouse position to world coordinates of mainview
            player.update(dtAsSeconds, Mouse::getPosition());                           // update the player
            Vector2f playerPosition = player.getCenter();                               //  Make a note of the player's new position
            mainView.setCenter(player.getCenter());

            for (int i = 0; i < numZombies; i++)
            {
                if (zombies[i].getIsAlive())
                {
                    zombies[i].update(dtAsSeconds, playerPosition);
                }
            }
            
            // UPDATE THE BULLETS in flisht
            for (int i = 0; i < 100; i++)
            {
                if (bullets[i].isInFlight())
                {
                    bullets[i].update(dtAsSeconds);
                }
                
            }
        }

        


        //  DRAW PART OF THE FRAME
        if (state == State::PLAYING)
        {
            window.clear();
            window.setView(mainView); //  change to the main view to draw upon
            window.draw(background, &backgroundTexture);

            for (int i = 0; i < numZombies; i++)
            {
                window.draw(zombies[i].getSprite());
            }
            
            //  DRAW THE BULLETS
            for (int i = 0; i < 100; i++)
            {
                if (bullets[i].isInFlight())
                {
                    window.draw(bullets[i].getShape());
                }
            }
            
            
            window.draw(player.getSprite()); //  draw the player
        }

        if (state == State::LEVELING_UP)
        {
        }

        if (state == State::PAUSED)
        {
        }

        if (state == State::GAME_OVER)
        {
        }

        window.display();
    }

    return 0;
}
