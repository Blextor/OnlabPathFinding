#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "coomon.h"
#include "palya.h"

class Menu{
    enum Mstate{
        changeSize, editor, simulate
    };

    Palya palya;// = Palya(false);

    Mstate state;

    float step_cntf = 0;
    int step_cnt = 0;


    void drawChangeSize(SDL_Renderer &renderer, SDL_Window &window);
    void drawEditor(SDL_Renderer &renderer, SDL_Window &window);
    void drawSimulate(SDL_Renderer &renderer, SDL_Window &window);

public:

    Menu();

    void draw(SDL_Renderer &renderer, SDL_Window &window);

    event(SDL_Event &ev);
};


#endif // MENU_H_INCLUDED
