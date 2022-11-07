#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "src/characters/characters.h"
#include "src/characters/ghosts.h"
#include "main.h"
#include "src/game/menu.h"
#include "src/game/game.h"
#include "src/entity.h"
#include "src/map.h"
#include "src/callbacks.h"

int main(void) {
    // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }


    if (TTF_Init() != 0) {
        printf("error initializing TTF: %s\n", SDL_GetError());
        return 1;
    }

     // initialize SDL_mixer
     // if audio lags change sample size (last argument)
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 1, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("PacMan",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT,0);
    if (!win)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
	    return 1;
    }

    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    

    struct Music music;
    load_music(&music);

    struct Game game;
    game.lives = 3;
    game.points = 0;

    init_game(&game);

    struct Entity tiles_entities[TILES_NUMBER_X * TILES_NUMBER_Y]; 
    init_map(rend, win, tiles_entities, &game);

    struct Entity player;
    player.surface = NULL;
    player.texture = NULL;
    init_entity(rend, win, &player, "resources/player1.png");
    init_character(&player, TILE_WIDTH * 8 + TILE_WIDTH * (CHARACTER_SCALE-1) / 2, TILE_HEIGHT * 5 + TILE_HEIGHT * (CHARACTER_SCALE-1) / 2, 'p');

    // initialize 4 ghosts
    struct Entity ghosts[4];
    init_ghosts(rend, win, ghosts);

    // initialize menu
    struct Menu menu;
    init_menu(&menu);

    // initialize main startup menu
    init_main_menu(rend, win, &menu);
    init_end_menu(rend, win, &menu);

    // set to 1 when window close button is pressed
    int close_requested = 0;

 
    while (!close_requested)
    {
        // main menu
        Mix_HaltChannel(-1);
        if (menu.is_main_menu && !close_requested) Mix_PlayChannel( -1, music.gBeginning, 0 );
        while (menu.is_main_menu && !close_requested) {
            SDL_Event event;
            while(SDL_PollEvent(&event)) {
                 switch (event.type)
            {
            case SDL_QUIT:
                close_requested = 1;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_SPACE:
                    menu.is_main_menu = false;
                break;
            }
        }
        }


                    
            render_main_menu(rend, win, &menu);

            SDL_Delay(100);
        }

        Mix_HaltChannel(-1);

        main_game_loop(rend, win, &game, &menu, &music, &player, ghosts, tiles_entities, &close_requested, sizeof(tiles_entities)/sizeof(tiles_entities[0]));


                if (menu.is_end_menu && !close_requested)  Mix_PlayChannel( -1, music.gBeginning, 0 );
                while (menu.is_end_menu && !close_requested) {
                        SDL_Event event;
                        while(SDL_PollEvent(&event)) {
                            switch (event.type)
                        {
                        case SDL_QUIT:
                        close_requested = 1;
                                break;
                         case SDL_KEYDOWN:
                            switch (event.key.keysym.scancode)
                        {
                             case SDL_SCANCODE_SPACE:
                                menu.is_end_menu = false;
                                game.lives = 3;
                                game.lost = false;
                                game.points = 0;
                                break;
            }
        }
        }

                    
                    render_end_menu(rend, win, &menu, &game);
                    SDL_Delay(100);
                }
    }
    

    // clean up resources before exiting
    SDL_DestroyTexture(player.texture);
    for (int i = 0; i < 4; i++)
    {
        SDL_DestroyTexture(ghosts[i].texture);
    }

    for (int i = 0; i < TILES_NUMBER_X * TILES_NUMBER_Y; i++)
    {
         SDL_DestroyTexture(tiles_entities[i].texture);
    }

    close_music(&music);
    TTF_CloseFont(menu.font_s);
    TTF_CloseFont(menu.font_m);
    TTF_CloseFont(menu.font_l);

    // destroy every texture here later
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);

    IMG_Quit();
    SDL_Quit();
}