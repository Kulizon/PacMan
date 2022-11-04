#ifndef GAME_H
#define GAME_H

void main_game_loop(SDL_Renderer *rend, SDL_Window *win, struct Game *game, struct Menu *menu, struct Music *music, struct Entity *player, struct Entity ghosts[], struct Entity tiles_entities[], int *close_requested, int tiles_entities_size);
void init_game(struct Game *game);

#endif