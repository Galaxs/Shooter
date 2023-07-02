#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "sstream"
#include "fstream"
#include <iostream>
#include "Player.h"
#include "Bullet.h"
#include "Arena.h"
#include "TextureHolder.h"
#include "Pickup.h"

using namespace sf;
int main()
{
    void setOriginToCenter(sf::Transformable& transformable);
    // Instaces of Classes
    TextureHolder textureHolder;
    Player player;
    Pickup healthPickup(1);
    Pickup ammoPickup(2);

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
    const unsigned int BULLETS_REMAINING_START = 24; // 24
    const unsigned int BULLETS_IN_CLIP_START = 6;    // 6
    const unsigned int CLIP_SIZE_START = 6;  // 6
    const unsigned int FIRE_RATE_START = 3;  // 3
    Bullet bullets[100];
    int currentBullet = 0;
    int bulletsRemaining = BULLETS_REMAINING_START; 
    int bulletsInClip = BULLETS_IN_CLIP_START; 
    int clipSize = CLIP_SIZE_START;
    float fireRate = FIRE_RATE_START; 
    Time lastFired;             // when was the last bullet fired

    // about game
    int score = 0;
    int hiScore = 0;

    // For the Home and Game Over screen
    Sprite spriteGameOver = Sprite(TextureHolder::getTexture("graphics/gameOverBG.png"));
    Sprite ammoIcon = Sprite(TextureHolder::getTexture("graphics/ammoPickup.png"));
    ammoIcon.setScale(1.5f, 1.5f);
    ammoIcon.setPosition(resolution.x * .475f, resolution.y * .088f);
    spriteGameOver.setScale(1.5, 1.5);
    
    // Create a view for the HUD
    View hudView(FloatRect(0, 0, resolution.x, resolution.y));

    // Load the font
    Font font;
    Font fontLvlUp;
    Font fontScore;
    font.loadFromFile("font/KOMIKAP_.ttf");
    fontLvlUp.loadFromFile("font/WABECK.otf");
    fontScore.loadFromFile("font/BitfalsFont.otf");

    // When is Paused
    Text pausedText;
    pausedText.setFont(font);
    pausedText.setCharacterSize(60);
    pausedText.setString("Press Enter\nto continue");
    pausedText.setOutlineThickness(1);
    pausedText.setOutlineColor(Color::Black);
    pausedText.setFillColor(Color::White);
    pausedText.setOrigin((pausedText.getLocalBounds().left + pausedText.getLocalBounds().width) / 2, (pausedText.getLocalBounds().top + pausedText.getLocalBounds().height) / 2);
    pausedText.setPosition(resolution.x * .5f, resolution.y * .4f);

    //  When is Game Over
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(Color::White);
    gameOverText.setString("Press Enter to play");
    gameOverText.setOutlineThickness(1);
    gameOverText.setOutlineColor(Color::Black);
    // gameOverText.setOrigin((gameOverText.getLocalBounds().left + gameOverText.getLocalBounds().width) / 2, (gameOverText.getLocalBounds().top + gameOverText.getLocalBounds().height) / 2);
    setOriginToCenter(gameOverText);   
    gameOverText.setPosition(resolution.x * .5f, resolution.y * .4f);

    // When leveling up
    Text levelUpText;
    levelUpText.setFont(fontScore);
    levelUpText.setCharacterSize(20);
    levelUpText.setFillColor(Color::White);
    levelUpText.setOutlineThickness(1);
    levelUpText.setOutlineColor(Color::Black);
    // levelUpText.setOrigin((levelUpText.getLocalBounds().left + levelUpText.getLocalBounds().width) / 2, (levelUpText.getLocalBounds().top + levelUpText.getLocalBounds().height) / 2);
    std::stringstream levelUpStream;
    levelUpStream << "T. Increase Rate of Fire" << 
                    "\nY. Increase clip size" << 
                    "\nU. Increase Max HP" <<
                    "\nI. Increase run speed" <<
                    "\nO. Upgrade Health Pickups"
                    "\nP. Upgrade Ammo Pickups";
    levelUpText.setString(levelUpStream.str());
    setOriginToCenter(levelUpText);
    levelUpText.setPosition(resolution.x * .5f, resolution.y * .7f);

    // Ammo
    Text ammoText;
    ammoText.setFont(font);
    ammoText.setCharacterSize(15);
    ammoText.setFillColor(Color(0,255,0,255));
    ammoText.setOutlineThickness(0.4);
    ammoText.setOrigin((ammoText.getLocalBounds().left + ammoText.getLocalBounds().width) / 2, (ammoText.getLocalBounds().top + ammoText.getLocalBounds().height) / 2);
    ammoText.setPosition(resolution.x * .5f, resolution.y * .1f);

    // Score
    Text scoreText;
    scoreText.setFont(fontScore);
    scoreText.setCharacterSize(25);
    scoreText.setFillColor(Color::White);
    scoreText.setString("0");
    scoreText.setOutlineThickness(1);
    scoreText.setOutlineColor(Color::Black);
    scoreText.setOrigin((scoreText.getLocalBounds().left + scoreText.getLocalBounds().width) / 2, (scoreText.getLocalBounds().top + scoreText.getLocalBounds().height) / 2);
    scoreText.setPosition(resolution.x * .02f, resolution.y * .9f);

    // File I/O
    //Load the high score from a file
    std::ifstream inputFile("gameData/hiScores.txt");
    if (inputFile.is_open())
    {
        // >> Reads the data
        inputFile >> hiScore;
        inputFile.close();
    }
    

    // High Score
    Text hiScoreText;
    hiScoreText.setFont(fontScore);
    hiScoreText.setCharacterSize(25);
    hiScoreText.setFillColor(Color::White);
    std::stringstream s;
    s << "Record: " << hiScore;
    hiScoreText.setString(s.str());
    hiScoreText.setOutlineThickness(1);
    hiScoreText.setOutlineColor(Color::Black);
    hiScoreText.setOrigin((hiScoreText.getLocalBounds().left + hiScoreText.getLocalBounds().width) / 2, (hiScoreText.getLocalBounds().top + hiScoreText.getLocalBounds().height) / 2);
    hiScoreText.setPosition(resolution.x * .78f, resolution.y * .9f);

    // Zombies Remaining
    Text zombiesRemainingText;
    zombiesRemainingText.setFont(font);
    zombiesRemainingText.setCharacterSize(25);
    zombiesRemainingText.setFillColor(Color::White);
    zombiesRemainingText.setOrigin((zombiesRemainingText.getLocalBounds().left + zombiesRemainingText.getLocalBounds().width) / 2, (zombiesRemainingText.getLocalBounds().top + zombiesRemainingText.getLocalBounds().height) / 2);
    zombiesRemainingText.setPosition(resolution.x * .8f, resolution.y * .1f);
    zombiesRemainingText.setString("Zombies: 100");

    // Wave number
    int wave = 0;
    Text waveNumberText;
    waveNumberText.setFont(font);
    waveNumberText.setCharacterSize(25);
    waveNumberText.setFillColor(Color::White);
    waveNumberText.setOrigin((waveNumberText.getLocalBounds().left + waveNumberText.getLocalBounds().width) / 2, (waveNumberText.getLocalBounds().top + waveNumberText.getLocalBounds().height) / 2);
    waveNumberText.setPosition(resolution.x / 2, resolution.y / 20);
    waveNumberText.setString("Wave 0");

    // Health bar
    RectangleShape healthBar;
    healthBar.setFillColor(Color::Green);
    // healthBar.setSize(Vector2f(resolution.x * .0025f, resolution.y * .01f));
    healthBar.setOrigin((healthBar.getLocalBounds().left + healthBar.getLocalBounds().width) / 2, (healthBar.getLocalBounds().top + healthBar.getLocalBounds().height) / 2);
    healthBar.setPosition(resolution.x * .005f, resolution.y * .002f);

    // Set the cross-hair and hides the mouse
    window.setMouseCursorVisible(false);
    Sprite mouseCrossHair = Sprite(TextureHolder::getTexture("graphics/crosshair001.png"));
    mouseCrossHair.setOrigin((mouseCrossHair.getLocalBounds().left + mouseCrossHair.getLocalBounds().width) / 2, (mouseCrossHair.getLocalBounds().top + mouseCrossHair.getLocalBounds().height) / 2);

    // When did we last updated the HUD?
    int frameSinceLastHUDUpdate = 0;
    
    // how often in frames do we update the hud
    int fpsFrameInterval = 100;

    // SOUND
    // Prepare the hit sound
    SoundBuffer hitBuffer;
    hitBuffer.loadFromFile("./sound/hit.wav");
    Sound hitSound;
    hitSound.setBuffer(hitBuffer);

    //  Splat
    SoundBuffer splatBuffer;
    splatBuffer.loadFromFile("./sound/splat.wav");
    Sound splatSound;
    splatSound.setBuffer(splatBuffer);

    // Soot Sound
    SoundBuffer shootBuffer;
    shootBuffer.loadFromFile("./sound/shoot.wav");
    Sound shootSound;
    shootSound.setBuffer(shootBuffer);
    shootSound.setVolume(10);

    // Reload Sound
    SoundBuffer reloadBuffer;
    reloadBuffer.loadFromFile("./sound/reload.wav");
    Sound reloadSound;
    reloadSound.setBuffer(reloadBuffer);

    // Reload Failed Sound
    SoundBuffer reloadFailedBuffer;
    reloadFailedBuffer.loadFromFile("./sound/reload_failed.wav");
    Sound reloadFailedSound;
    reloadFailedSound.setBuffer(reloadFailedBuffer);

    // Power UP sound
    SoundBuffer powerUpBuffer;
    powerUpBuffer.loadFromFile("./sound/powerup.wav");
    Sound powerUpSound;
    powerUpSound.setBuffer(powerUpBuffer);

    // PickUp Sound
    SoundBuffer pickUpBuffer;
    pickUpBuffer.loadFromFile("./sound/pickup.wav");
    Sound pickUpSound;
    pickUpSound.setBuffer(pickUpBuffer);

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
                    // Reset Stats 
                    wave = 0;
                    score = 0;
                    
                    // Prepare the gun and ammo for the next game
                    currentBullet = 0;
                    bulletsRemaining = BULLETS_REMAINING_START;
                    bulletsInClip = BULLETS_IN_CLIP_START;
                    clipSize = CLIP_SIZE_START;
                    fireRate = FIRE_RATE_START;

                    // Reset the player stats
                    player.resetPlayerStats();

                    // Reset the pickup stats
                    healthPickup.resetStats();
                    ammoPickup.resetStats();
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
                        bulletsRemaining -= (clipSize - bulletsInClip);
                        bulletsInClip = clipSize;
                        reloadSound.play();

                    }
                    else if (bulletsRemaining > 0 && bulletsRemaining < clipSize)
                    {
                        // few bullets left
                        bulletsInClip = bulletsRemaining;
                        bulletsRemaining = 0;
                        reloadSound.play();
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
                    bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldCoords.x, mouseWorldCoords.y);
                    currentBullet++;   
                    if (currentBullet > 99)
                    {
                        currentBullet = 0;
                    }   
                    lastFired = timeTotal;
                    shootSound.play();
                    bulletsInClip--;
                }
            }
        } // end of wasd while plying


        //  Handle the LEVELING UP state
        if (state == State::LEVELING_UP)
        {
            if (event.key.code == Keyboard::T)
            {
                // Increase Fire Rate
                fireRate++;
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Y)
            {
                // Increase Clip Size
                clipSize += clipSize;
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::U)
            {
                // Increase Health
                player.upgradeHealth();
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::I)
            {
                // Upgrae Speed
                player.upgradeSpeed();
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::O)
            {
                // Upgrade Health PickUps
                healthPickup.upgrade();
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::P)
            {
                // Upgrade Ammo PickUps
                ammoPickup.upgrade();
                state = State::PLAYING;
            }

            if (state == State::PLAYING) //  prepare the level
            {
                // Increase the wave number
                wave++;

                if (wave > 3)
                {
                    arena.width = (10 * 64) * wave / 4;
                    arena.height = (10 * 64) * wave / 4;
                }
                else if (wave > 1 && wave < 4)
                {
                    arena.width = (6 * 64) * wave;
                    arena.height = (6 * 64) * wave;
                }
                else 
                {
                    arena.width = (10 * 64);
                    arena.height = (10 * 64);
                }
                arena.left = 0;
                arena.top = 0;

                int tileSize = createBackground(background, arena);

                // spawn the player in the middle of the arena
                player.spawn(arena, resolution, tileSize);

                //  configure the pickup
                healthPickup.setArena(arena);
                ammoPickup.setArena(arena);
                
                // Create the Horde of zombies
                numZombies = 5 * wave;
                delete[] zombies;       // delete the previous allocate memory if exists
                zombies = createHorde(numZombies, arena);
                zombiesAlive = numZombies;

                powerUpSound.play();
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

            //  Update the cross-hair
            mouseCrossHair.setPosition(mouseWorldCoords);
            
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
            
            // UPDATE THE BULLETS on the fly
            for (int i = 0; i < 100; i++)
            {
                if (bullets[i].isInFlight())
                {
                    bullets[i].update(dtAsSeconds);
                }
                
            }
            // update the pickup
            healthPickup.update(dtAsSeconds);
            ammoPickup.update(dtAsSeconds);

            // Collision detection
            // Have any zombie being shoot?
            for (int i = 0; i < 100; i++) // if only we had all zombies alive in a array
                                                // this would have a better perfomance
            {
                for (int j = 0; j < numZombies; j++)
                {
                    if (bullets[i].isInFlight() && zombies[j].getIsAlive()) // 100 bullets by the way
                    {
                        if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
                        {
                            //  Stop the bullet
                            bullets[i].stopBullet();

                            // register the hit
                            if (zombies[j].hit())
                            {
                                // not just a hit but a kill too
                                score += 10;

                                if (score > hiScore)
                                {
                                    hiScore = score;
                                }

                                zombiesAlive--;

                                if (zombiesAlive == 0)
                                {
                                    state = State::LEVELING_UP;
                                }
                            }
                            
                            // Collision Bullet / Zombie
                            // Play Splat Sound
                            splatSound.play();
                        }
                    }
                }
                
            }

            // Have any zombie touched the player?
            for (int i = 0; i < numZombies; i++)
            {
                if (zombies[i].getIsAlive() && player.getPosition().intersects(zombies[i].getPosition()))
                {
                    if (player.hit(timeTotal))
                    {
                        hitSound.play();
                    }

                    if (player.getHP() <= 0)
                    {

                        state = State::GAME_OVER;

                        std::ofstream outputFile("gameData/hiScores.txt");
                        // << Write the data
                        outputFile << hiScore;
                        outputFile.close();
                    }
                }
            }

            // Has the player touched the pickup?
            if (healthPickup.isSpawned() && player.getPosition().intersects(healthPickup.getPosition()))
            {
                player.healUp(healthPickup.getValue());
                pickUpSound.play();
            }

            // Has the player touched the ammo pickup?
            if (ammoPickup.isSpawned() && player.getPosition().intersects(ammoPickup.getPosition()))
            {
                bulletsRemaining += ammoPickup.getValue();
                reloadSound.play();
            }

            // size up the health bar
            healthBar.setSize(Vector2f(player.getHP() * 2, resolution.y * .04f));

            // increase the number of frames since last update
            frameSinceLastHUDUpdate++;

            // recalculate the fps update hub frames
            if (frameSinceLastHUDUpdate > fpsFrameInterval)
            {
                std::stringstream ssAmmo, ssScore, ssHiScore, ssWave, ssZombiesAlive;
                
                // Update the ammo text
                ssAmmo <<  bulletsInClip << " / " << bulletsRemaining;
                ammoText.setString(ssAmmo.str());

                // Update the score text
                ssScore << "" << score;
                scoreText.setString(ssScore.str());

                // Update the high score
                ssHiScore << "Record: " << hiScore;
                hiScoreText.setString(ssHiScore.str());

                // Update the wave number
                ssWave << "Wave " << wave;
                waveNumberText.setString(ssWave.str());

                // Update the zombies alive text
                ssZombiesAlive << "Zombies: " << zombiesAlive;
                zombiesRemainingText.setString(ssZombiesAlive.str());

                frameSinceLastHUDUpdate = 0;

            }
        }  // end of Update playing state

        


        //  DRAW PART OF THE FRAME
        if (state == State::PLAYING)
        {
            window.clear();
            window.setView(mainView); //  change to the main view to draw upon
            window.draw(background, &backgroundTexture);

            if (healthPickup.isSpawned())
            {
                window.draw(healthPickup.getSprite());
            }
            if (ammoPickup.isSpawned())
            {
                window.draw(ammoPickup.getSprite());
            }
            
            

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
            window.draw(mouseCrossHair);

            // Switch to the HUD view
            window.setView(hudView);

            // Draw all the hud elements
            window.draw(ammoIcon);
            window.draw(ammoText);
            window.draw(scoreText);
            window.draw(hiScoreText);
            window.draw(healthBar);
            window.draw(waveNumberText);
            window.draw(zombiesRemainingText);

        }

        if (state == State::LEVELING_UP)
        {
            window.draw(spriteGameOver);
            window.draw(levelUpText);
        }

        if (state == State::PAUSED)
        {
            window.draw(pausedText);
        }

        if (state == State::GAME_OVER)
        {
            window.draw(spriteGameOver);
            window.draw(gameOverText);
            window.draw(scoreText);
            window.draw(hiScoreText);
        }

        window.display();
    }

    return 0;
}


void setOriginToCenter(sf::Transformable& transformable)
{
    sf::FloatRect bounds;

    if (auto sprite = dynamic_cast<sf::Sprite*>(&transformable))
    {
        bounds = sprite->getLocalBounds();
    }
    else if (auto text = dynamic_cast<sf::Text*>(&transformable))
    {
        bounds = text->getLocalBounds();
    }
    else
    {
        // Handle unsupported transformable types
        return;
    }

    transformable.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}