#include "coomon.h"
#include "palya.h"

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

void megjelenites(SDL_Renderer &renderer, SDL_Window &window, Palya &palya, int step_cnt, float step_cntf = -1.0f){
    SDL_SetRenderDrawColor(&renderer,100,0,0,255);
    SDL_RenderClear(&renderer);
    /*
    for (int i=0; i<PGX; i++){
        for (int j=0; j<PGY; j++){
            boxRGBA(&renderer,SZELES/PGX*i,MAGAS/PGY*j,SZELES/PGX*i+SZELES/PGX,MAGAS/PGY*j+MAGAS/PGY,155-(i%2)*20,155-(j%2)*20,155-30,255);
        }
    }
    */
    //cout<<"pos size: "<<palya.jarokelok[0].pozicio.size()<<endl;

    if (step_cntf<0){
        for (size_t i=0; i<palya.jarokelok.size(); i++){
            //cout<<palya.jarokelok[i].r<<" "<<palya.jarokelok[i].g<<" "<<palya.jarokelok[i].b<<endl;
            //cout<<palya.jarokelok[i].pozicio[0][0]<<" "<<palya.jarokelok[i].pozicio[0][1]<<endl;

            filledCircleRGBA(&renderer,palya.jarokelok[i].pozicio[step_cnt][0]*SZELES/PGX+SZELES/PGX/2,palya.jarokelok[i].pozicio[step_cnt][1]*MAGAS/PGY+MAGAS/PGY/2,
                             SZELES/PGX/3,palya.jarokelok[i].r,palya.jarokelok[i].g,palya.jarokelok[i].b,255);

        }
    } else {
        float c,f;
        //cout<<"valami okosat pls: "<<ceil(step_cntf)<<" "<<floor(step_cntf)<<" "<<step_cntf<<endl;
        if (step_cntf!=floor(step_cntf)){
            c = (ceil(step_cntf)-step_cntf);
            f = (step_cntf-floor(step_cntf));
            //cout<<c<<" "<<f<<endl;
            //cout<<"valami okosat pls: "<<ceil(step_cntf)<<" "<<floor(step_cntf)<<" "<<step_cntf<<endl;
        }

        for (size_t i=0; i<palya.jarokelok.size(); i++){
            float x, y;
            if (step_cntf==floor(step_cntf) || palya.jarokelok[i].pozicio[ceil(step_cntf)][0]==-1){
                x = palya.jarokelok[i].pozicio[(int)step_cntf][0]*SZELES/PGX+SZELES/PGX/2;
                y = palya.jarokelok[i].pozicio[(int)step_cntf][1]*MAGAS/PGY+MAGAS/PGY/2;
            } else {
                x = f * (palya.jarokelok[i].pozicio[ceil(step_cntf)][0]-0.5f)*SZELES/PGX+SZELES/PGX/2 + c * (palya.jarokelok[i].pozicio[floor(step_cntf)][0]-0.5f)*SZELES/PGX+SZELES/PGX/2;
                y = f * (palya.jarokelok[i].pozicio[ceil(step_cntf)][1]-0.5f)*MAGAS/PGY+MAGAS/PGY/2 + c * (palya.jarokelok[i].pozicio[floor(step_cntf)][1]-0.5f)*MAGAS/PGY+MAGAS/PGY/2;
            }
            filledCircleRGBA(&renderer,
                             x,//palya.jarokelok[i].pozicio[step_cnt][0]*SZELES/PGX+SZELES/PGX/2,
                             y,//palya.jarokelok[i].pozicio[step_cnt][1]*MAGAS/PGY+MAGAS/PGY/2,
                             SZELES/PGX/3,
                             palya.jarokelok[i].r,
                             palya.jarokelok[i].g,
                             palya.jarokelok[i].b,
                             255);
        }
    }

    /*
    for (int i=0; i<PGX; i++){
        for (int j=0; j<PGY; j++){
            for (int k=0; k<9; k++){
                if (palya.mezok[step_cnt][i][j].iranyok[k/3][k%3])
                    filledCircleRGBA(&renderer,SZELES/PGX/2+i*(SZELES/PGX)+(k/3-1)*SZELES/PGX/3,MAGAS/PGY/2+j*(MAGAS/PGY)+(k%3-1)*MAGAS/PGY/3,PGX/5,255,0,0,255);
            }
        }
    }
    */
    SDL_RenderPresent( &renderer );

}

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

    Palya palya;  // Időigényes konstruktor (editor esetén kellene hozzá valami paraméter)
                // kellene bele különböző falak
                // ajtók, amik csukódnak
                // és ezt mind egy editorban kéne össze gyűjteni

    SDL_Event ev;
    cout<<"tick "<<clock()-t1<<endl;
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
            if (ev.type==SDL_KEYUP){
                if (ev.key.keysym.sym==SDLK_a){
                    a=false;
                }
            }
            if (ev.type==SDL_KEYDOWN){
                if (ev.key.keysym.sym==SDLK_r){
                    step_cnt=0;
                    cout<<"step_cnt: "<<step_cnt<<endl;
                }

                if (ev.key.keysym.sym==SDLK_s){
                    if(step_cnt+1<STEPS){
                        step_cnt++;
                        cout<<"step_cnt: "<<step_cnt<<endl;
                    }
                }

                if (ev.key.keysym.sym==SDLK_a){
                    a = true;
                    /*
                    if(step_cnt+1<STEPS){
                        step_cnt+=1.f/5.f;
                        cout<<"step_cnt: "<<step_cnt<<endl;
                    }
                    */
                }

                if (ev.key.keysym.sym==SDLK_d){
                    if(step_cnt>0){
                        step_cnt--;
                        cout<<"step_cnt: "<<step_cnt<<endl;
                    }
                }
            }
            if (ev.type == SDL_QUIT)
                exit(3);
        }

        if (frame){
            if (a)
                step_cnt+=1.f/FLUID;
            //clock_t t = clock();
            megjelenites(renderer,window,ref(palya),step_cnt, step_cnt);
            frames++;
            //cout<<clock()-t<<endl;
        }
        if (fps_t+1000<=clock()){
            fps_t=clock();
            cout<<"fps: "<<frames<<endl;
            frames=0;
        }
    }
}
