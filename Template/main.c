#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
//#include <math.h>
//#include <string.h>
//#include <stdint.h>

#include <SDL.h>
//#include "SDL2_gfx/SDL2_gfxPrimitives.h"
#include "SDL2_gfx/SDL2_framerate.h"

#ifdef WIN32
#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif


Uint32 time_passed = 0;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~O~~~~~~~~~~| M A I N |~~~~~~~~~~~O~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char *argv[]){

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    int width = 640;
    int height = 480;
    int cx, cy;
    int loop = 1;
    int zoomI = 0;
    double zoom = 1;
    double tx = 0, ty = 0;
    int mouseX, mouseY, pmouseX, pmouseY;

    Uint32 then, now, frames;
    FPSmanager fpsm;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }
    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }
    //SDL_MaximizeWindow( window );
    SDL_GetWindowSize( window, &width, &height );
    cx = width / 2;
    cy = height / 2;

    //srand (time(NULL));// prime the random number generator


    const SDL_Color black = {0, 0, 0, 255};
    const SDL_Color white = {255, 255, 255, 255};
    Uint32 rmask, gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        //puts("SDL_BIG_ENDIAN");
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        //puts("SDL_LIL_ENDIAN");
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif


    /*
    

    DECLARE, LOAD AND INITIALIZE YOUR STUFF HERE
        

    */
    
    SDL_initFramerate(&fpsm);
    SDL_setFramerate(&fpsm, 60);
    puts("<<Entering Loop>>");
    while ( loop ) { //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> L O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
        
        while( SDL_PollEvent(&event) ){
            switch (event.type) {
                case SDL_QUIT:
                    loop = 0;
                    break;
                case SDL_KEYDOWN://keyPressed
                    //if( event.key.keysym.sym == 'w' || event.key.keysym.sym == SDLK_UP ){   
                    break;
                case SDL_KEYUP://keyReleased
                         
                    break;
                case SDL_MOUSEMOTION:

                    //keeping track of current and previous mouse position. Not Mandatory!
                	pmouseX = mouseX;
        			pmouseY = mouseY;
                    mouseX = event.motion.x;
                    mouseY = event.motion.y;
    
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    break;
                case SDL_MOUSEBUTTONUP:
                    break;
                case SDL_MOUSEWHEEL:;
                    
                    break;
            }
        }

        // clear previous frame
        SDL_SetRenderDrawColor( renderer, 200, 200, 200, 255);
        SDL_RenderClear( renderer );

        /*
        

        RENDER THINGS HERE


        */

        // throw things up onscreen
        SDL_RenderPresent(renderer);

        // maintain constant framerate
        time_passed = SDL_framerateDelay(&fpsm);

    }//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> / L O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

