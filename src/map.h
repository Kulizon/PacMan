#ifndef MAP_H
#define MAP_H

void init_map(SDL_Renderer *rend, SDL_Window *win, struct Entity tiles_entities[], struct Game *game );
int check_map_collisions(struct Entity tiles_entities[], struct Entity *entity, struct Game *game, struct Music *music, bool is_next);


#endif