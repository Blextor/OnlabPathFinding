#include "bolt.h"

void bolt_kiir(PGData &data){
    system("cls");
    std::cout<<"             Bolt:"<<std::endl<<std::endl<<std::endl<< "Penz: "<<data.money<<"$"<<std::endl<<std::endl<<
    "             Blokkok:"<<std::endl<<std::endl<<
    "(0) generator:  500$, jelenleg van "<<data.block_db[0]<<" db."<<std::endl<<
    "(1)      push:   50$, jelenleg van "<<data.block_db[1]<<" db."<<std::endl<<
    "(2)   bedrock:   10$, jelenleg van "<<data.block_db[2]<<" db."<<std::endl<<
    "(3)    merger:  300$, jelenleg van "<<data.block_db[3]<<" db."<<std::endl<<
    "(4) collecter:  200$, jelenleg van "<<data.block_db[4]<<" db."<<std::endl<<
    "(5)duplicator: 5000$, jelenleg van "<<data.block_db[5]<<" db."<<std::endl<<
    std::endl<<
    "             Palya:"<<std::endl<<
    "(6)Terulet noveles: 500, jelenleg "<<data.mapsize<<"x"<<data.mapsize<< " a merete."<<std::endl<<
    std::endl<<
    "Nyomd meg az egyik szamot a vasarlashoz!"<<std::endl<<
    "Bolt bezarasahoz nyomd meg a 'b'-t."<<std::endl<<
    "Jatek folytatasahoz nyomd meg a SPACE-t.";
}

void bolt_vesz(PGData &data,int szam,std::vector<std::vector<Block> > &playground){

    int price[7]={500,50,10,300,200,5000,500};
    if (data.money>=price[szam]){
        data.money-=price[szam];
        if (szam<6)
            data.block_db[szam]+=1;
        else
            playground_bovit(data,playground);
        bolt_kiir(data);
    }
    else {
        if      (szam==0) std::cout<<"Nincs eleg penzed egy ujabb generatorra."<<std::endl;

        else if (szam==1) std::cout<<"Nincs eleg penzed egy ujabb push blokkra."<<std::endl;

        else if (szam==2) std::cout<<"Nincs eleg penzed egy ujabb bedrock-ra."<<std::endl;

        else if (szam==3) std::cout<<"Nincs eleg penzed egy ujabb merger-re."<<std::endl;

        else if (szam==4) std::cout<<"Nincs eleg penzed egy ujabb collecter-re."<<std::endl;

        else if (szam==5) std::cout<<"Nincs eleg penzed egy ujabb duplicator-ra."<<std::endl;

        else if (szam==6) std::cout<<"Nincs eleg penzed, hogy bovitsed a teruleted."<<std::endl;
    }
}




