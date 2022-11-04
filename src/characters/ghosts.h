#ifndef GHOSTS_H
#define GHOSTS_H

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

void init_character(struct Entity *entity, int initial_pos_x, int initial_pos_y, char entity_type);
void init_ghosts(SDL_Renderer *rend,  SDL_Window *win, struct Entity ghosts[]);

void handle_ghost_logic(struct Entity *ghost, struct Entity tiles_entities[], struct Game *game, struct Music *music);
#endif



