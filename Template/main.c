#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <SDL.h>

#ifdef WIN32
#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~O~~~~~~~~~~| M A I N |~~~~~~~~~~~O~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char *argv[]){

    SDL_Window *window;
    SDL_Renderer *renderer;
    
    int width = 640;
    int height = 480;
    int loop = 1;
    int mouseX, mouseY, pmouseX, pmouseY;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }
    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }
    SDL_GetWindowSize( window, &width, &height );


    //srand (time(NULL));// prime the random number generator


    /*
    

    DECLARE, LOAD AND INITIALIZE YOUR STUFF HERE
        

    */
    
    puts("<<Entering Loop>>");
    while ( loop ) { //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> L O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
        
        SDL_Event event;
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

    }//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> / L O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

