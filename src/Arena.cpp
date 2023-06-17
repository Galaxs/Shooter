#include "Arena.h"

using namespace sf;

int createBackground(VertexArray& rVertexArray, IntRect arena)
{
    // Making changes to rVertexArray

    // tile
    const int TILE_SIZE = 64;
    const int TILE_TYPES = 3;
    const int VERTS_IN_QUAD = 4;

    int worldWidth = arena.width / TILE_SIZE;
    int worldHeight = arena.height / TILE_SIZE;
    
    // what type of primitive are we using
    rVertexArray.setPrimitiveType(Quads);

    // set the size of the vertex array
    rVertexArray.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

    // start at the beginning of th vertex array
    int currentVertex = 0;
    for (short w = 0; w < worldWidth; w++)
    {
        for (short h = 0; h < worldHeight; h++)
        {
            // Position each vertex in the current quad
            rVertexArray[currentVertex + 0].position = Vector2f(w * TILE_SIZE, h * TILE_SIZE);
            rVertexArray[currentVertex + 1].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE);
            rVertexArray[currentVertex + 2].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);
            rVertexArray[currentVertex + 3].position = Vector2f(w * TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);

            // Define the position in the texture for current quad
            // either 3 floors or wall

            if (h == 0 || h == worldHeight - 1 || w == 0 || w == worldWidth - 1)
            {
                // Use the wall texture
                rVertexArray[currentVertex + 0].texCoords = Vector2f(0, 0 + TILE_TYPES * TILE_SIZE);    // assuming that its the last texture in a vertival spritesheet
                rVertexArray[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);
                rVertexArray[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
                rVertexArray[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);
            }
            else
            {
                // Use a random floor texture
                srand((int)time(0) + w * h);
                int choosenTile = rand() % TILE_TYPES;        // that's why 3 tiles == TILE_TYPES (reminder can be maximun 3)
                int verticalOffset = choosenTile * TILE_SIZE;

                rVertexArray[currentVertex + 0].texCoords = Vector2f(0, 0 + verticalOffset);
                rVertexArray[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + verticalOffset);
                rVertexArray[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, verticalOffset + TILE_SIZE);
                rVertexArray[currentVertex + 3].texCoords = Vector2f(0, verticalOffset + TILE_SIZE);
            }

            // position ready for the next four vertices
            currentVertex += VERTS_IN_QUAD;
        }
        
    }
    
    return TILE_SIZE;
}
