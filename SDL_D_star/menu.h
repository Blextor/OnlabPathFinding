#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "coomon.h"
#include "palya.h"

class Menu{
    enum Mstate{
        choose, changeSize, editor, simulate
    };

    Palya palya = Palya(false);

    Mstate state;

    float step_cntf = 0;
    int step_cnt = 0;

    vector<string> modok;

    // TODO szépítés
    bool a = false;
    bool b = false;
    bool c = false;
    Pos mouse = Pos(0,0);

    void drawChangeSize(SDL_Renderer &renderer, SDL_Window &window);
    void drawEditor(SDL_Renderer &renderer, SDL_Window &window);
    void drawSimulate(SDL_Renderer &renderer, SDL_Window &window);


    // kód duplikáció, refact
    void eventchoose(SDL_Event &ev);
    void eventchangeSize(SDL_Event &ev);
    void eventeditor(SDL_Event &ev);
    void eventsimulate(SDL_Event &ev);

    void drawchoose(SDL_Renderer &renderer, SDL_Window &window);
    void drawchangeSize(SDL_Renderer &renderer, SDL_Window &window);
    void draweditor(SDL_Renderer &renderer, SDL_Window &window);
    void drawsimulate(SDL_Renderer &renderer, SDL_Window &window);

    void framechoose();
    void framechangeSize();
    void frameeditor();
    void framesimulate();

    void add15x15Fal();

public:

    Menu();

    void draw(SDL_Renderer &renderer, SDL_Window &window);

    void event(SDL_Event &ev);
    void frame();
};


#endif // MENU_H_INCLUDED
