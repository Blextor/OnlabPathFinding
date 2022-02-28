#include "blocks.h"
#include "window.h"

bool isBlockhave(Block_id type, int block_db[6]){
    if (type!=tilt)
        return block_db[type]>0;
    return false;
}

int title_pos_x(int x, PGData data){
    //int real_x=x-(SZELES-MAGAS);
    //if (real_x<0) return -1;
    //int pos_x=real_x/(MAGAS/data.zoom);
    int pos_x=x/(MAGAS/data.zoom);
    return pos_x+data.upst;
}

int title_pos_y(int y, PGData data){
    int pos_y=y/(MAGAS/data.zoom);
    return pos_y+data.leftst;
}

bool isPosfree(int x, int y, std::vector<std::vector<Block> > &playground, PGData &data) {
    if (data.status==pause && playground[title_pos_x(x, data)][title_pos_y(y, data)].type==tilt)
            return true;
    return false;
}

bool isCoin(int x, int y, std::vector<std::vector<Block> > &playground, PGData &data) {
    if (playground[title_pos_x(x, data)][title_pos_y(y, data)].type>tilt)
        return true;
    return false;
}

int coin_to_money(Block_id type){
    if      (type==bronze)  return 1;
    else if (type==silver)  return 5;
    else if (type==gold)    return 30;
    else if (type==jade)    return 200;
    else return 0;
}

void Coin_pickup(int x, int y, std::vector<std::vector<Block> > &playground, PGData &data){
    data.money+=coin_to_money(playground[title_pos_x(x, data)][title_pos_y(y, data)].type);
    playground[title_pos_x(x, data)][title_pos_y(y, data)].type=tilt;
}

bool block_place(int x, int y, std::vector<std::vector<Block> > &playground, PGData &data){
    if(isPosfree(x,y,playground,data) && !isCoin(x,y,playground,data) && isBlockhave(data.now_id,data.block_db)){
        playground[title_pos_x(x, data)][title_pos_y(y, data)].set_alap(data.now_id,data.now_way);
        data.block_db[data.now_id]-=1;
        //std::cout<<x<<" ah "<<y<<" "<<title_pos_x(x, data)<<" "<<title_pos_y(y, data)<<std::endl;
        //std::cout<<playground[title_pos_x(x, data)][title_pos_y(y, data)].type;
        return true;
    }
    return false;
}

bool block_pickup(int x, int y, std::vector<std::vector<Block> > &playground, PGData &data){
    if(!isPosfree(x,y,playground,data) && !isCoin(x,y,playground,data) /*&& x>=160*/ ){
        data.block_db[playground[title_pos_x(x, data)][title_pos_y(y, data)].type]+=1;
        playground[title_pos_x(x, data)][title_pos_y(y, data)].type=tilt;
        playground[title_pos_x(x, data)][title_pos_y(y, data)].way=right;
        //std::cout<<x<<" eh "<<y<<" "<<title_pos_x(x, data)<<" "<<title_pos_y(y, data)<<std::endl;
        return true;
    }
    return false;
}

void block_rotate(int x, int y, std::vector<std::vector<Block> > &playground, PGData &data){

    if(isPosfree(x,y,playground,data)){
        if (data.now_way==left) data.now_way=up;
        else if (data.now_way==down) data.now_way=left;
        else if (data.now_way==right) data.now_way=down;
        else if (data.now_way==up) data.now_way=right;
    }
    else if (data.status==pause && !isCoin(x,y,playground,data)){
        if (playground[title_pos_x(x, data)][title_pos_y(y, data)].way==nowhere)
            return ;
        if (playground[title_pos_x(x, data)][title_pos_y(y, data)].way==left) playground[title_pos_x(x, data)][title_pos_y(y, data)].way=up;
        else if (playground[title_pos_x(x, data)][title_pos_y(y, data)].way==down) playground[title_pos_x(x, data)][title_pos_y(y, data)].way=left;
        else if (playground[title_pos_x(x, data)][title_pos_y(y, data)].way==right) playground[title_pos_x(x, data)][title_pos_y(y, data)].way=down;
        else if (playground[title_pos_x(x, data)][title_pos_y(y, data)].way==up) playground[title_pos_x(x, data)][title_pos_y(y, data)].way=right;
    }
}

int irany_to_x(Way way){
    if (way==right)
        return 1;
    if (way==left)
        return -1;
    return 0;
}

int irany_to_y(Way way){
    if (way==down)
        return 1;
    if (way==up)
        return -1;
    return 0;
}

bool isCointype(Block_id type){
    if (type==bronze || type==silver || type==gold || type==jade)
        return true;
    return false;
}

bool work_generator(int i, int j, std::vector<std::vector<Block> > &playground, PGData &data){
    int vx=irany_to_x(playground[i][j].way), vy=irany_to_y(playground[i][j].way);

    if (i+vx==data.mapsize || j+vy==data.mapsize || i+vx<0 || j+vy<0)
        return false;

    if (isCointype(playground[i+vx][j+vy].type)){
        int hany=0;
        for (int r=1; playground[i+r*vx][j+r*vy].type!=tilt; r++){
            if (i+(r+1)*vx==data.mapsize || j+(r+1)*vy==data.mapsize || i+(r+1)*vx<0 || j+(r+1)*vy<0)
                return false;
            if (isDolgos(playground[i+r*vx][j+r*vy].type) || playground[i+r*vx][j+r*vy].type==bedrock)
                return false;
            hany+=1;
        }
        //Block_id temp;
        for (int r=hany; r>0; r--)
            playground[i+(r+1)*vx][j+(r+1)*vy].type=playground[i+r*vx][j+r*vy].type;
        playground[i+vx][j+vy].type=tilt;

    }

    if (playground[i+vx][j+vy].type==tilt){
        playground[i+vx][j+vy].type=bronze;
        return true;
    }

    return false;
}

bool work_push(int i, int j, std::vector<std::vector<Block> > &playground, PGData &data){
    int vx=irany_to_x(playground[i][j].way), vy=irany_to_y(playground[i][j].way);

    if (i+vx==data.mapsize || j+vy==data.mapsize || i+vx<0 || j+vy<0)
        return false;

    if (isCointype(playground[i+vx][j+vy].type)){
        int hany=0;
        for (int r=1; playground[i+r*vx][j+r*vy].type!=tilt; r++){
            if (i+(r+1)*vx==data.mapsize || j+(r+1)*vy==data.mapsize || i+(r+1)*vx<0 || j+(r+1)*vy<0)
                return false;
            if (isDolgos(playground[i+r*vx][j+r*vy].type) || playground[i+r*vx][j+r*vy].type==bedrock)
                return false;
            hany+=1;
        }
        //Block_id temp;
        for (int r=hany; r>0; r--)
            playground[i+(r+1)*vx][j+(r+1)*vy].type=playground[i+r*vx][j+r*vy].type;
        playground[i+vx][j+vy].type=tilt;
        return true;
    }

    return false;
}

bool work_merger(int i, int j, std::vector<std::vector<Block> > &playground, PGData &data){
    int vx=irany_to_x(playground[i][j].way), vy=irany_to_y(playground[i][j].way);

    if (i+1==data.mapsize || j+1==data.mapsize || i-1<0 || j-1<0)
        return false;

    if (isCointype(playground[i-vx][j-vy].type) && isCointype(playground[i+vy][j-vx].type) && isCointype(playground[i-vy][j+vx].type) && playground[i+vx][j+vy].type==tilt){
        if (playground[i-vx][j-vy].type==playground[i+vy][j-vx].type && playground[i+vy][j-vx].type==playground[i-vy][j+vx].type && playground[i-vx][j-vx].type!=jade){
            if (playground[i-vx][j-vy].type==bronze) playground[i+vx][j+vy].type=silver;
            else if (playground[i-vx][j-vy].type==silver) playground[i+vx][j+vy].type=gold;
            else if (playground[i-vx][j-vy].type==gold) playground[i+vx][j+vy].type=jade;
            playground[i-vx][j-vy].type=tilt; playground[i+vy][j-vx].type=tilt; playground[i-vy][j+vx].type=tilt;
            return true;
        }
    }

    return false;
}

bool work_collecter(int i, int j, std::vector<std::vector<Block> > &playground, PGData &data){
    int vx=irany_to_x(playground[i][j].way), vy=irany_to_y(playground[i][j].way);

    if (i-vx==data.mapsize || j-vy==data.mapsize || i-vx<0 || j-vy<0)
        return false;

    if (isCointype(playground[i-vx][j-vy].type)){
        data.money+=coin_to_money(playground[i-vx][j-vy].type);
        playground[i-vx][j-vy].type=tilt;
        //return true;
    }

    return false;
}

bool work_duplicator(int i, int j, std::vector<std::vector<Block> > &playground, PGData &data){
    int vx=irany_to_x(playground[i][j].way), vy=irany_to_y(playground[i][j].way);

    if (i+vx==data.mapsize || j+vy==data.mapsize || i+vx<0 || j+vy<0 || i-vx==data.mapsize || j-vy==data.mapsize || i-vx<0 || j-vy<0)
        return false;

    if (isCointype(playground[i+vx][j+vy].type) && playground[i-vx][j-vy].type==tilt){
        playground[i-vx][j-vy].type=playground[i+vx][j+vy].type;
        return true;
    }
    return false;
}


bool block_dolgoz(int i, int j, std::vector<std::vector<Block> > &playground, PGData &data){
    if (playground[i][j].type==generator) return work_generator(i,j,playground,data);
    else if (playground[i][j].type==push) return work_push(i,j,playground,data);
    else if (playground[i][j].type==merger) return work_merger(i,j,playground,data);
    else if (playground[i][j].type==collecter) return work_collecter(i,j,playground,data);
    else if (playground[i][j].type==duplicator) return work_duplicator(i,j,playground,data);
    return false;
}

void blocks_reset_moved_tf(std::vector<std::vector<Block> > &playground, PGData &data){
    for (int i=0; i<data.mapsize; i++){
        for (int j=0; j<data.mapsize; j++){
            playground[i][j].moved_tf=false;
        }
    }
}

void playground_bovit(PGData &data, std::vector<std::vector<Block> > &playground){
    Block a; data.mapsize+=1;
    for (int i=0; i<data.mapsize-1;i++) playground[i].push_back(a);
    std::vector<Block>  v;
    for (int i=0; i<data.mapsize; i++) v.push_back(a);
    playground.push_back(v);
}



