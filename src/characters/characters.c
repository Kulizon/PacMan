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
#include "ghosts.h"


// check for any collision
bool is_collision(int x_posA, int y_posA, int x_posB, int y_posB, SDL_Rect  destA, SDL_Rect  destB,  bool check_center) {
        float leftA = x_posA;
        float rightA = x_posA + destA.w;
        float topA = y_posA;
        float bottomA = y_posA + destA.h;

        float leftB = x_posB ;
        float rightB = x_posB + destB.w ;
        float topB = y_posB ;
        float bottomB = y_posB + destB.h ;


    // for player&ghost collision
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

        // move simulation
        // asign next values to check if turn is neccessary for next move
        if (is_next == 1) {
        (*entity).x_vel_next = (*entity).y_vel_next = 0;
        if (direction == 'u') (*entity).y_vel_next = -speed;
        if (direction == 'd') (*entity).y_vel_next = speed;
        if (direction == 'l') (*entity).x_vel_next = -speed;
        if (direction == 'r') (*entity).x_vel_next = speed;

        if ((direction == 'r' ||  direction == 'l') ) (*entity).x_pos_next += (*entity).x_vel_next / 60;
        if ((direction == 'd' || direction == 'u')) (*entity).y_pos_next += (*entity).y_vel_next / 60;
        } else {
        // normal move
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
        // check if ghost is disoriented and asign speed accordingly
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

// passing through map
void check_map_boundaries(struct Entity *entity) {
    if ((*entity).x_pos <= -TILE_WIDTH - 1) {
        (*entity).x_pos = MAP_WIDTH;
    } else if ((*entity).x_pos >= MAP_WIDTH) {
        (*entity).x_pos = -TILE_WIDTH - 1;
    }
}
