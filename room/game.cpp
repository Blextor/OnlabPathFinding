#include "common.h"

using namespace std;

#include "VecX.h"
#include <list>
#include <set>

struct View{
    vec2 position = vec2(-100.f,-100.f);

    vec2 middle = vec2(0.f,0.f);

    float zoom=1;
    float rotateAngle=0;



    void KeysDown(bool w, bool a, bool s, bool d, bool u, bool z){
        if (w){
            middle.y-=1;
        }
        if (a){
            middle.x-=1;
        }
        if (s){
            middle.y+=1;
        }
        if (d){
            middle.x+=1;
        }
        if (u){
            zoom*=1.1f;
        }
        if (z){
            zoom/=1.1f;
        }
        position=middle-vec2(SZELES/2.f,MAGAS/2.f)/zoom;
    }

    vec2 getRealPos(float relX, float relY){
        vec2 RealPos = position + vec2(relX,relY)/zoom;
        return RealPos;
    }

    vec2 getRealPos(vec2 rel){
        vec2 RealPos = position + vec2(rel.x,rel.y)/zoom;
        return RealPos;
    }

    vec2 getRelativePos(float realX, float realY){
        vec2 RealativePos = (vec2(realX,realY)-position)*zoom;
        return RealativePos;
    }

    vec2 getRelativePos(vec2 real){
        vec2 RealativePos = (vec2(real.x,real.y)-position)*zoom;
        return RealativePos;
    }

    bool inRender(vec2 RelPos, int radius){
        if (RelPos.x < -radius*zoom || RelPos.x - radius*zoom > SZELES )
            return false;
        if (RelPos.y < -radius*zoom || RelPos.y - radius*zoom > MAGAS )
            return false;
        return true;
    }

};

class Magnet{
    vec2 position;

    float power;
    float radius;

public:
    Magnet(float x, float y, float p, float r=5){
        position=vec2(x,y);
        power=p;
        radius=r;
    }

    vec2 getForce(vec2 pos){
        vec2 f = (position-pos).normalize();
        //float dis = position.dist(pos);
        //f/=(dis*dis+0.001)/power;
        f*=power;
        return f;
    }

    void draw(SDL_Renderer &renderer, View view){
        filledCircleRGBA(&renderer,(position.x-view.position.x)*view.zoom,(position.y-view.position.y)*view.zoom,radius*view.zoom,200,200,200,255);
    }

};


struct Csucs{
    static int counter;

    //Haromszog **haromszogek;
    bool torolt = false;
    //unsigned int haromszogID = 0;
    vector<int> haromszogID;

    int id;
    vec2 pos;

    Csucs(){}
    Csucs(vec2 newpos){pos=newpos; id=counter; counter++;}
    Csucs(vec2 newpos, bool fs){
        pos=newpos;
    }

    void addHaromszogID(int ID){
        haromszogID.push_back(ID);
    }
};

/*
list<Csucs>::[]operator (int i){

}
*/



int Csucs::counter = 0;

struct Haromszog{
    bool torolt = false;
    Csucs *A, *B, *C;
    int id;

    Haromszog(Csucs *vA, Csucs *vB, Csucs *vC, int ID=-1){
        //Csucs.counter-=3;
        A=vA; B=vB; C=vC;
        if (ID!=-1){
            id = ID;
            vA->addHaromszogID(ID);
            vB->addHaromszogID(ID);
            vC->addHaromszogID(ID);
        }
    }

    double area()
    {
       return abs((A->pos.x*(B->pos.y-C->pos.y) + B->pos.x*(C->pos.y-A->pos.y)+ C->pos.x*(A->pos.y-B->pos.y))/2.0);
    }

    bool benneVanAPont(vec2 pont){
        Csucs csucs(pont,false);
        //Csucs *cs1 = &csucs;
        double elvileg = Haromszog(A,B,&csucs).area() + Haromszog(A,C,&csucs).area() + Haromszog(C,B,&csucs).area();
        double epszilon = 0.01;
        if (area()+epszilon>=elvileg)
            return true;
        return false;
    }

    Haromszog(){
        //A=nullptr; B=nullptr; C=nullptr;
    }
};

class Wall{

public:
    vec2 Pos, Size;

};

class Player{
    vec2 position;
    vec2 velocity;

    float weight;
    float radius;

    float speed = 100.f;

public:
    Player(float x, float y, float vx=0, float vy=0, float w=1, float r=1){
        position=vec2(x,y);
        velocity=vec2(vx,vy);
        weight=w;
        radius=r;
    }

    void Simulate(float dt, bool up, bool down, bool left, bool right){
        if (up){
            velocity.y -= dt * speed;
        }
        if (down){
            velocity.y += dt * speed;
        }
        if (left){
            velocity.x -= dt * speed;
        }
        if (right){
            velocity.x += dt * speed;
        }

        velocity*=0.98f;
        //cout<<velocity.x<<" "<<velocity.y<<endl;
        position += velocity * dt;
    }

    void Simulate2(float dt, vector<Player> &players, vector<Magnet> &magnets){

        for (size_t i=0; i<magnets.size();i++){
            vec2 force = magnets[i].getForce(position);
            force*=(dt/1000.0f);
            velocity+=force/weight;
        }

        //velocity*=0.99f;

        position+=velocity*dt;
    }

    void draw(SDL_Renderer &renderer, View view){
        //filledCircleRGBA(&renderer,(position.x-view.position.x)*view.zoom,(position.y-view.position.y)*view.zoom,radius*view.zoom,255,255,34,255);
        vec2 RelPos = view.getRelativePos(position.x,position.y);
        filledCircleRGBA(&renderer,RelPos.x,RelPos.y,radius*view.zoom,255,255,34,255);
    }

};

void draw(SDL_Window &window, SDL_Renderer &renderer, vector<Player> &players, vector<Magnet> &magnets, View &view){



    for (size_t i=0; i<magnets.size(); i++){
        magnets[i].draw(renderer,view);
    }

    for (size_t i=0; i<players.size(); i++){
        players[i].draw(renderer,view);
    }

}

struct Data{

    float csucsR = 4.f;

    list<Csucs> csucsok;

    Csucs* getCsucsP(int idx){
        list<Csucs>::iterator ptr = csucsok.begin();
        advance(ptr,idx);
        return &*ptr;
    }

    Csucs getCsucsR(int idx){
        list<Csucs>::iterator ptr = csucsok.begin();
        advance(ptr,idx);
        return *ptr;
    }

    Haromszog* getHaromszogP(int idx){
        list<Haromszog>::iterator ptr = haromszogek.begin();
        advance(ptr,idx);
        return &*ptr;
    }

    Haromszog getHaromszogR(int idx){
        list<Haromszog>::iterator ptr = haromszogek.begin();
        advance(ptr,idx);
        return *ptr;
    }

    vector<vector<vector<int>>> teruletekFoglaltsagaCsucs;
    int szeleteltseg = 4, palyameret = 2;
    /// egy terület 4 pixel széles, a kezdõ képernyõ kétszeresét fedi le ez a megoldás

    list<Haromszog> haromszogek;
    vector<vector<vector<int>>> teruletekFoglaltsagaHaromszog;
    int szeleteltsegH = 4;//, palyameretH = 2;

    bool inSquare(vec2 p){
        if (p.x>=-SZELES*palyameret/2 && p.y>=-MAGAS*palyameret/2){
            if (p.x<SZELES*palyameret/2 && p.y<MAGAS*palyameret/2)
                return true;
        }
        return false;
    }

    Data() {
        //clock_t t = clock();
        vector<vector<int>> z; z.resize(MAGAS/2*palyameret/2);
        teruletekFoglaltsagaCsucs.resize(SZELES/2*palyameret/2);
        teruletekFoglaltsagaHaromszog.resize(SZELES/2*palyameret/2);
        for (int i=0; i<SZELES/2; i++){
            teruletekFoglaltsagaCsucs[i]=z;
            teruletekFoglaltsagaHaromszog[i]=z;
        }
        //cout<<"Const: "<<clock()-t<<endl;
    }

    int getExistCsucs(vec2 csucs){
        vec2 nxy = csucs+vec2(SZELES*palyameret/2,MAGAS*palyameret/2);
        vec2 n2xy = nxy/szeleteltseg;
        vector<int> csk;
        if (n2xy.x<0 || n2xy.y<0 || n2xy.x>SZELES*palyameret/szeleteltseg-1 || n2xy.y>MAGAS*palyameret/szeleteltseg-1){}
        else
            csk = teruletekFoglaltsagaCsucs[n2xy.x][n2xy.y];

        int id = -1; float dis=100;
        for (size_t i=0; i<csk.size(); i++){
            float dis2 = (getCsucsR(csk[i]).pos-csucs).length();
            if (dis2<dis && dis2<csucsR*2){
                id = csk[i]; dis=dis2;
            }
        }
        return id;
    }

    int addNewCsucs(vec2 csucs){
        int CsucsID = getExistCsucs(csucs);
        if (CsucsID==-1){
            //Csucs a = Csucs(csucs);
            csucsok.push_back(Csucs(csucs));
            if ((size_t)getCsucsR(csucsok.size()-1).id!=csucsok.size()-1){
                cout<<"BAJ VAN! "<<getCsucsR(csucsok.size()-1).id<<" "<<csucsok.size()-1<<endl;
            }
            size_t id = csucsok.size()-1;
            vec2 nxy = csucs+vec2(SZELES*palyameret/2,MAGAS*palyameret/2);
            vec2 n2xy = nxy/szeleteltseg;
            cout<<"ID: "<<id<<endl;
            for (int i=n2xy.x-2; i<=n2xy.x+2; i++){
                for (int j=n2xy.y-2; j<=n2xy.y+2; j++){
                    if (n2xy.x<0 || n2xy.y<0 || n2xy.x>SZELES*palyameret/szeleteltseg-1 || n2xy.y>SZELES*palyameret/szeleteltseg-1){
                        //cout<<n2xy.x<<" "<<n2xy.y<<" "<<SZELES*palyameret/szeleteltseg-1<<" "<<csucs.x<<" "<<csucs.y<<" "<<nxy.x<<" "<<nxy.y<<endl;
                        //cout<<"K";
                    }
                    else {
                        if ((vec2(i,j)*szeleteltseg-nxy).length()<csucsR*3.f){
                            //
                            teruletekFoglaltsagaCsucs[i][j].push_back(id);
                        }
                    }
                }
            }
            cout<<endl;
            return getCsucsR(csucsok.size()-1).id;
        }
        cout<<"CsucsID: "<<CsucsID<<endl;
        return CsucsID;
    }

    void haromszogTeruletfoglalas(int id, int a=-1, int b=-1, int c=-1){

        //Haromszog h = Haromszog(csucsok[a],csucsok[b],csucsok[c]);


        // TODO nagyvonalú
        int minX = min(min(getCsucsR(a).pos.x,getCsucsR(b).pos.x),getCsucsR(c).pos.x);
        int minY = min(min(getCsucsR(a).pos.y,getCsucsR(b).pos.y),getCsucsR(c).pos.y);
        int maxX = max(max(getCsucsR(a).pos.x,getCsucsR(b).pos.x),getCsucsR(c).pos.x);
        int maxY = max(max(getCsucsR(a).pos.y,getCsucsR(b).pos.y),getCsucsR(c).pos.y);

        int xp = SZELES*palyameret/2, yp = MAGAS*palyameret/2;
        minX+=xp; maxX+=xp; minY+=yp; maxY+=yp;
        minX/=szeleteltsegH, minY/=szeleteltsegH, maxX/=szeleteltsegH, maxY/=szeleteltsegH;



        for (int i=minX; i<=maxX+1; i++){
            for (int j=minY; j<=maxY+1; j++){
                if (i>=0 && j>=0 && i<SZELES*palyameret/szeleteltsegH && j<MAGAS*palyameret/szeleteltsegH){
                    teruletekFoglaltsagaHaromszog[i][j].push_back(id);
                }
            }
        }
    }

    int getHaromszogFromPont (vec2 real){
        int HID = -1;
        if (inSquare(real)){
            vec2 epos2 = (real+vec2(SZELES*palyameret/2,MAGAS*palyameret/2))/szeleteltseg;
            vector<int> temp = teruletekFoglaltsagaHaromszog[epos2.x][epos2.y];

            if (temp.size()>0){
                for (size_t i=0; i<temp.size(); i++){
                    if (getHaromszogR(temp[i]).benneVanAPont(real)){
                        HID = temp[i];
                        cout<<"HID: "<<temp[i]<<endl;
                    }
                }
                return HID;
            }
        }
        return -1;
    }

    void printAll(){
        for (size_t i=0; i<csucsok.size(); i++){
            cout<<"Csucs: id: "<<getCsucsR(i).id<<" "<<getCsucsR(i).pos.x<<" "<<getCsucsR(i).pos.y<<"\t "<<(Csucs*)getCsucsP(i)<<endl;
        }
        list<Haromszog>::iterator ptr = haromszogek.begin();
            //advance(ptr,i);
        for (size_t i=0; i<haromszogek.size(); i++){
            cout<<"Haromszog: id: "<<ptr->id<<" "<<ptr->A->pos.x<<" "<<ptr->A->pos.y<<" id: "<<ptr->A<<"\t "
                                                <<ptr->B->pos.x<<" "<<ptr->B->pos.y<<" id: "<<ptr->B<<"\t "
                                                <<ptr->C->pos.x<<" "<<ptr->C->pos.y<<" id: "<<ptr->C<<"\t "

            <<endl;
            ptr++;
        }
    }

    int createNewTris(int a, int b, int c){



        //printAll();
        haromszogek.push_back(Haromszog(getCsucsP(a),getCsucsP(b),getCsucsP(c),haromszogek.size()));
        //printAll();

        haromszogTeruletfoglalas(haromszogek.size()-1,a,b,c);

        //for (int i=0; i<csucsok.size(); i++)
            //cout<<"Csucs: "<<csucsok[i].pos.x<<" "<<csucsok[i].pos.y<<endl;
        return 0;
    }

    void draw(SDL_Renderer &renderer, View view){

        for (size_t i=0; i<haromszogek.size(); i++){
            list<Haromszog>::iterator ptr = haromszogek.begin();
            advance(ptr,i);
            vec2 A = view.getRelativePos(ptr->A->pos);
            vec2 B = view.getRelativePos(ptr->B->pos);
            vec2 C = view.getRelativePos(ptr->C->pos);
            //bool a1 = view.inRender(cs1,0), a2 = view.inRender(cs2,0), a3 = view.inRender(cs3,0);
            //if (a1||a2||a3)
            filledTrigonRGBA(&renderer,A.x,A.y,B.x,B.y,C.x,C.y,130,140,210,100);
            //if (a1||a2)
                lineRGBA(&renderer,A.x,A.y,B.x,B.y,255,108,160,200);
            //if (a1||a3)
                lineRGBA(&renderer,A.x,A.y,C.x,C.y,255,108,160,200);
            //if (a3||a2)
                lineRGBA(&renderer,C.x,C.y,B.x,B.y,255,108,160,200);
        }

        for (size_t i=0; i<csucsok.size(); i++){
            vec2 RelPos = view.getRelativePos(getCsucsR(i).pos.x,getCsucsR(i).pos.y);
            if (view.inRender(RelPos,csucsR))
                filledCircleRGBA(&renderer,RelPos.x,RelPos.y,csucsR*view.zoom,255,150,34,255);
        }
    }

    void createRandomTris(int cnt, View view){
        vec2 mid = view.middle;
        mid -= vec2(SZELES/2.f,MAGAS/2.f);
        for (int i=0;i<cnt; i++){
            for (int j=0; j<3;j++){
                csucsok.push_back(mid+vec2(rand()%SZELES,rand()%MAGAS));
            }
        }
    }
};

struct CreateTri{
    int state = 0;

    vec2 csucsok[3];
    bool newcs[3];
    int csucsokID[3];

    int radius = 4;

    void getEvent(SDL_Event &ev, View view, Data &data){
        if (ev.type == SDL_KEYDOWN){
            if (ev.key.keysym.sym==SDLK_m){
                if (state>=0)
                    state=-1;
                else
                    state=0;
            }
        }

        if (state!=-1){
            if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == 1){
                //cout<<(int)ev.button.button<<endl;
                if (ev.button.button==1){
                    if (state>=0 && state<3){
                        csucsok[state] = view.getRealPos(ev.button.x,ev.button.y);

                        bool ok = true;
                        for (int i=0; i<state; i++){
                            if ((csucsok[i]-csucsok[state]).length()<8.f){
                                ok = false;
                            }
                        }

                        if (ok){
                            csucsokID[state] = data.addNewCsucs(csucsok[state]);
                            //newcs[state] = true;
                            state++;
                        }
                    }
                }
            }
        }

        if (state==3){
            for (int i=0; i<3;i++){
                if (newcs[i])
                    data.addNewCsucs(csucsok[i]);
                    //data.csucsok.push_back(Csucs(csucsok[i]));
            }
            data.createNewTris(csucsokID[0],csucsokID[1],csucsokID[2]);
            state=0;
            for (int i=0;i<3;i++)
                csucsokID[i]=-1;
        }

        if (state!=-1)
            csucsok[state] = view.getRealPos(ev.motion.x,ev.motion.y);

        //cout<<state<<endl;
    }

    void draw(SDL_Renderer &renderer, View view){
        for (int i=0; i<=state && i<3; i++){
            vec2 RelPos = view.getRelativePos(csucsok[i].x, csucsok[i].y);
            filledCircleRGBA(&renderer,RelPos.x,RelPos.y,radius*view.zoom,255,200,123,255);
        }
    }
};

struct UtvonalElem{
    vec2 posStart =vec2(0,0);
    vec2 posEnd=vec2(0,0);
    float startT=-1, durationT=-1, endT=-1;

    UtvonalElem(){}

    UtvonalElem(vec2 startP, vec2 endP){posStart=startP; posEnd=endP;}

};


struct UtPos{
    Csucs csucs;
    float eddigMegtett;
    float becsult;
    float ossz;

    vector<Csucs> eddigiCsucsok;

    UtPos(){ossz=-1;}

    UtPos(Csucs cs, float m, float b, UtPos utp=UtPos()){
        csucs=cs; eddigMegtett=m; becsult=b; ossz=m+b;
        if (utp.ossz!=-1){
            eddigiCsucsok=utp.eddigiCsucsok;
            eddigiCsucsok.push_back(utp.csucs);
        }
    }
};

inline bool operator<(const UtPos& lhs, const UtPos& rhs){
    /// Aranyat érő két sor
    if (lhs.ossz==rhs.ossz){
        if (lhs.eddigMegtett == rhs.eddigMegtett){
            //if (lhs.csucs.id == rhs.csucs.id){
              //  return lhs.csucs. > rhs.y;
            //}
            return lhs.csucs.id > rhs.csucs.id;
        }
        return lhs.eddigMegtett > rhs.eddigMegtett;
    }
    /// Eddig
    // orvosolt rengeteg mindent + az uthossz várakozásnál 0 most
    return lhs.ossz < rhs.ossz;
}


struct UtPosSeged{
    int id;
    float ossz;

    UtPosSeged(int i, float o=99999.f){id=i; ossz=o;}
};

inline bool operator<(const UtPosSeged& lhs, const UtPosSeged& rhs){
    return lhs.id < rhs.id;
}

struct UtvonalKereso{
    Data *data;

    UtvonalKereso(Data *DATA){data=DATA;}

    vector<UtvonalElem> UtvonalKeresesBasic(vec2 RealStart, vec2 RealEnd, int HIDcel, int HIDstart){
        //set<Csucs> csucsok;

        //Haromszog h1 = data->getHaromszogR(HIDstart);
        /*int CSIDstart = h1.A->id;
        float closestStart = ((h1.A->pos)-RealStart).length();
        if (closestStart>((h1.B->pos)-RealStart).length() || closestStart > ((h1.C->pos)-RealStart).length()){
            if (((h1.B->pos)-RealStart).length()>((h1.C->pos)-RealStart).length()){
                CSIDstart = h1.C->id;
                closestStart = ((h1.C->pos)-RealStart).length();
            } else {
                CSIDstart = h1.B->id;
                closestStart = ((h1.B->pos)-RealStart).length();
            }
        }*/

        /*
        Haromszog h2 = data->getHaromszogR(HIDcel);
        int CSIDend = h2.A->id;
        float closestEnd = ((h2.A->pos)-RealEnd).length();
        if (closestEnd>((h2.B->pos)-RealEnd).length() || closestEnd > ((h2.C->pos)-RealEnd).length()){
            if (((h2.B->pos)-RealEnd).length()>((h2.C->pos)-RealEnd).length()){
                CSIDstart = h2.C->id;
                closestEnd = ((h2.C->pos)-RealEnd).length();
            } else {
                CSIDstart = h2.B->id;
                closestEnd = ((h2.B->pos)-RealEnd).length();
            }
        }
        */

        set<int> celIDk;
        celIDk.insert(data->getHaromszogR(HIDcel).A->id); celIDk.insert(data->getHaromszogR(HIDcel).B->id); celIDk.insert(data->getHaromszogR(HIDcel).C->id);
        vector<UtvonalElem> ret;
        set<UtPos> csucsok;
        set<UtPosSeged> visitedCsucsId;
        Haromszog h1 = data->getHaromszogR(HIDstart);
        UtPos starter1 = UtPos(*(h1.A),(h1.A->pos - RealStart).length(),(h1.A->pos - RealEnd).length());
        UtPos starter2 = UtPos(*(h1.B),(h1.B->pos - RealStart).length(),(h1.B->pos - RealEnd).length());
        UtPos starter3 = UtPos(*(h1.C),(h1.C->pos - RealStart).length(),(h1.C->pos - RealEnd).length());
        csucsok.insert(starter1); csucsok.insert(starter2); csucsok.insert(starter3);
        visitedCsucsId.insert(UtPosSeged(h1.A->id,starter1.ossz)); visitedCsucsId.insert(UtPosSeged(h1.B->id,starter2.ossz)); visitedCsucsId.insert(UtPosSeged(h1.C->id,starter3.ossz));
        while (csucsok.size()>0){
            cout<<"csucsok.size() "<<csucsok.size()<<endl;

            UtPos temp = *(csucsok.begin());
            cout<<"csucs ossz "<<temp.ossz<<endl;
            csucsok.erase(csucsok.begin());
            vector<int> haromszogek = temp.csucs.haromszogID;
            for (int i=0; i<haromszogek.size(); i++){
                Haromszog tempH = data->getHaromszogR(haromszogek[i]);

                UtPos cs1 = UtPos(*(tempH.A),temp.eddigMegtett+(tempH.A->pos - temp.csucs.pos).length(),(tempH.A->pos - RealEnd).length(),temp);
                UtPosSeged segA(tempH.A->id,cs1.ossz);
                if (visitedCsucsId.find(segA)!=visitedCsucsId.end()){
                    if ((*(visitedCsucsId.find(segA))).ossz>segA.ossz){
                        visitedCsucsId.erase(visitedCsucsId.find(segA));
                        visitedCsucsId.insert(segA);
                        cout<<"ADDCSUCS"<<endl;
                        csucsok.insert(cs1);
                    }
                } else {
                    visitedCsucsId.insert(segA);
                    cout<<"ADDCSUCS"<<endl;
                    csucsok.insert(cs1);
                }

                UtPos cs2 = UtPos(*(tempH.B),temp.eddigMegtett+(tempH.B->pos - temp.csucs.pos).length(),(tempH.B->pos - RealEnd).length(),temp);
                UtPosSeged segB(tempH.B->id,cs2.ossz);
                if (visitedCsucsId.find(segB)!=visitedCsucsId.end()){
                    if ((*(visitedCsucsId.find(segB))).ossz>segB.ossz){
                        visitedCsucsId.erase(visitedCsucsId.find(segB));
                        visitedCsucsId.insert(segB);
                        cout<<"ADDCSUCS"<<endl;
                        csucsok.insert(cs2);
                    }
                } else {
                    visitedCsucsId.insert(segB);
                    cout<<"ADDCSUCS"<<endl;
                    csucsok.insert(cs2);
                }

                UtPos cs3 = UtPos(*(tempH.C),temp.eddigMegtett+(tempH.C->pos - temp.csucs.pos).length(),(tempH.C->pos - RealEnd).length(),temp);
                UtPosSeged segC(tempH.C->id,cs3.ossz);
                if (visitedCsucsId.find(segC)!=visitedCsucsId.end()){
                    if ((*(visitedCsucsId.find(segC))).ossz>segC.ossz){
                        visitedCsucsId.erase(visitedCsucsId.find(segC));
                        visitedCsucsId.insert(segC);
                        cout<<"ADDCSUCS"<<endl;
                        csucsok.insert(cs3);
                    }
                } else {
                    visitedCsucsId.insert(segC);
                    cout<<"ADDCSUCS"<<endl;
                    csucsok.insert(cs3);
                }


                UtPos veg=UtPos();
                bool e1=false, e2=false, e3=false;
                int ec=0;
                if (celIDk.find(cs1.csucs.id)!=celIDk.end()){
                    veg=cs1;
                    e1=true; ec++;
                }
                if (celIDk.find(cs2.csucs.id)!=celIDk.end()){
                    veg=cs2;
                    e2=true; ec++;
                }
                if (celIDk.find(cs3.csucs.id)!=celIDk.end()){
                    veg=cs3;
                    e3=true; ec++;
                }
                if (veg.ossz!=-1){
                    if (ec>1){
                        float eossz = 1000000.f;
                        if (e1){
                            veg=cs1;
                            eossz = veg.ossz + (veg.csucs.pos-RealEnd).length();
                        }
                        if (e2 && cs2.ossz+(cs2.csucs.pos-RealEnd).length()<eossz){
                            veg=cs2;
                            eossz = veg.ossz + (veg.csucs.pos-RealEnd).length();
                        }
                        if (e3 && cs3.ossz+(cs3.csucs.pos-RealEnd).length()<eossz){
                            veg=cs3;
                            eossz = veg.ossz + (veg.csucs.pos-RealEnd).length();
                        }
                        cout<<"veg.ossz: "<<eossz<<endl;
                    }

                    vector<Csucs> utvonal = veg.eddigiCsucsok;
                    utvonal.push_back(veg.csucs);
                    UtvonalElem elem = UtvonalElem(RealStart,utvonal[0].pos);
                    ret.push_back(elem);
                    for (int j=0;j<utvonal.size()-1;j++){
                        ret.push_back(UtvonalElem(utvonal[j].pos,utvonal[j+1].pos));
                    }
                    ret.push_back(UtvonalElem(utvonal[utvonal.size()-1].pos,RealEnd));
                    cout<<"Gabriel"<<endl;
                    return ret;
                }
            }
        }

        return ret;
    }

    vector<UtvonalElem> UtvonalKereses(vec2 RealStart, vec2 RealEnd){
        vector<UtvonalElem> ret;

        int HIDstart = data->getHaromszogFromPont(RealStart);
        int HIDcel = data->getHaromszogFromPont(RealEnd);

        cout<<(float)HIDstart<<" "<<true<<" "<<(HIDstart!=-1)<<endl;
        if (HIDstart<0 || HIDcel<0){
            return ret;
        } else {
            if (HIDcel==HIDstart){
                cout<<(float)HIDstart<<" aha "<<true<<" "<<(HIDstart!=-1)<<endl;
                ret.push_back(UtvonalElem(RealStart,RealEnd));
            }
            else{
                ret = UtvonalKeresesBasic(RealStart,RealEnd,HIDcel,HIDstart);
            }
        }

        return ret;
    }
};

class Player2{
    Data *data;
    UtvonalKereso *gps;
    View *view;
    vec2 pos;
    vec2 velo;

    vector<UtvonalElem> utvonal;

    bool stop=true;

    int id;

    char red = rand()%256, green = rand()%256, blue = rand()%256;

    float speed, radius, mass;

public:
    Player2(Data *Ddata, View *View, UtvonalKereso *GPS,
             vec2 Pos = vec2(0,0), float Speed = 1.f, float Radius = 5.f, float Mass = 1.f, vec2 Velo = vec2(0,0)){
        data=Ddata; view=View; gps=GPS;
        pos=Pos; speed=Speed; radius=Radius; mass=Mass; velo=Velo;
    }


    void simulate(float dt){
        if (stop)
            return;
        if (utvonal.size()>0){
            velo += (vec2(utvonal[0].posEnd.x, utvonal[0].posEnd.y) - pos)*dt*speed;

        }
        velo*=0.98f;
        if (velo.length()>speed)
            velo=velo.normalize()*speed;

    }

    void getEvent(SDL_Event &ev){
        if (ev.type==SDL_KEYDOWN){
            if (ev.key.keysym.sym == SDLK_l){
                stop=!stop;
            }
            //if (ev.key.keysym.sym == SDLK_l){
              //  stop!=stop;
            //}
        }
        if (ev.type==SDL_MOUSEBUTTONDOWN){
            cout<<"HAPCI "<<(int)ev.button.button<<endl;
            if (ev.button.button==3){
                vec2 epos(ev.button.x,ev.button.y);
                vec2 Relepos = view->getRealPos(epos);
                cout<<"Start"<<endl;
                utvonal = gps->UtvonalKereses(pos,Relepos); // így már jó a felelősség
                cout<<"Bef"<<endl;

                if (utvonal.size()>0)
                    cout<<utvonal[0].posStart.x<<" "<<utvonal[0].posStart.y<<", "<<utvonal[utvonal.size()-1].posEnd.x<<" "<<utvonal[utvonal.size()-1].posEnd.y<<endl;
                else
                    cout<<"Nincs utvonal."<<endl;
            }
        }
    }

    void findPath(vec2 cel){
        //Data
    }


    void draw(SDL_Renderer &renderer){
        for (size_t i=0; i<utvonal.size(); i++){
            vec2 RS = view->getRelativePos(utvonal[i].posStart), RE = view->getRelativePos(utvonal[i].posEnd);
            lineRGBA(&renderer,RS.x,RS.y,RE.x,RE.y,240,20,10,255);
        }
        vec2 RelPos = view->getRelativePos(pos.x,pos.y);
        if (view->inRender(RelPos,radius))
            filledCircleRGBA(&renderer,RelPos.x,RelPos.y,radius*view->zoom,red,green,blue,255);
    }

};

void jatek( SDL_Window &window, SDL_Renderer &renderer){

    clock_t t1 = clock();
    float delay = 65.f;
    bool stop = false;
    srand(100);

    //float step_cnt=0;
    int frames = 0;
    clock_t fps_t = clock();
    SDL_Event ev;

    /*
    vector<Magnet> magnets;
    vector<Player> players;

    magnets.push_back(Magnet(0,0,10000));
    players.push_back(Player(50,30,0,0,1,5));
    */



    Data data;
    UtvonalKereso gps(&data);
    CreateTri crTri;
    View view;

    Player2 player(&data,&view,&gps);


    clock_t dt_timer = clock();
    clock_t viewEvT=0, frameResetT=0, drawT=0, crTriDrawT=0, dataDrawT=0, RenderPresentT=0;

    bool w=false,a=false,s=false,d=false,z=false,u=false;// o=false;
    //bool up=false, down=false, right=false, left=false;

    while(!stop){
        bool frame=false;
        if (clock()>t1+CLOCKS_PER_SEC/delay){
            t1=clock();
            frame=true;
        } else {
            Sleep(1);
        }


        if (SDL_PollEvent(&ev)){
            if (ev.type == SDL_QUIT)
                exit(0);
            crTri.getEvent(ev,view,data);

            player.getEvent(ev);
            /*
            if (ev.type==SDL_MOUSEBUTTONDOWN){
                int x = ev.button.x;
                int y = ev.button.y;
                vec2 RealPos = view.getRealPos(x,y);
                players.push_back(Player(RealPos.x,RealPos.y,0,0,1,5));
            }
            */

            if (ev.type==SDL_KEYDOWN){
                if (ev.key.keysym.sym == SDLK_w){
                    w=true;
                }
                if (ev.key.keysym.sym == SDLK_a){
                    a=true;
                }
                if (ev.key.keysym.sym == SDLK_s){
                    s=true;
                }
                if (ev.key.keysym.sym == SDLK_d){
                    d=true;
                }
                if (ev.key.keysym.sym == SDLK_u){
                    u=true;
                }
                if (ev.key.keysym.sym == SDLK_z){
                    z=true;
                }
                if (ev.key.keysym.sym == SDLK_o){
                        //data.createRandomTris(100,view);
                    data.printAll();
                    //o=true;
                }
                /*
                if (ev.key.keysym.sym == SDLK_UP){
                    up=true;
                }
                if (ev.key.keysym.sym == SDLK_DOWN){
                    down=true;
                }
                if (ev.key.keysym.sym == SDLK_LEFT){
                    left=true;
                }
                if (ev.key.keysym.sym == SDLK_RIGHT){
                    right=true;
                }
                */
                if (ev.key.keysym.sym == SDLK_r){
                    view.zoom=1;
                    cout<<view.middle.x<<" "<<view.middle.y<<" "<<view.position.x<<" "<<view.position.y<<endl;
                }
            }
            if (ev.type==SDL_KEYUP){
                if (ev.key.keysym.sym == SDLK_w){
                    w=false;
                }
                if (ev.key.keysym.sym == SDLK_a){
                    a=false;
                }
                if (ev.key.keysym.sym == SDLK_s){
                    s=false;
                }
                if (ev.key.keysym.sym == SDLK_d){
                    d=false;
                }
                if (ev.key.keysym.sym == SDLK_u){
                    u=false;
                }
                if (ev.key.keysym.sym == SDLK_z){
                    z=false;
                }
                if (ev.key.keysym.sym == SDLK_o){
                    //o=false;
                }
                /*
                if (ev.key.keysym.sym == SDLK_UP){
                    up=false;
                }
                if (ev.key.keysym.sym == SDLK_DOWN){
                    down=false;
                }
                if (ev.key.keysym.sym == SDLK_LEFT){
                    left=false;
                }
                if (ev.key.keysym.sym == SDLK_RIGHT){
                    right=false;
                }
                */
            }
        }


        if (frame){

            float dt = clock()-dt_timer;
            dt*=SPEED;
            dt/=1000.f;
            dt_timer=clock();
            player.simulate(dt);
            //if (o)

            clock_t tFrame = clock();
            view.KeysDown(w,a,s,d,u,z);

            viewEvT+=clock()-tFrame;
            tFrame=clock();

            /*
            for (int i=0; i<players.size(); i++){
                //players[i].Simulate(dt,players,magnets);
                players[i].Simulate(dt,up,down,left,right);
            }
            */
            SDL_SetRenderDrawColor(&renderer,100,100,100,255);
            SDL_RenderClear(&renderer);
            frameResetT+=clock()-tFrame;
            tFrame=clock();

//            draw(window,renderer,players,magnets,view);


            crTri.draw(renderer,view);
            crTriDrawT+=clock()-tFrame;
            tFrame=clock();

            data.draw(renderer,view);
            dataDrawT+=clock()-tFrame;
            tFrame=clock();

            player.draw(renderer);
            drawT+=clock()-tFrame;
            tFrame=clock();

            SDL_RenderPresent(&renderer);
            RenderPresentT+=clock()-tFrame;
            tFrame=clock();

            frames++;
        }

        if (fps_t+1000<=clock()){
            fps_t=clock();
            //cout<<"fps: "<<frames<<endl;
            frames=0;
            //cout<<"viewEvT: "<<viewEvT<<", frameResetT: "<<frameResetT<<", drawT: "<<drawT<<", dataDrawT: "<<dataDrawT<<", RenderPresentT: "<<(clock_t)RenderPresentT<<endl;
            viewEvT=0, frameResetT=0, drawT=0, crTriDrawT=0, dataDrawT=0, RenderPresentT=0;
        }
    }
}

//Vége
