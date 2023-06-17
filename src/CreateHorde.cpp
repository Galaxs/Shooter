#include "Arena.h"

Zombie* createHorde(int numZombies, IntRect arena)
{
    Zombie* zombies = new Zombie[numZombies];

    int maxY = arena.height - 60;
    int maxX = arena.width - 60;
    int minY = arena.top + 60;
    int minX = arena.left + 60;

    for (int i = 0; i < numZombies; i++)
    {
        // which side should the zombie spawn
        srand((int)time(0) * i);
        int side = (rand() % 4);        // 4 == sides of wall
        float x, y;

        switch (side)
        {
        case 0:
            // left
            x = minX;
            y = rand() % maxY + minY;
            break;
        
        case 1:
            //top
            x = rand() % maxX + minX;
            y = minY;
            break;
        
        case 2:
            // right
            x = maxX;
            y = rand() % maxY + minY;
            break;

        case 3:
            // bottom
            x = rand() % maxX + minX;
            y = maxY;
            break; 
        }

        // Bloater, crawler, or runner
        srand((int)time(0) * i * 2);
        int type = rand() % 3;

        // Spawn the new zombie into the array
        zombies[i].spawn(x, y, type, i);
        if (i == 2)
        {
            for (int i = 0; i < 5; i++)
            {
                zombies[i].hit();
                /* code */
            }
            
        }
    }

    return zombies;
    

}