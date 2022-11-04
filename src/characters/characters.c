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
#include "../map.h"

const char directions[4] = {'u', 'r', 'd', 'l'};

bool is_collision(int x_posA, int y_posA, int x_posB, int y_posB, SDL_Rect  destA, SDL_Rect  destB,  bool check_center) {
        float leftA = x_posA;
        float rightA = x_posA + destA.w;
        float topA = y_posA;
        float bottomA = y_posA + destA.h;

        float leftB = x_posB ;
        float rightB = x_posB + destB.w ;
        float topB = y_posB ;
        float bottomB = y_posB + destB.h ;



    if (check_center) {
        leftA = rightA = x_posA + destA.w / 2;
        topA = bottomA = y_posA + destA.h / 2;
    }
    

    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    return true;
}

void determine_velocity(struct Entity *entity, char direction, bool is_next, bool is_disoriented, bool is_vulnerable) {

        int speed = is_disoriented ? DISORIENTED_SPEED : is_vulnerable ? VULNREABLE_SPEED : SPEED;

        // check for change direction with next move
        if (is_next == 1) {
        (*entity).x_vel_next = (*entity).y_vel_next = 0;
        if (direction == 'u') (*entity).y_vel_next = -speed;
        if (direction == 'd') (*entity).y_vel_next = speed;
        if (direction == 'l') (*entity).x_vel_next = -speed;
        if (direction == 'r') (*entity).x_vel_next = speed;

        if ((direction == 'r' ||  direction == 'l') ) (*entity).x_pos_next += (*entity).x_vel_next / 60;
        if ((direction == 'd' || direction == 'u')) (*entity).y_pos_next += (*entity).y_vel_next / 60;
        } else {
        (*entity).x_vel = (*entity).y_vel = 0;
        if (direction == 'u') (*entity).y_vel = -speed;
        if (direction == 'd') (*entity).y_vel = speed;
        if (direction == 'l') (*entity).x_vel = -speed;
        if (direction == 'r') (*entity).x_vel = speed;

        // update positions
        if ((direction == 'r' ||  direction == 'l') ) (*entity).x_pos += (*entity).x_vel / 60;
        if ((direction == 'd' || direction == 'u')) (*entity).y_pos += (*entity).y_vel / 60;

        }

}
void handle_movement(struct Entity tiles_entities[], struct Entity *entity, struct Game *game, struct Music *music) {
        // check if ghost is disoriented
        bool is_disoriented = (entity->ghost_disoriented && entity->type == 'g' ) ? true : false;
        bool is_vulnerable = (game->powerup && entity->type == 'g' ) ? true : false;

        // check if direction change is possible for entity
        determine_velocity(entity, (*entity).direction_next, true, is_disoriented, is_vulnerable);
        int next_ghost_collision_happened = check_map_collisions(tiles_entities, entity, game, music, true);
        if (next_ghost_collision_happened == 0 && (*entity).direction_next != '0') {
            (*entity).direction = (*entity).direction_next;
            (*entity).direction_next = '0';
        } 

        // determine entity velocity
        determine_velocity(entity, (*entity).direction, false, is_disoriented, is_vulnerable);
}

void set_dest(struct Entity *entity) {
        (*entity).dest.y = (*entity).y_pos_next = (int) (*entity).y_pos;
        (*entity).dest.x = (*entity).x_pos_next = (int) (*entity).x_pos;
}


void init_character(struct Entity *entity, int initial_pos_x, int initial_pos_y, char entity_type) {
    (*entity).dest.w = TILE_WIDTH / CHARACTER_SCALE;
    (*entity).dest.h = TILE_HEIGHT / CHARACTER_SCALE;
    
    // set initial velocity
    (*entity).x_pos =  (*entity).x_pos_next = initial_pos_x;
    (*entity).y_pos  =  (*entity).y_pos_next = initial_pos_y;

    // set initial velocity
    (*entity).x_vel =   (*entity).y_vel = (*entity).x_vel_next =  (*entity).y_vel_next = 0;


    (*entity).direction = (*entity).direction_next =  '0';

    (*entity).type = entity_type;
}

void init_ghosts(SDL_Renderer *rend,  SDL_Window *win, struct Entity ghosts[]) {
    for (int i = 0; i < 4; i++)
    {
    struct Entity ghost;
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



// teleport to other side
void check_map_boundaries(struct Entity *entity) {
    if ((*entity).x_pos <= -TILE_WIDTH - 1) {
        (*entity).x_pos = MAP_WIDTH;
    } else if ((*entity).x_pos >= MAP_WIDTH) {
        (*entity).x_pos = -TILE_WIDTH - 1;
    }
}
