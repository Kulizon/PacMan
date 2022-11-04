
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
#include "../callbacks.h"
#include "../map.h"
#include "../entity.h"
#include "../characters/characters.h"
#include "../characters/ghosts.h"



 void init_game(struct Game *game) {
    game->lost = false;
    game->powerup = false;
    game->powerup_timer = 0;
    game->map_points = 0;
}


void main_game_loop(SDL_Renderer *rend, SDL_Window *win, struct Game *game, struct Menu *menu, struct Music *music, struct Entity *player, struct Entity ghosts[], struct Entity tiles_entities[], int *close_requested, int tiles_entities_size) {
        
    // make the game run smoothly on all devices
    Uint32 frame_start;
    int frame_time;

    int animation_frame = 0;

        while (game->lives > 0 && !*close_requested) {      
            // redirect to end screen if lost
            if (*close_requested || (game->lives == 0 && game->map_points > 0)) break;

            SDL_RenderClear(rend);

            SDL_RemoveTimer(game->powerup_timer_id);
            init_game(game);
            init_map(rend, win, tiles_entities, game);
            init_entity(rend, win, player, "resources/player1.png");
            init_character(player, TILE_WIDTH * 8 + TILE_WIDTH * (CHARACTER_SCALE-1) / 2, TILE_HEIGHT * 5 + TILE_HEIGHT * (CHARACTER_SCALE-1) / 2, 'p');

            init_ghosts(rend, win, ghosts);

            set_dest(player);
            for (int i = 0; i < 4; i++) set_dest(&ghosts[i]);

            render_menu(rend, win, menu, game);

            SDL_RenderCopy(rend, player->texture, NULL, &player->dest);
            for (int i = 0; i < tiles_entities_size; i++) SDL_RenderCopy(rend, tiles_entities[i].texture, NULL, &tiles_entities[i].dest);
            for (int i = 0; i < 4; i++) SDL_RenderCopy(rend, ghosts[i].texture, NULL, &ghosts[i].dest);

            SDL_RenderPresent(rend);

            // initial enter
            if (game->lives == 3 && game->points == 0) {
                Mix_PlayChannel( -1, music->gBeginning, 0 );
                SDL_Delay(2500);
            }

            SDL_Delay(2000);  





        // main game loop
        while (!game->lost && game->map_points > 0) {
         if( Mix_PlayingMusic() == 0 || Mix_PausedMusic() == 1) {
        // play the siren sound effect
            Mix_PlayMusic( music->gSiren, -1 );
        }  

        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                *close_requested = 1;
                game->lives = 0;
                game->lost = true;
                break;
            case SDL_USEREVENT: 
                // listen to heal ghost callback
                if (event.user.code == 0x1) {
                    int id = event.user.data1;
                    struct Entity *ghost_to_heal;

                    for (int i = 0; i < 4; i++)
                    {
                        if (ghosts[i].ghost_id == id) {
                            ghost_to_heal = &ghosts[i];
                            game->called_timers_ghosts_ids[i] = 0;
                            };
                        
                    }


                    heal_ghost(rend, win, ghost_to_heal, game);
                } // listen to turn power off callback
                else if (event.user.code == 0x2) {
                    for (int i = 0; i < 4; i++) {   
                        char img_path[22];
                        sprintf(img_path, "resources/ghost%d.png", ghosts[i].ghost_id);


                        if (! ((ghosts[i]).ghost_disoriented))  init_entity(rend, win, &ghosts[i], img_path);
             
                    }

                    turn_powerup_off(game, rend, win);
                }

                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    player->direction_next = 'u';

                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    player->direction_next = 'l';
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    player->direction_next = 'd';
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    player->direction_next = 'r';
                    break;
                }
                break;

            }
        }

        frame_start = SDL_GetTicks();


        // check if direction change is possible for player and determine player velocity
        check_map_boundaries(player);
        handle_movement(tiles_entities, player, game, music);
        int player_collision_happened = check_map_collisions(tiles_entities, player, game, music, false);

        // check if direction change is possible for ghost and determine ghost velocity
        for (int i = 0; i < 4; i++)
        {   
            handle_movement(tiles_entities, &ghosts[i], game, music);
            check_map_boundaries(&ghosts[i]);
        }

        // check for player collision with ghost
        for (int i = 0; i < 4; i++)
        {
            if (is_collision(player->x_pos, player->y_pos, ghosts[i].x_pos, ghosts[i].y_pos, player->dest, ghosts[i].dest, true)) {
                if (game->powerup == true && !ghosts[i].ghost_disoriented) {
                    init_entity(rend, win, &ghosts[i], "resources/ghost_disoriented.png");
                    ghosts[i].ghost_disoriented = true;
                    game->points += 10;
                    Mix_PlayChannel( -1, music->gEatGhost, 0 );

                    bool add_timer = true;

                    for (int j = 0; j < 4; j++)
                    {
                        if (ghosts[i].ghost_id == game->called_timers_ghosts_ids[j]) add_timer = false;
                    }
  

                    if (add_timer) SDL_AddTimer(10000, heal_ghost_callback, &ghosts[i].ghost_id);

                    game->called_timers_ghosts_ids[i] = ghosts[i].ghost_id;
                } else if (!ghosts[i].ghost_disoriented) {
                    game->lost = true;
                }
            
        } 
        }

        for (int i = 0; i < 4; i++)
        {
            handle_ghost_logic(&ghosts[i], tiles_entities, game, music);

        }
        

        // set the positions in the struct
        set_dest(player);
        for (int i = 0; i < 4; i++) set_dest(&ghosts[i]);
        
        
        // clear the window
        SDL_RenderClear(rend);

        // draw the images to the window
        for (int i = 0; i < tiles_entities_size; i++) SDL_RenderCopy(rend, tiles_entities[i].texture, NULL, &tiles_entities[i].dest);
        
        int deg = 0;

        if (player->direction == 'd') deg = 90;
        else if (player->direction == 'l') deg = 180;
        else if (player->direction == 'u') deg = 270;

        SDL_RenderCopyEx(rend, player->texture, NULL, &player->dest, deg, NULL, SDL_FLIP_VERTICAL);
        for (int i = 0; i < 4; i++) SDL_RenderCopy(rend, ghosts[i].texture, NULL, &ghosts[i].dest);


        render_menu(rend, win, menu, game);

        SDL_RenderPresent(rend);

        frame_time = SDL_GetTicks() - frame_start;
        int timer_value = floor((10000 - (SDL_GetTicks() - game->powerup_timer_start)) / 1000) + 1;
        if (game->powerup) game->powerup_timer = timer_value;

        if (timer_value == 10 && game->powerup) {
            for (int i = 0; i < 4; i++)
            {   
                if (!ghosts[i].ghost_disoriented) init_entity(rend, win, &ghosts[i], "resources/ghost_vulnerable.png");
            }
        } 


        if (!player_collision_happened) {
        animation_frame += 1;

        // animate pacman each 8 frames
        if (animation_frame % PLAYER_ANIMATION_INTERVAL == 0) {
            char img_path[25];
            sprintf(img_path, "resources/player%d.png", animation_frame / PLAYER_ANIMATION_INTERVAL);

            init_entity(rend, win, player, img_path);
        }

        // cycle animation
        if( animation_frame / PLAYER_ANIMATION_INTERVAL >= PLAYER_ANIMATION_FRAMES ) {
            animation_frame = 0;
        }

        }

        // animate pacman dying
        if (game->lost && !*close_requested) {
            int initial_dest_x = player->dest.x;
            int initial_dest_y = player->dest.y;

            Mix_HaltChannel(-1);
            Mix_PauseMusic();
            SDL_Delay(750);
            Mix_PlayChannel( -1, music->gDeath, 0 );
            for (int i = 0; i < 10; i++)
            {
                if (i % 2 == 0) {
                    player->dest.x = -TILE_WIDTH;
                    player->dest.y = -TILE_HEIGHT;
                } else {
                    player->dest.x = initial_dest_x;
                    player->dest.y = initial_dest_y;
                }

                SDL_RenderClear(rend);
                for (int i = 0; i < tiles_entities_size; i++) SDL_RenderCopy(rend, tiles_entities[i].texture, NULL, &tiles_entities[i].dest);
                SDL_RenderCopyEx(rend, player->texture, NULL, &player->dest, deg, NULL, SDL_FLIP_VERTICAL);
                for (int i = 0; i < 4; i++) SDL_RenderCopy(rend, ghosts[i].texture, NULL, &ghosts[i].dest);
                render_menu(rend, win, menu, game);

                SDL_RenderPresent(rend);
                SDL_Delay(115);
            }

            SDL_Delay(1000);
        }

        // stabilize fps
        if (1000/FPS > frame_time) {
            SDL_Delay(1000/FPS  - frame_time);
        }

        
            }
            // fire music when player gets all points and reset map
            if (!game->lost) {
                Mix_HaltChannel(-1);
                Mix_PauseMusic();
                SDL_Delay(1000);
                Mix_PlayChannel( -1, music->gWin, 0 );
                SDL_Delay(2000);
                };

            if (!*close_requested && !game->lost) SDL_Delay(500);

            // if there are points left then player lost
            if (game->map_points > 0) game->lives -= 1;

            // if no more lives then redirect to end screen
            if (game->lives == 0) menu->is_end_menu = true;
        }
}


