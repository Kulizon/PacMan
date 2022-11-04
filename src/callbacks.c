
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


struct HealGhostParams {
    struct Entity *ghost;
    SDL_Renderer *rend;
    SDL_Window *win;
};


Uint32 turn_powerup_off_callback(Uint32 interval)
{
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = 2;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return NULL;
}

void heal_ghost( SDL_Renderer *rend, SDL_Window *win, struct Entity *ghost, struct Game *game) { 
    if (game->powerup) {
        init_entity(rend, win, ghost, "resources/ghost_vulnerable.png");
    } else {
        char img_path[22];
        sprintf(img_path, "resources/ghost%d.png", ghost->ghost_id);

        init_entity(rend, win, ghost, img_path);
    }

    
    ghost->ghost_disoriented = false;
}

void turn_powerup_off( void* game, SDL_Renderer *rend, SDL_Window *win ) {
    ((struct Game*) game)->powerup = false;
     ((struct Game*) game)->powerup_timer = 0;
}


Uint32 heal_ghost_callback(Uint32 interval, void* ghost_id)
{
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = 1;
    userevent.data1 =  *((int*)ghost_id);

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return NULL;
}






