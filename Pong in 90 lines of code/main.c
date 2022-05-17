#include "basics.h"
#include "primitives.h"


#ifdef WIN32
#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~O~~~~~~~~~~| M A I N |~~~~~~~~~~~O~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char *argv[]){

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    int width = 600;
    int height = 400;
    int cx, cy;
    int loop = 1;


    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }
    if (SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    cx = width / 2;
    cy = height / 2;

    // prime the random number generator
    srand (time(NULL));
    

    // paddle height and width, and their halves
    int H = 70;
    int hH = H / 2;
    int W = 16;
    int hW = W / 2;
    //paddle rects. {x, y, w, h}, (x,y) = top-left corner
    SDL_Rect P1_rect = (SDL_Rect){ 15, cy-hH, W, H };
    SDL_Rect P2_rect = (SDL_Rect){ width -15 -W, cy-hH, W, H };
    //paddle velocity
    int pvel = 4;

    //scores
    int p1s = 0;
    int p2s = 0;

    // ball coordinates
    float bx = 300;
    float by = 200;
    //ball radius
    float br = 10;

    double a = 0;
    int coin = random(1, 10);
    if( coin > 5 ) a = randomD( -QUARTER_PI, QUARTER_PI );
    else a = randomD( 3*QUARTER_PI, 5*QUARTER_PI );
    // ball velocity magnitude
    float vel = 4;
    // ball velocity 2D
    float vbx = vel * cos(a);
    float vby = vel * sin(a);

    //control variables. they will store the state of each control key (up or down for the given player)
    int p1u = 0, p1d = 0, p2u = 0, p2d = 0;
    
    //our desired frame period
    int frame_period = lrint( 1000 / 60.0 );

    puts("<<Entering Loop>>");
    while ( loop ) { //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> L O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
        
        while( SDL_PollEvent(&event) ){
            switch (event.type) {
                case SDL_QUIT:
                    loop = 0;
                    break;
                case SDL_KEYDOWN:
                         if( event.key.keysym.sym == 'w' )       p1u = 1;
                    else if( event.key.keysym.sym == 's' )       p1d = 1;
                    else if( event.key.keysym.sym == SDLK_UP )   p2u = 1;
                    else if( event.key.keysym.sym == SDLK_DOWN ) p2d = 1; 
                    break;
                case SDL_KEYUP:
                         if( event.key.keysym.sym == 'w' )       p1u = 0;
                    else if( event.key.keysym.sym == 's' )       p1d = 0;
                    else if( event.key.keysym.sym == SDLK_UP )   p2u = 0;
                    else if( event.key.keysym.sym == SDLK_DOWN ) p2d = 0;
                    break;
            }
        }

        // move ball forward
        bx += vbx;
        by += vby;

        //effect paddle controls
        if( p1u ) P1_rect.y = constrain( P1_rect.y - pvel, 0, height - H );
        if( p1d ) P1_rect.y = constrain( P1_rect.y + pvel, 0, height - H );
        if( p2u ) P2_rect.y = constrain( P2_rect.y - pvel, 0, height - H );
        if( p2d ) P2_rect.y = constrain( P2_rect.y + pvel, 0, height - H );

        if( by < br ){// ball ceiling collision
          vby *= -1;
          by = br;//restitution of the position avoids bugs!
        }
        if( by > height-br ){// ball floor collision
          vby *= -1;
          by = height-br;
        }

        //ball paddle collisions
        if( bx < P1_rect.x + W + br && by > P1_rect.y && by < P1_rect.y + H ){
            vbx *= -1;
            bx = P1_rect.x + W + br;
        }
        else if( bx > P2_rect.x -br && by > P2_rect.y && by < P2_rect.y + H ){
            vbx *= -1;
            bx = P2_rect.x -br;
        }
        
        bool reset_ball = 0;
        if( bx < 0 ){
          ++p2s;
          reset_ball = 1;
        }
        if( bx > width ){
          ++p1s;
          reset_ball = 1;
        }

        if( reset_ball ){
            bx = 300;
            by = 200;

            a = 0;
            int coin = random(1, 10);
            if( coin > 5 ) a = randomD( -QUARTER_PI, QUARTER_PI );
            else a = randomD( 3*QUARTER_PI, 5*QUARTER_PI );

            // ball velocity 2D
            vbx = vel * cos(a);
            vby = vel * sin(a);
        }

        // clear previous frame
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
        SDL_RenderClear( renderer );

        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255);

        //Render paddles...
        SDL_RenderFillRect( renderer, &P1_rect );
        SDL_RenderFillRect( renderer, &P2_rect );
        //Center line,
        SDL_RenderDrawLine( renderer, cx, 0, cx, height );
        //ball.
        gp_fill_16circle( renderer, bx, by, br );

        // throw things up onscreen
        SDL_RenderPresent(renderer);

        // try to maintain constant framerate
        SDL_framerateDelay( frame_period );

    }//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> / L O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

