#ifndef PALYA_H_INCLUDED
#define PALYA_H_INCLUDED

#include "coomon.h"


#include <list>
#include <mutex>
#include <set>

using namespace std;


void pr(std::string str);

struct UtPos{ // az útvonal egy megállója

    // tervezéshez szükséges adatok
    int x, y, time, becsles, elozoindex; // hol van, mikor van, mennyi az addig megtett út, a becsült összhossza az útnak, és az elõzõ megálló index-e
    int megtettUt;

    // útvonal létrehozásához szükséges adatok
    bool irany[3][3];
    int utazasi_ido=0;

    UtPos(int X, int Y, int T, bool utveg);

    // teljes konstruktor
    UtPos(int X, int Y, int Time, int m, int b, int i=-1);

    // beállítja, hogy milyen irányba, és mennyi ideig fog haladni ebbõl a pontból a járókelõ
    void setIranyAndTime(int vx, int vy, int vtime);
};

inline bool operator<(const UtPos& lhs, const UtPos& rhs);

class Jarokelo{     // egy gyalogos osztálya
public:
    // használaton kívül
    /*
    float irany;
    float sebesseg;
    float fordulas;
    */

    vector<UtPos> utvonal;  // merre halad majd
    int start_time;         // mikor van jelen elõször a világban
    int cel[2];           // hova halad

    // idõpillanatonkénti pozíció
    vector<vector<float>> pozicio;

    // megjelenítéshez
    int r=0, g=0, b=0;

    Jarokelo();

    Jarokelo(float posx, float posy, float celx, float cely, vector<UtPos> utvn, int st_time=0, int sarok=-1);

    void setUtvonal(vector<UtPos> utvn);
};

class Mezo{ // a pálya egyetlen mezõje
public:
    bool iranyok[3][3]; // minden irányról eltárolja, hogy szabad-e az áthaladás, vagy sem

    Mezo(int x=0, int y=0, int Sx=0, int Sy=0); // létrehozza a mezõt, default értékekkel nem túl hasznos

    void reset(int x=0, int y=0, int Sx=0, int Sy=0);

    bool fal = false;
};

int dis(int x, int xv, int y, int yv);

class Fal{
public:
    int x, y;
    int start_time=0, end_time=STEPS;
    bool mozo=false;
    int kesleltetes;
    int xd, yd;

    bool kerek=false;

    Fal(){}

    Fal(int xv, int yv, bool kerekv=false, int start_timev=0){
        x=xv, y=yv, start_time=start_time, kerek=kerekv;
    }

};

struct Pos{
    int x=0, y=0;

    Pos(int a, int b){x=a, y=b;}
};

class Palya{
    public:
    vector<Jarokelo> jarokelok; // eltárolom a járókelõket

    list<Fal> falak;

    vector<vector<vector<Mezo>>> mezok; // eltárolom a STEPS idõpillanatnyi, PGX*PGY mérteû pályát

    vector<vector<int>> hapci;

    vector<vector<vector<int>>> opt_becsult;

    int Sx = PGX, Sy = PGY;

    int step = 0; // pálya melyik idõpillanatát nézem

    // Játékosok generálásához

    vector<Pos> cel_pos;
    int sarok = 0; // sorba megy a 4 sarkon

    int dis(int x, int xv, int y, int yv);

    void calc_opt_becsult();

    void celok_letrehozasa();

    void scanMezokInWay(int x, int y, int time, bool hova[3][3]);

    bool utvonalKereses(vector<UtPos> &utvonal, int x, int y, int celx, int cely, int time=0);

    void FalakLehelyezese();

    void addFal(Fal fal, bool setMezo = false);

    void removeFal(Fal fal, bool setMezo = false);

    void palyakepModositAtlosan(UtPos pos);

    void palyakepModositHelyben(UtPos pos, bool vege=false);

    void palyakepModositKeresztben(UtPos pos);

    void palyakepModosit(UtPos pos);

    void palyakepFrissites(vector<UtPos> utvonal, Jarokelo jarokelo);

    void JarokelokLetetele();

    Palya();

    Palya(bool ures);

    Palya(int sx, int sy, int steps=STEPS);
};


#endif // PALYA_H_INCLUDED
