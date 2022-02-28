#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include "blocks.h"

void kep_rajzol(SDL_Renderer &renderer, SDL_Texture &kep, SDL_Rect scr_in, SDL_Rect dest_in);

void kep_rajzol_ex(SDL_Renderer &renderer, SDL_Texture &kep, SDL_Rect scr_in, SDL_Rect dest_in, Way way);

int way_to_irany(Way way);

void playground_draw(SDL_Renderer &renderer, SDL_Texture* blocks_tex[12], PGData data, std::vector<std::vector<Block> > playground, Eger_state eger);

void event_kezel(SDL_Event ev, bool &stop, SDL_Renderer &renderer, Eger_state &eger, PGData &data, std::vector<std::vector<Block> > &playground, SDL_Texture* blocks_tex[12]);

bool isDolgos(Block_id type);

void jatek_tick(SDL_Renderer &renderer, PGData &data, std::vector<std::vector<Block> > &playground, SDL_Texture* blocks_tex[12], Eger_state eger);

#endif // WINDOW_H_INCLUDED
