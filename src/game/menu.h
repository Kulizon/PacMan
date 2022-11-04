#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


void init_end_menu(SDL_Renderer *rend, SDL_Window *win, struct Menu *menu);
void render_end_menu(SDL_Renderer *rend, SDL_Window *win, struct Menu *menu, struct Game *game);
void init_main_menu(SDL_Renderer *rend, SDL_Window *win, struct Menu *menu);
void render_main_menu(SDL_Renderer *rend, SDL_Window *win, struct Menu *menu);
void init_menu(struct Menu *menu);
void render_menu(SDL_Renderer *rend, SDL_Window *win, struct Menu *menu, struct Game *game);

#endif