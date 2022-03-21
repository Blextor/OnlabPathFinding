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

    UtPos(int X, int Y, int T, bool utveg);

    // teljes konstruktor
    UtPos(int X, int Y, int Time, int m, int b, int i=-1);

    // be�ll�tja, hogy milyen ir�nyba, �s mennyi ideig fog haladni ebb�l a pontb�l a j�r�kel�
    void setIranyAndTime(int vx, int vy, int vtime);
};

inline bool operator<(const UtPos& lhs, const UtPos& rhs);

class Jarokelo{     // egy gyalogos oszt�lya
public:
    // haszn�laton k�v�l
    /*
    float irany;
    float sebesseg;
    float fordulas;
    */

    vector<UtPos> utvonal;  // merre halad majd
    int start_time;         // mikor van jelen el�sz�r a vil�gban
    int cel[2];           // hova halad

    // id�pillanatonk�nti poz�ci�
    vector<vector<float>> pozicio;

    // megjelen�t�shez
    int r, g, b;

    Jarokelo();

    Jarokelo(float posx, float posy, float celx, float cely, vector<UtPos> utvn, int st_time=0);

    void setUtvonal(vector<UtPos> utvn);
};

class Mezo{ // a p�lya egyetlen mez�je
public:
    bool iranyok[3][3]; // minden ir�nyr�l elt�rolja, hogy szabad-e az �thalad�s, vagy sem

    Mezo(int x=0, int y=0); // l�trehozza a mez�t, default �rt�kekkel nem t�l hasznos

    void reset(int x=0, int y=0);


};

int dis(int x, int xv, int y, int yv);


class Palya{
    public:
    vector<Jarokelo> jarokelok; // elt�rolom a j�r�kel�ket

    vector<vector<vector<Mezo>>> mezok; // elt�rolom a STEPS id�pillanatnyi, PGX*PGY m�rte� p�ly�t

    int step = 0; // p�lya melyik id�pillanat�t n�zem

    // J�t�kosok gener�l�s�hoz
    int sarok = 0; // sorba megy a 4 sarkon

    void scanMezokInWay(int x, int y, int time, bool hova[3][3]);

    bool utvonalKereses(vector<UtPos> &utvonal, int x, int y, int celx, int cely, int time=0);


    void palyakepModositAtlosan(UtPos pos);

    void palyakepModositHelyben(UtPos pos, bool vege=false);

    void palyakepModositKeresztben(UtPos pos);

    void palyakepModosit(UtPos pos);

    void palyakepFrissites(vector<UtPos> utvonal, Jarokelo jarokelo);

    Palya();


};


#endif // PALYA_H_INCLUDED
