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

bool load_music(struct Music *music)
{
    // loading success flag
    bool success = true;

    // load music
    music->gSiren = Mix_LoadMUS( "resources/sounds/siren.wav" );
    if( music->gSiren == NULL )
    {
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    // load sound effects
    music->gMunch = Mix_LoadWAV( "resources/sounds/munch.wav" );
    if( music->gMunch == NULL )
    {
        printf( "Failed to load munch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    // load sound effects
    music->gBeginning = Mix_LoadWAV( "resources/sounds/beginning.wav" );
    if( music->gBeginning == NULL )
    {
        printf( "Failed to load beginning sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    music->gDeath = Mix_LoadWAV( "resources/sounds/death.wav" );
    if( music->gDeath == NULL )
    {
        printf( "Failed to load death sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    music->gPowerup = Mix_LoadWAV( "resources/sounds/powerup.wav" );
    if( music->gPowerup == NULL )
    {
        printf( "Failed to load powerup effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    music->gWin = Mix_LoadWAV( "resources/sounds/win.wav" );
    if( music->gWin == NULL )
    {
        printf( "Failed to load win sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    music->gEatGhost = Mix_LoadWAV( "resources/sounds/eat_ghost.wav" );
    if( music->gEatGhost == NULL )
    {
        printf( "Failed to load eat ghost sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    return success;
}


// clean up music before closing
void close_music(struct Music *music) {
    // free the sound effects
    Mix_FreeChunk( music->gDeath );
    Mix_FreeChunk( music->gMunch );
    Mix_FreeChunk( music->gMunch  );
    Mix_FreeChunk( music->gWin );
    music->gDeath = NULL;
    music->gMunch = NULL;
    music->gPowerup = NULL;
    music->gWin = NULL;
    
    // free the music
    Mix_FreeMusic( music->gSiren );
    music->gSiren = NULL;

    Mix_Quit();
}



void init_end_menu(SDL_Renderer *rend, SDL_Window *win, struct Menu *menu) {
    struct Entity end_menu_points_text;
    struct Entity end_menu_restart_text;
    struct Entity end_menu_lost_text;

    menu->end_menu_points_text = end_menu_points_text;
    menu->end_menu_restart_text = end_menu_restart_text;
    menu->end_menu_lost_text = end_menu_lost_text;
}

void render_end_menu(SDL_Renderer *rend, SDL_Window *win, struct Menu *menu, struct Game *game) {
    SDL_RenderClear(rend);

    init_text_entity(rend, menu, &(*menu).end_menu_lost_text, "You lost!", 'l', false);
    (*menu).end_menu_lost_text.dest.x = (WINDOW_WIDTH - (*menu).end_menu_lost_text.dest.w ) / 2;
    (*menu).end_menu_lost_text.dest.y = 200;

    // convert current points to string
    char points_text[22];
    if (game->points != 1)    sprintf(points_text, "You've got %d points", (*game).points);
    else    sprintf(points_text, "You've got %d point", (*game).points);

    init_text_entity(rend, menu, &(*menu).end_menu_points_text, points_text, 'm', false);
    (*menu).end_menu_points_text.dest.x = (WINDOW_WIDTH - (*menu).end_menu_points_text.dest.w ) / 2;
    (*menu).end_menu_points_text.dest.y = menu->end_menu_lost_text.dest.y + menu->end_menu_lost_text.dest.h + 40;
          
    init_text_entity(rend, menu, &(*menu).end_menu_restart_text, "Press SPACEBAR to restart", 'm', true);
    (*menu).end_menu_restart_text.dest.x = (WINDOW_WIDTH - (*menu).end_menu_restart_text.dest.w ) / 2;
    (*menu).end_menu_restart_text.dest.y = MAP_HEIGHT - 100;

    SDL_RenderPresent(rend);
}

void init_main_menu(SDL_Renderer *rend, SDL_Window *win, struct Menu *menu) {
    struct Entity menu_text;
    menu_text.surface = NULL;
    menu_text.texture = NULL;
    init_entity(rend, win, &menu_text, "resources/main_menu_text.png");
    menu_text.dest.w = 500;
    menu_text.dest.h = 170;
    menu_text.dest.x = (WINDOW_WIDTH - menu_text.dest.w ) / 2;
    menu_text.dest.y = 200;

    struct Entity main_menu_author_text;
    struct Entity main_menu_start_text;

    main_menu_author_text.surface = NULL;
    main_menu_author_text.texture = NULL;

    main_menu_start_text.surface = NULL;
    main_menu_start_text.texture = NULL;

    menu->main_menu_text = menu_text;
    menu->main_menu_author_text = main_menu_author_text;
    menu->main_menu_start_text = main_menu_start_text;
}

void render_main_menu(SDL_Renderer *rend, SDL_Window *win, struct Menu *menu) {
    SDL_RenderClear(rend);
    SDL_RenderCopy(rend, (*menu).main_menu_text.texture, NULL, &(*menu).main_menu_text.dest);

    init_text_entity(rend, menu, &(*menu).main_menu_author_text, "Made with <3 by Kacper", 's', false);
    (*menu).main_menu_author_text.dest.x = (WINDOW_WIDTH - (*menu).main_menu_author_text.dest.w ) / 2;
    (*menu).main_menu_author_text.dest.y = menu->main_menu_text.dest.y + menu->main_menu_text.dest.h + 40;
        
    init_text_entity(rend, menu, &(*menu).main_menu_start_text, "Press SPACEBAR to start", 'm', true);
    (*menu).main_menu_start_text.dest.x = (WINDOW_WIDTH - (*menu).main_menu_start_text.dest.w ) / 2;
    (*menu).main_menu_start_text.dest.y = MAP_HEIGHT - 100;

    SDL_RenderPresent(rend);

}

// initialize main fonts and colors
void init_menu(struct Menu *menu) {
    TTF_Font* font_s = TTF_OpenFont("resources/Font.ttf", 20);
    TTF_Font* font_m = TTF_OpenFont("resources/Font.ttf", 24);
    TTF_Font* font_l = TTF_OpenFont("resources/Font.ttf", 40);
    SDL_Color color = {255, 255, 255};
    SDL_Color color_alt = {255, 222, 89};
    menu->is_main_menu = true;
    menu->is_end_menu = false;


    menu->font_s = font_s;
    menu->font_m = font_m;
    menu->font_l = font_l;
    menu->color = color;
    menu->color_alt = color_alt;

}


void render_menu(SDL_Renderer *rend, SDL_Window *win, struct Menu *menu, struct Game *game) {

    char points_text[12];
    sprintf(points_text, "%d", (*game).points);

    // show powerup timer only if powerup is on
    char powerup_timer_text[12];
    if (game->powerup && game->powerup_timer > 0) {
        sprintf(powerup_timer_text, "%d", game->powerup_timer);
    } else {
        sprintf(powerup_timer_text, "");
    }

    // show powerup indicator only if powerup is on
    char powerup_text[16];
    if (game->powerup) {
        strcpy(powerup_text, "Powerup!");
    } else {
       strcpy(powerup_text, "");
    }

    // display user's left lives
    char lives_text[10];
    sprintf(lives_text, "Lives x%d", game->lives);

    init_text_entity(rend, menu, &menu->powerup_indicator, powerup_text, 'm', false);
    init_text_entity(rend, menu, &menu->powerup_counter, powerup_timer_text, 'm', true);
    init_text_entity(rend, menu, &menu->points_counter, points_text, 'l', false);
    init_text_entity(rend, menu, &menu->lives_counter, lives_text, 'm', true);

    menu->points_counter.dest.x = (WINDOW_WIDTH - menu->points_counter.dest.w) / 2;   
    menu->points_counter.dest.y = (WINDOW_HEIGHT - MENU_HEIGHT / 2 - menu->points_counter.dest.h / 2); 

    menu->powerup_indicator.dest.x = 25;   
    menu->powerup_indicator.dest.y = (WINDOW_HEIGHT - MENU_HEIGHT / 2 - menu->powerup_indicator.dest.h / 2); 

    menu->powerup_counter.dest.x = menu->powerup_indicator.dest.w + 40;   
    menu->powerup_counter.dest.y = (WINDOW_HEIGHT - MENU_HEIGHT / 2 - menu->powerup_counter.dest.h / 2); 

    menu->lives_counter.dest.x = MAP_WIDTH - menu->lives_counter.dest.w - 25;   
    menu->lives_counter.dest.y = (WINDOW_HEIGHT - MENU_HEIGHT / 2 - menu->lives_counter.dest.h / 2); 
}
