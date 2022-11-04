#ifndef MAIN_H
#define MAIN_H

#define SPEED (180)
#define VULNREABLE_SPEED (90)
#define DISORIENTED_SPEED (15)

#define TILE_WIDTH (50)
#define TILE_HEIGHT (50)
#define MENU_HEIGHT (100)

#define TILES_NUMBER_X (16)
#define TILES_NUMBER_Y (16)

#define MAP_WIDTH (TILE_WIDTH * TILES_NUMBER_X)
#define MAP_HEIGHT (TILE_HEIGHT * TILES_NUMBER_Y)

#define WINDOW_WIDTH  (TILE_WIDTH * TILES_NUMBER_X)
#define WINDOW_HEIGHT (MENU_HEIGHT + (TILE_HEIGHT * TILES_NUMBER_Y))

#define CHARACTER_SCALE (1.04)

#define FPS (60)    
#define PLAYER_ANIMATION_FRAMES (4)
#define PLAYER_ANIMATION_INTERVAL (4)

struct Game {
    int points;
    int map_points;
    int time;
    bool lost;
    int lives;
    bool powerup;
    SDL_TimerID powerup_timer_id;
    long powerup_timer;
    long powerup_timer_start;
    int  called_timers_ghosts_ids[4];

};

struct Entity {
SDL_Surface* surface;
SDL_Texture* texture;
SDL_Rect dest;
char type; // g - ghost, p - player, w - wall, c - coin, l - lightning (powerup)
float x_pos;
float y_pos;
float x_pos_next;
float y_pos_next;
float x_vel;
float y_vel;
float x_vel_next;
float y_vel_next;
char direction;
char direction_next;
bool ghost_disoriented;
int ghost_id;
};

struct Menu {
    struct Entity powerup_indicator;
    struct Entity powerup_counter;
    struct Entity points_counter;
    struct Entity lives_counter;
    TTF_Font *font_s;
    TTF_Font *font_m;
    TTF_Font *font_l;
    SDL_Color color;
    SDL_Color color_alt;
    bool is_main_menu;
    bool is_end_menu;
    struct Entity main_menu_start_text;
    struct Entity main_menu_text;
    struct Entity main_menu_author_text;
    struct Entity end_menu_points_text;
    struct Entity end_menu_lost_text;
    struct Entity end_menu_restart_text;
};

struct Music {
// music
Mix_Music *gSiren;

//sound effects
Mix_Chunk *gDeath;
Mix_Chunk *gWin;
Mix_Chunk *gMunch;
Mix_Chunk *gPowerup;
Mix_Chunk *gEatGhost;
Mix_Chunk *gBeginning;
};

#endif