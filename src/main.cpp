#include "Engine.h"
#include "config.h"

#include <SFML/Graphics.hpp>

int main()
{
    Engine game(SCREEN_DIM, GRID_DIM, TILE_SIZE_PX);
    game.run(TARGET_FPS);
}
