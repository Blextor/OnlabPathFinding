#include "coomon.h"
//#include "palya.h"
#include "menu.h"

#include <list>
#include <mutex>
#include <set>

using namespace std;

#define FOLYAMATOS true
#define DEBUG_VIZ false
#define ATLOS true

//void pr(std::string str){if (DEBUG) std::cout<<str<<std::endl;}

mutex draw;


///Palya palya;  // Időigényes konstruktor helyére került DELETE



//cout<<"tick"<<endl;


void jatek( SDL_Window &window, SDL_Renderer &renderer){

    //cout<<straight_matrix_M15_2[0][0]<<" "<<true<<endl;
    //thread t;
    //thread frame(megjelenites,ref(renderer),ref(mezok),ref(window));

    clock_t t1 = clock();
    float delay = 65.f;
    bool stop = false;
    srand(100);
    bool a = false;

    //Palya palya;  // Időigényes konstruktor (editor esetén kellene hozzá valami paraméter)
                // kellene bele különböző falak
                // ajtók, amik csukódnak
                // és ezt mind egy editorban kéne össze gyűjteni
    cout<<"alma"<<endl;
    Menu menu;
    cout<<"balma"<<endl;
    SDL_Event ev;
    cout<<"tick "<<clock()-t1<<endl;
    float step_cnt=0;
    int frames = 0;
    clock_t fps_t = clock();
    cout<<"calma"<<endl;
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
            //megjelenites(renderer,window,ref(palya),step_cnt, step_cnt);
            menu.draw(renderer,window);
            frames++;
            //cout<<"AAA"<<clock()-t<<endl;
        }
        if (fps_t+1000<=clock()){
            fps_t=clock();
            cout<<"fps: "<<frames<<endl;
            frames=0;
        }
    }
}
