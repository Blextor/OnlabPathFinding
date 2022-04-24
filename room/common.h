#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <vector>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <time.h>
#include <math.h>


#include <time.h>
#include <math.h>
#include <thread>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <system_error>
#include <windows.h>
#include <stdlib.h>


#define SZELES 480
#define MAGAS 480
#define PALYASZELES 480.f
#define PALYAMAGAS 480.f


#define STEPS 3000           // idõpillanatok száma
#define FLUID 4.0f          // folyamatos mozgás során milyen bontást alkalmazzon (lehetőleg egészet)
#define PGX 15              // pálya szélessége
#define PGY 15              // és magassága
#define EXTRA_STEPS 0.3f    // pálya magasságához és szélességéhez képest mennyi extra lépést engedünk meg
#define PLY 300            // kezdeti véletlen gyalogosok száma
#define FORGALOM 2
#define FALCNT 30

#define SPEED 1.0f

#define DEBUG true



class Eger_state {
public:
    int x, y;
    int wheel_y;

    Eger_state() {x=0; y=0; wheel_y=0;}

    void set_wheel(int vy) {wheel_y=vy;}
    void set_pos(int vx, int vy) {x=vx; y=vy;}
};


void jatek( SDL_Window &window, SDL_Renderer &renderer);


#endif // COMMON_H_INCLUDED
