#include <iostream>
#include <fstream>

#include "VecX.h"

using namespace std;



void loadFileDisz(string file){
    ifstream F; F.open(file);
    ofstream Fo; string o;
    cout<<"Mentesi fajl: ";
    cin>>o;
    Fo.open(o);
    if (F.fail() || Fo.fail()){
        cout<<"fajl betoltes hiba"<<endl;
        return;
    }
    while (!F.eof()){
        float x1, y1, x2, y2;
        F>>x1>>y1>>x2>>y2;
        vec2 X(x1,y1), Y(x2,y2);

        vec2 YX = X-Y;
        vec2 meroleges = YX.ortho().normalize();
        YX = YX.normalize();
        Y-=YX;
        X+=YX;
        vec2 A = X + meroleges;
        vec2 B = X - meroleges;
        vec2 C = Y + meroleges;
        vec2 D = Y - meroleges;

        Fo<<A.x<<" "<<A.y<<" "<<B.x<<" "<<B.y<<" "<<C.x<<" "<<C.y<<" ";
        Fo<<D.x<<" "<<D.y<<" "<<B.x<<" "<<B.y<<" "<<C.x<<" "<<C.y<<endl;
    }
    Fo.close();
}

void loadFileFelez(string file){
    ifstream F; F.open(file);
    ofstream Fo; string o;
    cout<<"Mentesi fajl: ";
    cin>>o;
    Fo.open(o);
    if (F.fail() || Fo.fail()){
        cout<<"fajl betoltes hiba"<<endl;
        return;
    }
    while (!F.eof()){
        float x, y;
        F>>x>>y;
        Fo<<x/2.0f<<" "<<y/2.0f<<endl;
    }
    Fo.close();
}

int main()
{
    ofstream ofs; ofs.open("alma.txt");
    ofs<<12.34f<<" "<<23.45f;
    ofs.close();

    string F;// = "I_4_falak.txt";
    cout<<"Beloltendo fajl: "<<endl;
    cin>>F;

    cout<<"Mode: ";
    int k=0;
    cin>>k;



    if (k==0)
        loadFileDisz(F);
    if (k==1)
        loadFileFelez(F);
}
