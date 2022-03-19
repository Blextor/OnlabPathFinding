#include "coomon.h"

using namespace std;

int main( int argc, char * argv[] )
{

    srand(time(NULL));
    cout<<time(NULL)<<endl;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer( SZELES, MAGAS, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &window, &renderer );
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );
    jatek( *window, *renderer);
    return 0;
}
