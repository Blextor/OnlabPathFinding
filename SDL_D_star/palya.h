#ifndef PALYA_H_INCLUDED
#define PALYA_H_INCLUDED

#include "coomon.h"


#include <list>
#include <mutex>
#include <set>

using namespace std;


void pr(std::string str);

struct UtPos{ // az �tvonal egy meg�ll�ja

    // tervez�shez sz�ks�ges adatok
    int x, y, time, becsles, elozoindex; // hol van, mikor van, mennyi az addig megtett �t, a becs�lt �sszhossza az �tnak, �s az el�z� meg�ll� index-e
    int megtettUt;

    // �tvonal l�trehoz�s�hoz sz�ks�ges adatok
    bool irany[3][3];
    int utazasi_ido=0;

    UtPos(int X, int Y, int T);

    // teljes konstruktor
    UtPos(int X, int Y, int Time, int m, int b, int i=-1);

    // be�ll�tja, hogy milyen ir�nyba, �s mennyi ideig fog haladni ebb�l a pontb�l a j�r�kel�
    void setIranyAndTime(int vx, int vy, int vtime);
};

struct Pos{
    int x=0, y=0;

    Pos(int a, int b){x=a, y=b;}

    void Set(int a, int b){x=a, y=b;}
};

inline bool operator<(const UtPos& lhs, const UtPos& rhs);

inline bool operator<(const Pos& a, const Pos& b);

class Jarokelo{     // egy gyalogos oszt�lya, purit�n
public:
    // l�nyeg�ben ez egy struct, csak adatokat t�rol, nagy sz�m�t�sokat nem v�gez, csak megjelen�t


    vector<UtPos> utvonal;  // merre halad majd
    int start_time;         // mikor van jelen el�sz�r a vil�gban
    int cel[2];           // hova halad

    // id�pillanatonk�nti poz�ci�
    vector<vector<float>> pozicio;

    // megjelen�t�shez
    int r=0, g=0, b=0;

    Jarokelo();

    Jarokelo(float posx, float posy, float celx, float cely, vector<UtPos> utvn, int st_time=0, int sarok=-1);  // l�nyegi konstruktora

    void setUtvonal(vector<UtPos> utvn);
};

class Mezo{ // a p�lya egyetlen mez�je
public:
    bool iranyok[3][3]; // minden ir�nyr�l elt�rolja, hogy szabad-e az �thalad�s, vagy sem

    Mezo(int x=0, int y=0, int Sx=0, int Sy=0); // l�trehozza a mez�t, default �rt�kekkel nem t�l hasznos

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





class Palya{
    public:
    vector<Jarokelo> jarokelok; // elt�rolom a j�r�kel�ket

    list<Fal> falak;

    vector<vector<vector<Mezo>>> mezok; // elt�rolom a STEPS id�pillanatnyi, PGX*PGY m�rte� p�ly�t

    //vector<vector<int>> hapci;

    vector<vector<vector<int>>> opt_becsult;

    int Sx = PGX, Sy = PGY;

    int step = 0; // p�lya melyik id�pillanat�t n�zem

    // J�t�kosok gener�l�s�hoz

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
