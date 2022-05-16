#include "menu.h"
#include "palya.h"

// menü állapotai, és hogy mit kell megjeleníteniük


void megjelenites(SDL_Renderer &renderer, SDL_Window &window, Palya &palya, int step_cnt, float step_cntf = -1.0f){
    SDL_SetRenderDrawColor(&renderer,100,0,0,255);
    SDL_RenderClear(&renderer);

    if (step_cntf<0){
        for (size_t i=0; i<palya.jarokelok.size(); i++){
            filledCircleRGBA(&renderer,palya.jarokelok[i].pozicio[step_cnt][0]*SZELES/PGX+SZELES/PGX/2,palya.jarokelok[i].pozicio[step_cnt][1]*MAGAS/PGY+MAGAS/PGY/2,
                             SZELES/PGX/3,palya.jarokelok[i].r,palya.jarokelok[i].g,palya.jarokelok[i].b,255);
        }
    } else {
        float c,f;
        if (step_cntf!=floor(step_cntf)){
            c = (ceil(step_cntf)-step_cntf);
            f = (step_cntf-floor(step_cntf));
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
                             x,
                             y,
                             SZELES/PGX/3,
                             palya.jarokelok[i].r,
                             palya.jarokelok[i].g,
                             palya.jarokelok[i].b,
                             255);
        }
    }

    SDL_RenderPresent( &renderer );

}


Menu::Menu(){
    state=choose;
    modok.push_back("diskret");
}

void Menu::drawchoose(SDL_Renderer &renderer, SDL_Window &window){
    SDL_SetRenderDrawColor(&renderer,5,131,102,255);
    SDL_RenderClear(&renderer);

    // mágikus számok
    int h = 160, w = 160, spacebetween = 40;
    int ra=239, ga=228, ba=176;
    int ri=159, gi=156, bi=132;

    for (int i=0; i<3; i++){
        int x = spacebetween+i*(h+spacebetween);
        int y = 55;
        if (modok.size()>i){
            boxRGBA(&renderer,x,y,x+w,y+h,ra,ga,ba,255);
            stringRGBA(&renderer,x+50,y+50,modok[i].c_str(),0,0,0,255);
        } else {
            boxRGBA(&renderer,x,y,x+w,y+h,ri,gi,bi,255);
        }
    }

    for (int i=0; i<3; i++){
        int x = spacebetween+i*(h+spacebetween);
        int y = 260;
        if (modok.size()>i+3){
            boxRGBA(&renderer,x,y,x+w,y+h,ra,ga,ba,255);
            stringRGBA(&renderer,x,y,modok[i].c_str(),0,0,0,255);
        } else {
            boxRGBA(&renderer,x,y,x+w,y+h,ri,gi,bi,255);
        }
    }

    SDL_RenderPresent( &renderer );
}

void Menu::drawchangeSize(SDL_Renderer &renderer, SDL_Window &window){
    SDL_SetRenderDrawColor(&renderer,68,151,112,255);
    SDL_RenderClear(&renderer);
    SDL_RenderPresent( &renderer );
    cout<<"Add meg a meretet: X, Y, (STEPS): ";
    int a,b,c;
    cin>>a>>b;//>>c;
    cout<<endl;
    palya = Palya(a,b);
    state = editor;

}

void Menu::draweditor(SDL_Renderer &renderer, SDL_Window &window){
    SDL_SetRenderDrawColor(&renderer,168,151,112,255);
    SDL_RenderClear(&renderer);
    std::list<Fal>::iterator it;
    for (it = palya.falak.begin(); it != palya.falak.end(); ++it){
        Fal fal = *it;
        if (!fal.kerek){
            int x = fal.x*PALYASZELES/palya.Sx+PALYASZELES/palya.Sx/2;
            int y = fal.y*PALYAMAGAS/palya.Sy+PALYAMAGAS/palya.Sy/2;
            boxRGBA(&renderer,
                                 x-PALYASZELES/palya.Sx/3,
                                 y-PALYAMAGAS/palya.Sy/3,
                                 x+PALYASZELES/palya.Sx/3,
                                 y+PALYAMAGAS/palya.Sy/3,
                                 100,
                                 100,
                                 100,
                                 255);
        } else {
            int x = fal.x*PALYASZELES/palya.Sx+PALYASZELES/palya.Sx/2;
            int y = fal.y*PALYAMAGAS/palya.Sy+PALYAMAGAS/palya.Sy/2;
            filledCircleRGBA(&renderer,
                                 x,
                                 y,
                                 PALYASZELES/palya.Sx/3,
                                 100,
                                 100,
                                 100,
                                 255);
        }
    }


    SDL_RenderPresent( &renderer );
}

void Menu::drawsimulate(SDL_Renderer &renderer, SDL_Window &window){
    SDL_SetRenderDrawColor(&renderer,100,0,0,255);
    SDL_RenderClear(&renderer);

    if (step_cntf<0){
        for (size_t i=0; i<palya.jarokelok.size(); i++){
            filledCircleRGBA(&renderer,
                             palya.jarokelok[i].pozicio[step_cnt][0]*PALYASZELES/palya.Sx+PALYASZELES/palya.Sx/2,
                             palya.jarokelok[i].pozicio[step_cnt][1]*PALYAMAGAS/palya.Sy+PALYAMAGAS/palya.Sy/2,
                             PALYASZELES/palya.Sx/3,
                             palya.jarokelok[i].r,
                             palya.jarokelok[i].g,
                             palya.jarokelok[i].b,
                             255);

        }
    } else {
        float c,f;
        if (step_cntf!=floor(step_cntf)){
            c = (ceil(step_cntf)-step_cntf);
            f = (step_cntf-floor(step_cntf));
        }
        for (size_t i=0; i<palya.jarokelok.size(); i++){
            if (palya.jarokelok[i].start_time<=step_cntf && palya.jarokelok[i].pozicio[ceil(step_cntf)][0]!=-1){
                float x, y;
                if (step_cntf==floor(step_cntf) || palya.jarokelok[i].pozicio[ceil(step_cntf)][0]==-1){
                    x = palya.jarokelok[i].pozicio[(int)step_cntf][0]*PALYASZELES/palya.Sx+PALYASZELES/palya.Sx/2;
                    y = palya.jarokelok[i].pozicio[(int)step_cntf][1]*PALYAMAGAS/palya.Sy+PALYAMAGAS/palya.Sy/2;
                } else {
                    x = f * (palya.jarokelok[i].pozicio[ceil(step_cntf)][0]-0.5f)*PALYASZELES/palya.Sx+PALYASZELES/palya.Sx/2 +
                            c * (palya.jarokelok[i].pozicio[floor(step_cntf)][0]-0.5f)*PALYASZELES/palya.Sx+PALYASZELES/palya.Sx/2;
                    y = f * (palya.jarokelok[i].pozicio[ceil(step_cntf)][1]-0.5f)*PALYAMAGAS/palya.Sy+PALYAMAGAS/palya.Sy/2 +
                            c * (palya.jarokelok[i].pozicio[floor(step_cntf)][1]-0.5f)*PALYAMAGAS/palya.Sy+PALYAMAGAS/palya.Sy/2;
                }
                filledCircleRGBA(&renderer,
                                 x,
                                 y,
                                 PALYASZELES/palya.Sx/3,
                                 palya.jarokelok[i].r,
                                 palya.jarokelok[i].g,
                                 palya.jarokelok[i].b,
                                 255);
            }
        }
    }
    std::list<Fal>::iterator it;
    for (it = palya.falak.begin(); it != palya.falak.end(); ++it){
        Fal fal = *it;
        if (fal.start_time<=step_cntf && fal.end_time>=step_cntf ){
            //cout<<"fal: "<<fal.start_time<<" "<<fal.end_time<<" "<<step_cntf<<endl;
            if (!fal.kerek){
                int x = fal.x*PALYASZELES/palya.Sx+PALYASZELES/palya.Sx/2;
                int y = fal.y*PALYAMAGAS/palya.Sy+PALYAMAGAS/palya.Sy/2;
                boxRGBA(&renderer,
                                     x-PALYASZELES/palya.Sx/3,
                                     y-PALYAMAGAS/palya.Sy/3,
                                     x+PALYASZELES/palya.Sx/3,
                                     y+PALYAMAGAS/palya.Sy/3,
                                     100,
                                     100,
                                     100,
                                     255);
            } else {
                int x = fal.x*PALYASZELES/palya.Sx+PALYASZELES/palya.Sx/2;
                int y = fal.y*PALYAMAGAS/palya.Sy+PALYAMAGAS/palya.Sy/2;
                filledCircleRGBA(&renderer,
                                     x,
                                     y,
                                     PALYASZELES/palya.Sx/3,
                                     100,
                                     100,
                                     100,
                                     255);
            }
        }
    }
    SDL_RenderPresent( &renderer );
}

void Menu::eventchoose(SDL_Event &ev){
    // mágikus számok
    int h = 160, w = 160, spacebetween = 40;
    int yu = 55;
    int yd = 260;
    if (ev.type==SDL_MOUSEBUTTONDOWN){
        int mx = ev.button.x, my = ev.button.y;
        cout<<mx<<" "<<my<<endl;
        for (int i=0; i<3; i++){
            int x = spacebetween+i*(h+spacebetween);
            if (modok.size()>i){
                if (mx>x && mx<x+w && my>yu && my<yu+h){
                    state=changeSize;
                }
            }
        }
        for (int i=0; i<3; i++){
            int x = spacebetween+i*(h+spacebetween);
            if (modok.size()>i+3){
                if (mx>x && mx<x+w && my>yd && my<yd+h){
                    state=changeSize;
                }
            }
        }
    }
}

void Menu::eventchangeSize(SDL_Event &ev){}


void Menu::add15x15Fal(){
    for (int i=1; i<14; i++){
        for (int j=0; j<8; j++){
            Fal fal(j,i,true);
            for (int k=0; k<10; k++){
                fal.start_time=(i%4)*7 + k * 28;
                fal.end_time = fal.start_time+6;
                palya.addFal(fal,false);
            }
        }
        for (int j=8; j<15; j++){
            Fal fal(j,i,true);
            for (int k=0; k<10; k++){
                fal.start_time=(3-(i%4))*7 + k * 28;
                fal.end_time = fal.start_time+6;
                palya.addFal(fal,false);
            }
        }
    }
}

void Menu::eventeditor(SDL_Event &ev){
    if (ev.type == SDL_MOUSEBUTTONDOWN){
        if (ev.button.button == 1){
            b=true;
            int xm = ev.button.x, ym = ev.button.y;
            int x = xm*palya.Sx/PALYASZELES, y = ym*palya.Sy/PALYAMAGAS;
            if (!palya.mezok[0][x][y].fal){
                palya.addFal(Fal(x,y,a),true);
            }
        }
        if (ev.button.button == 3){
            c=true;
            int xm = ev.button.x, ym = ev.button.y;
            int x = xm*palya.Sx/PALYASZELES, y = ym*palya.Sy/PALYAMAGAS;
            if (palya.mezok[0][x][y].fal){
                palya.removeFal(Fal(x,y),true);
            }
        }
    }
    if (ev.type == SDL_MOUSEBUTTONUP){
        if (ev.button.button == 1)
            b=false;
        if (ev.button.button == 3)
            c=false;
    }

    if (ev.type == SDL_MOUSEMOTION){
        mouse.x = ev.motion.x;
        mouse.y = ev.motion.y;
    }


    if (ev.type == SDL_KEYDOWN){
        if (ev.key.keysym.sym==SDLK_a){
            a=!a;
        }
        if (ev.key.keysym.sym==SDLK_f){
            a=false;
            state = simulate;

            add15x15Fal();
            palya.FalakLehelyezese(); // HELYTELEN
            palya.JarokelokLetetele(); // HELYTELEN
        }

    }
}

void Menu::eventsimulate(SDL_Event &ev){

    if (ev.type==SDL_KEYUP){
        if (ev.key.keysym.sym==SDLK_a){
            a=false;
        }
    }
    if (ev.type==SDL_KEYDOWN){
        if (ev.key.keysym.sym==SDLK_r){
            step_cntf=0;
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
        }

        if (ev.key.keysym.sym==SDLK_s){
            if (step_cntf<STEPS-1-1.f/FLUID)
                step_cntf+=1.f/FLUID;
            cout<<"step_cntf: "<<step_cntf<<endl;
        }

        if (ev.key.keysym.sym==SDLK_d){
            if (step_cntf>0)
                step_cntf-=1.f/FLUID;
            cout<<"step_cntf: "<<step_cntf<<endl;
        }

    }
    if (ev.type == SDL_QUIT)
        exit(3);
}


void Menu::framechoose(){}
void Menu::framechangeSize(){}
void Menu::frameeditor(){
    if (b){
        int xm = mouse.x, ym = mouse.y;
        int x = xm*palya.Sx/PALYASZELES, y = ym*palya.Sy/PALYAMAGAS;
        if (!palya.mezok[0][x][y].fal){
            palya.addFal(Fal(x,y,a),true);
        }
    }
    if (c){
            cout<<"OKI"<<endl;
        int xm = mouse.x, ym = mouse.y;
        int x = xm*palya.Sx/PALYASZELES, y = ym*palya.Sy/PALYAMAGAS;
        if (palya.mezok[0][x][y].fal){
            palya.removeFal(Fal(x,y,a),true);
        }
    }
}

void Menu::framesimulate(){
    //cout<<"true"<<a<<true<<endl;
    if (a){
        if (step_cntf<STEPS-1-1.f/FLUID)
        step_cntf+=1.f/FLUID;
        cout<<"step_cntf: "<<step_cntf<<endl;
    }
}

void Menu::event(SDL_Event &ev){

    if (ev.type == SDL_KEYDOWN){        // HARD RESET
        if (ev.key.keysym.sym==SDLK_p){
            state = choose;
        }
    }

    if (state==choose)
        eventchoose(ev);
    else if (state==changeSize)
        eventchangeSize(ev);
    else if (state==editor)
        eventeditor(ev);
    else if (state==simulate)
        eventsimulate(ev);
}

void Menu::draw(SDL_Renderer &renderer, SDL_Window &window){
    if (state==choose)
        drawchoose(renderer,window);
    else if (state==changeSize)
        drawchangeSize(renderer,window);
    else if (state==editor)
        draweditor(renderer,window);
    else if (state==simulate)
        drawsimulate(renderer,window);
}

void Menu::frame(){
    if (state==choose)
        framechoose();
    else if (state==changeSize)
        framechangeSize();
    else if (state==editor)
        frameeditor();
    else if (state==simulate)
        framesimulate();
}

