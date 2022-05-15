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

    UtPos(int X, int Y, int T);

    // teljes konstruktor
    UtPos(int X, int Y, int Time, int m, int b, int i=-1);

    // beállítja, hogy milyen irányba, és mennyi ideig fog haladni ebbõl a pontból a járókelõ
    void setIranyAndTime(int vx, int vy, int vtime);
};

struct Pos{
    int x=0, y=0;

    Pos(int a, int b){x=a, y=b;}

    void Set(int a, int b){x=a, y=b;}
};

inline bool operator<(const UtPos& lhs, const UtPos& rhs);

inline bool operator<(const Pos& a, const Pos& b);

class Jarokelo{     // egy gyalogos osztálya, puritán
public:
    // lényegében ez egy struct, csak adatokat tárol, nagy számításokat nem végez, csak megjelenít


    vector<UtPos> utvonal;  // merre halad majd
    int start_time;         // mikor van jelen elõször a világban
    int cel[2];           // hova halad

    // idõpillanatonkénti pozíció
    vector<vector<float>> pozicio;

    // megjelenítéshez
    int r=0, g=0, b=0;

    Jarokelo();

    Jarokelo(float posx, float posy, float celx, float cely, vector<UtPos> utvn, int st_time=0, int sarok=-1);  // lényegi konstruktora

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

class Fal{ // egyetlen fal
public:
    int x, y;   // helye
    int start_time=0, end_time=STEPS; // mikor is létezik (pl nem egy mozgó ajtó)
    bool mozgo=false; // mozog-e
    int kesleltetes; // mikor jelenjen meg
    int xd, yd; // és hova mozogjon át

    bool kerek=false; // kerek vagy szöglete, az átlós áthaladás okán

    Fal(){} // default

    Fal(int xv, int yv, bool kerekv=false, int start_timev=0){ // mikor a szerkesztőben lehelyezek egy teljes értékű falat
        x=xv, y=yv, start_time=start_time, kerek=kerekv;
    }

};





class Palya{ // nagy adathalmaz

public:
    vector<Jarokelo> jarokelok; // eltárolom a járókelõket

    list<Fal> falak; // eltárolom a falakat

    vector<vector<vector<Mezo>>> mezok; // eltárolom a STEPS idõpillanatnyi, PGX*PGY mérteû pályát

    vector<vector<vector<int>>> opt_becsult; // eltárolom a célokhoz vezetű út hosszat minden mezőhöz

    int Sx = PGX, Sy = PGY;     // pálya mérete

    int step = 0; // pálya melyik idõpillanatát nézem

    // Játékosok generálásához

    vector<Pos> cel_pos;    // eltárolom a célokat
    int sarok = 0; // sorba megy a 4 sarkon

    int dis(int x, int xv, int y, int yv); // megmondja, hogy két mező között mi a legrövidebb út (ez még nem elég)

    void calc_opt_becsult(); // csak kirendelt függvény, kiszámolja előre a legrövidebb útvonal között

    void celok_letrehozasa(); // a beleégetett célok létrehozása

    void scanMezokInWay(int x, int y, int time, bool hova[3][3]); // megvizsgálja, hogy addott mezőről adott időpillanatban hova lehet tovább lépni (vagy lehet-e helyben maradni), hova-ba kerül a "return"

    bool utvonalKereses(vector<UtPos> &utvonal, int x, int y, int celx, int cely, int time=0); // megkeresi a útvonalba a legrövidebb útvonalat x,y és celx,cely között time kezdéssel

    void FalakLehelyezese(); // mikor létrehoztam a falakat, ekkor történik meg a falak tényleges mozgást blokkoló szerepének létrejötte

    void addFal(Fal fal, bool setMezo = false); // hozzáad egy fal-at, vagy komolyan, vagy csak a listához

    void removeFal(Fal fal, bool setMezo = false); // szintén, csak kitörléssel

    void palyakepModositAtlosan(UtPos pos); // útvonal elemeinként módosítja a pályán történő mozgási lehetőséget, ez az átlós mozgásokra van

    void palyakepModositHelyben(UtPos pos, bool vege=false); // ez a helyben maradásra van

    void palyakepModositKeresztben(UtPos pos); // ez a négy égtáj irányába történő mozgásra van

    void palyakepModosit(UtPos pos); // ez dönti el, hogy melyik is kell az előző három közül az adott UtPos-hoz

    void palyakepFrissites(vector<UtPos> utvonal, Jarokelo jarokelo); // ez intézi a teljes útvonalra, plusz a lezárást, mikor megérkezik a célba

    void JarokelokLetetele(); // beleégetett járókelők letétele (nem beleégetett, mert véletlen, de a módszer az beleégetett)

    Palya(); // teszt default konstruktor, teljes értékű tesztelésre jó konsturktor (nem egy "üres" Palya-t hoz létre)

    Palya(bool ures); // üreset ez hoz létre

    Palya(int sx, int sy, int steps=STEPS); // ez meg egy megfelelő méretűt hoz létre
    // amennyire tudtam optimalizáltam
};


#endif // PALYA_H_INCLUDED
