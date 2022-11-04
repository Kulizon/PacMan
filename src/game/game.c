
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




init_game(struct Game *game) {
    game->lost = false;
    game->powerup = false;
    game->powerup_timer = 0;
    game->map_points = 0;
}
