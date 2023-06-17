#pragma once
#include <SFML/Graphics.hpp>
#include "Zombie.h"

using namespace sf;

int createBackground(VertexArray& rVertexArray, IntRect arena);
Zombie* createHorde(int numZombies, IntRect arena);