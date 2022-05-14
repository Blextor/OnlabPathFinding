#include "palya.h"

#include "movement_pattern.h"

#include <list>
#include <mutex>
#include <set>


using namespace std;

void pr(std::string str){if (DEBUG) std::cout<<str<<std::endl;}

UtPos::UtPos(int X, int Y, int T){ // a "default" konstruktor, lényegében ami az alapokt szolgáltatja
    for (int i=0; i<9; i++)
        irany[i/3][i/3]=false;
    x=X; y=Y; time=T;
    irany[1][1]=true;
    utazasi_ido=1;
}

// teljes konstruktor
UtPos::UtPos(int X, int Y, int Time, int m, int b, int i){x=X; y=Y; time=Time; megtettUt=m; becsles=b; elozoindex=i;
    for (int k=0;k<9;k++)
        irany[k/3][k%3]=false;
};

// beállítja, hogy milyen irányba, és mennyi ideig fog haladni ebből a pontból a járókelő
void UtPos::setIranyAndTime(int vx, int vy, int vtime){
    // a vx és a vy a következő uticélt jelenti
    // a vtime a megérkezés időpontja
    // kérdés, hogy mikor van az indulás és milyen iránya, erre van ez

    /// kimatekozom, hogy melyik irányba haladok (3x3)
                            // lehet hogy különbözik a két idõpont
    int k = 0;              // irány (jelenleg balra fel az)
    if (vx==x){             // ha x szerint nem halad
        k++;                // az k-t osztva 1-et jelent
    } else if (vx>x) {      // ha meg jobbra halad tőle
        k+=2;               // az k-t osztva 2-et jelent
    }
    if (vy==y){             // ha y szerint nem halad
        k+=3;               // k%3 az 1
    } else if (vy>y){       // ha meg +1 et halad az y-on
        k+=6;               // k%3 az 2
    }

    /// az irányból meg következik az irány
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

}

inline bool operator<(const UtPos& lhs, const UtPos& rhs) // UtPos rendező elve
{
    /// Aranyat érő néhány sor, szíve, lelke a sort algoritmusnak, ez alapján rendezi az UtPos-okat
    if (lhs.becsles==rhs.becsles){ // legyen várhatóan a legrövidebb
        if (lhs.megtettUt == rhs.megtettUt){ // és a megtett út is a legkevesebb
            if (lhs.x == rhs.x){    // pozíció szerinti rendezés, nagyon fontos, mert különben nem tudná sorba rendezni az elemeket, mert mindegyiket ugyan annak titulálná, pedig más a pozíciója
                return lhs.y > rhs.y;
            }
            return lhs.x > rhs.x;
        }
        return lhs.megtettUt > rhs.megtettUt;
    }
    return lhs.becsles < rhs.becsles;
}

inline bool operator<(const Pos& a, const Pos& b) // szintén a rendezés miatt kell
{
    if (a.x==b.x){
        return a.y > b.y;
    }
    return a.x < b.x;
}



Jarokelo::Jarokelo(){ // használaton kívül
    pozicio.resize(STEPS);
    // véletlen színt szolgáltat neki, jelenleg nem használt
    r = rand()%255;
    g = rand()%255;
    b = rand()%255;
}

Jarokelo::Jarokelo(float posx, float posy, float celx, float cely, vector<UtPos> utvn, int st_time, int sarok){ // használ konstruktor

    cel[0]=celx; cel[1]=cely;   // beállítom a célt
    if (sarok==-1){
        r = rand()%255;             // és a három színt
        g = rand()%255;
        b = rand()%255;
    } else {
        if (sarok==1)
            r = 255;
        if (sarok==2)
            g = 255;
        if (sarok==3)
            b = 255;
    }
    start_time=st_time;         // a kezdő időpontot
    utvonal=utvn;               // az útvonalat
    pozicio.clear();            // és a pozíciót is törlöm, hogy feltölthessem valóssal
    size_t temp = 0;                // hogy melyik megállónál is tartunk
    for (int i=0; i<STEPS; i++){        // csak az adott időkereben gondolkodom
        vector<float> pos; pos.resize(2);   // a pozíciója valami,ami akár nem egész is lehet
        if (start_time>i){                  // ha az aktuális időpillanat kisebb, mint mikor létrejön a gyalogos
            pos[0]=-1; pos[1]=-1;           // akkor elrejtem a képernyőről (TODO, igényesebb megoldás kell) benne maradt :)
        } else {                            // viszont ha létezik már
            // és éppen az aktuális megállóból való indulás, plusz az utazási idő már az aktuális időpillanattal legalább egyenlő
            if (temp<utvonal.size() && utvonal[temp].time+utvonal[temp].utazasi_ido<i){ // TODO
                temp++;                                             // akkor az adott megállóba értünk biztosan
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
        pozicio.push_back(pos);
    }
}

void Jarokelo::setUtvonal(vector<UtPos> utvn){ // beállítható külön az útvonal
    utvonal=utvn;
}



Mezo::Mezo(int x, int y, int Sx, int Sy){reset(x,y,Sx,Sy);} // létrehozza a mezőt, egy átírányítás a reset-re

void Mezo::reset(int x, int y, int Sx, int Sy){       // a pály szélere ügyel, x,y a helyzete, Sx,Sy a pálya mérete
    for (int i=0; i<9;i++){         // 3x3-as lehetőség közül (szélrózsa 4 fő + 4 kereszt iránya + 1 helyben maradás)
        bool ok=false;              // egyik sincs blokkolva
        if (y+i%3-1>Sy-1 || y+i%3-1<0 || x+i/3-1>Sx-1 || x+i/3-1<0)   // kivéve, ha a pálya széléről van szó
            ok=true;                                                    // itt ugyanis ki lehetne sétálni a pályáról tiltás nélkül
        iranyok[i/3][i%3]=ok;       // a blokkolásállapotát eltárolom
    }
}



int Palya::dis(int x, int xv, int y, int yv){  // "becsült" hátralévő út kiszámítása
    for (int i=0; i<cel_pos.size(); i++){           // amelyik célba tart
        if (cel_pos[i].x==xv && cel_pos[i].y==yv){  // azt kiválasztva
            return opt_becsult[x][y][i]*5;          // meg lehet kapni, hogy mennyi a hátralévő út, ez előre kiszámolt
            break;
        }
    }
    /// Régi megoldás, de az sokkal lasabb volt.
    //int mi = min(abs(x-xv),abs(y-yv));  // megnézi hogy az új és régi koordináták közül melyik között kisebb a különbség, és azt veszi
    //int ma = max(abs(x-xv),abs(y-yv));  // itt meg a nagyobbat veszi
    //return mi*15+(ma-mi)*10;            // amíg min-szerint mind a kettő szerint kell korrigálni, az megtehető 15-tel, utána már csak 10-zel
    return 100000; // egy nagy, elriasztó szám
}

void Palya::scanMezokInWay(int x, int y, int time, bool hova[3][3]){ // lekérdezi a hova-ba, hogy melyik irányba lehet menni, adott időpillanatban, adott helyen
    for (int i=0; i<3; i++){ // x szerint -1,0,1
        for (int j=0;j<3;j++) { // y szerint -1,0,1
            hova[i][j] = mezok[time][x][y].iranyok[i][j]; // majd az értékadás
        }
    }
}

bool Palya::utvonalKereses(vector<UtPos> &utvonal, int x, int y, int celx, int cely, int time){ // megkeresi az útvonalat

    int maxDis = dis(x,celx,y,cely);        // kiszámolja az elméleti minimum távolságot

    set<UtPos> aktualisMezok;       // létrehoz egy konténert, hogy mit kéne esetleg még felfedezni, set->folyton rendezett valami szerint, hogy melyik a legkecsegtetőbb
    UtPos kezdo(x,y,time,0,maxDis); // létrehozza a start pozíziót
    aktualisMezok.insert(kezdo);    // be is helyezi a bejárandó területek közé

    int bejartIndex = 0;            // számolja, hogy hány területet járt már be, és milyen hosszú így a vektor
    vector<UtPos> bejartmezok;      // ezen vektor, amelyben õsszegyűjtöm azon mezõket, melyeket bejártam valamikor

    int legkisebbBecsles = 10000000;        // talán hasznos lehet az elvi minimuma az útvonal hosszának

    int debug_cnt=0;        // számba veszem, hány mezőt vizsgál meg
    clock_t t2 = clock();   // az időt is mérem a hatásfok vizsgálásához

    /* bejárt mezők debug-ja, igazán érdekes, főképp a pontos becslés okán
    vector<int> temp; temp.resize(Sx);
    for (int i=0; i<Sx; i++){
        hapci.push_back(temp);
    }
    */

    while (true){                                                  // amíg nem jutunk célba
        debug_cnt++;                                                // számolja, hogy hány mezõt fedezett fel
        if (debug_cnt>Sx*Sy*EXTRA_STEPS){       // a pályaméret * megengedett extra mezőszámot meglépett már, akkor leállítom a keresést
            break;
        }

        set<UtPos> ujmezok;                                         // az új mezõket ebben összegyûjtöm
        std::set<UtPos>::iterator it = aktualisMezok.begin();       // lekérem a legrövidebb útvonallal kecsegtetõ új felfedezendõ mezõt
        while ((*it).time>=STEPS && it!=aktualisMezok.end()){       // ha több lépésbe fájna, mint amennyire maximális kapacitásunk van
            advance(it,1);                                          // akkor iteráljon egyet tovább, hátha az jó
        }
        if (aktualisMezok.end()==it){                                                   // ha nem talál olyat, akkor az bajos
        //if (STEPS-1<(*it).time){                                                      // más fajta lekezelése a dolognak
            cout<<"nincs utvonal x lepesen belul "<<debug_cnt<<" "<<clock()-t2<<endl;   // kiírja, hogy milyen lassan nem talált útvonalat

            /* bejárt mezők debug-ja, igazán érdekes
            for (int i=0; i<Sx; i++){
                for (int j=0; j<Sy; j++){
                    cout<<hapci[i][j]<<'\t';
                }
                cout<<endl;
            }
            */

            return false;
        }
        int meret = aktualisMezok.size();   // elmentem a méretét, mert változhat a ciklus során
        for (int zzz=0; zzz<meret; zzz++){  // és végigmegyek akár az összes felfedezendõ területen
            //advance(it,i);                // nem szükséges az iterátor természetéből
            bool hova[3][3];                // a irányból merre mehetek majd
            UtPos akt = *it;                // ahhoz kell a legkedvezõbb mezõ
            bejartmezok.push_back(*it);     // itt már biztos hogy bejárom
            aktualisMezok.erase(it);        // és nem is akarom többször bejárni

            scanMezokInWay(akt.x,akt.y,akt.time,ref(hova));     // a legkedvezõbb mezõből kell a hely és az idő, hogy tudjam milyen irányban haladhatok onnan tovább

            bool haveFind;                      // flag, hogy ha találok egyetlen új mezõt is
            for (int i=0; i<9; i++){            // végigmegyek a hova 3*3-as tömbjén
                if (!hova[i/3][i%3]){           // ha valid egy bizonyos irányba történõ mozgás
                    int timeP = 1;              // akkor ha ez helyben állás, akkor 1 egység idõbe kerül
                    if (i/3 != 1 && i%3 != 1)   // ha nem a keresztben van
                        timeP=3;                // akkor 3 egység idõbe kerül
                    else if (i/3 != i%3)
                        timeP=2;                // és már csak a 2 egység idõbe kerülõk maradnak
                    int uthossz = 5; if (timeP>1) uthossz=timeP*5;
                    int X = akt.x+i/3-1, Y = akt.y+i%3-1;                                   // elmentem, hogy hova történne a lépés
                    int becsles=akt.megtettUt+uthossz+dis(X,celx,Y,cely);                   // az onnan való továbblépésrõl számolok egy becslést az út hosszáról
                    UtPos uj(X,Y,akt.time+timeP,akt.megtettUt+uthossz,becsles,bejartIndex); // ezeket el is tárolom egy új UtPos-ban
                    //hapci[X][Y]++;
                    ujmezok.insert(uj);                 // melyeket hozzá is adom az új lehetséges útvonalakhoz
                    if (becsles<legkisebbBecsles)       // ha kell
                        legkisebbBecsles=becsles;       // frissítem a legkisebb becslést
                    haveFind=true;                      // továbbá elmentem, hogy találtam egy járható utat
                    if (X==celx && Y==cely){            // ha ez a járható út a célmezõ

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
                        /* bejárt mezők debug-ja, igazán érdekes, hogy mennyit gyorsított
                        for (int i=0; i<Sx; i++){
                            for (int j=0; j<Sy; j++){
                                cout<<hapci[i][j]<<'\t';
                            }
                            cout<<endl;
                        }
                        */
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
                cout<<"nincs útvonal egyaltalan "<<debug_cnt<<" "<<clock()-t2<<endl;
                /* bejárt mezők debug-ja, igazán érdekes
                for (int i=0; i<Sx; i++){
                    for (int j=0; j<Sy; j++){
                        cout<<hapci[i][j]<<'\t';
                    }
                    cout<<endl;
                }
                */
                return false;
            }
        }
    }

    /*
    for (int i=0; i<Sx; i++){
        for (int j=0; j<Sy; j++){
            cout<<hapci[i][j]<<'\t';
        }
        cout<<endl;
    }
    */
    return false;      // ide csak útvonal nélkül juthat el
}


void Palya::palyakepModositAtlosan(UtPos pos){ // beszédes
    int rot = 0;
    if (pos.irany[0][2])
        rot=1;
    else if (pos.irany[2][2])
        rot=2;
    else if (pos.irany[2][0])
        rot=3;
    for (int i=-1; i<3; i++){  // TIME COM i<2 ? i<3
        if (pos.time+i>=0 && pos.time+i<STEPS){
            bool matrix[15][15];
            if (i==-3)
                matrix_cpy(slash_matrix_M15,matrix);
            else if (i==-2)
                matrix_cpy(slash_matrix_M10,matrix);
            else if (i==-1)
                matrix_cpy(slash_matrix_M5,matrix);
            else if (i==0){
                matrix_cpy(slash_matrix_0,matrix);

            }
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
                    if (pos.x+x>=0 && pos.x+x<Sx && pos.y+y>=0 && pos.y+y<Sy){
                        for (int k=0; k<9; k++){
                            mezok[pos.time+i][pos.x+x][pos.y+y].iranyok[k/3][k%3] = mezok[pos.time+i][pos.x+x][pos.y+y].iranyok[k/3][k%3] || matrix[(x+2)*3+k/3][(y+2)*3+k%3];
                        }
                    }
                }
            }

        }
    }
}

void Palya::palyakepModositHelyben(UtPos pos, bool vege){ // szintén beszédes
    int m = -1;
    if (vege)
        m=0;
    for (int i=m; i<1; i++){
        if (pos.time+i>=0 && pos.time+i<STEPS){
            for (int x=0; x<3; x++){
                for (int y=0; y<3; y++){
                    int Xpos=x+pos.x-1, Ypos=y+pos.y-1;
                    if (Xpos>=0 && Xpos<Sx && Ypos>=0 && Ypos<Sy){
                        for (int k=0;k<9;k++){
                            mezok[pos.time+i][Xpos][Ypos].iranyok[k/3][k%3]= mezok[pos.time+i][Xpos][Ypos].iranyok[k/3][k%3] || slash_matrix_P15[(x)*3+k/3][(y)*3+k%3];
                        }
                    }
                }
            }
        }
    }
}

void Palya::palyakepModositKeresztben(UtPos pos){ // és végül ez is
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
                    if (pos.x+x>=0 && pos.x+x<Sx && pos.y+y>=0 && pos.y+y<Sy){
                        for (int k=0; k<9; k++){
                            mezok[pos.time+i][pos.x+x][pos.y+y].iranyok[k/3][k%3] = mezok[pos.time+i][pos.x+x][pos.y+y].iranyok[k/3][k%3] || matrix[(x+2)*3+k/3][(y+2)*3+k%3];
                        }
                    }
                }
            }

        }
    }
}

void Palya::palyakepModosit(UtPos pos){ // eldönti az irány alapján, hogy miként is kell módosítani a pályát
    if (pos.irany[0][0] || pos.irany[0][2] || pos.irany[2][0] || pos.irany[2][2]){
        palyakepModositAtlosan(pos);
    } else if (pos.irany[1][1]){
        palyakepModositHelyben(pos);
    } else {
        palyakepModositKeresztben(pos);
    }
}

void Palya::palyakepFrissites(vector<UtPos> utvonal, Jarokelo jarokelo){ // a kész útvonal esetén itt kerül a mozgás blokkolásra az időben és térben
    for (size_t i=0; i<utvonal.size(); i++){
        palyakepModosit(utvonal[i]);   // az útvonal szinjén
    }
    palyakepModositHelyben(UtPos(jarokelo.cel[0],jarokelo.cel[1],utvonal.back().time+utvonal.back().utazasi_ido),true);
}

Palya::Palya(int sx, int sy, int steps){
    clock_t t;
    //srand(time(NULL));
    Sx=sx, Sy=sy;
    pr("Palya konstruktor START");

    /* 1. konstruktor */
    /// Működik, de olyan (11-18)-szor lassabb, mint a 2. konstruktor
    /*
    t = clock();
    for (int i=0; i<steps; i++){ // létrehoz STEPS időpillanatnyi pályaképet
        vector<vector<Mezo>> temp; // egy pályakép
        temp.resize(sx); // PGX-szer ...
        for (int j=0; j<sx; j++){
            temp[j].resize(sy); // PGY méretű
            for (int k=0; k<sy; k++){
                temp[j][k].reset(j,k,sx,sy); // melyeket alaphelyzetbe állítom (a pálya szélein nem szabad hagyni hogy kiindexeljen az útvonalkereső)
            }
        }
        mezok.push_back(temp); // feltöltöm időképenként a tömböt
    }
    cout<<"Palya konstruktor: mezok letrehozva "<<(clock()-t)<<endl;
    */

    /* 2. konstruktor */
    /// Jelenlegi legjobb, kevésbé szétaprózott, mint a 3.
    t=clock();
    mezok.clear();
    cout<<"Palya konstruktor: mezok resetelve "<<(clock()-t)<<endl;

    vector<vector<Mezo>> temp; // egy pályakép
    temp.resize(sx); // PGX-szer ...
    for (int j=0; j<sx; j++){
        temp[j].resize(sy); // PGY méretű
        for (int k=0; k<sy; k++){
            temp[j][k].reset(j,k,sx,sy); // melyeket alaphelyzetbe állítom (a pálya szélein nem szabad hagyni hogy kiindexeljen az útvonalkereső)
        }
    }

    for (int i=0; i<steps; i++){ // létrehoz STEPS időpillanatnyi pályaképet
        mezok.push_back(temp); // feltöltöm időképenként a tömböt
    }
    cout<<"Palya konstruktor2: mezok letrehozva "<<(clock()-t)<<endl;

    /* 3. konstruktor */
    /// Jelenlegi legjobb lenne, de valamiért mégsem. És nem is működik jól. (iterlálással lehet a baj)
    /*
    t=clock();
    mezok.clear();
    cout<<"Palya konstruktor: mezok resetelve "<<(clock()-t)<<endl;

    vector<vector<Mezo>> temp2; // egy pályakép
    temp2.resize(sx); // PGX-szer ...
    vector<Mezo> a, b, c; a.resize(sx); b.resize(sx); c.resize(sx);

    Mezo ap; ap.reset(1,0,sx,sy);
    Mezo bp; bp.reset(1,1,sx,sy);
    Mezo cp; cp.reset(0,sy-1,sx,sy);

    for (int i=1; i<sx-1; i++){
        a[i]=ap;
        b[i]=bp;
        c[i]=cp;
    }

    a[0].reset(0,0,sx,sy);
    a[sx-1].reset(sx-1,0,sx,sy);

    b[0].reset(0,1,sx,sy);
    b[sx-1].reset(sx-1,1,sx,sy);

    c[0].reset(0,sy-1,sx,sy);
    c[sx-1].reset(sx-1,sy-1,sx,sy);

    temp2.push_back(a);
    for (int i=1; i<sx-1; i++)
        temp2.push_back(b);
    temp2.push_back(c);

    for (int i=0; i<steps; i++){ // létrehoz STEPS időpillanatnyi pályaképet
        mezok.push_back(temp2); // feltöltöm időképenként a tömböt
    }
    cout<<"Palya konstruktor3: mezok letrehozva "<<(clock()-t)<<endl;
    */
}

Palya::Palya(){ // konstruktor, jelenleg nem paraméterezhető, csak iskolapédákat tápláltam bele
    clock_t t = clock();
    pr("Palya konstruktor START");
    for (int i=0; i<STEPS; i++){ // létrehoz STEPS időpillanatnyi pályaképet
        vector<vector<Mezo>> temp; // egy pályakép
        temp.resize(PGX); // PGX-szer ...
        for (int j=0; j<PGX; j++){
            temp[j].resize(PGY); // PGY méretű
            for (int k=0; k<PGY; k++){
                temp[j][k].reset(j,k,PGX,PGY); // melyeket alaphelyzetbe állítom (a pálya szélein nem szabad hagyni hogy kiindexeljen az útvonalkereső)
            }
        }
        mezok.push_back(temp); // feltöltöm időképenként a tömböt
    }
    cout<<"Palya konstruktor: mezok letrehozva "<<(clock()-t)<<endl;
    cout<<"hapi"<<endl;

    t=clock();
    int ply_cnt = 0; // létrehozott játékosokat számlálja
    int ply_try_cnt=0;
    int cnt1=0, cnt2=0, cnt3=0;

    while (ply_cnt<PLY && ply_try_cnt<PLY*PLYTRY){ // amíg nem sikerült a kívánt mennyiséget létrehoznia
        int start_time = ply_try_cnt/FORGALOM;
        ply_try_cnt++;
        //cout<<ply_cnt<<endl; // kiírja a következő játékos sorszámát
        int x = rand()%(PGX/2); //x = 2; // véletlenszerű első...
        int y = rand()%(PGY/2); //y = 3; // és második koordinátát választ neki
        //x=6; y=6;
        if (!(*(mezok.begin()))[x][y].iranyok[1][1]){ // ha az még nem foglalt (CSAK 0. időpillanatot néz)
            int celx, cely; // akkor kell még egy cél is neki
            //sarok=0;
            if (sarok==0){ // valamelyik sarok, felváltva
                celx=0; cely=0;
                x+=PGX/2;
                y+=PGY/2;
            } else if (sarok==1){
                celx=PGX-1; cely=0;
                y+=PGY/2;
            } else if (sarok==2){
                celx=PGX-1; cely=PGY-1;
            } else {
                celx=0; cely=PGY-1;
                x+=PGX/2;
            }
            sarok++;
            if (sarok>3)
                sarok=0;

            vector<UtPos> utvonal;  // a lehendő útvonal
            bool siker = utvonalKereses(ref(utvonal), x, y, celx, cely, start_time); // az utvonal-ba összeállítja az útvonalat (kéne hibát majd dobnia, ha nincs lehetséges útvonal)
            if (siker){
                Jarokelo jarokelo(x,y,celx,cely,utvonal,start_time,sarok); // majd betáplálom az újdonsült járókelőbe

                palyakepFrissites(utvonal,jarokelo);
                jarokelok.push_back(jarokelo);  // és végül a járókelőkben eltárolom az újoncot
                ply_cnt++;                      // majd jöhet a következő játékos index
            }
            cnt3++;
        } else if ((*(mezok.begin()))[x][y].fal){
            cout<<"FAL"<<endl;
            ply_try_cnt--;
            cnt1++;
        } else {
            ply_try_cnt--;
            cnt2++;
            cout<<"miss: "<<x<<" "<<y<<endl;
        }
    }
    cout<<"CNTS: "<<cnt1<<" "<<cnt2<<" "<<cnt3<<endl;
    cout<<"Probalkozasok szama: "<<ply_try_cnt<<endl;

    cout<<"Jarokelok szama: "<<jarokelok.size()<<" Blaha "<<ply_cnt<<" aha"<<endl;
    int t2 = (clock()-t);
    cout<<"Palya konstruktor: vege "<<t2<<endl;;
}

void Palya::JarokelokLetetele(){

    clock_t t=clock();
    int ply_cnt = 0; // létrehozott játékosokat számlálja
    int ply_try_cnt=0;
    int cnt1=0, cnt2=0, cnt3=0;

    while (ply_cnt<PLY && ply_try_cnt<PLY*PLYTRY){ // amíg nem sikerült a kívánt mennyiséget létrehoznia
        int start_time = ply_try_cnt/FORGALOM;
        ply_try_cnt++;
        //cout<<ply_cnt<<endl; // kiírja a következő játékos sorszámát
        int x = rand()%(Sx/2); //x = 2; // véletlenszerű első...
        int y = rand()%(Sy/2); //y = 3; // és második koordinátát választ neki

        if (!(*(mezok.begin()))[x][y].iranyok[1][1]){ // ha az még nem foglalt (CSAK 0. időpillanatot néz)
            int celx, cely; // akkor kell még egy cél is neki
            //sarok=0;
            if (sarok==0){ // valamelyik sarok, felváltva
                celx=0; cely=0;
                x+=Sx/2;
                y+=Sy/2;
            } else if (sarok==1){
                celx=Sx-1; cely=0;
                y+=Sy/2;
            } else if (sarok==2){
                celx=Sx-1; cely=Sy-1;
            } else {
                celx=0; cely=Sy-1;
                x+=Sx/2;
            }
            sarok++;
            if (sarok>3)
                sarok=0;

            vector<UtPos> utvonal;  // a lehendő útvonal
            bool siker = utvonalKereses(ref(utvonal), x, y, celx, cely, start_time); // az utvonal-ba összeállítja az útvonalat (kéne hibát majd dobnia, ha nincs lehetséges útvonal)
            if (siker){
                Jarokelo jarokelo(x,y,celx,cely,utvonal,start_time,sarok); // majd betáplálom az újdonsült járókelőbe
                palyakepFrissites(utvonal,jarokelo);
                jarokelok.push_back(jarokelo);  // és végül a járókelőkben eltárolom az újoncot
                ply_cnt++;                      // majd jöhet a következő játékos index
            }
            cnt3++;
        } else if ((*(mezok.begin()))[x][y].fal){
            cout<<"FAL"<<endl;
            ply_try_cnt--;
            cnt1++;
        } else {
            cnt2++;
            cout<<"miss: "<<x<<" "<<y<<endl;
        }
    }
    cout<<"CNTS: "<<cnt1<<" "<<cnt2<<" "<<cnt3<<endl;
    cout<<"Probalkozasok szama: "<<ply_try_cnt<<endl;
    cout<<"Jarokelok szama: "<<jarokelok.size()<<" "<<ply_cnt<<endl;
    int t2 = (clock()-t);
    cout<<"Palya konstruktor: vege "<<t2<<endl;;
}

    Palya::Palya(bool ures){
        cout<<"sizeof(Mezo): "<<sizeof(Mezo)<<endl;
    }

    void Palya::addFal(Fal fal, bool setMezo){
        falak.push_back(fal);
        if (setMezo){
            mezok[0][fal.x][fal.y].fal=true;
        }
    }

    void Palya::removeFal(Fal fal, bool setMezo){
        if (setMezo){
            mezok[0][fal.x][fal.y].fal=false;
        }
        std::list<Fal>::iterator it;
        cout<<"hm "<<falak.size()<<endl;
        for (it = falak.begin(); it != falak.end(); it++){
            Fal falt = *it;
            cout<<"remFal: "<<fal.x<<" "<<fal.y<<" "<<falt.x<<" "<<falt.y<<endl;
            if (fal.x == falt.x && fal.y == falt.y){

                falak.erase(it);
                cout<<"oks"<<endl;
                break;
            }
        }
    }

    void Palya::FalakLehelyezese(){
        clock_t t = clock();
        cout<<"FalakLehelyezese"<<endl;
        std::list<Fal>::iterator it;
        for (it = falak.begin(); it != falak.end(); ++it){
            Fal fal = *it;
        //for (int j=0; j<falak.size();j++){
            //Fal fal = falak[j];

            for (int i=fal.start_time-3; i<fal.end_time; i++){
                if (i>=0){
                    mezok[i][fal.x][fal.y].fal = true;
                    bool matrix[15][15];
                    if (!fal.kerek)
                        matrix_cpy(fal_matrix,matrix);
                    else
                        matrix_cpy(fal_matrix_kerek,matrix);
                    for (int x=-2; x<3; x++){
                        for (int y=-2; y<3; y++){
                            if (fal.x+x>=0 && fal.x+x<Sx && fal.y+y>=0 && fal.y+y<Sy){
                                for (int k=0; k<9; k++){
                                    mezok[i][fal.x+x][fal.y+y].iranyok[k/3][k%3] = mezok[i][fal.x+x][fal.y+y].iranyok[k/3][k%3] || matrix[(x+2)*3+k/3][(y+2)*3+k%3];
                                }
                            }
                        }
                    }
                }
            }
        }
        cout<<"FalakLehelyezese vege: "<<(clock()-t)<<endl; t=clock();
        celok_letrehozasa();
        calc_opt_becsult();
    }

    void Palya::celok_letrehozasa(){
        if (!mezok[0][0][0].fal)
            cel_pos.push_back(Pos(0,0));
        if (!mezok[0][Sx-1][0].fal)
            cel_pos.push_back(Pos(Sx-1,0));
        if (!mezok[0][0][Sy-1].fal)
            cel_pos.push_back(Pos(0,Sy-1));
        if (!mezok[0][Sx-1][Sy-1].fal)
            cel_pos.push_back(Pos(Sx-1,Sy-1));
    }

    void Palya::calc_opt_becsult(){
        clock_t t = clock();
        cout<<"calc_opt_becsult"<<endl;
        size_t celok = cel_pos.size();
        vector<int> temp; temp.resize(celok);
        for (int i=0; i<celok; i++){
            temp[i]=-1;
        }
        vector<vector<int>> temp2;
        for (int j=0; j<Sy; j++){
            temp2.push_back(temp);
        }
        for (int i=0; i<Sx; i++){
            opt_becsult.push_back(temp2);
        }

        int cnt2 = 0, cnt3 = 0;
        for (int zzz=0; zzz<celok; zzz++){
            cout<<"celok: "<<zzz<<" ";
            int time=0, maxTime = Sx*Sy*3;
            //set<Pos> aktualismezok; aktualismezok.insert(Pos(cel_pos[zzz].x,cel_pos[zzz].y)); // 256 lehet max a pálya mérete
            //set<Pos>::iterator it;
            vector<vector<Pos>> aktualismezok; aktualismezok.resize(Sx*Sy*3+5); aktualismezok[0].push_back(Pos(cel_pos[zzz].x,cel_pos[zzz].y));
            int cnt=1;
            opt_becsult[cel_pos[zzz].x][cel_pos[zzz].y][zzz]=0;
            while (time<maxTime){

                vector<Pos> ujmezok;

                //cout<<"ahapci"<<endl;
                //cout<<aktualismezok.size()<<endl;
                for (int i=0; i<aktualismezok[time].size(); i++){

                    cnt2++;
                    Pos pos = aktualismezok[time][i]; //*it;//aktualismezok[i];
                    Pos ujpos=Pos(0,0);
                    int aktdis = opt_becsult[pos.x][pos.y][zzz];
                    if (pos.x>0){
                        if (opt_becsult[pos.x-1][pos.y][zzz]==-1 && !mezok[0][pos.x-1][pos.y].fal){
                            ujpos.Set(pos.x-1,pos.y);
                            aktualismezok[aktdis+2].push_back(ujpos);
                            opt_becsult[pos.x-1][pos.y][zzz]=aktdis+2;
                        }
                        if (pos.y>0 && !mezok[0][pos.x-1][pos.y-1].fal && opt_becsult[pos.x-1][pos.y-1][zzz]==-1){
                            ujpos.Set(pos.x-1,pos.y-1);
                            aktualismezok[aktdis+3].push_back(ujpos);
                            opt_becsult[pos.x-1][pos.y-1][zzz]=aktdis+3;
                        }
                        if (pos.y<Sy-1 && !mezok[0][pos.x-1][pos.y+1].fal && opt_becsult[pos.x-1][pos.y+1][zzz]==-1){
                            ujpos.Set(pos.x-1,pos.y+1);
                            aktualismezok[aktdis+3].push_back(ujpos);
                            opt_becsult[pos.x-1][pos.y+1][zzz]=aktdis+3;
                        }
                    }
                    if (pos.x<Sx-1){
                        if (opt_becsult[pos.x+1][pos.y][zzz]==-1 && !mezok[0][pos.x+1][pos.y].fal){
                            ujpos.Set(pos.x+1,pos.y);
                            aktualismezok[aktdis+2].push_back(ujpos);
                            opt_becsult[pos.x+1][pos.y][zzz]=aktdis+2;
                        }
                        if (pos.y>0 && !mezok[0][pos.x+1][pos.y-1].fal && opt_becsult[pos.x+1][pos.y-1][zzz]==-1){
                            ujpos.Set(pos.x+1,pos.y-1);
                            aktualismezok[aktdis+3].push_back(ujpos);
                            opt_becsult[pos.x+1][pos.y-1][zzz]=aktdis+3;
                        }
                        if (pos.y<Sy-1 && !mezok[0][pos.x+1][pos.y+1].fal && opt_becsult[pos.x+1][pos.y+1][zzz]==-1){
                            ujpos.Set(pos.x+1,pos.y+1);
                            aktualismezok[aktdis+3].push_back(ujpos);
                            opt_becsult[pos.x+1][pos.y+1][zzz]=aktdis+3;
                        }
                    }
                    if (pos.y>0 && !mezok[0][pos.x][pos.y-1].fal && opt_becsult[pos.x][pos.y-1][zzz]==-1){
                        ujpos.Set(pos.x,pos.y-1);
                        aktualismezok[aktdis+2].push_back(ujpos);
                        opt_becsult[pos.x][pos.y-1][zzz]=aktdis+2;
                    }
                    if (pos.y<Sy-1 && !mezok[0][pos.x][pos.y+1].fal && opt_becsult[pos.x][pos.y+1][zzz]==-1){
                        ujpos.Set(pos.x,pos.y+1);
                        aktualismezok[aktdis+2].push_back(ujpos);
                        opt_becsult[pos.x][pos.y+1][zzz]=aktdis+2;
                    }
                }
                time++;
                //cout<<"bhapci"<<endl;
                //aktualismezok.insert(ujmezok.begin(), ujmezok.end());
                //aktualismezok=ujmezok;
                //cout<<"chapci"<<endl;
                //cnt+=ujmezok.size();
                //cout<<"dhapci"<<endl;
            }

            cout<<cnt<<" "<<cnt2<<endl;
            /*
            for (int i=0;i<Sx; i++){
                for (int j=0; j<Sy; j++){
                    cout<<opt_becsult[i][j][zzz]<<"\t";
                }
                cout<<endl;
            }
            */
        }
        cout<<"calc_opt_becsult vege: "<<(clock()-t)<<endl; t=clock();
    }
