#include "coomon.h"
#include "window.h"
#include "blocks.h"
#include "bolt.h"

void kep_rajzol(SDL_Renderer &renderer, SDL_Texture &kep, SDL_Rect scr_in, SDL_Rect dest_in) {        //Fõment kirajzolja, mint háttér
        SDL_Rect src = scr_in;
        SDL_Rect dest = dest_in;
        SDL_RenderCopy(&renderer, &kep, &src, &dest);
}

void kep_rajzol_ex(SDL_Renderer &renderer, SDL_Texture &kep, SDL_Rect scr_in, SDL_Rect dest_in, Way way) {        //Fõment kirajzolja, mint háttér
        SDL_Rect src = scr_in;
        SDL_Rect dest = dest_in;
        SDL_RenderCopyEx(&renderer, &kep, &src, &dest,way_to_irany(way),0,SDL_FLIP_NONE);
        //std::cout<<"barack";
}

int way_to_irany(Way way){
    if (way==right)
        return 0;
    else if(way==down)
        return 90;
    else if(way==left)
        return 180;
    else if(way==up)
        return 270;
    else
        return 0;
}

/*int type_to_block(Block_id type){
    if (type==tilt)

}*/

void playground_draw(SDL_Renderer &renderer, SDL_Texture* blocks_tex[12], PGData data, std::vector<std::vector<Block> > playground, Eger_state eger){
    SDL_RenderClear( &renderer );

    SDL_Rect src={0,0,160,480}, dest{0,0,160,480};
    //kep_rajzol(renderer,*blocks_tex[11],src,dest);
    for (int j=0; j<data.zoom; j++){
        for (int i=0;i<data.zoom; i++){
            //dest={161+60*8/data.zoom*i,1+60*8/data.zoom*j,59*8.0/data.zoom,59*8.0/data.zoom };
            dest={60*8/data.zoom*i,1+60*8/data.zoom*j,59*8/data.zoom,59*8/data.zoom };
            //SDL_Texture *vmi=blocks_tex[playground[data.upst+i][data.leftst+j].type-5];
            //std::cout<<playground[data.upst+i][data.leftst+j].type;
            if (isCointype(playground[data.upst+i][data.leftst+j].type))
                kep_rajzol_ex(renderer,*blocks_tex[6],src,dest,right);
            kep_rajzol_ex(renderer,*blocks_tex[playground[data.upst+i][data.leftst+j].type],src,dest,playground[data.upst+i][data.leftst+j].way);
        }
    }
    if (data.status==pause){
        if (data.now_id!=tilt){
            //blocks_tex[data.now_id]
            SDL_Rect src={0,0,59,59}, dest={eger.x-15/data.zoom*8,eger.y-15/data.zoom*8,2*15/data.zoom*8,2*15/data.zoom*8};
            kep_rajzol_ex(renderer,*blocks_tex[data.now_id],src,dest,data.now_way);
            SDL_RenderPresent( &renderer );
        }
    }
    SDL_RenderPresent( &renderer );
}

void jatek_kiir(PGData data){
    system("cls");

    std::cout<<"Jatek folyamatban..."<<std::endl<<
    "Penz: "<<data.money<<"$";

}

void pause_kiir(PGData data){
    system("cls");
    std::cout<<"Jelenlegi irany: ";
    if (data.now_way==down)
        std::cout<<"lefele";
    else if (data.now_way==left)
        std::cout<<"balra";
    else if (data.now_way==up)
        std::cout<<"felfele";
    else
        std::cout<<"jobbra";
    std::cout<<std::endl<<"Aktualis blokk: ";
    if (data.now_id==generator)
        std::cout<<"generator";
    else if (data.now_id==push)
        std::cout<<"push blokk";
    else if (data.now_id==bedrock)
        std::cout<<"bedrock";
    else if (data.now_id==merger)
        std::cout<<"merger";
    else if (data.now_id==collecter)
        std::cout<<"collecter";
    else if (data.now_id==duplicator)
        std::cout<<"duplicator";
    else
        std::cout<<"semmi";
    std::cout<<std::endl;
    for (int i=0;i<6;i++){
        if (i==data.now_id)
            std::cout<<"["<<data.block_db[i]<<"] ";
        else
            std::cout<<"("<<data.block_db[i]<<") ";
    }
    std::cout<<std::endl<<"Penz: "<<data.money<<"$";

    std::cout<<std::endl<<std::endl<<"Jatek folytatasahoz nyomd meg a SPACE-t."<<std::endl<<
    "Bolt megnyitasahoz nyomd meg a 'b'-t."<<std::endl<<
    "Jatekbol kilepeshez nyomd meg barmikor az 'x'-et.";
}

void event_kezel(SDL_Event ev, bool &stop, SDL_Renderer &renderer, Eger_state &eger, PGData &data, std::vector<std::vector<Block> > &playground, SDL_Texture* blocks_tex[12]){

    if (ev.button.button==SDL_BUTTON_LEFT && ev.type==SDL_MOUSEBUTTONDOWN){
        if (data.status==pause){
            if (!block_pickup(ev.motion.x,ev.motion.y,playground,data))
                block_place(ev.motion.x,ev.motion.y,playground,data);
            pause_kiir(data);
            playground_draw(renderer,blocks_tex,data,playground,eger);
        }

        if (isCoin(ev.motion.x,ev.motion.y,playground,data)){
            Coin_pickup(ev.motion.x,ev.motion.y,playground,data);
            playground_draw(renderer,blocks_tex,data,playground,eger);
            if (data.status==pause) pause_kiir(data);
            else if (data.status==play) jatek_kiir(data);
            else bolt_kiir(data);
        }


    }

    else if (ev.button.button==SDL_BUTTON_RIGHT && ev.type==SDL_MOUSEBUTTONDOWN){
        block_rotate(ev.motion.x,ev.motion.y,playground,data);
        if (data.status==pause)
            pause_kiir(data);
        playground_draw(renderer,blocks_tex,data,playground,eger);
    }

    else if (ev.type==SDL_MOUSEWHEEL){
        if(ev.wheel.y > 0){
            if (data.zoom>8)
                data.zoom-=1;
        }
        else if(ev.wheel.y < 0){
            if (data.zoom<data.mapsize){
                if (data.upst+data.zoom<data.mapsize && data.leftst+data.zoom<data.mapsize)
                    data.zoom+=1;
                else {
                    if (data.upst+data.zoom==data.mapsize) data.leftst-=1;
                    if (data.leftst+data.zoom==data.mapsize) data.upst-=1;
                    data.zoom+=1;
                }
            }
        }
        playground_draw(renderer,blocks_tex,data,playground,eger);
    }
    else if (ev.type==SDL_MOUSEMOTION){
        eger.x=ev.motion.x;
        eger.y=ev.motion.y;
        if (data.status==pause){
            playground_draw(renderer,blocks_tex,data,playground,eger);
            /*if (data.now_id!=tilt){
                //blocks_tex[data.now_id]
                std::cout<<data.now_id;
                SDL_Rect src={0,0,59,59}, dest={eger.x-15/data.zoom*8,eger.y-15/data.zoom*8,2*15/data.zoom*8,2*15/data.zoom*8};
                kep_rajzol_ex(renderer,*blocks_tex[data.now_id],src,dest,data.now_way);
                SDL_RenderPresent( &renderer );
            }
            */
        }
    }

    else if (ev.key.type==SDL_KEYDOWN){
        if (ev.key.keysym.sym==SDLK_SPACE){
            if (data.status==play) {
                data.status=pause;
                pause_kiir(data);
            }
            else if (data.status==bolt){
                data.status=pause;
                pause_kiir(data);
            }
            else
                data.status=play;
        }
        else if (ev.key.keysym.sym==SDLK_a){
            if (data.upst>0)
                data.upst-=1;
        }
        else if (ev.key.keysym.sym==SDLK_w){
            if (data.leftst>0)
                data.leftst-=1;
        }
        else if (ev.key.keysym.sym==SDLK_d){
            if (data.upst+data.zoom<data.mapsize)
                data.upst+=1;
        }
        else if (ev.key.keysym.sym==SDLK_s){
            if (data.leftst+data.zoom<data.mapsize)
                data.leftst+=1;
        }
        else if (ev.key.keysym.sym==SDLK_x){
            stop=true;
        }
        else if (ev.key.keysym.sym==SDLK_b){
            if (data.status!=bolt){
                bolt_kiir(data);
                data.status=bolt;
            }
            else {
                data.status=pause;
                pause_kiir(data);
            }
        }
        else if (ev.key.keysym.sym==SDLK_r){
            block_rotate(eger.x,eger.y,playground,data);
            if (data.status==pause) pause_kiir(data);
        }
        else if (ev.key.keysym.sym==SDLK_0 ){
            if (data.status==bolt)
                bolt_vesz(data,0,playground);
            else if (data.status==pause){
                data.now_id=generator;
                pause_kiir(data);
            }
        }
        else if (ev.key.keysym.sym==SDLK_1 ){
            if (data.status==bolt)
                bolt_vesz(data,1,playground);
            else if (data.status==pause){
                data.now_id=push;
                pause_kiir(data);
            }
        }
        else if (ev.key.keysym.sym==SDLK_2 ){
            if (data.status==bolt)
                bolt_vesz(data,2,playground);
            else if (data.status==pause){
                data.now_id=bedrock;
                pause_kiir(data);
            }
        }
        else if (ev.key.keysym.sym==SDLK_3 ){
            if (data.status==bolt)
                bolt_vesz(data,3,playground);
            else if (data.status==pause){
                data.now_id=merger;
                pause_kiir(data);
            }
        }
        else if (ev.key.keysym.sym==SDLK_4 ){
            if (data.status==bolt)
                bolt_vesz(data,4,playground);
            else if (data.status==pause){
                data.now_id=collecter;
                pause_kiir(data);
            }
        }
        else if (ev.key.keysym.sym==SDLK_5 ){
            if (data.status==bolt)
                bolt_vesz(data,5,playground);
            else if (data.status==pause){
                data.now_id=duplicator;
                pause_kiir(data);
            }
        }
        else if (ev.key.keysym.sym==SDLK_6 ){
            if (data.status==bolt)
                bolt_vesz(data,6,playground);
            else if (data.status==pause){
                data.now_id=tilt;
                pause_kiir(data);
            }
        }
        playground_draw(renderer,blocks_tex,data,playground,eger);
    }


}

bool isDolgos(Block_id type){
    if (type==generator || type==push)
        return true;
    if (type==merger || type==collecter || type==duplicator)
        return true;
    return false;
}

int hany_dolgos_blokk(std::vector<std::vector<Block> > &playground, PGData &data){
    int ret=0;
    for (int i=0; i<data.mapsize; i++){
        for (int j=0; j<data.mapsize; j++){
            if (isDolgos(playground[i][j].type))
                ret+=1;
        }
    }
    return ret;
}

void jatek_tick(SDL_Renderer &renderer, PGData &data, std::vector<std::vector<Block> > &playground, SDL_Texture* blocks_tex[12], Eger_state eger){
    if (data.status==play){
        blocks_reset_moved_tf(playground,data);
        //jatek_kiir(data);
        int dolgosok=hany_dolgos_blokk(playground,data);
        int mar_lepett=0, regen_lepett=0;
        for (int r=0; r<dolgosok; r++){
            for (int i=0; i<data.mapsize; i++){
                for (int j=0; j<data.mapsize; j++){
                    if (isDolgos(playground[i][j].type) && playground[i][j].moved_tf==false){
                        if (block_dolgoz(i,j,playground,data)){
                            mar_lepett+=1;
                            playground[i][j].moved_tf=true;
                        }
                    }
                }
            }
            if (mar_lepett==regen_lepett)
                break;
            regen_lepett=mar_lepett;
        }
        jatek_kiir(data);
        playground_draw(renderer,blocks_tex,data,playground,eger);
    }
}
