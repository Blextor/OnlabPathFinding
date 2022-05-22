#include "common.h"
#include <fstream>

using namespace std;

#include "VecX.h"
#include <list>
#include <set>
#include <algorithm>


static unsigned long xxx=123456789, yyy=362436069, zyx=521288629;

unsigned long rand2(void) {          //period 2^96-1
unsigned long t;
    xxx ^= xxx << 16;
    xxx ^= xxx >> 5;
    xxx ^= xxx << 1;

   t = xxx;
   xxx = yyy;
   yyy = zyx;
   zyx = t ^ xxx ^ yyy;

  return zyx;
}


bool Metszi(vec2 A1, vec2 A2, vec2 B1, vec2 B2){
    // Line AB represented as a1x + b1y = c1
    double a1 = A2.y - A1.y;
    double b1 = A1.x - A2.x;
    double c1 = a1*(A1.x) + b1*(A1.y);

    // Line CD represented as a2x + b2y = c2
    double a2 = B2.y - B1.y;
    double b2 = B1.x - B2.x;
    double c2 = a2*(B1.x)+ b2*(B1.y);

    double determinant = a1*b2 - a2*b1;

    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        //return make_pair(FLT_MAX, FLT_MAX);

    }
    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        if (min(A1.x,A2.x) <= x && x <= max(A1.x,A2.x)  &&  min(A1.y,A2.y) <= y && y <= max(A1.y,A2.y)){
            if (min(B1.x,B2.x) <= x && x <= max(B1.x,B2.x)  &&  min(B1.y,B2.y) <= y && y <= max(B1.y,B2.y)){
                return true;
            }
        }
    }
    return false;
}


int falonkiv = 0;

struct Csucs{
    static int counter;

    //Haromszog **haromszogek;
    bool torolt = false;
    bool uj = true;
    //unsigned int haromszogID = 0;
    vector<int> haromszogID;

    int id = -1;
    vec2 pos = vec2(-1,-1);

    Csucs(){}
    Csucs(vec2 newpos){pos=newpos; id=counter; counter++;
    }
    Csucs(vec2 newpos, bool fs){
        pos=newpos;
    }

    void addHaromszogID(int ID){
        //cout<<"CsucsHaromSzogID : "<<id<<" "<<ID<<endl;
        haromszogID.push_back(ID);
    }
};



float PontVonalTavolsag(vec2 P, vec2 A, vec2 B){
    float x = (P-A).length();
    float y = (P-B).length();
    float z = (A-B).length();

    float a = (x*x - y*y + z*z) / (2.f*z);

    vec2 ret = (A*(z-a)+B*a)/z;
    float epsz = 0.001f;
    if ((ret-A).length()+(ret-B).length()<(A-B).length()+epsz){
        return (ret-P).length();
    } else {
        return min((ret-A).length(),(ret-B).length());
    }

    //return (ret-P).length();
}

long int metszinemcnt = 0;


vec2 HolMetszi(vec2 A1, vec2 A2, vec2 B1, vec2 B2){
    // Line AB represented as a1x + b1y = c1
    double a1 = A2.y - A1.y;
    double b1 = A1.x - A2.x;
    double c1 = a1*(A1.x) + b1*(A1.y);

    // Line CD represented as a2x + b2y = c2
    double a2 = B2.y - B1.y;
    double b2 = B1.x - B2.x;
    double c2 = a2*(B1.x)+ b2*(B1.y);

    double determinant = a1*b2 - a2*b1;

    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        //return make_pair(FLT_MAX, FLT_MAX);
    }
    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        if (min(A1.x,A2.x) <= x && x <= max(A1.x,A2.x)  &&  min(A1.y,A2.y) <= y && y <= max(A1.y,A2.y)){
            if (min(B1.x,B2.x) <= x && x <= max(B1.x,B2.x)  &&  min(B1.y,B2.y) <= y && y <= max(B1.y,B2.y)){
                return vec2(x,y);
            }
        }
    }
    return vec2(-1369,-1369);
}

bool MetsziNemCsakErenti(vec2 *A1, vec2 *A2, vec2 *B1, vec2 *B2, /*Data *data, */vec2 &hol, bool bo = false){
    metszinemcnt++;
    // Line AB represented as a1x + b1y = c1
    double a1 = A2->y - A1->y;
    double b1 = A1->x - A2->x;
    double c1 = a1*(A1->x) + b1*(A1->y);

    // Line CD represented as a2x + b2y = c2
    //                        a2/b2 * x - c2/b2 = -y
    double a2 = B2->y - B1->y;
    double b2 = B1->x - B2->x;
    double c2 = a2*(B1->x)+ b2*(B1->y);

    double determinant = a1*b2 - a2*b1;
    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        //return make_pair(FLT_MAX, FLT_MAX);
        //cout<<"MI A NULLA OSZTAS EREDMENYE"<<endl;
        //double x = (b2*c1 - b1*c2)/determinant;
        //double y = (a1*c2 - a2*c1)/determinant;
        //hol = vec2(x,y);
        float epsz = 0.001f;  /// Ritka, falakok át vezető útvonal generálás oka
        if (c1/b1-epsz<c2/b2 && c2/b2<c1/b1+epsz)
            return true;
        return false;
    }
    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        hol = vec2(x,y);
        if (min(A1->x,A2->x) <= x && x <= max(A1->x,A2->x)  &&  min(A1->y,A2->y) <= y && y <= max(A1->y,A2->y)){
            if (min(B1->x,B2->x) <= x && x <= max(B1->x,B2->x)  &&  min(B1->y,B2->y) <= y && y <= max(B1->y,B2->y)){
                if (((*A1)!=vec2(x,y)) && ((*A2)!=vec2(x,y))){
                    //if ((*A2))
                    //cout<<"hol "<<(double)x<<" "<<(double)y<<", A1: "<<(*A1).x<<" "<<(*A1).y<<", A2: "<<(*A2).x<<" "<<(*A2).y<<" "<<true<<((*A1)!=vec2(x,y))<<((*A2)!=vec2(x,y))<<((*A2)!=vec2(15,20))<<endl;
                    //bool ok = false;
                    //for (int i=0; i<data->)
                    return true;
                }
                /* Csúcsok lekezelésére van ez itt, de már van külön megoldás rá
                } else {
                    vec2 kerdeses = vec2(0,0);
                    if (A1==vec2(x,y)){
                        kerdeses = A1 + (A2-A1).normalize()*1.0f;
                    } else {
                        kerdeses = A2 + (A1-A2).normalize()*1.0f;
                    }
                    if (data->getHaromszogIDFromPont(kerdeses)==-1)
                        return true;
                }
                */
            }
        }
    }
    return false;
}

struct View{
    vec2 position = vec2(-100.f,-100.f);

    vec2 middle = vec2(0.f,0.f);

    float zoom=1;
    float rotateAngle=0;

    float moveSpeed = 3.f;

    void KeysDown(bool w, bool a, bool s, bool d, bool u, bool z){
        if (w){
            middle.y-=1 / zoom * moveSpeed;
        }
        if (a){
            middle.x-=1 / zoom * moveSpeed;
        }
        if (s){
            middle.y+=1 / zoom * moveSpeed;
        }
        if (d){
            middle.x+=1 / zoom * moveSpeed;
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

    bool inRenderCircle(vec2 RelPos, int radius){
        if (RelPos.x < -radius*zoom || RelPos.x - radius*zoom > SZELES ){
            //cout<<"HAPCI3 "<<RelPos.x<<" "<<-radius*zoom<<" "<<RelPos.x - radius*zoom<<" "<<SZELES<<endl;
            return false;
        }
        if (RelPos.y < -radius*zoom || RelPos.y - radius*zoom > MAGAS ){
            //cout<<"HAPCI2 "<<RelPos.y<<" "<<-radius*zoom<<" "<<RelPos.y - radius*zoom<<" "<<MAGAS<<endl;
            return false;
        }
        return true;
    }

    bool inRenderLine(vec2 A, vec2 B){
        if (inRenderCircle(A,0) || (inRenderCircle(B,0)))
            return true;

        vec2 BF = vec2(0,0);
        vec2 JF = vec2(SZELES,0);
        vec2 BL = vec2(0,MAGAS);
        vec2 JL = vec2(SZELES,MAGAS);

        if (Metszi(A,B,BF,JF) ||
            Metszi(A,B,BF,BL) ||
            Metszi(A,B,JL,JF) ||
            Metszi(A,B,JL,BL)
            )
                return true;

        return false;
    }

    bool inRenderTri(vec2 A, vec2 B, vec2 C){
        return (inRenderLine(A,B) || inRenderLine(A,C) || inRenderLine(C,B));
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
            A->addHaromszogID(ID);
            B->addHaromszogID(ID);
            C->addHaromszogID(ID);
            /*
            cout<<"New Haromszog: ID: "<<id<<", csucsok: A: "<<A->id<<", B: "<<B->id<<", C: "<<C->id<<endl;
            cout<<A<<" ";
            for (size_t i=0; i<A->haromszogID.size(); i++)
                cout<<A->haromszogID[i]<<" ";
            cout<<endl;
            cout<<B<<" ";
            for (size_t i=0; i<B->haromszogID.size(); i++)
                cout<<B->haromszogID[i]<<" ";
            cout<<endl;
            cout<<C<<" ";
            for (size_t i=0; i<C->haromszogID.size(); i++)
                cout<<C->haromszogID[i]<<" ";
            cout<<endl;
            */
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

class Fal{
    View* view;
public:
    vec2 a, b, c;

    Fal(vec2 A, vec2 B, vec2 C, View* viewcs){
        a=A; b=B; c=C; view=viewcs;
    }

    void draw(SDL_Renderer &renderer, bool asztal=false){
        vec2 rela = view->getRelativePos(a);
        vec2 relb = view->getRelativePos(b);
        vec2 relc = view->getRelativePos(c);
        if (asztal)
            filledTrigonRGBA(&renderer,rela.x,rela.y,relb.x,relb.y,relc.x,relc.y,207,165,141,255);
        else
            filledTrigonRGBA(&renderer,rela.x,rela.y,relb.x,relb.y,relc.x,relc.y,45,45,45,255);
    }
};

class Wall{

public:
    Csucs *cs1, *cs2;

    Wall(){}

    Wall(Csucs *a, Csucs *b){
        if (a->id < b->id) {
            cs1=a; cs2=b;
        } else {
            cs1=b; cs2=a;
        }
    }

};

inline bool operator==(const Wall& lhs, const Wall& rhs){
    if (lhs.cs1->id == rhs.cs1->id){
        if (lhs.cs2->id == rhs.cs2->id)
            return true;
    }
    return false;
}

/*
inline bool operator<(const Wall& lhs, const Wall& rhs){
    /// Aranyat érő két sor
    if (lhs.ossz==rhs.ossz){
        if (lhs.eddigMegtett == rhs.eddigMegtett){
            return lhs.csucs.id > rhs.csucs.id;
        }
        return lhs.eddigMegtett > rhs.eddigMegtett;
    }
    /// Eddig
    // orvosolt rengeteg mindent + az uthossz várakozásnál 0 most
    return lhs.ossz < rhs.ossz;
}
*/



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

    float csucsR = 2.f;

    int idx_cnt = 0;

    list<Csucs> csucsok;
    int listCsL = 0;

    vector<Csucs> csucsokV;
    int vecCsL = 0;

    vector<vector<vector<int>>> teruletekFoglaltsagaCsucs;
    int szeleteltseg = 4, palyameret = 16;
    /// egy terület 4 pixel széles, a kezdõ képernyõ kétszeresét fedi le ez a megoldás

    list<Haromszog> haromszogek;
    vector<vector<vector<int>>> teruletekFoglaltsagaHaromszog;
    int szeleteltsegH = 4;//, palyameretH = 2;

    list<Wall> falak;
    int falakCnt = 0;
    vector<Wall> falakV;
    int falakVCnt = 0;


    vector<Fal> diszFalak;
    vector<Fal> diszAsztalok;
    bool csucsokDrawSwitch=false, diszfalakDrawSwitch=true, falakDrawSwitch=false, diszAsztalokDrawSwitch=true, haromszogekDrawSwitch=false;


    Csucs failSafeCs = Csucs();
    Haromszog failSafeHar = Haromszog();

    Csucs* getCsucsP(int idx){
        if (idx==-1)
            return &failSafeCs;
        list<Csucs>::iterator ptr = csucsok.begin();
        advance(ptr,idx);
        return &*ptr;
    }

    Csucs getCsucsR(int idx){
        if (listCsL!=vecCsL){
            csucsokV.resize(csucsok.size());
            list<Csucs>::iterator it = csucsok.begin();
            for (size_t i=0; i<csucsok.size(); i++){
                csucsokV[i]=(*it);
                it++;
            }
            listCsL=csucsok.size();
            vecCsL=csucsokV.size();
        }
        if (listCsL!=vecCsL)
            cout<<"Error getCsucsR: "<<listCsL<<" "<<vecCsL<<endl;
        if (idx==-1)
            return Csucs();
        return csucsokV[idx];
    }

    void frissitFalakV(){
        if (falakCnt!=falakVCnt){
            falakV.resize(falak.size());
            list<Wall>::iterator it = falak.begin();
            for (int i=0; it!=falak.end();i++){
                falakV[i]=*it;
                it++;
            }
            falakVCnt=falakV.size();
        }
    }

    Haromszog* getHaromszogP(int idx){
        list<Haromszog>::iterator ptr = haromszogek.begin();
        advance(ptr,idx);
        return &*ptr;
    }

    Haromszog getHaromszogR(int idx){
        if (idx==-1)
            return Haromszog();
        list<Haromszog>::iterator ptr = haromszogek.begin();
        advance(ptr,idx);
        return *ptr;
    }

    void saveMapToFile(string file){
        ofstream F; F.open(file);
        list<Haromszog>::iterator it = haromszogek.begin();
        for (size_t i = 0; i<haromszogek.size(); i++){
            F<<(*it).A->pos.x<<" "<<(*it).A->pos.y<<endl;
            F<<(*it).B->pos.x<<" "<<(*it).B->pos.y<<endl;
            F<<(*it).C->pos.x<<" "<<(*it).C->pos.y<<endl;
            F<<endl;
            it++;
        }
        F.close();
    }

    bool inSquare(vec2 *p){
        if (p->x>=-SZELES*palyameret/2 && p->y>=-MAGAS*palyameret/2){
            if (p->x<SZELES*palyameret/2 && p->y<MAGAS*palyameret/2)
                return true;
        }
        return false;
    }

    Data() {
        //clock_t t = clock();
        vector<vector<int>> z; z.resize(MAGAS*palyameret/szeleteltseg);
        teruletekFoglaltsagaCsucs.resize(SZELES*palyameret/szeleteltseg);
        teruletekFoglaltsagaHaromszog.resize(SZELES*palyameret/szeleteltseg);
        for (int i=0; i<SZELES*palyameret/szeleteltseg; i++){
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
            cout<<"new csucs"<<endl;
            //Csucs a = Csucs(csucs);

            csucsok.push_back(Csucs(csucs));
            listCsL++;
            if ((size_t)getCsucsR(csucsok.size()-1).id!=csucsok.size()-1){
                if (DEBUG) cout<<"BAJ VAN! "<<getCsucsR(csucsok.size()-1).id<<" "<<csucsok.size()-1<<endl;
            }
            size_t id = csucsok.size()-1;
            vec2 nxy = csucs+vec2(SZELES*palyameret/2,MAGAS*palyameret/2);
            vec2 n2xy = nxy/szeleteltseg;
            if (DEBUG) cout<<"ID: "<<id<<endl;
            cout<<n2xy.x<<" "<<n2xy.y<<" "<<SZELES*palyameret/szeleteltseg-1<<" "<<csucs.x<<" "<<csucs.y<<" "<<nxy.x<<" "<<nxy.y<<endl;
            for (int i=n2xy.x-2; i<=n2xy.x+2; i++){
                for (int j=n2xy.y-2; j<=n2xy.y+2; j++){
                    if (n2xy.x<0 || n2xy.y<0 || n2xy.x>SZELES*palyameret/szeleteltseg-1 || n2xy.y>SZELES*palyameret/szeleteltseg-1){
                        cout<<n2xy.x<<" "<<n2xy.y<<" "<<SZELES*palyameret/szeleteltseg-1<<" "<<csucs.x<<" "<<csucs.y<<" "<<nxy.x<<" "<<nxy.y<<endl;
                        //cout<<"K";
                    }
                    else {
                        cout<<(vec2(i,j)*szeleteltseg-nxy).length()<<" "<<(vec2(i,j)*szeleteltseg-nxy).x<<" "<<(vec2(i,j)*szeleteltseg-nxy).y<<" "<<csucsR<<" "<<csucsR*3.f<<endl;
                        if ((vec2(i,j)*szeleteltseg-nxy).length()<csucsR*3.f){
                            //
                            cout<<"ADD"<<endl;
                            teruletekFoglaltsagaCsucs[i][j].push_back(id);
                        }
                    }
                }
                cout<<endl;
            }
            if (DEBUG) cout<<endl;
            return getCsucsR(csucsok.size()-1).id;
        }
        if (DEBUG) cout<<"CsucsID: "<<CsucsID<<" "<<csucs.x<<" "<<csucs.y<<endl;
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
        minX/=szeleteltseg, minY/=szeleteltseg, maxX/=szeleteltseg, maxY/=szeleteltseg;



        for (int i=minX-1; i<=maxX+1; i++){
            for (int j=minY-1; j<=maxY+1; j++){
                if (i>=0 && j>=0 && i<SZELES*palyameret/szeleteltseg && j<MAGAS*palyameret/szeleteltseg){
                    teruletekFoglaltsagaHaromszog[i][j].push_back(id);
                }
            }
        }
    }

    int GHIFP=0;

    int getHaromszogIDFromPont (vec2 *real){
        GHIFP++;
        int HID = -1;
        if (inSquare(real)){
            vec2 epos2 = (*real+vec2(SZELES*palyameret/2,MAGAS*palyameret/2))/szeleteltseg;
            vector<int> temp = teruletekFoglaltsagaHaromszog[epos2.x][epos2.y];

            if (temp.size()>0){
                for (size_t i=0; i<temp.size(); i++){
                    if (getHaromszogR(temp[i]).benneVanAPont(*real)){
                        HID = temp[i];
                        ///cout<<"HID: "<<temp[i]<<endl;
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
        Csucs *ac=getCsucsP(a), *bc=getCsucsP(b), *cc=getCsucsP(c);

        haromszogek.push_back(Haromszog(ac,bc,cc,haromszogek.size()));
        //cout<<"HAROMSZOGEK.SIZE: "<<haromszogek.size()<<endl;
        bool ah = (*ac).uj, bh=(*bc).uj, ch=(*cc).uj;


        if (ah || bh){
            Wall temp(ac,bc);
            falak.push_back(temp);
            //cout<<"ujAB"<<endl;
        } else {
            list<Wall>::iterator temp = find(falak.begin(), falak.end(), Wall(ac,bc));
            if (temp != falak.end()){
                falak.erase(temp);
                //cout<<"torolAB"<<endl;
            } else {
                Wall temp1(ac,bc);
                falak.push_back(temp1);
                //cout<<"ujCB"<<endl;
            }
        }

        if (ah || ch){
            Wall temp(ac,cc);
            falak.push_back(temp);
            //cout<<"ujAC"<<endl;
        } else {
            list<Wall>::iterator temp = find(falak.begin(), falak.end(), Wall(ac,cc));
            if (temp != falak.end()){
                falak.erase(temp);
                //cout<<"torolAC"<<endl;
            } else {
                Wall temp1(ac,cc);
                falak.push_back(temp1);
                //cout<<"ujCB"<<endl;
            }
        }
        if (ch || bh){
            Wall temp(cc,bc);
            falak.push_back(temp);
            //cout<<"ujCB"<<endl;
        } else {
            list<Wall>::iterator temp = find(falak.begin(), falak.end(), Wall(cc,bc));
            if (temp != falak.end()) {
                falak.erase(temp);
                //cout<<"torolCB"<<endl;
            } else {
                Wall temp1(cc,bc);
                falak.push_back(temp1);
                //cout<<"ujCB"<<endl;
            }
        }




        //cout<<"falak szama: "<<falak.size()<<endl;
        //printAll();

        ac->uj=false;
        bc->uj=false;
        cc->uj=false;

        haromszogTeruletfoglalas(haromszogek.size()-1,a,b,c);

        //for (int i=0; i<csucsok.size(); i++)
            //cout<<"Csucs: "<<csucsok[i].pos.x<<" "<<csucsok[i].pos.y<<endl;
        falakCnt=falak.size();
        return 0;
    }

    bool haromszogInView(Haromszog *haromszog, View *view){
        if (view->inRenderCircle(haromszog->A->pos,0) || view->inRenderCircle(haromszog->B->pos,0) || view->inRenderCircle(haromszog->C->pos,0))
            return true;
        //if ()
    }

    void draw(SDL_Renderer &renderer, View view){

        for (size_t i=0; i<haromszogek.size() && haromszogekDrawSwitch; i++){
            list<Haromszog>::iterator ptr = haromszogek.begin();
            advance(ptr,i);
            vec2 A = view.getRelativePos(ptr->A->pos);
            vec2 B = view.getRelativePos(ptr->B->pos);
            vec2 C = view.getRelativePos(ptr->C->pos);
            //bool a1 = view.inRender(cs1,0), a2 = view.inRender(cs2,0), a3 = view.inRender(cs3,0);
            //if (a1||a2||a3)
            if (view.inRenderTri(A,B,C)){
                filledTrigonRGBA(&renderer,A.x,A.y,B.x,B.y,C.x,C.y,130,140,210,100);
            //if (a1||a2)
                lineRGBA(&renderer,A.x,A.y,B.x,B.y,255,108,160,200);
            //if (a1||a3)
                lineRGBA(&renderer,A.x,A.y,C.x,C.y,255,108,160,200);
            //if (a3||a2)
                lineRGBA(&renderer,C.x,C.y,B.x,B.y,255,108,160,200);
            }
        }


        list<Wall>::iterator it = falak.begin();
        for (size_t i=0; i<falak.size() && falakDrawSwitch; i++){
            vec2 A = view.getRelativePos(it->cs1->pos), B = view.getRelativePos(it->cs2->pos);
            if (view.inRenderLine(A,B))
                lineRGBA(&renderer,A.x,A.y,B.x,B.y,50,50,255,200);
            it++;
        }

        for (int i=0; i<diszAsztalok.size() && diszAsztalokDrawSwitch; i++){
            if (view.inRenderTri(view.getRelativePos(diszAsztalok[i].a),view.getRelativePos(diszAsztalok[i].b),view.getRelativePos(diszAsztalok[i].c)))
                diszAsztalok[i].draw(renderer,true);
        }

        for (int i=0; i<diszFalak.size() && diszfalakDrawSwitch; i++){
            if (view.inRenderTri(view.getRelativePos(diszFalak[i].a),view.getRelativePos(diszFalak[i].b),view.getRelativePos(diszFalak[i].c)))
                diszFalak[i].draw(renderer,false);
        }


        for (size_t i=0; i<csucsok.size() && csucsokDrawSwitch; i++){
            vec2 RelPos = view.getRelativePos(getCsucsR(i).pos.x,getCsucsR(i).pos.y);
            if (view.inRenderCircle(RelPos,csucsR)){
                filledCircleRGBA(&renderer,RelPos.x,RelPos.y,csucsR*view.zoom,255,150,34,255);
                stringRGBA(&renderer,RelPos.x,RelPos.y,to_string(i).c_str(),255,255,255,255);
            }
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


    void createFal(vec2 A, vec2 B, vec2 C, View* view){
        Fal temp(A,B,C,view);
        diszFalak.push_back(temp);
    }

    void loadFileFalak(string file, View* view){
        ifstream F; F.open(file);
        if (F.fail())
            return;
        int cnt = 0;
        vec2 tempCsucsok[3];
        while (!F.eof()){
            float x, y;
            F>>x>>y;
            tempCsucsok[cnt] = vec2(x,y);
            cnt++;
            if (cnt==3){
                Fal temp(tempCsucsok[0],tempCsucsok[1],tempCsucsok[2],view);
                diszFalak.push_back(temp);
                cnt=0;
            }
        }
    }

    void loadFileAsztalok(string file, View* view){
        ifstream F; F.open(file);
        if (F.fail())
            return;
        int cnt = 0;
        vec2 tempCsucsok[3];
        while (!F.eof()){
            float x, y;
            F>>x>>y;
            tempCsucsok[cnt] = vec2(x,y);
            cnt++;
            if (cnt==3){
                Fal temp(tempCsucsok[0],tempCsucsok[1],tempCsucsok[2],view);
                diszAsztalok.push_back(temp);
                cnt=0;
            }
        }
    }
};

bool FalonKivulCsucsokKozt(Csucs *A1, Csucs *A2, Data *data){
    falonkiv++;
    vec2 kerdeses = A1->pos + (A2->pos-A1->pos).normalize();
    if (data->getHaromszogIDFromPont(&kerdeses)==-1){
        return true;
    }
    return false;
}




struct CreateTri{
    int state = 0;

    vec2 csucsok[3];
    bool newcs[3];
    int csucsokID[3];

    View* view;
    Data* data;

    int radius = 5;

    CreateTri(View* v, Data* d){view=v; data=d;}

    void loadFile(string file){
        ifstream F; F.open(file);
        if (F.fail()){
                cout<<"fajl betoltes hiba"<<endl;
            return;
        }
        state = 0;
        while (!F.eof()){
            float x, y;
            F>>x>>y;

            if (state>=0 && state<3){
                csucsok[state] = vec2(x,y); //view->getRealPos(x,y);

                bool ok = true;
                for (int i=0; i<state; i++){
                    if ((csucsok[i]-csucsok[state]).length()<radius/2.f){
                        ok = false;
                    }
                }

                if (ok){
                    csucsokID[state] = data->addNewCsucs(csucsok[state]);
                    //newcs[state] = true;
                    state++;
                }
            }
            //cout<<state<<endl;
            if (state==3){
                for (int i=0; i<3;i++){
                    if (newcs[i])
                        data->addNewCsucs(csucsok[i]);
                        //data.csucsok.push_back(Csucs(csucsok[i]));
                }
                data->createNewTris(csucsokID[0],csucsokID[1],csucsokID[2]);
                state=0;
                for (int i=0;i<3;i++)
                    csucsokID[i]=-1;
            }
        }
    }

    void getEvent(SDL_Event &ev, View view, Data &data){
        if (ev.type == SDL_KEYDOWN){
            if (ev.key.keysym.sym==SDLK_m){
                if (state>=0)
                    state=-1;
                else
                    state=0;
            }
            if (ev.key.keysym.sym==SDLK_f){
                if (radius==5)
                    radius=1;
                else
                    radius=5;
            }
        }

        if (state!=-1){
            if (ev.type == SDL_MOUSEBUTTONDOWN){
                //cout<<(int)ev.button.button<<endl;
                if (ev.button.button==1){
                    if (state>=0 && state<3){
                        csucsok[state] = view.getRealPos(ev.button.x,ev.button.y);

                        bool ok = true;
                        for (int i=0; i<state; i++){
                            if ((csucsok[i]-csucsok[state]).length()<radius*2.f){
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
    bool first = false;

    Csucs elozoCsucs;
    float elozoMegtett = 0;

    int utolsoCsucs = 0;

    vector<Csucs> eddigiCsucsok;

    UtPos(){ossz=-1;}

    UtPos(Csucs cs, float m, float b, UtPos utp=UtPos(), bool f=false){
        csucs=cs; eddigMegtett=m; becsult=b; ossz=m+b; first=f;
        if (utp.ossz!=-1){
            eddigiCsucsok=utp.eddigiCsucsok;
            if (!first){
                ///cout<<"!FIRST "<<cs.id<<" "<<utp.csucs.id<<endl;
                eddigiCsucsok.push_back(utp.csucs);

            }
        }
    }

    void removeNotValidId(){
        for (int i=0; i<eddigiCsucsok.size(); i++){
            if (eddigiCsucsok[i].id==-1){
                eddigiCsucsok.erase(eddigiCsucsok.begin()+i);
                i--;
            }
        }
    }
};

inline bool operator<(const UtPos& lhs, const UtPos& rhs){
    /// Aranyat érő két sor
    if (lhs.ossz==rhs.ossz){
        if (lhs.eddigMegtett == rhs.eddigMegtett){
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

    clock_t vagas=0, kereses=0, vagas1 = 0, vagas2 = 0, vagas3 = 0;
    clock_t t=0, t2, t3;
    int vagasok = 0, vagasok1 = 0, vagasok2 = 0;

    UtvonalKereso(Data *DATA){data=DATA;}

    UtPos UtvonalKeresesVagas(UtPos input, vec2 startP, vec2 endP){
        //int utolsoCsucs = input.utolsoCsucs;
        vagasok++;
        //if (input.utolsoCsucs!=0)
            //cout<<"YEY "<<input.utolsoCsucs<<endl;
        if (input.eddigiCsucsok.size()==0){
            // ez az első csúcs, nincs mit rövidíteni
            ///cout<<"Kezdo csucs"<<endl;
            return input;
        }
        else if (input.eddigiCsucsok.size()==1){
            ///cout<<"Egyetlen csucs"<<endl;
            // ez a második csúcs, ekkor a kiindulópontból kell indítani a szakaszt, nem csúcsból így külön eset
            t3 = clock();

            if (data->getHaromszogIDFromPont(&startP)==-1){
                cout<<"OKIDOKI"<<endl;
                return input;
            }

            list<Wall>::iterator it = data->falak.begin();
            vec2 hol = vec2(0,0);
            float A = 1000.f, B = 1000.f;
            for (size_t i=0; i<data->falak.size(); i++){
                if (MetsziNemCsakErenti(&startP,&input.csucs.pos,&((*it).cs1->pos),&((*it).cs2->pos),/*data,*/hol)){
                    //utolsoCsucs=1;
                    return input;
                }
                A = min(A,PontVonalTavolsag(startP,((*it).cs1->pos),((*it).cs2->pos)));
                B = min(B,PontVonalTavolsag(input.csucs.pos,((*it).cs1->pos),((*it).cs2->pos)));
                it++;
            }
            /*
            vec2 C = startP;
            vec2 D = input.csucs.pos;
            vec2 Depsz = ((C-D).normalize()*0.1f+D), Cepsz = ((D-C).normalize()*0.1f+C);
            bool Cok = (data->getHaromszogIDFromPont(&Cepsz) != -1);
            bool Dok = (data->getHaromszogIDFromPont(&Depsz) != -1);
            */

            float epsz = 0.01f;
            if (A < epsz && B < epsz){
                //cout<<"palacsinta"<<endl;
                return input;

            }
            UtPos ret2 = UtPos(input.csucs,(startP-input.csucs.pos).length(),input.becsult,UtPos(),true);
            vagas1+=clock()-t3;
            return ret2;

        }
        else {
            ///cout<<"YEY"<<endl;
            // közös két csúcs, de a másik két oldal közül valamelyiket elmetszi?
            vector<Csucs> utvonal; utvonal.resize(1+1+input.eddigiCsucsok.size());
            utvonal[0]=Csucs(startP, false);
            for (size_t i=0; i<input.eddigiCsucsok.size(); i++){
                utvonal[i+1]=input.eddigiCsucsok[i];
            }
            utvonal[utvonal.size()-1]=input.csucs;

            /*
            for (int i=utvonal.size()-1; i>0; i--){ // végétől nézve megnézem, hogy meddig tudok egyenesen menni a kezdőcsúcsból
                list<Wall>::iterator it = data->falak.begin();
                for (size_t j=0; j<data->falak.size(); j++){
                    if (MetsziNemCsakErenti(utvonal[0].pos,utvonal[i].pos,(*it).cs1->pos,(*it).cs2->pos))
                        break;
                    it++;
                    if (j==data->falak.size()-1){
                        vector<Csucs> temp; temp.resize(utvonal.size()-i+1);
                        temp[0]=utvonal[0];
                        for (size_t k=i; k<utvonal.size(); k++)
                            temp[k-i+1]=utvonal[k];
                        utvonal=temp;
                        i=0;
                        break;
                    }
                }
            }
            */

            ///*  // TODO
            bool A = false;
            ///cout<<"YEY"<<endl;
            //cout<<utvonal[utvonal.size()-1].id
            ///cout<<"Kifejtett Csucs id: "<<utvonal[utvonal.size()-1].id<<" "<<utvonal[utvonal.size()-1].pos.x<<" "<<utvonal[utvonal.size()-1].pos.y<<endl;

            for (size_t i=0; i<utvonal.size()-1; i++){ // elejétől nézve megnézem, hogy mettől tudok egyenesen menni a végcsúcsba
                //list<Wall>::iterator it = data->falak.begin();
                vec2 hol = vec2(0,0);
                float Atav = 1000.f, Btav = 1000.f;
                bool B = false;

                ///cout<<"   Csucs id: "<<utvonal[i].id<<" "<<utvonal[i].pos.x<<" "<<utvonal[i].pos.y<<endl; //<<data->falakV.size()<<endl;
                for (int j=0; j<data->falakV.size(); j++){
                        //cout<<"HM";
                    /*
                    if (MetsziNemCsakErenti(&utvonal[utvonal.size()-1].pos,&utvonal[i].pos,&((*it).cs1->pos),&((*it).cs2->pos),data,hol) ) // MAJD FELADAT
                        break;
                    if (!A && (utvonal[utvonal.size()-1].id==(*it).cs1->id || utvonal[utvonal.size()-1].id==(*it).cs2->id))
                        A=true;
                    //cout<<"B"<<endl;
                    if (utvonal[i].id==(*it).cs1->id || utvonal[i].id==(*it).cs2->id)
                        B=true;
                    it++;
                    */
                    if (MetsziNemCsakErenti(&utvonal[utvonal.size()-1].pos,&utvonal[i].pos,&(data->falakV[j].cs1->pos),&(data->falakV[j].cs2->pos),/*data,*/hol) ) { // MAJD FELADAT{
                        ///cout<<"TTT"<<endl;
                        break;
                    }
                    //cout<<Atav<<" "<<Btav<<endl;
                    Atav = min(Atav,PontVonalTavolsag(startP,(data->falakV[j].cs1->pos),(data->falakV[j].cs2->pos)));
                    ///if (min(Btav,PontVonalTavolsag(input.csucs.pos,(data->falakV[j].cs1->pos),(data->falakV[j].cs2->pos)))<Btav){
                        ///cout<<input.csucs.id<<" "<<input.csucs.pos.x<<" "<<input.csucs.pos.y<<" "<<data->falakV[j].cs1->id<<" "<<data->falakV[j].cs2->id<<" ";
                        ///cout<<min(Btav,PontVonalTavolsag(input.csucs.pos,(data->falakV[j].cs1->pos),(data->falakV[j].cs2->pos)))<<endl;
                    ///}
                    Btav = min(Btav,PontVonalTavolsag(input.csucs.pos,(data->falakV[j].cs1->pos),(data->falakV[j].cs2->pos)));

                    //cout<<Atav<<" "<<Btav<<endl;
                    if (!A && (utvonal[utvonal.size()-1].id==data->falakV[j].cs1->id || utvonal[utvonal.size()-1].id==data->falakV[j].cs2->id))
                        A=true;
                    //cout<<"B"<<endl;
                    if (utvonal[i].id==data->falakV[j].cs1->id || utvonal[i].id==data->falakV[j].cs2->id)
                        B=true;

                    if (j==data->falak.size()-1){
                        ///cout<<"VVV"<<endl;
                        float epsz = 0.01f;

                        vec2 C = utvonal[utvonal.size()-1].pos;
                        vec2 D = utvonal[i].pos;
                        vec2 Depsz = ((C-D).normalize()*0.1f+D), Cepsz = ((D-C).normalize()*0.1f+C);
                        bool Cok = (data->getHaromszogIDFromPont(&Cepsz) != -1);
                        bool Dok = (data->getHaromszogIDFromPont(&Depsz) != -1);

                        if (!Cok || !Dok){
                        //if (Atav < epsz && Btav < epsz ){
                            ///cout<<"palacsinta2"<<endl;
                            break;

                        }
                        if (A&&B){
                            if (FalonKivulCsucsokKozt(&utvonal[utvonal.size()-1],&utvonal[i],data)){
                                    cout<<"Talan"<<endl;
                                break;
                            }
                        }

                        ///cout<<"ZZZ"<<endl;
                        vector<Csucs> temp; temp.resize(i+2);
                        //temp[0]=utvonal[0];
                        for (size_t k=0; k<=i; k++)
                            temp[k]=utvonal[k];
                        temp[temp.size()-1]=utvonal[utvonal.size()-1];
                        utvonal=temp;
                        //input.utolsoCsucs = i;
                        i=utvonal.size();
                        break;
                    }
                }
            }
            //*/

            UtPos ret;
            ret.eddigiCsucsok=utvonal; ret.eddigiCsucsok.pop_back();
            ret.csucs=utvonal[utvonal.size()-1];
            ret.becsult = (ret.csucs.pos-endP).length();
            for (size_t i=0;i+1<utvonal.size();i++){
                ret.eddigMegtett+=(utvonal[i+1].pos-utvonal[i].pos).length();
            }
            ret.ossz=ret.becsult+ret.eddigMegtett;
            //ret.utolsoCsucs=input.utolsoCsucs;
            return ret;
            //return input;
        }

    }

    vector<UtvonalElem> UtvonalKeresesBasic(vec2 RealStart, vec2 RealEnd, int HIDcel, int HIDstart){

        //set<Csucs> csucsok;

        //Haromszog h1 = data->getHaromszogR(HIDstart);
        bool Tdebug = false;
        bool Tdebug2 = false;
        bool Tdebug3 = false;

        set<int> celIDk, startIDk;
        startIDk.insert(data->getHaromszogR(HIDstart).A->id); startIDk.insert(data->getHaromszogR(HIDstart).B->id); startIDk.insert(data->getHaromszogR(HIDstart).C->id);
        if (Tdebug) cout<<"data->getHaromszogR(HIDstart).A->id "<<data->getHaromszogR(HIDstart).A->id<<" "<<data->getHaromszogR(HIDstart).B->id<<" "<<data->getHaromszogR(HIDstart).C->id<<endl;
        if (Tdebug) cout<<"data->getHaromszogR(HIDcel).A->id "<<data->getHaromszogR(HIDcel).A->id<<" "<<data->getHaromszogR(HIDcel).B->id<<" "<<data->getHaromszogR(HIDcel).C->id<<endl;
        celIDk.insert(data->getHaromszogR(HIDcel).A->id); celIDk.insert(data->getHaromszogR(HIDcel).B->id); celIDk.insert(data->getHaromszogR(HIDcel).C->id);
        vector<UtvonalElem> ret, emptyRet;
        vector<UtvonalElem> retBest;
        float shortestPath = 1000000.f;
        set<UtPos> csucsok;
        set<UtPosSeged> visitedCsucsId;
        Haromszog h1 = data->getHaromszogR(HIDstart);
        // kiválasztom a kezdő három csúcsot, melyekkel kezdek el dolgozni
        UtPos starter1 = UtPos(*(h1.A),(h1.A->pos - RealStart).length(),(h1.A->pos - RealEnd).length(),UtPos(),true);
        UtPos starter2 = UtPos(*(h1.B),(h1.B->pos - RealStart).length(),(h1.B->pos - RealEnd).length(),UtPos(),true);
        UtPos starter3 = UtPos(*(h1.C),(h1.C->pos - RealStart).length(),(h1.C->pos - RealEnd).length(),UtPos(),true);
        csucsok.insert(starter1); csucsok.insert(starter2); csucsok.insert(starter3);
        // ezeknek az id-ját is elmentem, hogy ne derítsem fel őket később újra
        visitedCsucsId.insert(UtPosSeged(h1.A->id,starter1.ossz)); visitedCsucsId.insert(UtPosSeged(h1.B->id,starter2.ossz)); visitedCsucsId.insert(UtPosSeged(h1.C->id,starter3.ossz));
        //amíg ki nem fogyunk csúcsokból

        int celIDcnt = 0;       // nem csak a célháromszög egyik csúcsát fejtem ki, hanem közelező az összeset is
        set<int> felfedezettCelIDk; // ebbe gyűjtöm a felfedezetteket

        while (csucsok.size()>0){
            if (Tdebug) cout<<"csucsok.size() "<<csucsok.size()<<endl;

            UtPos temp = *(csucsok.begin());    // kiválasztom az első legalkalmasabbad
            if (Tdebug) cout<<"csucs ossz "<<temp.ossz<<endl;
            csucsok.erase(csucsok.begin());     // törlöm is
            if (Tdebug2){
                cout<<"Csucs ID: "<<temp.csucs.id<<" "<<temp.becsult<<" "<<temp.eddigMegtett<<" "<<temp.elozoMegtett<<" "<<temp.ossz<<endl;
                for (size_t i=0; i<temp.eddigiCsucsok.size(); i++)
                    cout<<temp.eddigiCsucsok[i].id<<" ";
                cout<<endl;
            }

            // először megvizsgálom, hogy ez a célháromszög egyik csúcsa-e,
            // ha rátérek ennek a csúcsnak a kifejtésére, akkor biztosan nem tudok ide már gyorsabban eljutni, így ez a legjobb út
            if (celIDk.find(temp.csucs.id)!=celIDk.end()){
                UtPos temp2 = temp;
                // debug
                //cout<<"hapci1"<<endl;
                if (felfedezettCelIDk.find(temp.csucs.id)==felfedezettCelIDk.end()){ // ha még nem volt felfedezve
                    celIDcnt++; felfedezettCelIDk.insert(temp.csucs.id); // akkor ez újnak számít, és el is mentem a csúcs id-ját
                }
                if (Tdebug) cout<<temp.csucs.id<<" "<<*(celIDk.find(temp.csucs.id))<<" "<<celIDk.size()<<endl;

                temp.eddigiCsucsok.push_back(temp.csucs);
                temp.csucs=Csucs(RealEnd,false);  // TODO
                temp = UtvonalKeresesVagas(temp,RealStart,RealEnd); //re
                temp.removeNotValidId();

                if (Tdebug2){
                    for (size_t i=0; i<temp.eddigiCsucsok.size(); i++)
                        cout<<temp.eddigiCsucsok[i].id<<" ";
                    cout<<endl;
                }

                vector<Csucs> utvonal = temp.eddigiCsucsok; // mely csúcsokon is haladtunk végig?
                if (Tdebug) cout<<"utvonal.size() "<<utvonal.size()<<endl; // debug
                utvonal.push_back(temp.csucs); // kéne a végére a most kifejtendő súcs is
                if (utvonal[0].id==-1)
                    utvonal.erase(utvonal.begin());
                if (utvonal[utvonal.size()-1].id==-1)
                    utvonal.pop_back();
                if (utvonal.size()==0){
                    ret.push_back(UtvonalElem(RealStart,RealEnd));
                    if (Tdebug2) cout<<"AHA"<<endl;
                    return ret;
                }
                float pathLenght = 0;
                UtvonalElem elem = UtvonalElem(RealStart,utvonal[0].pos); // és az első útszakasz, ahol még a csúcs nélkül indul ki
                pathLenght+=(RealStart-utvonal[0].pos).length();
                if (Tdebug2) {
                    cout<<"pathLenght start: "<<pathLenght<<" "<<utvonal.size()<<endl;
                    for (size_t j=0; j<utvonal.size(); j++)
                        cout<<utvonal[j].id<<" ";
                    cout<<"kalap: ";
                }
                ret.push_back(elem); // ez lesz az első útszakasz
                if (utvonal.size()>0){
                    for (size_t j=0;j<utvonal.size()-1;j++){ // útszakaszokat létrehozom az összes csúcsra
                        ret.push_back(UtvonalElem(utvonal[j].pos,utvonal[j+1].pos));
                        pathLenght+=(utvonal[j].pos-utvonal[j+1].pos).length();
                        if (Tdebug2) cout<<utvonal[j].id<<" "<<pathLenght<<" ";
                    }
                }
                if (Tdebug2) cout<<endl;

                ret.push_back(UtvonalElem(utvonal[utvonal.size()-1].pos,RealEnd)); // és a végpontba is az utoló csúccsal
                pathLenght+=(RealEnd-utvonal[utvonal.size()-1].pos).length();
                if (Tdebug2) cout<<"pathLenght end: "<<pathLenght<<endl;
                if (Tdebug) cout<<"Gabriel"<<endl;
                if (pathLenght<shortestPath){
                    retBest=ret;
                    shortestPath=pathLenght;
                }
                if (Tdebug2) cout<<"RetBest: "<<retBest.size()<<endl;

                ret=emptyRet;
                if (Tdebug2) cout<<"RetBest: "<<retBest.size()<<" "<<celIDcnt<<" "<<pathLenght<<temp2.csucs.id<<endl;
                if (celIDcnt==3){
                    if (Tdebug2) cout<<"RetBest: "<<retBest.size()<<" "<<shortestPath<<endl;
                    return retBest; // és itt végeztünk is

                }


                temp=temp2;
            }

            ///cout<<"hapci2"<<endl;
            vector<int> haromszogek = ((data->getCsucsP(temp.csucs.id))->haromszogID); //temp.csucs.haromszogID;   // és lekérem a hozzá tartozó háromszögök ID-ját
            ///cout<<&((data->getCsucsP(temp.csucs.id))->haromszogID)<<" haromszogID Size: "<<haromszogek.size()<<endl;
            for (size_t i=0; i<haromszogek.size(); i++){ // ezeken zongorázok végig
                Haromszog tempH = data->getHaromszogR(haromszogek[i]);  // lekérem a konkrét háromszöget

                ///cout<<"   haromszog ID: "<<tempH.id<<", csucsok: "<<tempH.A->id<<" "<<tempH.B->id<<" "<<tempH.C->id<<endl;

                //cout<<"tempH IDA: "<<tempH.A->id<<endl;
                //3-szor megejtem ez, a háromszög 3 csúcsára egyszer-egyszer
                // létrehozom a lehetséges új köztes uticélt, hogy melyik csúcsról is lenne szó, mennyi az addig megtett út, a becsült hátralévő és az előző megálló
                UtPos cs1 = UtPos(*(tempH.A),temp.eddigMegtett+(tempH.A->pos - temp.csucs.pos).length(),(tempH.A->pos - RealEnd).length(),temp);
                /* RÖVIDÍTÉS */
                //cs1 = UtvonalKeresesVagas(cs1,RealStart);
                //ehhez kell egy kis segítség is

                if (startIDk.find(tempH.A->id)==startIDk.end()){
                    t=clock();
                    cs1 = UtvonalKeresesVagas(cs1,RealStart,RealEnd);
                    cs1.removeNotValidId();

                    /*
                    //cout<<"kifejtes: Csucs ID: "<<cs1.csucs.id<<", "<<cs1.
                    cout<<"      kifejtes: Csucs ID: "<<cs1.csucs.id<<" "<<cs1.becsult<<" "<<cs1.eddigMegtett<<" "<<cs1.elozoMegtett<<" "<<cs1.ossz<<endl;
                    cout<<"         ";
                    for (size_t i=0; i<cs1.eddigiCsucsok.size(); i++)
                        cout<<cs1.eddigiCsucsok[i].id<<" ";
                    cout<<endl;
                    */

                    vagas += clock()-t;
                    UtPosSeged segA(tempH.A->id,cs1.ossz);
                    // ugyanis az ID-kat nem akarom felfedezni újra, csak ha találtam jobb megközelítést, ahhoz a csúcshoz
                    if (visitedCsucsId.find(segA)!=visitedCsucsId.end()){

                        // ha már felfedeztük egyszer a csúcsot, akkor kérdés, hogy ez jobb megközelítést adott-e hozzá
                        if ((*(visitedCsucsId.find(segA))).ossz>segA.ossz){
                            // ha a felülírni kívánt eddigi megközelítésnél jobb a mostani
                            visitedCsucsId.erase(visitedCsucsId.find(segA));
                            //kitöröljük az előzőt
                            visitedCsucsId.insert(segA);
                            // felülírjuk a sajátommal
                            if (Tdebug) cout<<"ADDCSUCS"<<endl;
                            csucsok.insert(cs1);
                            // és így az útvonalat is hozzáadom, a megállóhelyet
                        }
                    } else {
                        // ha meg még nem fedeztem fel, akkor mindent hozzáadok törlés nélkül
                        visitedCsucsId.insert(segA);
                        if (Tdebug) cout<<"ADDCSUCS"<<endl;
                        csucsok.insert(cs1);
                    }
                }

                // ugyan ezt a B...
                UtPos cs2 = UtPos(*(tempH.B),temp.eddigMegtett+(tempH.B->pos - temp.csucs.pos).length(),(tempH.B->pos - RealEnd).length(),temp);
                ///cout<<"tempH IDB: "<<tempH.B->id<<endl;
                if (startIDk.find(tempH.B->id)==startIDk.end()){
                    t =clock();
                    cs2 = UtvonalKeresesVagas(cs2,RealStart,RealEnd);
                    cs2.removeNotValidId();

                    /*
                    cout<<"      kifejtes: Csucs ID: "<<cs2.csucs.id<<" "<<cs2.becsult<<" "<<cs2.eddigMegtett<<" "<<cs2.elozoMegtett<<" "<<cs2.ossz<<endl;
                    cout<<"         ";
                    for (size_t i=0; i<cs2.eddigiCsucsok.size(); i++)
                        cout<<cs2.eddigiCsucsok[i].id<<" ";
                    cout<<endl;
                    */

                    vagas += clock()-t;
                    UtPosSeged segB(tempH.B->id,cs2.ossz);
                    if (visitedCsucsId.find(segB)!=visitedCsucsId.end()){
                        if ((*(visitedCsucsId.find(segB))).ossz>segB.ossz){
                            visitedCsucsId.erase(visitedCsucsId.find(segB));
                            visitedCsucsId.insert(segB);
                            if (Tdebug) cout<<"ADDCSUCS"<<endl;
                            csucsok.insert(cs2);
                        }
                    } else {
                        visitedCsucsId.insert(segB);
                        if (Tdebug) cout<<"ADDCSUCS"<<endl;
                        csucsok.insert(cs2);
                    }
                }

                // és C csúcsra is
                UtPos cs3 = UtPos(*(tempH.C),temp.eddigMegtett+(tempH.C->pos - temp.csucs.pos).length(),(tempH.C->pos - RealEnd).length(),temp);
                ///cout<<"tempH IDC: "<<tempH.C->id<<endl;
                if (startIDk.find(tempH.C->id)==startIDk.end()){
                    t =clock();
                    cs3 = UtvonalKeresesVagas(cs3,RealStart,RealEnd);
                    cs3.removeNotValidId();

                    /*
                    cout<<"      kifejtes: Csucs ID: "<<cs3.csucs.id<<" "<<cs3.becsult<<" "<<cs3.eddigMegtett<<" "<<cs3.elozoMegtett<<" "<<cs3.ossz<<endl;
                    cout<<"         ";
                    for (size_t i=0; i<cs3.eddigiCsucsok.size(); i++)
                        cout<<cs3.eddigiCsucsok[i].id<<" ";
                    cout<<endl;
                    */

                    vagas += clock()-t;
                    UtPosSeged segC(tempH.C->id,cs3.ossz);
                    if (visitedCsucsId.find(segC)!=visitedCsucsId.end()){
                        if ((*(visitedCsucsId.find(segC))).ossz>segC.ossz){
                            visitedCsucsId.erase(visitedCsucsId.find(segC));
                            visitedCsucsId.insert(segC);
                            if (Tdebug) cout<<"ADDCSUCS"<<endl;
                            csucsok.insert(cs3);
                        }
                    } else {
                        visitedCsucsId.insert(segC);
                        if (Tdebug) cout<<"ADDCSUCS"<<endl;
                        csucsok.insert(cs3);
                    }
                }
            }
        }

        if (retBest.size()!=0){
            cout<<"Error: "<<celIDcnt<<endl;
            return retBest;
        }
        return ret;
    }

    vector<UtvonalElem> UtvonalKereses(vec2 RealStart, vec2 RealEnd){
        vector<UtvonalElem> ret;
        data->frissitFalakV();
        int HIDstart = data->getHaromszogIDFromPont(&RealStart);
        int HIDcel = data->getHaromszogIDFromPont(&RealEnd);

        //cout<<(float)HIDstart<<" "<<true<<" "<<(HIDstart!=-1)<<endl;
        cout<<"keres "<<HIDstart<<" "<<HIDcel<<endl;
        if (HIDstart<0 || HIDcel<0){
            return ret;
        } else {
            if (HIDcel==HIDstart){
                //cout<<(float)HIDstart<<" aha "<<true<<" "<<(HIDstart!=-1)<<endl;
                ret.push_back(UtvonalElem(RealStart,RealEnd));
            }
            else{
                t2 = clock();
                ret = UtvonalKeresesBasic(RealStart,RealEnd,HIDcel,HIDstart);
            kereses += clock()-t2;
            }
        }

        return ret;
    }
};

struct ajtoSeged{
    vec2 csuklo, ajtoVege;
    float hossz, szog, vastagsag;

    ajtoSeged(){}

    ajtoSeged(vec2 cs, vec2 av, float h, float sz, float v){
        csuklo=cs; ajtoVege=av; hossz=h; szog=sz; vastagsag=v;
    }

    vec2 realProjection(vec2 P, vec2 A, vec2 B, bool tovabb = false, bool aranyt=false){
        float x = (P-A).length();
        float y = (P-B).length();
        float z = (A-B).length();
        float a = (x*x - y*y + z*z) / (2.f*z);
        if (!tovabb){
            if (a<=0)
                return A;
            if (a>=z)
                return B;
        }
        if (aranyt)
            return vec2(a,0);
        vec2 ret = (A*(z-a)+B*a)/z;
        return ret;
    }


    bool jatekosOnTheHouse(vec2 pos, float radius){
        vec2 Pv1 = realProjection(pos,csuklo,ajtoVege,false);
        vec2 Pv2 = realProjection(pos,csuklo,ajtoVege,true);
        if (Pv2!=Pv1 && (Pv2-Pv1).length()<radius+vastagsag/2.0f && (pos-Pv2).length()<radius+vastagsag/2.0f){
            vec2 orthoP = (ajtoVege + (ajtoVege-csuklo).ortho().normalize()*vastagsag/2.0f);
            vec2 orthoM = (ajtoVege - (ajtoVege-csuklo).ortho().normalize()*vastagsag/2.0f);
            //cout<<"OKIDOKI "<<true<<((((pos-orthoM).length()<radius) || ((pos-orthoP).length()<radius)) && ((pos-Pv1).length()<radius+vastagsag/2.0f))<<endl;
            return ((((pos-orthoM).length()<radius) || ((pos-orthoP).length()<radius)) && ((pos-Pv1).length()<radius+vastagsag/2.0f));
        } else {
            return ((pos-Pv1).length()<radius+vastagsag/2.0f);
        }
    }

    vec2 helpForPlayer(vec2 pos, float rad){
        vec2 orthoP = (csuklo + (ajtoVege-csuklo).ortho());
        vec2 orthoM = (csuklo - (ajtoVege-csuklo).ortho());
        if ((orthoM-pos).length()<(orthoP-pos).length()){
            return (ajtoVege-csuklo).normalize();
        } else {
            return (ajtoVege-csuklo).normalize();
        }
    }
};

class Player2{
    Data *data;
    UtvonalKereso *gps;
    View *view;

    vec2 velo;

    //int r = rand2(), g = rand2(), b = rand2();

    int rang = 0;



    vector<UtvonalElem> utvonal;
    float utvonalHossz = 0;


    //int aktualisUtvonalCel = 0;

    char red = rand()%256, green = rand()%256, blue = rand()%256;

    int falonkeresztulvezet = 0;
    int maxFalonKeresztul = 2;

public:

    vec2 avgSpeed;
    int cntSample = 0, maxSample = 30;
    float maxRealVelo = 0.1f;
    float maxSpeed = 0.3f;

    static bool stop;
    static bool debugDraw;

    float speed, radius, mass;
    float ajtoHelpStr = 2.0f;

    float nextUtPosDis = 0, upcomingUtPosDis = 0;

    vec2 pos;

    int team = 1;

    int id=-1;

    vec2 cel;
    bool vanUtvonalVege = false;


    Player2(){}


    Player2(Data *Ddata, View *View, UtvonalKereso *GPS,
             vec2 Pos = vec2(0,0), int player_team=0, float Speed = 100.f, float Radius = 5.f, float Mass = 1.f, vec2 Velo = vec2(0,0)){
        data=Ddata; view=View; gps=GPS; team = player_team;
        pos=Pos; speed=Speed; radius=Radius; mass=Mass; velo=Velo;
    }


    void preSimulate(float dt, vector<Player2> players, ajtoSeged ajtoSeg){
        vec2 preVelo(0,0);
        if (stop)
            return;
        //cout<<"Sim "<<dt<<endl;

        if (utvonal.size()>0){
            //if (utvonal.size()>0)
                preVelo += (vec2(utvonal[0].posEnd.x, utvonal[0].posEnd.y) - pos).normalize()*dt*speed;

        }

        for (int i=0; i<players.size(); i++){
            if ((players[i].pos-pos).length()<radius+players[i].radius){
                if (players[i].team>=team) {
                    preVelo+=(pos-players[i].pos).normalize()*(radius+players[i].radius - (players[i].pos-pos).length())*1.0f/**(radius+2.f)*/*dt*speed*2.5f;
                } else {
                    cout<<"ALMA"<<endl;
                    preVelo+=(pos-players[i].pos).normalize()*(radius+players[i].radius - (players[i].pos-pos).length())*1.0f/**(radius+2.f)*/*dt*speed/2.0f;
                }
            }
        }


        if (preVelo.length()>1.5f)
            preVelo = preVelo.normalize()*1.5f;


        velo+=preVelo;
        vec2 EO = (ajtoSeg.csuklo-ajtoSeg.ajtoVege).ortho().normalize()*ajtoSeg.vastagsag/2.0f;
        vec2 MO = ajtoSeg.csuklo-EO;
        EO = ajtoSeg.csuklo+EO;
        bool joOldalt = utvonal.size()>0 && ( ((utvonal[0].posEnd-EO).length() < (utvonal[0].posEnd-MO).length()) && ((pos-EO).length() < (pos-MO).length()) ||
                                              !((utvonal[0].posEnd-EO).length() < (utvonal[0].posEnd-MO).length()) && !((pos-EO).length() < (pos-MO).length()));
        if (!joOldalt && ajtoSeg.jatekosOnTheHouse(pos,radius+0.2f)){
            vec2 P  = realProjection(pos,ajtoSeg.csuklo,ajtoSeg.ajtoVege);
            vec2 Pv = realProjection(pos,ajtoSeg.csuklo,ajtoSeg.ajtoVege,true);
            if (P!=Pv){
                if (utvonal.size()>0){
                    vec2 cPos = utvonal[0].posEnd;
                    vec2 cPosv= realProjection(cPos,ajtoSeg.csuklo,ajtoSeg.ajtoVege,true);
                    velo+=(cPos-cPosv).normalize() * ajtoHelpStr;
                }
            }
            velo+=ajtoSeg.helpForPlayer(pos,radius) * ajtoHelpStr;

        }

        if (utvonalHossz>radius)
            velo*=0.95f;
        else
            velo*=0.8f;
        if (velo.length()>speed)
            velo=velo.normalize()*speed;


    }

    double vec2PointMinus(const vec2& a, const vec2& b){
        return a.x - b.x + a.y - b.y;
    }

    double vec2PointMultiply(const vec2& a, const vec2& b){
        return a.x * b.x + a.y * b.y;
    }

    vec2 realProjection(vec2 P, vec2 A, vec2 B, bool tovabb = false){
        float x = (P-A).length();
        float y = (P-B).length();
        float z = (A-B).length();

        float a = (x*x - y*y + z*z) / (2.f*z);


        if (!tovabb){
            if (a<=0)
                return A;
            if (a>=z)
                return B;
        }

        vec2 ret = (A*(z-a)+B*a)/z;
        return ret;
    }

    bool onALine(vec2 P, vec2 A, vec2 B){
        vec2 Pv = realProjection(P,A,B,false);
        if (P==Pv)
            return true;
        return false;
    }

    vec2 projection(vec2 posnew, Haromszog haromszog, vec2 from, Haromszog nextHaromszog = Haromszog(), bool benneVan = true) {
        if (!benneVan && haromszog.benneVanAPont(posnew)){
            return posnew;
        }
        vec2 AB = haromszog.B->pos - haromszog.A->pos;
        vec2 AC = haromszog.C->pos - haromszog.A->pos;
        vec2 CB = haromszog.B->pos - haromszog.C->pos;
        vec2 S = (haromszog.A->pos + haromszog.B->pos + haromszog.C->pos) / 3.f;

        bool ok = false;
        vec2 temp = HolMetszi(posnew,S,haromszog.B->pos,haromszog.A->pos);
        vec2 ret;
        if (!ok && temp!= vec2(-1369,-1369)){
            ret = realProjection(posnew,haromszog.B->pos,haromszog.A->pos,true);
            ok=true;
            if (data->getHaromszogIDFromPont(&ret)==-1){
                cout<<"BAJ107"<<endl;
                ret = realProjection(posnew,haromszog.B->pos,haromszog.A->pos,false);
                if (data->getHaromszogIDFromPont(&ret)==-1){
                    cout<<"BAJ106"<<endl;
                    ok=false;
                }
            }
            if (from==ret)
                ok=false;
        }

        temp = HolMetszi(posnew,S,haromszog.C->pos,haromszog.A->pos);
        if (!ok && temp!= vec2(-1369,-1369)){
            ret = realProjection(posnew,haromszog.C->pos,haromszog.A->pos,true);
            ok=true;
            if (data->getHaromszogIDFromPont(&ret)==-1){
                cout<<"BAJ107"<<endl;
                ret = realProjection(posnew,haromszog.C->pos,haromszog.A->pos,false);
                if (data->getHaromszogIDFromPont(&ret)==-1){
                    cout<<"BAJ106"<<endl;
                    ok=false;
                }
            }
            if (from==ret)
                ok=false;
        }

        temp = HolMetszi(posnew,S,haromszog.B->pos,haromszog.C->pos);
        if (!ok && temp!= vec2(-1369,-1369)){
            ret = realProjection(posnew,haromszog.B->pos,haromszog.C->pos,true);
            ok=true;
            if (data->getHaromszogIDFromPont(&ret)==-1){
                cout<<"BAJ107"<<endl;
                ret = realProjection(posnew,haromszog.B->pos,haromszog.C->pos,false);
                if (data->getHaromszogIDFromPont(&ret)==-1){
                    cout<<"BAJ106"<<endl;
                    ok=false;
                }
            }
            if (from==ret)
                ok=false;
        }
        if (!ok){
            cout<<"HUPSZ"<<endl;
            if (utvonal.size()>1){
                cout<<"BAJ115"<<endl;
                vec2 newPos = utvonal[1].posStart+(utvonal[1].posEnd-utvonal[1].posStart).normalize()*0.01f;
                if ((newPos-from).length()<=0.1f){
                    cout<<"BAJ116"<<endl;
                    utvonal.erase(utvonal.begin());
                    utvonalHossz=0;
                    for (int i=0; i<utvonal.size(); i++){
                        utvonalHossz+=(utvonal[i].posEnd-utvonal[i].posStart).length();
                    }
                    //cout<<"BAJ113"<<endl;
                    if (data->getHaromszogIDFromPont(&newPos)!=-1)
                        return newPos;
                    else
                        cout<<"BAJ110"<<endl;
                } else {
                    cout<<"BAJ117 "<<from.x<<" "<<from.y<<" "<<utvonal[0].posEnd.x<<" "<<utvonal[0].posEnd.y<<endl;
                    cout<<utvonal[1].posStart.x<<" "<<utvonal[1].posStart.y<<" "<<newPos.x<<" "<<newPos.y<<endl;
                    vec2 ret2 = from + (utvonal[0].posEnd-from).normalize()*0.01f;
                    cout<<ret2.x<<" "<<ret2.y<<endl;
                    if (data->getHaromszogIDFromPont(&ret2)!=-1)
                        return ret2;
                    else
                        cout<<"BAJ111"<<endl;
                }
            } else if (utvonal.size()==1) {
                vec2 ret2 = from + (utvonal[0].posEnd-from).normalize()*0.01f;
                if (data->getHaromszogIDFromPont(&ret2)!=-1)
                    return ret2;
                else {
                    utvonalkereses(cel);
                    cout<<"BAJ112"<<endl;
                }
            }
            cout<<"HIPSZ"<<endl;
            return from;

        }

        if (data->getHaromszogIDFromPont(&ret)==-1){
            cout<<"Bakker"<<endl;
            return from;
        }

        if (ret == from) {
            cout<<"BAJ105"<<endl;
        }
        return ret;

    }

    // http://www.sunshine2k.de/coding/java/PointOnLine/PointOnLine.html

    float perpDotProduct(vec2 a, vec2 b, vec2 c)
    {
      return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
    }

    float dotProduct ( vec2 a, vec2 b){
        return perpDotProduct(a,b,vec2(0,0));
    }

    vec2 getProjectedPointOnLine(vec2 p, vec2 v1, vec2 v2)
    {
      // get dot product of e1, e2
      vec2 e1 = vec2(v2.x - v1.x, v2.y - v1.y);
      vec2 e2 = vec2(p.x - v1.x, p.y - v1.y);
      double valDp = dotProduct(e1, e2);
      // get length of vectors
      double lenLineE1 = sqrt(e1.x * e1.x + e1.y * e1.y);
      double lenLineE2 = sqrt(e2.x * e2.x + e2.y * e2.y);
      double cos = valDp / (lenLineE1 * lenLineE2);
      // length of v1P'
      double projLenOfLine = cos * lenLineE2;
      vec2 ret = vec2((int)(v1.x + (projLenOfLine * e1.x) / lenLineE1),
                          (int)(v1.y + (projLenOfLine * e1.y) / lenLineE1));
      return ret;
    }

    vec2 projection2(vec2 pos, Haromszog haromszog, bool benneVan = true) {
        if (!benneVan && haromszog.benneVanAPont(pos)){
            return pos;
        }
        vec2 AB = haromszog.B->pos - haromszog.A->pos;
        vec2 AC = haromszog.C->pos - haromszog.A->pos;
        vec2 CB = haromszog.B->pos - haromszog.C->pos;
        vec2 S = (haromszog.A->pos + haromszog.B->pos +haromszog.C->pos) / 3.f;

        if (Metszi(pos,S,haromszog.B->pos,haromszog.A->pos)){
            pos = getProjectedPointOnLine(pos,haromszog.B->pos,haromszog.A->pos);
        }

        if (Metszi(pos,S,haromszog.C->pos,haromszog.A->pos)){
            pos = getProjectedPointOnLine(pos,haromszog.C->pos,haromszog.A->pos);
        }

        if (Metszi(pos,S,haromszog.B->pos,haromszog.C->pos)){
            pos = getProjectedPointOnLine(pos,haromszog.B->pos,haromszog.C->pos);
        }

        return pos;
    }


    bool baj = false;

    void simulate(float dt, ajtoSeged ajtoSeg){
        if (stop)
            return;

        int harom = data->getHaromszogIDFromPont(&pos);

        if (utvonal.size()>0 && harom==-1 && !baj){ // nem szokott előfordulni, de jelzésnek beteszem
            cout<<"BAJ101"<<endl;
            baj=true;
        }

        if (harom==-1){ // nem szokott előfordulni, de jelzésnek beteszem
            cout<<"BAJ102"<<endl;
            return;
        }

        vec2 nowpos = pos;

        pos+=velo*dt;

        if (data->getHaromszogIDFromPont(&pos)==-1){
            //pos-=velo*dt;
            double p_proj, p3_proj;
            Haromszog haromszog = data->getHaromszogR(harom);
            bool ok = false;

            //cout<<"hapci"<<endl;
            pos = projection(pos,haromszog,pos-velo*dt);
            if (pos == nowpos) {
                cout<<"BAJ103"<<endl;
            }
            //cout<<"hapci2"<<endl;

            // AB
            /*
            p_proj = vec2PointMinus(pos,haromszog.A->pos)*vec2PointMinus(haromszog.B->pos,haromszog.A->pos);
            p3_proj = vec2PointMinus(haromszog.B->pos,haromszog.A->pos)*vec2PointMinus(haromszog.B->pos,haromszog.A->pos);

            if (!ok && (p_proj >= 0) && (p_proj <= p3_proj)){
                pos = projection(pos,(haromszog.A->pos-haromszog.B->pos)); //(haromszog.A->pos * (p3_proj-p_proj) + haromszog.B->pos * p_proj)/p3_proj;
                ok = true;
            }

            // AC
            p_proj = vec2PointMinus(pos,haromszog.A->pos)*vec2PointMinus(haromszog.C->pos,haromszog.A->pos);
            p3_proj = vec2PointMinus(haromszog.C->pos,haromszog.A->pos)*vec2PointMinus(haromszog.C->pos,haromszog.A->pos);

            if (!ok && (p_proj >= 0) && (p_proj <= p3_proj)){
                pos = projection(pos,(haromszog.A->pos-haromszog.C->pos));
                ok = true;
            }

            // CB
            p_proj = vec2PointMinus(pos,haromszog.C->pos)*vec2PointMinus(haromszog.B->pos,haromszog.C->pos);
            p3_proj = vec2PointMinus(haromszog.B->pos,haromszog.C->pos)*vec2PointMinus(haromszog.B->pos,haromszog.C->pos);

            if (!ok && (p_proj >= 0) && (p_proj <= p3_proj)){
                pos = projection(pos,(haromszog.C->pos-haromszog.B->pos));
                ok = true;
            }
            */
        }



        if (ajtoSeg.jatekosOnTheHouse(pos,radius)){ // ajtó végébe/szélébe belemenni még rossz dolog
            vec2 PN = realProjection(nowpos,ajtoSeg.csuklo,ajtoSeg.ajtoVege,true);
            vec2 P = realProjection(pos,ajtoSeg.csuklo,ajtoSeg.ajtoVege,true);
            vec2 Pv= realProjection(pos,ajtoSeg.csuklo,ajtoSeg.ajtoVege,false);
            if (Pv==P){
                vec2 temp = P + (pos-PN).normalize()*(ajtoSeg.vastagsag/2.0f+radius);
                if (data->getHaromszogIDFromPont(&temp)!=-1){
                    pos=temp;
                }
            } else {
                //cout<<"GYERE IDE1"<<endl;
                vec2 ajtoraMeroleges = (ajtoSeg.ajtoVege-ajtoSeg.csuklo).ortho().normalize();
                vec2 A = ajtoSeg.csuklo+ajtoraMeroleges*ajtoSeg.vastagsag/2.0f;
                vec2 B = ajtoSeg.csuklo-ajtoraMeroleges*ajtoSeg.vastagsag/2.0f;
                vec2 C = ajtoSeg.ajtoVege+ajtoraMeroleges*ajtoSeg.vastagsag/2.0f;
                vec2 D = ajtoSeg.ajtoVege-ajtoraMeroleges*ajtoSeg.vastagsag/2.0f;

                vec2 temp;
                if ((pos-P).length()<ajtoSeg.vastagsag/2.0f){
                    if ((pos-A).length()<(pos-C).length()){
                        temp = realProjection(pos,A,B);
                        temp+= (ajtoSeg.csuklo-ajtoSeg.ajtoVege).normalize()*radius;
                    } else {
                        temp = realProjection(pos,C,D);
                        temp+= (ajtoSeg.ajtoVege-ajtoSeg.csuklo).normalize()*radius;
                    }
                } else {
                    int which = -1;
                    float minDis = (radius+ajtoSeg.vastagsag)*5.0f;
                    if ((pos-A).length()<minDis) { which=0; minDis=(pos-A).length();}
                    if ((pos-B).length()<minDis) { which=1; minDis=(pos-B).length();}
                    if ((pos-C).length()<minDis) { which=2; minDis=(pos-C).length();}
                    if ((pos-D).length()<minDis) { which=3; minDis=(pos-D).length();}

                    if (which==0) temp = A + (pos-A).normalize()*radius;
                    else if (which==1) temp = B + (pos-B).normalize()*radius;
                    else if (which==2) temp = C + (pos-C).normalize()*radius;
                    else if (which==3) temp = D + (pos-D).normalize()*radius;
                    else cout<<"BAJ120"<<endl;
                }
                if (data->getHaromszogIDFromPont(&temp)!=-1){
                    pos=temp;
                }
            }
        }

        if ((pos-nowpos-avgSpeed).length()>maxRealVelo && avgSpeed.length()<(pos-nowpos).length()){
            vec2 tempPos = nowpos + (pos-nowpos).normalize()*(avgSpeed.length()+maxRealVelo);
            if (data->getHaromszogIDFromPont(&tempPos)!=-1){
                pos=tempPos;
                //cout<<"YES"<<endl;
            }
        }


        avgSpeed*=cntSample;
        if (cntSample==maxSample){
            avgSpeed/=maxSample;
            cntSample--;
            avgSpeed*=cntSample;
        }
        avgSpeed+=(pos-nowpos);
        cntSample++;
        avgSpeed/=cntSample;

        if (avgSpeed.length()>maxSpeed){
            vec2 temp = nowpos + (pos-nowpos).normalize()*maxSpeed;
            if (data->getHaromszogIDFromPont(&temp)!=-1){
                pos=temp;
            } else cout<<"BAJ121"<<endl;
            avgSpeed.normalize();
            avgSpeed*=maxSpeed;
        }


        /*
        bool falonKeresztul = false;
        vec2 hol;
        for (int i=0; i<data->falakV.size() && utvonal.size()>0; i++){
            if (MetsziNemCsakErenti(&nowpos,&utvonal[0].posEnd,&data->falakV[i].cs1->pos,&data->falakV[i].cs2->pos,data,hol)){
                //cout<<"LOVAK"<<endl;
                falonKeresztul = true;
            }
        }
        if (falonKeresztul)
            falonkeresztulvezet++;
        if (falonkeresztulvezet>=maxFalonKeresztul){
            maxFalonKeresztul=0;
            utvonalkereses(cel);
        }
        */


        if (velo.length()>0.5f && (pos-nowpos).length()<1.0f && utvonal.size()>0){
            bool ok = false;
            vec2 hol;
            for (int i=0; i<data->falakV.size(); i++){
                ok = ok || MetsziNemCsakErenti(&utvonal[0].posStart,&utvonal[0].posEnd,&data->falakV[i].cs1->pos,&data->falakV[i].cs2->pos,/*data,*/hol);
            }
            if (ok){
                utvonalkereses(cel);
                cout<<"PACI"<<endl;
            }
        }


        if (utvonal.size()>0){
            utvonalHossz -= (utvonal[0].posStart-utvonal[0].posEnd).length();
            utvonal[0].posStart = pos;
            utvonalHossz += (utvonal[0].posStart-utvonal[0].posEnd).length();

            bool better = false, betterHelp1 = false;
            if (utvonal.size()>1){
                if (upcomingUtPosDis!=-1) {
                    if (upcomingUtPosDis>(utvonal[1].posEnd-pos).length()){
                        betterHelp1 = true;
                    }
                    upcomingUtPosDis = (utvonal[1].posEnd-pos).length();
                }
                upcomingUtPosDis = (utvonal[1].posEnd-pos).length();
            }


            if (nextUtPosDis!=-1){
                if (nextUtPosDis<(utvonal[0].posEnd-pos).length() && betterHelp1){
                    utvonalHossz-=(utvonal[0].posEnd-utvonal[0].posEnd).length();
                    utvonal.erase(utvonal.begin());
                    if (utvonal.size()>1){
                        nextUtPosDis=(utvonal[0].posEnd-pos).length();
                        upcomingUtPosDis=(utvonal[1].posEnd-pos).length();
                    }
                    utvonalkereses(cel);
                    //cout<<"JUPPI"<<endl;
                    return;
                }
            }
            nextUtPosDis = (utvonal[0].posEnd-pos).length();


            if (!vanUtvonalVege || utvonal.size()>1){
                // EZ LEHET nem is kell
                if ((utvonal[0].posEnd-pos).length()<0.0f /*radius*/){
                    //0++;
                    utvonalHossz -= (utvonal[0].posStart-utvonal[0].posEnd).length();
                    utvonal.erase(utvonal.begin());

                }
            }
        }
    }

    void utvonalkereses(vec2 realpos){
        bool Tdebug = false;
        if (Tdebug) cout<<"Start from: ("<<pos.x<<", "<<pos.y<<"), to: ("<<realpos.x<<", "<<realpos.y<<")"<<endl;
        utvonal = gps->UtvonalKereses(pos,realpos); // így már jó a felelősség
//        aktualisUtvonalCel=0;
        if (Tdebug) cout<<"Bef"<<endl;


            if (utvonal.size()>0){
                if (Tdebug) cout<<utvonal[0].posStart.x<<" "<<utvonal[0].posStart.y<<", "<<utvonal[utvonal.size()-1].posEnd.x<<" "<<utvonal[utvonal.size()-1].posEnd.y<<" "<<utvonal.size()<<endl;
                utvonalHossz=0;
                for (int i=0;i<utvonal.size();i++){
                    utvonalHossz += (utvonal[i].posStart-utvonal[i].posEnd).length();
                }
            }
            else
                if (Tdebug) cout<<"Nincs utvonal."<<endl;
        upcomingUtPosDis = -1, nextUtPosDis = -1;
    }

    void utvKerTest(int cnt){
        cout<<"utvKerTest Start, cnt: "<<cnt;//<<endl;
        clock_t t = clock();
        for (int i=0; i<cnt; i++){
            utvonalkereses(cel);
        }
        cout<<", time: "<<clock()-t<<endl;
    }

    void getEvent(SDL_Event &ev, int player_team){
        if (ev.type==SDL_KEYDOWN){
            if (ev.key.keysym.sym == SDLK_k){
                vanUtvonalVege=!vanUtvonalVege;
            }
        }
        if (ev.type==SDL_MOUSEBUTTONDOWN){
            //cout<<"HAPCI "<<(int)ev.button.button<<endl;
            if (ev.button.button==3 && (player_team == team || player_team == 0)){
                vec2 epos(ev.button.x,ev.button.y);
                vec2 Realepos = view->getRealPos(epos);
                cel = Realepos;
                utvonalkereses(Realepos);
            }
        }
    }

    void findPath(vec2 cel){
        //Data
    }


    void draw(SDL_Renderer &renderer){

        vec2 RelPos = view->getRelativePos(pos.x,pos.y);
        if (view->inRenderCircle(RelPos,radius)){
            filledCircleRGBA(&renderer,RelPos.x,RelPos.y,radius*view->zoom,red,green,blue,255);

        }
        for (size_t i=0; debugDraw &&  i<utvonal.size(); i++){
            vec2 RS = view->getRelativePos(utvonal[i].posStart), RE = view->getRelativePos(utvonal[i].posEnd);
            lineRGBA(&renderer,RS.x,RS.y,RE.x,RE.y,240,20,10,255);
        }
        if (debugDraw && view->inRenderCircle(RelPos,radius)){
        lineRGBA(&renderer,RelPos.x,RelPos.y,RelPos.x+velo.x,RelPos.y+velo.y,50,255,50,255);

        }
    }

};

bool Player2::stop = false;
bool Player2::debugDraw = false;

struct Ajto{
    vec2 csuklo, ajtoVege;
    float hossz;
    float szog, alapSzog;
    float szogSebesseg = 0, szogSpeed=2.5f, DegVeloMax=0.15f;
    float minSzog, maxSzog;
    float vastagsag;

    bool egyIranybanNyilik = true;
    bool negativIranybaNyilik = false;
    bool teljesenNyitva = false;
    bool hasznalatban = false;

    bool nyitja = false;

    clock_t timeLastUse = 0;
    clock_t closeDelay = 3000;

    ajtoSeged createAjtoSeged(){
        ajtoSeged temp(csuklo,ajtoVege,hossz,szog,vastagsag);
        return temp;
    }

    float degToRad(float deg){
        return deg*2.0f*3.1415f/360.f;
    }

    void draw(SDL_Renderer &renderer, View *view){
        ajtoVege = vec2(cos(degToRad(szog)),sin(degToRad(szog))).normalize()*hossz + csuklo;
        vec2 A = view->getRelativePos(csuklo + (ajtoVege-csuklo).ortho().normalize()*vastagsag/2);
        vec2 B = view->getRelativePos(csuklo - (ajtoVege-csuklo).ortho().normalize()*vastagsag/2);
        vec2 C = view->getRelativePos(ajtoVege + (ajtoVege-csuklo).ortho().normalize()*vastagsag/2);
        vec2 D = view->getRelativePos(ajtoVege - (ajtoVege-csuklo).ortho().normalize()*vastagsag/2);
        filledTrigonRGBA(&renderer,A.x,A.y,B.x,B.y,C.x,C.y,180,180,180,255);
        filledTrigonRGBA(&renderer,D.x,D.y,B.x,B.y,C.x,C.y,180,180,180,255);
    }

    Ajto(){}

    vec2 realProjection(vec2 P, vec2 A, vec2 B, bool tovabb = false, bool aranyt=false){
        float x = (P-A).length();
        float y = (P-B).length();
        float z = (A-B).length();

        float a = (x*x - y*y + z*z) / (2.f*z);


        if (!tovabb){
            if (a<=0)
                return A;
            if (a>=z)
                return B;
        }


        if (aranyt)
            return vec2(a,0);

        vec2 ret = (A*(z-a)+B*a)/z;
        return ret;
    }

    bool jatekosOnTheHouse(vec2 pos, float radius){

        vec2 Pv1 = realProjection(pos,csuklo,ajtoVege,false);
        vec2 Pv2 = realProjection(pos,csuklo,ajtoVege,true);

        if ((pos-Pv1).length()>vastagsag/2.0f+radius+0.2f)
            return false;

        if (Pv2!=Pv1){
            vec2 orthoP = (csuklo + (ajtoVege-csuklo).ortho());
            vec2 orthoM = (csuklo - (ajtoVege-csuklo).ortho());

            return ((((pos-orthoM).length()<radius) || ((pos-orthoP).length()<radius)) || ((pos-Pv1).length()<radius+vastagsag/2.0f));
        } else {
            return ((pos-Pv1).length()<radius+vastagsag/2.0f);
        }
    }

    Ajto(vec2 pos, float alapSzogv, float hosszv, float vastagsagv, float maxSzogv, float minSzogv){
        csuklo=pos; alapSzog=alapSzogv; hossz=hosszv; vastagsag=vastagsagv; szog=alapSzogv; maxSzog=maxSzogv; minSzog=minSzogv;
        ajtoVege = vec2(cos(degToRad(szog)),sin(degToRad(szog))).normalize()*hossz + csuklo;
    }

    void preSimulate(vector<Player2> players, float dt){
        float newDegVelo = 0;
        nyitja=false;
        vec2 orthoP = (csuklo + (ajtoVege-csuklo).ortho());
        vec2 orthoM = (csuklo - (ajtoVege-csuklo).ortho());
        for (int i=0; i<players.size(); i++){
            if (!egyIranybanNyilik ||
                (
                 (((orthoP-players[i].pos).length() < (orthoM-players[i].pos).length()) && !negativIranybaNyilik) ||
                 (!((orthoP-players[i].pos).length() < (orthoM-players[i].pos).length()) && negativIranybaNyilik)
                ) ||
                (players[i].pos-ajtoVege).length()<players[i].radius+5.5f
                ){
                nyitja=true;
            }


            if (jatekosOnTheHouse(players[i].pos,players[i].radius+0.5f)){
                timeLastUse = clock();
                //cout<<"DALMA"<<endl;
                vec2 Pv1 = realProjection(players[i].pos,csuklo,ajtoVege,false,true);

                if ((orthoP-players[i].pos).length() < (orthoM-players[i].pos).length()){
                    //cout<<"ALMA"<<endl;
                    if ((egyIranybanNyilik && !negativIranybaNyilik) || !egyIranybanNyilik)
                        newDegVelo+=abs(dt*szogSpeed*Pv1.x);
                    else
                        newDegVelo-=abs(dt*szogSpeed*Pv1.x);
                } else {
                    //cout<<"BALMA"<<endl;
                    if ((egyIranybanNyilik &&  negativIranybaNyilik) || !egyIranybanNyilik)
                        newDegVelo-=abs(dt*szogSpeed*Pv1.x);
                    else
                        newDegVelo+=abs(dt*szogSpeed*Pv1.x);
                }
            }

            /*vec2 Pv1 = realProjection(players[i].pos,csuklo,ajtoVege,false);
            vec2 Pv2 = realProjection(players[i].pos,csuklo,ajtoVege,false,true);
            if ((Pv1-players[i].pos).length()<vastagsag/2.0f+players[i].radius ){
                vec2 orthoP = (csuklo + (ajtoVege-csuklo).ortho());
                vec2 orthoM = (csuklo - (ajtoVege-csuklo).ortho());
                if ((orthoP-players[i].pos).length() < (orthoM-players[i].pos).length()){
                    szog+=abs(dt*10*Pv1.x);
                    if (szog>maxSzog)
                        szog=maxSzog;
                } else {
                    szog-=abs(dt*10*Pv1.x);
                    if (szog<minSzog)
                        szog=minSzog;
                }
            }
            */
        }
        if (abs(newDegVelo)>DegVeloMax){
            newDegVelo/=abs(newDegVelo);
            newDegVelo*=DegVeloMax;
        }
        //cout<<"newDegVelo: "<<newDegVelo<<endl;
        //if (abs(newDegVelo)>0.1f)
         //   timeLastUse=clock();
        //cout<<"timeLastUse: "<<timeLastUse<<" "<<timeLastUse+closeDelay<<" "<<clock()<<" "<<szog<<" "<<alapSzog<<" "<<newDegVelo<<" "<<true<<(szog-alapSzog!=0)<<(timeLastUse+closeDelay<clock())<<endl;
        if (timeLastUse+closeDelay<clock() && (szog-alapSzog!=0)){
            //cout<<((szog-alapSzog)/abs(szog-alapSzog))*min(0.5f,abs(szog-alapSzog)/5.0f)<<endl;
            szogSebesseg-=((szog-alapSzog)/abs(szog-alapSzog))*min(0.5f,abs(szog-alapSzog)/5.0f);
            newDegVelo=0;
            nyitja=true;
        }

        szogSebesseg+=newDegVelo;

        szogSebesseg*=0.8f;
        //cout<<"szogSebesseg: "<<szogSebesseg<<endl;
        //cout<<szogSebesseg<<endl;
    }

    void Simulate(float dt){
        if (!nyitja)
            return ;
        szog+=szogSebesseg;
        if (szog>maxSzog)
            szog=maxSzog;
        if (szog<minSzog)
            szog=minSzog;

    }

    void addForce(vec2 from, vec2 dir, float str){

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
    View view;
    CreateTri crTri(&view,&data);

    //Player2 player(&data,&view,&gps);

    //int player_cnt = 8;
    vector<Player2> players;
    //players.resize(player_cnt);
    /*for (int i=0; i<player_cnt; i++){
        players[i]=Player2(&data,&view,&gps);
        players[i].pos.x+=5.f*i;
        players[i].id=i;
    }*/

    //players[7].team=0;

    //cout<<"DAPCI"<<endl;
    Ajto ajto(vec2(7.5f,40),-90,20,2.5f,-0,-180);
    //cout<<"HAPCI"<<endl;
    ajtoSeged ajtoSeg = ajto.createAjtoSeged();
    //cout<<"KAPCI"<<endl;

    clock_t dt_timer = clock();
    clock_t viewEvT=0, frameResetT=0, drawT=0, crTriDrawT=0, dataDrawT=0, RenderPresentT=0;

    bool w=false,a=false,s=false,d=false,q=false,e=false;// o=false;
    bool p=false;
    int player_team = 0;
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
            if (!p)
                crTri.getEvent(ev,view,data);

            //player.getEvent(ev);

            for (int i=0; i<players.size(); i++){
                players[i].getEvent(ev,player_team);
            }



            if (ev.type==SDL_MOUSEBUTTONDOWN && p && ev.button.button==1){
                int x = ev.button.x;
                int y = ev.button.y;
                vec2 RealPos = view.getRealPos(x,y);
                if (data.getHaromszogIDFromPont(&RealPos)!=-1)
                    players.push_back(Player2(&data,&view,&gps,RealPos,player_team));
            }


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
                if (ev.key.keysym.sym == SDLK_e){
                    e=true;
                }
                if (ev.key.keysym.sym == SDLK_q){
                    q=true;
                }
                if (ev.key.keysym.sym == SDLK_u){
                    if (players.size()>0){
                        players[0].utvonalkereses(vec2(27.5332, -19.2027));
                    }
                }
                if (ev.key.keysym.sym == SDLK_z){
                        cout<<"alma"<<endl;
                    UtPos temp;
                    temp.eddigiCsucsok.push_back(data.csucsokV[25]);
                    temp.eddigiCsucsok.push_back(data.csucsokV[27]);
                    temp.eddigiCsucsok.push_back(data.csucsokV[41]);
                    temp.csucs = data.csucsokV[46];
                    data.frissitFalakV();
                    UtPos temp2 = gps.UtvonalKeresesVagas(temp,vec2(5,35),vec2(27.5332, -19.2027));
                    cout<<"kifejtes: Csucs ID: "<<temp2.csucs.id<<" "<<temp2.becsult<<" "<<temp2.eddigMegtett<<" "<<temp2.elozoMegtett<<" "<<temp2.ossz<<endl;
                    cout<<"   ";
                    for (size_t i=0; i<temp2.eddigiCsucsok.size(); i++)
                        cout<<temp2.eddigiCsucsok[i].id<<" ";
                    cout<<endl;

                }
                if (ev.key.keysym.sym == SDLK_1){
                    Player2::stop=!Player2::stop;
                }
                if (ev.key.keysym.sym == SDLK_g){
                    data.csucsokDrawSwitch= !data.csucsokDrawSwitch;
                    data.falakDrawSwitch= !data.falakDrawSwitch;
                    data.haromszogekDrawSwitch= !data.haromszogekDrawSwitch;
                    Player2::debugDraw=!Player2::debugDraw;
                }
                if (ev.key.keysym.sym == SDLK_t){
                    player_team++;
                    if (player_team>2){
                        player_team=0;
                    }
                    cout<<"player_team: "<<player_team<<endl;
                }
                if (ev.key.keysym.sym == SDLK_p){
                    p=!p;
                }
                if (ev.key.keysym.sym == SDLK_h){
                    data.loadFileFalak("h2",&view);
                    data.loadFileAsztalok("I4A",&view);
                }
                if (ev.key.keysym.sym == SDLK_h){
                    data.addNewCsucs(vec2(0,0));
                }
                if (ev.key.keysym.sym == SDLK_i){
                    float x, y;
                    cout<<"Uj jatekos letrehozasa"<<endl;
                    cout<<"Pozicioja (x,y): ";
                    cin>>x>>y;
                    vec2 RealPos(x,y); //= view.getRealPos(x,y);
                    if (data.getHaromszogIDFromPont(&RealPos)!=-1)
                        players.push_back(Player2(&data,&view,&gps,RealPos));
                }
                if (ev.key.keysym.sym == SDLK_c){
                    //z=true;
                    string fileF;
                    cout<<"Betolteni kívant fajl: ";
                    cin>>fileF;
                    crTri.loadFile(fileF);
                    fileF+="Walls";
                    data.loadFileFalak(fileF,&view);
                    crTri.state=-1;
                }
                if (ev.key.keysym.sym == SDLK_v){
                    string fileF;
                    cout<<"Mentesi fajl neve: ";
                    cin>>fileF;
                    data.saveMapToFile(fileF);
                }
                if (ev.key.keysym.sym == SDLK_o){
                        //data.createRandomTris(100,view);
                    //data.printAll();
                    falonkiv=0; data.GHIFP=0;
                    players[0].utvKerTest(1);

                    cout<<gps.kereses<<" "<<gps.vagas<<" "<<gps.vagas1<<" "<<gps.vagas2<<" "<<gps.vagas3<<" "<<data.idx_cnt<<" "<<gps.vagasok<<" "<<gps.vagasok1<<" "<<gps.vagasok2<<endl;
                    cout<<falonkiv<<" "<<data.GHIFP<<" "<<metszinemcnt<<endl;
                    gps.kereses=0; gps.vagas=0; gps.vagas1=0; gps.vagas2=0; gps.vagas3=0; data.idx_cnt=0; gps.vagasok=0; gps.vagasok1=0; gps.vagasok2=0;
                    falonkiv=0; data.GHIFP=0; metszinemcnt=0;
                    //o=true;
                }

                if (ev.key.keysym.sym == SDLK_UP){
                    //up=true;
                    crTri.loadFile("doorTest");
                    data.loadFileFalak("doorTestWalls",&view);
                }

                if (ev.key.keysym.sym == SDLK_DOWN){
                    clock_t t =clock();
                    vec2 h(-30,-30);
                    for (int i=0;i<10000;i++){
                        if (data.getHaromszogIDFromPont(&h)==-1){}
                            //cout<<":(";
                    }
                    cout<<endl;
                        //down=true;
                    cout<<"dataHarIDFromPoint Teszt 10000: "<<clock()-t<<endl;
                }
                if (ev.key.keysym.sym == SDLK_RIGHT){
                    Player2 temp;
                    vec2 temp1 = temp.realProjection(vec2(16,0),vec2(0,0),vec2(-1,-1),true);
                    vec2 temp2 = temp.realProjection(vec2(16,0),vec2(0,0),vec2(-1,-1),false);
                    cout<<temp1.x<<" "<<temp1.y<<" "<<temp2.x<<" "<<temp2.y<<endl;
                }
                if (ev.key.keysym.sym == SDLK_LEFT){
                //    left=true;
                    cout<<"start HolMetsziTeszt 10000: ";
                    clock_t t = clock();
                    for (int j=0; j< 10000; j++)
                        HolMetszi(vec2(rand2(),rand2()),vec2(rand2(),rand2()),vec2(rand2(),rand2()),vec2(rand2(),rand2()));
                    cout<<clock()-t<<endl;
                }
                if (ev.key.keysym.sym == SDLK_y){
                    float x1,y1,x2,y2;
                    cout<<"Szakasz: x,y  x,y: ";
                    cin>>x1>>y1>>x2>>y2;
                    vec2 S(x1,y1), E(x2,y2);
                    list<Wall>::iterator it = data.falak.begin();
                    vec2 hol = vec2(0,0);
                    for (size_t i=0; i<data.falak.size(); i++){
                        if (MetsziNemCsakErenti(&S,&E,&((*it).cs1->pos),&((*it).cs2->pos),/*&data,*/hol,true)){
                            cout<<"bajos "<<i<<" "<<(*it).cs1->pos.x<<" "<<(*it).cs1->pos.y<<" "<<(*it).cs2->pos.x<<" "<<(*it).cs2->pos.y<<endl;
                            break;  // ITT Tartok
                        }
                        it++;
                        if (i==data.falak.size()-1){ // k 144.716 23.9631 156.491 77.8208, l 88.8731 -29.091 127.077 2.45345
                            cout<<"OKS"<<endl;
                            list<Wall>::iterator it2 = data.falak.begin();
                            for (size_t j=0; j<data.falak.size(); j++){
                                if (Metszi(S,E,(*it2).cs1->pos,(*it2).cs2->pos)){
                                    cout<<"bajos2 "<<j<<" "<<(*it2).cs1->pos.x<<" "<<(*it2).cs1->pos.y<<" "<<(*it2).cs2->pos.x<<" "<<(*it2).cs2->pos.y<<endl;
                                    break;
                                }
                                it2++;
                                if (j==data.falak.size()-1)
                                    cout<<"dafak "<<j<<" "<<data.falak.size()<<" "<<i<<endl;
                            }
                            cout<<"hapci2"<<endl;
                        }
                    }
                    cout<<"hmm.."<<endl;
                  //  right=true;
                }
                if (ev.key.keysym.sym == SDLK_x){
                    size_t id;
                    cout<<"Csucs ID: ";
                    cin>>id;
                    cout<<endl;
                    if (id<data.csucsok.size()){
                        Csucs *temp = data.getCsucsP(id);
                        cout<<temp->pos.x<<" "<<temp->pos.y<<" "<<temp->uj<<" "<<temp->torolt<<endl;
                    }
                    else
                        cout<<"Tulindexelt csucs"<<endl;
                }
                //*/
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
                if (ev.key.keysym.sym == SDLK_e){
                    e=false;
                }
                if (ev.key.keysym.sym == SDLK_q){
                    q=false;
                }
                if (ev.key.keysym.sym == SDLK_p){

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

            float dt = min(clock()-dt_timer,(clock_t)16);

            dt*=SPEED;
            dt/=1000.f;
            dt_timer=clock();
            //player.simulate(dt,players);
            for (int i=0; i<1; i++){
                ajto.preSimulate(players,dt);
            }
            ajtoSeg=ajto.createAjtoSeged();
            for (int i=0; i<players.size(); i++)
                players[i].preSimulate(dt,players,ajtoSeg);



            for (int i=0; i<players.size(); i++)
                players[i].simulate(dt,ajtoSeg);
            ajto.Simulate(dt);

            clock_t tFrame = clock();
            view.KeysDown(w,a,s,d,e,q);

            viewEvT+=clock()-tFrame;
            tFrame=clock();

            SDL_SetRenderDrawColor(&renderer,150,64,12,255);
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

            //player.draw(renderer);
            for (int i=0; i<players.size(); i++){
                players[i].draw(renderer);
            }
            drawT+=clock()-tFrame;
            tFrame=clock();

            ajto.draw(renderer,&view);

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
