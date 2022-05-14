#include "coomon.h"
#include "menu.h"

#include <list>
#include <mutex>
#include <set>

using namespace std;

#define FOLYAMATOS true
#define DEBUG_VIZ false
#define ATLOS true

void jatek( SDL_Window &window, SDL_Renderer &renderer){

    clock_t t1 = clock();
    float delay = 65.f;
    bool stop = false;
    srand(100);
    bool a = false;

    //Palya palya;  // Időigényes konstruktor (editor esetén kellene hozzá valami paraméter)
                // kellene bele különböző falak
                // ajtók, amik csukódnak
                // és ezt mind egy editorban kéne össze gyűjteni
    Menu menu;
    SDL_Event ev;
    float step_cnt=0;
    int frames = 0;
    clock_t fps_t = clock();
    while(!stop){
        bool frame=false;
        if (clock()>t1+CLOCKS_PER_SEC/delay){
            t1=clock();
            frame=true;
        } else {
            Sleep(1);
        }
        if (SDL_PollEvent(&ev)){
            menu.event(ev);
        }

        if (frame){
            menu.frame();
            clock_t t = clock();
            menu.draw(renderer,window);
            frames++;
        }
        if (fps_t+1000<=clock()){
            fps_t=clock();
            //system("cls");
            cout<<"fps: "<<frames<<endl;
            frames=0;
        }
    }
}
