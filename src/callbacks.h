#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


Uint32 turn_powerup_off_callback(Uint32 interval);
void heal_ghost( SDL_Renderer *rend, SDL_Window *win, struct Entity *ghost, struct Game *game);
Uint32 heal_ghost_callback(Uint32 interval, void* ghost_id);
void turn_powerup_off( void* game, SDL_Renderer *rend, SDL_Window *win);

#endif