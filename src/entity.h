#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


void init_entity(SDL_Renderer *rend, SDL_Window *win, struct Entity *ent, char img_path[]);
void init_text_entity(SDL_Renderer *rend, struct Menu *menu, struct Entity *ent, char text[], char font_size, bool is_color_alt);


#endif