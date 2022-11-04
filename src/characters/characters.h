#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "../../main.h"

bool is_collision(int x_posA, int y_posA, int x_posB, int y_posB, SDL_Rect  destA, SDL_Rect  destB,  bool check_center);
void determine_velocity(struct Entity *entity, char direction, bool is_next, bool is_disoriented, bool is_vulnerable);
void handle_movement(struct Entity tiles_entities[], struct Entity *entity, struct Game *game, struct Music *music);
void set_dest(struct Entity *entity);
void init_character(struct Entity *entity, int initial_pos_x, int initial_pos_y, char entity_type);
void init_ghosts(SDL_Renderer *rend,  SDL_Window *win, struct Entity ghosts[]);
void handle_ghost_logic(struct Entity *ghost, struct Entity tiles_entities[], struct Game *game, struct Music *music);
void check_map_boundaries(struct Entity *entity);

#endif



