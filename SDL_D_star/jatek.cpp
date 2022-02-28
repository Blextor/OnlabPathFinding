#include "coomon.h"
#include "movement_pattern.h"

#include <list>
#include <mutex>
#include <set>

using namespace std;

#define FOLYAMATOS true
#define DEBUG_VIZ false
#define ATLOS true

void pr(std::string str){if (DEBUG) std::cout<<str<<std::endl;}

mutex draw;

bool straight_matrix_M15_2[2][2] =
{{0,0},
{0,1}};

struct UtPos{ // az útvonal egy megállója

    // tervezéshez szükséges adatok
    int x, y, time, becsles, elozoindex; // hol van, mikor van, mennyi az addig megtett út, a becsült összhossza az útnak, és az előző megálló index-e
    int megtettUt;

    // útvonal létrehozásához szükséges adatok
    bool irany[3][3];
    int utazasi_ido=0;

    UtPos(int X, int Y, int T, bool utveg){
        for (int i=0; i<9; i++)
            irany[i/3][i/3]=false;
        x=X; y=Y; time=T;
        irany[1][1]=true;
        utazasi_ido=1;
    }

    // teljes konstruktor
    UtPos(int X, int Y, int Time, int m, int b, int i=-1){x=X; y=Y; time=Time; megtettUt=m; becsles=b; elozoindex=i;
        for (int k=0;k<9;k++)
            irany[k/3][k%3]=false;
    };

    // beállítja, hogy milyen irányba, és mennyi ideig fog haladni ebből a pontból a járókelő
    void setIranyAndTime(int vx, int vy, int vtime){ // ugyanis a time, az odaérkezést tárolja az útvonalkeresésben, itt meg már a továbbindulás idõpontját

                                        // lehet hogy különbözik a két idõpont
        int k = 0;              // irány (jelenleg balra fel az)
        if (vx==x){             // ha x szerint nem halad
            k++;               // az k-t osztva 1-et jelent
        } else if (vx>x) {      // ha meg jobbra halad tőle
            k+=2;               // az k-t osztva 2-et jelent
        }
        if (vy==y){             // ha y szerint nem halad
            k+=3;                // k%3 az 1
        } else if (vy>y){       // ha meg +1 et halad az y-on
            k+=6;               // k%3 az 2
        }
        ///cout<<"K: "<<k<<endl;
        irany[k/3][k%3]=true;   // és azt az irányt be is állítom, mint célirány
        if (vx==x && vy==y){    // az utazási idő már könnyebb, ha azonos a két koordináta
            time=vtime-1;       // 1 ideig folyik az akció
            utazasi_ido=1;      // mert csak várakozik
        }
        else if (vx==x || vy==y){   // ha valamelyik egyezik, akkor csak keresztbe léphet
            time=vtime-2;           // az 2 egység ideig tart
            utazasi_ido=2;
        }
        else {
            time=vtime-3;           // ha egyik eset sem vált be, akkor 3 egység időt vesz igénybe
            utazasi_ido=3;
        }
        //cout<<"terv: "<<vx<<" "<<x<<" "<<vy<<" "<<y<<" "<<utazasi_ido<<" "<<time<<endl;
    }
};

inline bool operator<(const UtPos& lhs, const UtPos& rhs)
{
    /// Aranyat érő két sor
    if (lhs.becsles==rhs.becsles)
        return lhs.megtettUt > rhs.megtettUt;
    /// Eddig
    // orvosolt rengeteg mindent + az uthossz várakozásnál 0 most
    return lhs.becsles < rhs.becsles;
}



class Jarokelo{     // egy gyalogos osztálya
public:
    // használaton kívül
    /*
    float irany;
    float sebesseg;
    float fordulas;
    */

    vector<UtPos> utvonal;  // merre halad majd
    int start_time;         // mikor van jelen először a világban
    int cel[2];           // hova halad

    // időpillanatonkénti pozíció
    vector<vector<float>> pozicio;

    // megjelenítéshez
    int r, g, b;

    Jarokelo(){
        pozicio.resize(STEPS);
        // véletlen színt szolgáltat neki
        r = rand()%255;
        g = rand()%255;
        b = rand()%255;
    }

    Jarokelo(float posx, float posy, float celx, float cely, vector<UtPos> utvn, int st_time=0){
        /*
        irany=0.f;
        sebesseg=0.f;
        fordulas=0;
        */

        cel[0]=celx; cel[1]=cely;   // beállítom a célt
        r = rand()%255;             // és a három színt
        g = rand()%255;
        b = rand()%255;
        start_time=st_time;         // a kezdő időpontot
        utvonal=utvn;               // az útvonalat
        pozicio.clear();            // és a pozíciót is törlöm, hogy feltölthessem valóssal
        size_t temp = 0;                // hogy melyik megállónál is tartunk
        for (int i=0; i<STEPS; i++){        // csak az adott időkereben gondolkodom
            vector<float> pos; pos.resize(2);   // a pozíciója valami akár nem egész is lehet
            if (start_time>i){                  // ha az aktuális időpillanat kisebb, mint mikor létrejön a gyalogos
                pos[0]=-1; pos[1]=-1;           // akkor elrejtem a képernyőről (TODO, igényesebb megoldás kell)
            } else {                            // viszont ha létezik már
                // és éppen az aktuális megállóból való indulás, plusz az utazási idő már az aktuális időpillanattal legalább egyenlő
                if (temp<utvonal.size() && utvonal[temp].time+utvonal[temp].utazasi_ido<i){ // TODO
                    temp++;                                             // akkor az adott megállóba értünk biztosan
                    //cout<<"temp: "<<temp<<endl;
                }

                if (temp<utvonal.size()-1){ // ha még van hátra egy megálló legalább
                                            // akkor az utazási idővel arányosan van éppen a két megálló között a járókelő
                    pos[0]=((float)utvonal[temp].x*(utvonal[temp].time+utvonal[temp].utazasi_ido-i)+utvonal[temp+1].x*(i-utvonal[temp].time))/utvonal[temp].utazasi_ido;
                    pos[1]=((float)utvonal[temp].y*(utvonal[temp].time+utvonal[temp].utazasi_ido-i)+utvonal[temp+1].y*(i-utvonal[temp].time))/utvonal[temp].utazasi_ido;
                } else if (temp<utvonal.size()) { // ha ez az utolsó, akkor a következő megállót a céllal pótolom ki
                    pos[0]=((float)utvonal[temp].x*(utvonal[temp].time+utvonal[temp].utazasi_ido-i)+celx*(i-utvonal[temp].time))/utvonal[temp].utazasi_ido;
                    pos[1]=((float)utvonal[temp].y*(utvonal[temp].time+utvonal[temp].utazasi_ido-i)+cely*(i-utvonal[temp].time))/utvonal[temp].utazasi_ido;
                } else {                // ezt követő esetekben meg már csakis a célban van
                    if (utvonal.back().time==i){
                        pos[0]=celx;
                        pos[1]=cely;
                    } else {
                        pos[0]=-1;
                        pos[1]=-1;
                    }
                }
            }
            //cout<<pos[0]<<" "<<pos[1]<<endl;
            pozicio.push_back(pos);
            //cout<<"pos size: "<<pozicio.size()<<endl;
        }
    }

    void setUtvonal(vector<UtPos> utvn){
        utvonal=utvn;
    }
};




class Mezo{ // a pálya egyetlen mezője
public:
    bool iranyok[3][3]; // minden irányról eltárolja, hogy szabad-e az áthaladás, vagy sem

    Mezo(int x=0, int y=0){reset(x,y);} // létrehozza a mezőt, default értékekkel nem túl hasznos

    void reset(int x=0, int y=0){       // a reset viszont hasznos
        for (int i=0; i<9;i++){         // 3x3-as lehetőség közül (szélrózsa 4 fő + 4 kereszt iránya + 1 helyben maradás)
            bool ok=false;              // egyik sincs blokkolva
            if (y+i%3-1>PGY-1 || y+i%3-1<0 || x+i/3-1>PGX-1 || x+i/3-1<0)   // kivéve, ha a pálya széléről van szó
                ok=true;                                                    // itt ugyanis ki lehetne sétálni a pályáról tiltás nélkül
            iranyok[i/3][i%3]=ok;       // a blokkolásállapotát eltárolom
        }
    }


};

int dis(int x, int xv, int y, int yv){  // távolság számítás
    int mi = min(abs(x-xv),abs(y-yv));  // megnézi hogy az új és régi koordináták közül melyik között kisebb a különbség, és azt veszi
    int ma = max(abs(x-xv),abs(y-yv));  // itt meg a nagyobbat veszi
    return mi*15+(ma-mi)*10;            // amíg min-szerint mind a kettő szerint kell korrigálni, az megtehető 15-tel, utána már csak 10-zel
}


class Palya{ // eltárolja a játékot
public:
    vector<Jarokelo> jarokelok; // eltárolom a járókelőket

    vector<vector<vector<Mezo>>> mezok; // eltárolom a STEPS időpillanatnyi, PGX*PGY mérteű pályát

    int step = 0; // pálya melyik időpillanatát nézem

    // Játékosok generálásához
    int sarok = 0; // sorba megy a 4 sarkon

    void scanMezokInWay(int x, int y, int time, bool hova[3][3]){ // lekérdezni, hogy melyik irányba lehet menni, adott időpillanatban, adott helyen
        for (int i=0; i<3; i++){ // x szerint -1,0,1
            for (int j=0;j<3;j++) { // y szerint -1,0,1
                hova[i][j] = mezok[time][x][y].iranyok[i][j]; // majd az értékadás
            }
        }
    }

    bool utvonalKereses(vector<UtPos> &utvonal, int x, int y, int celx, int cely, int time=0){ // megkeresi az útvonalat
        int maxDis = dis(x,celx,y,cely);        // kiszámolja az elméleti minimum távolságot

        set<UtPos> aktualisMezok;       // létrehoz egy konténert, hogy mit kéne esetleg még felfedezni, set->folyton rendezett valami szerint
        UtPos kezdo(x,y,time,0,maxDis); // létrehozza a startpozíziót
        aktualisMezok.insert(kezdo);    // be is helyezi a bejárandó területek közé

        int bejartIndex = 0;            // számolja, hogy hány területet járt már be, és milyen hosszú így a vektor
        vector<UtPos> bejartmezok;      // ezen vektor, amelyben õsszegyûjtöm azon mezõket, melyeket bejártam valamikor

        //bool stop;                              // addig fut a ciklus, amíg nem jutunk célba, ez a flag-je
        int legkisebbBecsles = 10000000;        // talán hasznos lehet az elvi minimuma az útvonal hosszának
        //cout<<"utvonalKereses => cel_pos: "<<celx<<" "<<cely<<endl;   // kiírjuk hova is kéne eljutni


        int debug_cnt=0;        // számba veszem, hány mezőt vizsgál meg
        clock_t t2 = clock();   // az időt is mérem a hatásfok vizsgálásához
        while (true){                                                  // amíg nem jutunk célba
            debug_cnt++;                                                // számolja, hogy hány mezõt fedezett fel
            //if (debug_cnt%1000==0)
              //  cout<<"utKerDebCnt: "<<debug_cnt<<endl;
            if (debug_cnt>PGX*PGY*10)
                break;
            set<UtPos> ujmezok;                                         // az új mezõket ebben összegyûjtöm
            std::set<UtPos>::iterator it = aktualisMezok.begin();       // lekérem a legrövidebb útvonallal kecsegtetõ új felfedezendõ mezõt
            while ((*it).time>=STEPS && it!=aktualisMezok.end()){       // ha több lépésbe fájna, mint amennyire maximális kapacitásunk van
                advance(it,1);                                          // akkor iteráljon egyet tovább, hátha az jó
            }
            if (aktualisMezok.end()==it){                                                   // ha nem talál olyak, akkor az bajos
            //if (STEPS-1<(*it).time){                                                      // más fajta lekezelése a dolognak
                //cout<<"nincs utvonal x lepesen belul "<<debug_cnt<<" "<<clock()-t2<<endl;   // kiírja, hogy milyen lassan nem talált útvonalat
                return false;
            }
            int meret = aktualisMezok.size();   // elmentem a méretét, mert változhat a ciklus során
            for (int zzz=0; zzz<meret; zzz++){  // és végigmegyek akár a teljes felfedezendõ területeken
                //advance(it,i);                // nem szükséges az iterátor természetéből
                bool hova[3][3];                // a irányból merre mehetek majd
                UtPos akt = *it;                // ahhoz kell a legkedvezõbb mezõ
                bejartmezok.push_back(*it);     // itt már biztos hogy bejárom
                aktualisMezok.erase(it);        // és nem is akarom többször bejárni

                //cout<<"falma "<<akt.x<<" "<<akt.y<<" "<<akt.time<<" "<<akt.becsles<<" "<<akt.elozoindex<<endl; // kiiratom a mező adatait
                scanMezokInWay(akt.x,akt.y,akt.time,ref(hova));     // a legkedvezõbb mezõbõl kell a helye és az ideje, hogy tudjam milyen irányban haladhatok onnan tovább
                bool haveFind;                                      // flag, hogy ha találok egyetlen új mezõt is

                for (int i=0; i<9; i++){            // végigmegyek a hova 3*3-as tömbjén
                    if (!hova[i/3][i%3]){           // ha valid egy bizonyos irányba történõ mozgás
                        int timeP = 1;              // akkor ha ez helyben állás, akkor 1 egység idõbe kerül
                        if (i/3 != 1 && i%3 != 1)   // ha nem a keresztben van
                            timeP=3;                // akkor 3 egység idõbe kerül
                        else if (i/3 != i%3)
                            timeP=2;                // és már csak a 2 egység idõbe kerülõk maradnak

                        int uthossz = 0; if (timeP>1) uthossz=timeP*5;
                        int X = akt.x+i/3-1, Y = akt.y+i%3-1;                                   // elmentem, hogy hova történne a lépés
                        int becsles=akt.megtettUt+uthossz+dis(X,celx,Y,cely);                   // az onnan való továbblépésrõl számolok egy becslést az út hosszáról
                        UtPos uj(X,Y,akt.time+timeP,akt.megtettUt+uthossz,becsles,bejartIndex); // ezeket el is tárolom egy új UtPos-ban

                        ujmezok.insert(uj);                 // melyeket hozzá is adom az új lehetséges útvonalakhoz
                        if (becsles<legkisebbBecsles)       // ha kell
                            legkisebbBecsles=becsles;       // frissítem a legkisebb becslést
                        haveFind=true;                      // továbbá elmentem, hogy találtam egy járható utat

                        if (X==celx && Y==cely){            // ha ez a járható út a célmezõ

                            //cout<<"hm "<<debug_cnt<<" "<<clock()-t2<<endl;           // TODO
                                                                                // megvan, hogy melyik mezõrõl jutottunk ide, és ez rekurzívan vissza a kezdõ mezõhöz is megvan
                            UtPos akt2 = uj;                                // végéről indulunk
                            vector<UtPos> ujutvonal;                        // létrehozok egy útvonalat
                            while (akt2.elozoindex!=-1){                    // amíg el nem jutunk a kezdõ elemhez
                                UtPos temp = bejartmezok[akt2.elozoindex];  // megkeresem az elõzõ elemet
                                temp.setIranyAndTime(akt2.x,akt2.y,akt2.time);  // megnézem hogy onnan milyen irányban van az azt követõ elem, és mikor indult el onnan
                                ujutvonal.push_back(temp);                      // a beállított UtPos-t az útvonalhoz hozzácsapom
                                akt2 = bejartmezok[akt2.elozoindex];            // majd az elsőző mezőre lépek, hogy az azt megelőzővel játszhassam el ezt
                            }
                            utvonal.clear();                                        // reset-elem az útvonalat
                            for (size_t o=0; o<ujutvonal.size(); o++){              // mert most még fordítva van
                                utvonal.push_back(ujutvonal[ujutvonal.size()-1-o]); // és ezt orvosolja ez
                            }
                            //UtPos temp = UtPos(uj.x,uj.y,uj.time,true);
                            //temp.elozoindex=2;
                            //utvonal.push_back(temp);
                            return true;        // megáll az algoritmus
                        }
                    }
                }
                                    // amint talál egy új bejárt mezõt (minden mezõt bejár egy felfedezendõ mezõ körül)
                bejartIndex++;      // megnöveli a bejárt mezõk számát (eddig az ujmezok létrehozásához kellett ez)

                if (haveFind){                                              // azaz ha talált új mezõt
                    aktualisMezok.insert(ujmezok.begin(),ujmezok.end());    // akkor az aktuális mezõkhöz, hozzáveszem az újakat
                    break;                                                  // majd újraindítja a ciklust
                                                                            // ezzel nem jár semmi, csak soft reset-eli az algoritmust
                } else                                                  // de ha nem talált új mezõt, pedig megvizsgálta az összeset, és nem sikerült eljutnia odáig
                {                                                       // akkor BAJ VAN
                    // TODO
                    //cout<<"nincs útvonal egyaltalan "<<debug_cnt<<" "<<clock()-t2<<endl;
                    return false;
                }
            }
        }

        return false;      // ide csak útvonal nélkül juthat el
    }


    void palyakepModositAtlosan(UtPos pos){
        int rot = 0;
        if (pos.irany[0][2])
            rot=1;
        else if (pos.irany[2][2])
            rot=2;
        else if (pos.irany[2][0])
            rot=3;
        for (int i=0; i<3; i++){  // TIME COM i<2 ? i<3
            if (pos.time+i>=0 && pos.time+i<STEPS){
                bool matrix[15][15];
                //cout<<"alma"<<endl;
                if (i==-3)
                    matrix_cpy(slash_matrix_M15,matrix);
                else if (i==-2)
                    matrix_cpy(slash_matrix_M10,matrix);
                else if (i==-1)
                    matrix_cpy(slash_matrix_M5,matrix);
                else if (i==0)
                    matrix_cpy(slash_matrix_0,matrix);
                else if (i==1)
                    matrix_cpy(slash_matrix_P5,matrix);
                else if (i==2)
                    matrix_cpy(slash_matrix_P10,matrix);
                else
                    matrix_cpy(slash_matrix_P15,matrix);
                for (int r=0; r<rot; r++)
                    matrix_rotate(matrix);

                for (int x=-2; x<3; x++){
                    for (int y=-2; y<3; y++){
                        if (pos.x+x>=0 && pos.x+x<PGX && pos.y+y>=0 && pos.y+y<PGY){
                            for (int k=0; k<9; k++){
                                mezok[pos.time+i][pos.x+x][pos.y+y].iranyok[k/3][k%3] = mezok[pos.time+i][pos.x+x][pos.y+y].iranyok[k/3][k%3] || matrix[(x+2)*3+k/3][(y+2)*3+k%3];
                            }
                        }
                    }
                }

            }
        }
    }

    void palyakepModositHelyben(UtPos pos, bool vege=false){
        int m = -1;
        if (vege)
            m=0;
        //cout<<"PMH: "<<true<<" "<<m<<endl;
        for (int i=m; i<1; i++){
            if (pos.time+i>=0 && pos.time+i<STEPS){
                for (int x=0; x<3; x++){
                    for (int y=0; y<3; y++){
                        int Xpos=x+pos.x-1, Ypos=y+pos.y-1;
                        if (Xpos>=0 && Xpos<PGX && Ypos>=0 && Ypos<PGY){
                            for (int k=0;k<9;k++){
                                mezok[pos.time+i][Xpos][Ypos].iranyok[k/3][k%3]= mezok[pos.time+i][Xpos][Ypos].iranyok[k/3][k%3] || slash_matrix_P15[(x)*3+k/3][(y)*3+k%3];
                            }
                        }
                    }
                }
            }
        }
    }

    void palyakepModositKeresztben(UtPos pos){
        int rot = 0;
        if (pos.irany[0][1])
            rot=1;
        else if (pos.irany[1][2])
            rot=2;
        else if (pos.irany[2][1])
            rot=3;
        for (int i=-1; i<2; i++){
            if (pos.time+i>=0 && pos.time+i<STEPS){
                bool matrix[15][15];
                if (i==-3)
                    matrix_cpy(straight_matrix_M15,matrix);
                    //matrix = straight_matrix_M15;
                else if (i==-2)
                    matrix_cpy(straight_matrix_M10,matrix);
                else if (i==-1)
                    matrix_cpy(straight_matrix_M5,matrix);
                else if (i==0)
                    matrix_cpy(straight_matrix_0,matrix);
                else if (i==1)
                    matrix_cpy(straight_matrix_P5,matrix);
                else
                    matrix_cpy(straight_matrix_P10,matrix);
                for (int r=0; r<rot; r++)
                    matrix_rotate(matrix);

                for (int x=-2; x<3; x++){
                    for (int y=-2; y<3; y++){
                        if (pos.x+x>=0 && pos.x+x<PGX && pos.y+y>=0 && pos.y+y<PGY){
                            for (int k=0; k<9; k++){
                                mezok[pos.time+i][pos.x+x][pos.y+y].iranyok[k/3][k%3] = mezok[pos.time+i][pos.x+x][pos.y+y].iranyok[k/3][k%3] || matrix[(x+2)*3+k/3][(y+2)*3+k%3];
                            }
                        }
                    }
                }

            }
        }
    }

    void palyakepModosit(UtPos pos){

        //cout<<"udv"<<endl;
        if (pos.irany[0][0] || pos.irany[0][2] || pos.irany[2][0] || pos.irany[2][2]){
            //cout<<"1"<<endl;
            palyakepModositAtlosan(pos);
        } else if (pos.irany[1][1]){
            //cout<<"2"<<endl;
            palyakepModositHelyben(pos);
        } else {
            //cout<<"3"<<endl;

            palyakepModositKeresztben(pos);

        }

    }

    void palyakepFrissites(vector<UtPos> utvonal, Jarokelo jarokelo){
        for (size_t i=0; i<utvonal.size(); i++){
            palyakepModosit(utvonal[i]);
        }
        palyakepModositHelyben(UtPos(jarokelo.cel[0],jarokelo.cel[1],utvonal.back().time+utvonal.back().utazasi_ido,true),true);
    }

    Palya(){ // konstruktor, jelenleg nem paraméterezhető, csak iskolapédákat tápláltam bele
        clock_t t = clock();
        pr("Palya konstruktor START");
        for (int i=0; i<STEPS; i++){ // létrehoz STEPS időpillanatnyi pályaképet
            vector<vector<Mezo>> temp; // egy pályakép
            temp.resize(PGX); // PGX-szer ...
            for (int j=0; j<PGX; j++){
                temp[j].resize(PGY); // PGY méretű
                for (int k=0; k<PGY; k++){
                    temp[j][k].reset(j,k); // melyeket alaphelyzetbe állítom (a pálya szélein nem szabad hagyni hogy kiindexeljen az útvonalkereső)
                }
            }
            mezok.push_back(temp); // feltöltöm időképenként a tömböt
        }
        cout<<"Palya konstruktor: mezok letrehozva "<<(clock()-t)<<endl;
        //pr(str);
        t=clock();
        int ply_cnt = 0; // létrehozott játékosokat számlálja
        int ply_try_cnt=0;
        while (ply_cnt<PLY && ply_try_cnt<PLY*3){ // amíg nem sikerült a kívánt mennyiséget létrehoznia
            ply_try_cnt++;
            //cout<<ply_cnt<<endl; // kiírja a következő játékos sorszámát
            int x = rand()%PGX; //x = 2; // véletlenszerű első...
            int y = rand()%PGY; //y = 3; // és második koordinátát választ neki
            if (!(*(mezok.begin()))[x][y].iranyok[1][1]){ // ha az még nem foglalt (CSAK 0. időpillanatot néz)
                int celx, cely; // akkor kell még egy cél is neki
                if (sarok==0){ // valamelyik sarok, felváltva
                    celx=0; cely=0;
                } else if (sarok==1){
                    celx=PGX-1; cely=0;
                } else if (sarok==2){
                    celx=PGX-1; cely=PGY-1;
                } else {
                    celx=0; cely=PGY-1;
                }
                //celx=PGX-1; cely=0;
                sarok++;
                if (sarok>3)
                    sarok=0;
                //cout<<"B"<<endl;
                vector<UtPos> utvonal;  // a lehendő útvonal
                ///cout<<"A "<<ply_cnt<<endl; // a játékos sorszáma, aki potenciálisan tényleg a következő lesz
                bool siker = utvonalKereses(ref(utvonal), x, y, celx, cely); // az utvonal-ba összeállítja az útvonalat (kéne hibát majd dobnia, ha nincs lehetséges útvonal)
                //cout<<"B"<<endl;
                //for (size_t i=0; i<utvonal.size(); i++){   // ki is iratom a kreált utat
                    ///cout<<"tata "<<utvonal[i].x<<" "<<utvonal[i].y<<" "<<utvonal[i].time<<" "<<utvonal[i].becsles<<" "<<utvonal[i].elozoindex<<endl;
                ///}
                ///cout<<"B"<<endl;
                if (siker){
                    Jarokelo jarokelo(x,y,celx,cely,utvonal); // majd betáplálom az újdonsült járókelőbe

                    /*
                    for (size_t i=0; i<utvonal.size(); i++){   // ki is iratom a kreált utat
                        //cout<<"tata "<<utvonal[i].x<<" "<<utvonal[i].y<<" "<<utvonal[i].time<<" "<<utvonal[i].becsles<<" "<<utvonal[i].elozoindex<<endl;
                        //for (int k=0; k<9;k++){
                            //cout<<(bool)utvonal[i].irany[k/3][k%3];
                            //if (k%3==2)
                                //cout<<endl;
                        //}
                    }
                    */
                    palyakepFrissites(utvonal,jarokelo);
                    jarokelok.push_back(jarokelo);  // és végül a járókelőkben eltárolom az újoncot
                    ply_cnt++;                      // majd jöhet a következő játékos index
                }
            }
        }

        // teszt céljából tumultust vizsgáltatok
        /*
        int er = 10, Xer = 0, Yer = 0;
        for (int i=0; i<er*er; i++){
            vector<UtPos> utvonal;
            int X = Xer+i/er, Y = Yer+i%er;
            //int X = PGX-1-(Xer+i/er), Y = PGY-1-(Yer+i%er);
            bool siker = utvonalKereses(ref(utvonal), X, Y, PGX-1, PGY-1);
            if (siker){
                Jarokelo jarokelo(X,Y,PGX-1,PGY-1,utvonal);
                palyakepFrissites(utvonal);
                jarokelok.push_back(jarokelo);
            }
        }
        //*/


        /// távolsági sorrend
         /*
        for (int i=9; i>=0; i--){
            for (int j=i; j>=0; j--){
                vector<UtPos> utvonal;
                int X = i, Y = j;
                bool siker = utvonalKereses(ref(utvonal), X, Y, PGX-1, PGY-1);
                if (siker){
                    Jarokelo jarokelo(X,Y,PGX-1,PGY-1,utvonal);
                    palyakepFrissites(utvonal);
                    jarokelok.push_back(jarokelo);
                }
            }
        }

        for (int i=9; i>=0; i--){
            for (int j=9; j>=i; j--){
                vector<UtPos> utvonal;
                int X = i, Y = j;
                bool siker = utvonalKereses(ref(utvonal), X, Y, PGX-1, PGY-1);
                if (siker){
                    Jarokelo jarokelo(X,Y,PGX-1,PGY-1,utvonal);
                    palyakepFrissites(utvonal);
                    jarokelok.push_back(jarokelo);
                }
            }
        }
        //*/
        /*
        vector<UtPos> utvonal;
        utvonalKereses(utvonal,7,7,9,9,0);
        jarokelok.push_back(Jarokelo(7,7,9,9,utvonal));
        palyakepFrissites(utvonal);
        utvonalKereses(utvonal,8,8,9,9,0);
        jarokelok.push_back(Jarokelo(8,8,9,9,utvonal));
        palyakepFrissites(utvonal);
        utvonalKereses(utvonal,9,8,9,9,0);
        jarokelok.push_back(Jarokelo(9,8,9,9,utvonal));
        palyakepFrissites(utvonal);
        */
        cout<<"Jarokelok szama: "<<jarokelok.size()<<endl;
        int t2 = (clock()-t);
        cout<<"Palya konstruktor: vege "<<t2<<endl;;
        //pr(str);
        /*
        er = 5, Xer = 5, Yer = 5;
        for (int i=0; i<er*er; i++){
            vector<UtPos> utvonal;
            utvonalKereses(ref(utvonal), Xer+i/er, Yer+i%er, 0, 0);
            Jarokelo jarokelo(Xer+i/er,Yer+i%er,0,0,utvonal);
            jarokelok.push_back(jarokelo);
        }
        */
    }


};

Palya palya;  // Időigényes konstruktor

void megjelenites(SDL_Renderer &renderer, SDL_Window &window/*, Palya palya*/, int step_cnt){
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

    for (size_t i=0; i<palya.jarokelok.size(); i++){
        //cout<<palya.jarokelok[i].r<<" "<<palya.jarokelok[i].g<<" "<<palya.jarokelok[i].b<<endl;
        //cout<<palya.jarokelok[i].pozicio[0][0]<<" "<<palya.jarokelok[i].pozicio[0][1]<<endl;
        filledCircleRGBA(&renderer,palya.jarokelok[i].pozicio[step_cnt][0]*SZELES/PGX+SZELES/PGX/2,palya.jarokelok[i].pozicio[step_cnt][1]*MAGAS/PGY+MAGAS/PGY/2,
                         SZELES/PGX/3,palya.jarokelok[i].r,palya.jarokelok[i].g,palya.jarokelok[i].b,255);

    }

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

//cout<<"tick"<<endl;


void jatek( SDL_Window &window, SDL_Renderer &renderer){

    //cout<<straight_matrix_M15_2[0][0]<<" "<<true<<endl;
    //thread t;
    //thread frame(megjelenites,ref(renderer),ref(mezok),ref(window));

    clock_t t1 = clock();
    int delay = 60;
    bool stop = false;
    srand(100);


    SDL_Event ev;
    cout<<"tick "<<clock()-t1<<endl;
    int step_cnt=0;
    while(!stop){
        bool frame=false;
        if (clock()>t1+CLOCKS_PER_SEC/delay){
            t1=clock();
            frame=true;
        } else {
            Sleep(1);
        }

        if (SDL_PollEvent(&ev)){
            if (ev.type==SDL_KEYDOWN){
                if (ev.key.keysym.sym==SDLK_r){
                    step_cnt=0;
                    cout<<"step_cnt: "<<step_cnt<<endl;
                }

                if (ev.key.keysym.sym==SDLK_s){
                    if(step_cnt+1<STEPS){
                        step_cnt++;
                        cout<<"step_cnt: "<<step_cnt<<endl;
                    }
                }
                if (ev.key.keysym.sym==SDLK_d){
                    if(step_cnt>0){
                        step_cnt--;
                        cout<<"step_cnt: "<<step_cnt<<endl;
                    }
                }
            }
            if (ev.type == SDL_QUIT)
                exit(3);
        }

        if (frame){
            //clock_t t = clock();
            megjelenites(renderer,window/*,palya*/,step_cnt);
            //cout<<clock()-t<<endl;
        }
    }
}
