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

// init text
void init_entity(SDL_Renderer *rend, SDL_Window *win, struct Entity *ent, char img_path[]){

    // clean up on rerender
    if (ent->surface) {
        SDL_FreeSurface(ent->surface);
    }
    if (ent->texture != NULL) {
        SDL_DestroyTexture(ent->texture);
    }

    SDL_Surface* surface = IMG_Load(img_path);

    if (!surface)
    {
        printf("error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface);

    if (!texture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }

    ent->surface = surface;
    ent->texture = texture;
}



// init AND render text
void init_text_entity(SDL_Renderer *rend, struct Menu *menu, struct Entity *ent, char text[], char font_size, bool is_color_alt) {
    // clean up on rerender
    if (ent->surface) {
        SDL_FreeSurface(ent->surface);
    }
    if (ent->texture != NULL) {
        SDL_DestroyTexture(ent->texture);
    }

    // asign font size
    TTF_Font *font = 0;
    if (font_size == 's') font = menu->font_s;
    else if (font_size == 'm') font = menu->font_m;
    else if (font_size == 'l') font = menu->font_l;

    SDL_Color color = menu->color;
    if (is_color_alt) color = menu->color_alt;

    SDL_Surface* text_surface = TTF_RenderText_Solid(font, text, color); 

    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(rend, text_surface);
    SDL_RenderCopy(rend, text_texture, NULL, &ent->dest);

    // get text dimensions and asign proper width and height
    TTF_SizeText(font, text, &ent->dest.w, &ent->dest.h);
    
    (*ent).surface = text_surface;
    (*ent).texture = text_texture;
}
