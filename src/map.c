#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "../main.h"
#include "callbacks.h"
#include "entity.h"
#include "characters/characters.h"

const char tiles[TILES_NUMBER_Y][TILES_NUMBER_X * 2] = {
"1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1",
"1 0 c c c 1 c c c c c 1 c c 0 1",
"1 c 1 1 c 1 c 1 c 1 c 1 c 1 c 1",
"1 c 1 c c c c c c 1 c c l 1 c 1",
"1 c c l 1 1 c 1 c 1 1 1 c 1 c 1",
"1 c 1 c c c c 1 0 c c c c c c 1",
"1 1 1 c 1 c 1 1 c 1 1 c 1 c 1 1",
"c c c c 1 c c c c c 1 c 1 c c c",
"1 c 1 c c c 1 c 1 c c c 1 1 c 1",
"1 c 1 1 1 c 1 c 1 1 1 c 1 c c 1",
"1 c c c c c c c 1 c c c c c 1 1",
"1 c 1 c 1 c 1 1 1 c 1 c 1 c 1 1",
"1 c 1 c c c c c c c 1 c c c c 1",
"1 c 1 l 1 1 1 c 1 c 1 l 1 1 1 1",
"1 0 c c c c c c 1 c c c c c 0 1",
"1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1"};

void clean_up_map(struct Entity tiles_entities[]) {
    for (int n = 0; n < TILES_NUMBER_X * TILES_NUMBER_Y; n++)
    {
    SDL_FreeSurface(tiles_entities[n].surface);
    SDL_DestroyTexture(tiles_entities[n].texture); 
    }

}


// initialize map based on 2d array 
void init_map(SDL_Renderer *rend, SDL_Window *win, struct Entity tiles_entities[], struct Game *game){
int n = 0;

for (int i = 0; i < TILES_NUMBER_Y;  i = i + 1)
{
    for (int j = 0; j < TILES_NUMBER_X * 2; j = j + 2)
{

    struct Entity tile;

    tile.dest.x = 0;
    tile.dest.y = 0;

    tile.surface = NULL;
    tile.texture = NULL;


    tile.type = '0';
    if (tiles[i][j] != '0' && tiles[i][j] != ' ') {
    // collision wall
    if (tiles[i][j] == '1') {
        init_entity(rend, win, &tile, "resources/walls/1.png");
        SDL_QueryTexture(tile.texture, NULL, NULL, &tile.dest.w, &tile.dest.h);
        tile.type = 'w';
    // point to score
    } else if (tiles[i][j] == 'c') {
        init_entity(rend, win, &tile, "resources/point.png");
        SDL_QueryTexture(tile.texture, NULL, NULL, &tile.dest.w, &tile.dest.h);
        game->map_points += 1;
        tile.type = 'c';
    // powerup
     } else if (tiles[i][j] == 'l') {
        init_entity(rend, win, &tile, "resources/powerup.png");
        SDL_QueryTexture(tile.texture, NULL, NULL, &tile.dest.w, &tile.dest.h);
        tile.type = 'l';
    }

    tile.dest.w = TILE_WIDTH;
    tile.dest.h = TILE_HEIGHT;


    // set position identically to 2d array
    tile.dest.x = j / 2 * TILE_WIDTH;
    tile.dest.y = i * TILE_HEIGHT;

    if (tile.type == 'c') {
        double scale = 2.25;

        tile.dest.w = TILE_WIDTH - TILE_WIDTH / scale;
        tile.dest.h = TILE_HEIGHT - TILE_HEIGHT / scale;
        tile.dest.x = tile.dest.x  + TILE_WIDTH / (scale * 2);
        tile.dest.y = tile.dest.y + TILE_HEIGHT / (scale * 2);
    }


    }  
    tiles_entities[n] = tile;
    n++;
}
}
}

int check_map_collisions(struct Entity tiles_entities[], struct Entity *entity, struct Game *game, struct Music *music, bool is_next) {

            // check if turn is possible for direction character is goin to move in
            for (int i = 0; i < TILES_NUMBER_X * TILES_NUMBER_Y; i++) {
            if (tiles_entities[i].texture) {
                if (is_next == 1) {
                if (is_collision((*entity).x_pos_next, (*entity).y_pos_next, tiles_entities[i].dest.x, tiles_entities[i].dest.y
                    ,(*entity).dest, tiles_entities[i].dest, false
                )) {
                        if (tiles_entities[i].type == 'w') {
                            (*entity).x_pos_next -= (*entity).x_vel_next / 60;
                            (*entity).y_pos_next -= (*entity).y_vel_next / 60;
                            return 1;
                        }
 
                } 
                } else {
            // check collision in current position
            for (int i = 0; i < TILES_NUMBER_X * TILES_NUMBER_Y; i++) {
            if (tiles_entities[i].texture) {
                if (is_collision((*entity).x_pos, (*entity).y_pos, tiles_entities[i].dest.x, tiles_entities[i].dest.y
                    ,(*entity).dest, tiles_entities[i].dest, false
                )) {
                    // if wall then prevent from moving
                    if (tiles_entities[i].type == 'w') {
                    (*entity).x_pos -= (*entity).x_vel / 60;
                    (*entity).y_pos -= (*entity).y_vel / 60;
                    return 1;
                    }
                    // if coin then play music and adjust score
                    else if (tiles_entities[i].type == 'c' && (*entity).type == 'p') {
                            tiles_entities[i].dest.x = -TILE_WIDTH;
                            tiles_entities[i].dest.y = -TILE_HEIGHT;  
                            SDL_DestroyTexture(tiles_entities[i].texture);
                            game->points += 1;
                            game->map_points -= 1;
                            if (game->map_points > 0) Mix_PlayChannel( -1, music->gMunch, 0 );
                            return 0;
                    }
                    // if powerup then add timer for puwerup mode
                    else if (tiles_entities[i].type == 'l' && (*entity).type == 'p') {
                            // if powerup is already on then delete previous timer
                            if (game->powerup) {
                                SDL_RemoveTimer(game->powerup_timer_id);
                            }


                            tiles_entities[i].dest.x = -TILE_WIDTH;
                            tiles_entities[i].dest.y = -TILE_HEIGHT;        
                            game->powerup = true;
                            // remove any music playing and play theme
                            Mix_HaltChannel(-1);
                            Mix_PlayChannel( -1, music->gPowerup, 0 );
                            // get timer start for user menu
                            game->powerup_timer_start = SDL_GetTicks();
                            
                            game->powerup_timer_id = SDL_AddTimer(10000, turn_powerup_off_callback, NULL);

                            return 0;
                    }
                } 
            }
        }

                }



            }
        }

        // prevent from going up or down off screen when passing map
        if ((*entity).x_pos > MAP_WIDTH - 10 || (*entity).x_pos < -TILE_WIDTH + 10) return 1;

        return 0;

}