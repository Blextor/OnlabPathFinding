#ifndef COOMON_H_INCLUDED
#define COOMON_H_INCLUDED

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

#define PALYASZELES 480
#define PALYAMAGAS 480


#define STEPS 3000           // idõpillanatok száma
#define FLUID 4.0f          // folyamatos mozgás során milyen bontást alkalmazzon (lehetőleg egészet)
#define PGX 15              // pálya szélessége
#define PGY 15              // és magassága
#define EXTRA_STEPS 1000.0f    // pálya magasságához és szélességéhez képest mennyi extra lépést engedünk meg
#define PLY 400            // kezdeti véletlen gyalogosok száma
#define PLYTRY 6
#define FORGALOM 10
#define FALCNT 30

#define DEBUG true


void jatek( SDL_Window &window, SDL_Renderer &renderer);



#endif // COOMON_H_INCLUDED
