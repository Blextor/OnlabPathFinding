#include "menu.h"
#include "palya.h"

// menü állapotai, és hogy mit kell megjeleníteniük
/*
Kell egy pályaméret. X, Y
Kell egy Grid és választható objektumok. Fal, játékos, mozgó fal (ajtó)
Kell egy idõ léptetõs gomb, hogy mikor kerüljön le az a dolog.
A fal az felülbírál mindent.


*/


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

    cout<<palya.falak.size()<<endl;
/*
    for (int i=0; i<palya.falak.size(); i++){
        int x = palya.falak[i].x*SZELES/PGX+SZELES/PGX/2;
        int y = palya.falak[i].y*MAGAS/PGY+MAGAS/PGY/2;
        boxRGBA(&renderer,
                             x,//palya.jarokelok[i].pozicio[step_cnt][0]*SZELES/PGX+SZELES/PGX/2,
                             y,//palya.jarokelok[i].pozicio[step_cnt][1]*MAGAS/PGY+MAGAS/PGY/2,
                             SZELES/PGX/3,
                             SZELES/PGX/3,
                             100,
                             100,
                             100,
                             255);
    }
*/
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


Menu::Menu(){
    state=choose;
    modok.push_back("diskret");
}



void Menu::drawchoose(SDL_Renderer &renderer, SDL_Window &window){
    SDL_SetRenderDrawColor(&renderer,5,131,102,255);
    SDL_RenderClear(&renderer);

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
                                 x-PALYASZELES/palya.Sx/3,//palya.jarokelok[i].pozicio[step_cnt][0]*SZELES/PGX+SZELES/PGX/2,
                                 y-PALYAMAGAS/palya.Sy/3,//palya.jarokelok[i].pozicio[step_cnt][1]*MAGAS/PGY+MAGAS/PGY/2,
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
                                 x,//palya.jarokelok[i].pozicio[step_cnt][0]*SZELES/PGX+SZELES/PGX/2,
                                 y,//palya.jarokelok[i].pozicio[step_cnt][1]*MAGAS/PGY+MAGAS/PGY/2,
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
        //cout<<"valami okosat pls: "<<ceil(step_cntf)<<" "<<floor(step_cntf)<<" "<<step_cntf<<endl;
        if (step_cntf!=floor(step_cntf)){
            c = (ceil(step_cntf)-step_cntf);
            f = (step_cntf-floor(step_cntf));
            //cout<<c<<" "<<f<<endl;
            //cout<<"valami okosat pls: "<<ceil(step_cntf)<<" "<<floor(step_cntf)<<" "<<step_cntf<<endl;
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
                                 x,//palya.jarokelok[i].pozicio[step_cnt][0]*SZELES/PGX+SZELES/PGX/2,
                                 y,//palya.jarokelok[i].pozicio[step_cnt][1]*MAGAS/PGY+MAGAS/PGY/2,
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
    //for (int i=0; i<palya.falak.size(); i++){
        if (!fal.kerek){
            int x = fal.x*PALYASZELES/palya.Sx+PALYASZELES/palya.Sx/2;
            int y = fal.y*PALYAMAGAS/palya.Sy+PALYAMAGAS/palya.Sy/2;
            boxRGBA(&renderer,
                                 x-PALYASZELES/palya.Sx/3,//palya.jarokelok[i].pozicio[step_cnt][0]*SZELES/PGX+SZELES/PGX/2,
                                 y-PALYAMAGAS/palya.Sy/3,//palya.jarokelok[i].pozicio[step_cnt][1]*MAGAS/PGY+MAGAS/PGY/2,
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
                                 x,//palya.jarokelok[i].pozicio[step_cnt][0]*SZELES/PGX+SZELES/PGX/2,
                                 y,//palya.jarokelok[i].pozicio[step_cnt][1]*MAGAS/PGY+MAGAS/PGY/2,
                                 PALYASZELES/palya.Sx/3,
                                 100,
                                 100,
                                 100,
                                 255);
        }
    }

    /*
    for (int i=0; i<palya.Sx; i++){
        for (int j=0; j<palya.Sy; j++){
            for (int k=0; k<9; k++){
                if (palya.mezok[floor(step_cntf)][i][j].iranyok[k/3][k%3])
                    filledCircleRGBA(&renderer,PALYASZELES/palya.Sx/2+i*PALYASZELES/palya.Sx+(k/3-1)*PALYASZELES/palya.Sx/3,PALYAMAGAS/palya.Sy/2+j*PALYAMAGAS/palya.Sy+(k%3-1)*PALYAMAGAS/palya.Sx/3,palya.Sx/5,255,0,0,255);
            }
        }
    }
    //*/
    SDL_RenderPresent( &renderer );
}

void Menu::eventchoose(SDL_Event &ev){
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

void Menu::eventeditor(SDL_Event &ev){
    if (ev.type == SDL_MOUSEBUTTONDOWN){
        if (ev.button.button == 1){
            int xm = ev.button.x, ym = ev.button.y;
            int x = xm*palya.Sx/PALYASZELES, y = ym*palya.Sy/PALYAMAGAS;
            if (!palya.mezok[0][x][y].fal){
                palya.addFal(Fal(x,y,a),true);
            }
        }
        if (ev.button.button == 3){
            int xm = ev.button.x, ym = ev.button.y;
            int x = xm*palya.Sx/PALYASZELES, y = ym*palya.Sy/PALYAMAGAS;
            if (palya.mezok[0][x][y].fal){
                palya.removeFal(Fal(x,y),true);
            }
        }
    }


    if (ev.type == SDL_KEYDOWN){
        if (ev.key.keysym.sym==SDLK_a){
            a=!a;
        }
        if (ev.key.keysym.sym==SDLK_f){
            a=false;
            state = simulate;
            palya.FalakLehelyezese(); // HELYTELEN
            palya.JarokelokLetetele(); // HELYTELEN
        }

    }
}

void Menu::eventsimulate(SDL_Event &ev){

    //cout<<"alma"<<endl;
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
    //cout<<"balma"<<endl;
    if (ev.type == SDL_QUIT)
        exit(3);
}


void Menu::framechoose(){}
void Menu::framechangeSize(){}
void Menu::frameeditor(){}

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

