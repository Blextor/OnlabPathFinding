#ifndef BLOCKS_H_INCLUDED
#define BLOCKS_H_INCLUDED

#include "coomon.h"

#define TITLE "image/title_60x60.png"
#define GEN "image/gen_60x60.png"
#define PUSH "image/push_60x60.png"
#define BED "image/bed_60x60.png"
#define MERGE "image/merge_60x60.png"
#define COLLECT "image/collect_60x60.png"
//#define DUPLICATE "image/duplicator_60x60.png"
#define TOOLS "image/tools_160x480.png"
#define BRONZE "image/bronze_coin.png"
#define SILVER "image/silver_coin.png"
#define GOLD "image/gold_coin.png"
#define JADE "image/jade_coin.png"


class Block {
public:
    Block_id type;
    Way way;
    bool moved_tf;
    //int pos_x, pos_y;
//public:
    Block(){type=tilt; way=right; moved_tf=false;}
    Block(Block_id typ, Way wy){type=typ; way=wy;moved_tf=false;}
    //Block(Block_id typ, Way wy, int posx, int posy){type=typ; way=wy; pos_x=posx; pos_y=posy;}

    void kiir(){std::cout<<type<<" Alma "<<way;}

    void set_alap(Block_id id, Way wy) {type=id; way=wy;moved_tf=false;}
    //void set_evrth(Block_id id, Way wy, int x, int y) {type=id; way=wy; pos_x=x; pos_y=y;}
};



bool isBlockhave(Block_id type, int block_db[6]);

int title_pos_x(int x, PGData data);

int title_pos_y(int y, PGData data);

bool isPosfree(int x, int y, std::vector<std::vector<Block> > &playground, PGData &data);

bool isCoin(int x, int y, std::vector<std::vector<Block> > &playground, PGData &data);

bool isCointype(Block_id type);

void Coin_pickup(int x, int y, std::vector<std::vector<Block> > &playground, PGData &data);

bool block_place(int x, int y, std::vector<std::vector<Block> > &playground, PGData &data);

bool block_pickup(int x, int y, std::vector<std::vector<Block> > &playground, PGData &data);

void block_rotate(int x, int y, std::vector<std::vector<Block> > &playground, PGData &data);

bool block_dolgoz(int i, int j, std::vector<std::vector<Block> > &playground, PGData &data);

void blocks_reset_moved_tf(std::vector<std::vector<Block> > &playground, PGData &data);

void playground_bovit(PGData &data, std::vector<std::vector<Block> > &playground);

#endif // BLOCKS_H_INCLUDED
