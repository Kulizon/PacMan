
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "../../main.h"
#include "../entity.h"

const char directions[4] = {'u', 'r', 'd', 'l'};


void init_ghosts(SDL_Renderer *rend,  SDL_Window *win, struct Entity ghosts[]) {
    for (int i = 0; i < 4; i++)
    {
    struct Entity ghost;
    ghost.surface = NULL;
    ghost.texture = NULL;
    ghost.ghost_id = i + 1;

    char img_path[22];
    sprintf(img_path, "resources/ghost%d.png", ghost.ghost_id);

    init_entity(rend, win, &ghost, img_path);

    // get dimensions of ghost texture
    SDL_QueryTexture(ghost.texture, NULL, NULL, &ghost.dest.w, &ghost.dest.h);

    ghost.ghost_disoriented = false;

    int r = rand() % 4;
    ghost.direction_next = directions[r];

    int fix_x = 1;
    int fix_y = 1;

    switch (i)
    {
    case 0:
        init_character(&ghost, TILE_WIDTH + fix_x + 1, TILE_HEIGHT - fix_y - 1, 'g');
        break;
    case 1:
        init_character(&ghost, MAP_WIDTH - TILE_WIDTH * 2 + fix_x, TILE_HEIGHT- fix_y, 'g');
        break;
    case 2:
        init_character(&ghost, TILE_WIDTH + fix_x + 1, MAP_HEIGHT - TILE_HEIGHT * 2 + fix_y, 'g');
        // ghost gets stuck during restart, find more elegant solution later
        ghost.direction_next = (rand() % 2 == 0 ? 'r' : 'u');
        break;
    case 3:
        init_character(&ghost, MAP_WIDTH - TILE_WIDTH * 2 + fix_x, MAP_HEIGHT - TILE_HEIGHT * 2 - fix_y, 'g');
        break;
    }



    ghosts[i] = ghost;
    }
    

}

void handle_ghost_logic(struct Entity *ghost, struct Entity tiles_entities[], struct Game *game, struct Music *music) {
        // move if ghost met wall
        int collisionHappened = check_map_collisions(tiles_entities, ghost, game, music, false);
        if (collisionHappened == 1) {
             char next_random_ghost_direction;
             int r;
             int go_back;
            do {
            r = rand() % 4;
            next_random_ghost_direction = directions[r];

             go_back = rand() % 8;

            if (((*ghost).direction == 'r' && next_random_ghost_direction == 'l') 
            || ((*ghost).direction == 'l' && next_random_ghost_direction == 'r' )
            || ((*ghost).direction == 'd' && next_random_ghost_direction == 'u') 
            || ((*ghost).direction == 'u' && next_random_ghost_direction == 'd') )   {
                if (go_back != 7)  {
                    next_random_ghost_direction = (*ghost).direction;   
                }
                

            }



            } while ((*ghost).direction == next_random_ghost_direction);


            (*ghost).direction_next = directions[r];
        } 
}

