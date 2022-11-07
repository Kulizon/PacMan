#ifndef MAP_H
#define MAP_H

void init_map(SDL_Renderer *rend, SDL_Window *win, struct Entity tiles_entities[], struct Game *game );
void clean_up_map(struct Entity tiles_entities[]);
int check_map_collisions(struct Entity tiles_entities[], struct Entity *entity, struct Game *game, struct Music *music, bool is_next);


#endif