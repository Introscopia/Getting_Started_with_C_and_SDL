#include <SDL.h>

#define TWO_PI        6.2831853071795864769252867665590057683943387987502116419
#define PI            3.1415926535897932384626433832795028841971693993751058209
#define TWO_THIRDS_PI 2.0943951023931954923084289221863352561314462662500705473
#define HALF_PI       1.5707963267948966192313216916397514420985846996875529104
#define THIRD_PI      1.0471975511965977461542144610931676280657231331250352736
#define QUARTER_PI    0.7853981633974483096156608458198757210492923498437764552


void gp_fill_24circle(SDL_Renderer *R, float x, float y, float radius){
        SDL_FColor C;
        SDL_GetRenderDrawColorFloat(R, &(C.r), &(C.g), &(C.b), &(C.a));
        const float trig [5] = { 0.258819*radius, 0.500000*radius, 0.707107*radius, 0.866025*radius, 0.965926*radius};
        SDL_Vertex verts[24];
        verts[ 0] = (SDL_Vertex){ { x + radius , y           }, C, {0} };
        verts[ 1] = (SDL_Vertex){ { x + trig[4], y + trig[0] }, C, {0} };
        verts[ 2] = (SDL_Vertex){ { x + trig[3], y + trig[1] }, C, {0} };
        verts[ 3] = (SDL_Vertex){ { x + trig[2], y + trig[2] }, C, {0} };
        verts[ 4] = (SDL_Vertex){ { x + trig[1], y + trig[3] }, C, {0} };
        verts[ 5] = (SDL_Vertex){ { x + trig[0], y + trig[4] }, C, {0} };
        verts[ 6] = (SDL_Vertex){ { x          , y + radius  }, C, {0} };
        verts[ 7] = (SDL_Vertex){ { x - trig[0], y + trig[4] }, C, {0} };
        verts[ 8] = (SDL_Vertex){ { x - trig[1], y + trig[3] }, C, {0} };
        verts[ 9] = (SDL_Vertex){ { x - trig[2], y + trig[2] }, C, {0} };
        verts[10] = (SDL_Vertex){ { x - trig[3], y + trig[1] }, C, {0} };
        verts[11] = (SDL_Vertex){ { x - trig[4], y + trig[0] }, C, {0} };
        verts[12] = (SDL_Vertex){ { x - radius , y           }, C, {0} };
        verts[13] = (SDL_Vertex){ { x - trig[4], y - trig[0] }, C, {0} };
        verts[14] = (SDL_Vertex){ { x - trig[3], y - trig[1] }, C, {0} };
        verts[15] = (SDL_Vertex){ { x - trig[2], y - trig[2] }, C, {0} };
        verts[16] = (SDL_Vertex){ { x - trig[1], y - trig[3] }, C, {0} };
        verts[17] = (SDL_Vertex){ { x - trig[0], y - trig[4] }, C, {0} };
        verts[18] = (SDL_Vertex){ { x          , y - radius  }, C, {0} };
        verts[19] = (SDL_Vertex){ { x + trig[0], y - trig[4] }, C, {0} };
        verts[20] = (SDL_Vertex){ { x + trig[1], y - trig[3] }, C, {0} };
        verts[21] = (SDL_Vertex){ { x + trig[2], y - trig[2] }, C, {0} };
        verts[22] = (SDL_Vertex){ { x + trig[3], y - trig[1] }, C, {0} };
        verts[23] = (SDL_Vertex){ { x + trig[4], y - trig[0] }, C, {0} };
        int indices[66] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 
            6, 7, 0, 7, 8, 0, 8, 9, 0, 9, 10, 0, 10, 11, 0, 11, 12, 0, 12, 
            13, 0, 13, 14, 0, 14, 15, 0, 15, 16, 0, 16, 17, 0, 17, 18, 0, 
            18, 19, 0, 19, 20, 0, 20, 21, 0, 21, 22, 0, 22, 23 };
        SDL_RenderGeometry( R, NULL, verts, 24, indices, 66 );
}


int randomI( int min, int max ){
    return min + SDL_rand(max-min);
}
float randomF( float min, float max ){
    return min + SDL_randf() * (max-min);
}


int SDL_framerateDelay( int frame_period ){
    static Uint64 then = 0;
    Uint64 now = SDL_GetTicks();
    int elapsed = now - then;
    int delay = frame_period - elapsed;
    //printf("%d - (%d - %d) = %d\n", frame_period, now, then, delay );
    if( delay > 0 ){
        SDL_Delay( delay );
        elapsed += delay;
    }
    then = SDL_GetTicks();
    return elapsed;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~O~~~~~~~~~~| M A I N |~~~~~~~~~~~O~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char *argv[]){

    SDL_Window *window;
    SDL_Renderer *renderer;
    int width = 600;
    int height = 400;
    int cx, cy;
    int loop = 1;


    if( !SDL_Init(SDL_INIT_VIDEO) ){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }
    if( !SDL_CreateWindowAndRenderer( "Baby's first SDL Game: Pong", width, height, 
                                      SDL_WINDOW_INPUT_FOCUS, &window, &renderer) ){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    cx = width / 2;
    cy = height / 2;

    // prime the random number generator
    SDL_srand(0);
    

    // paddle height and width, and their halves
    int H = 70;
    int hH = H / 2;
    int W = 16;
    int hW = W / 2;
    //paddle rects. {x, y, w, h}, (x,y) = top-left corner
    SDL_FRect P1_rect = (SDL_FRect){ 15, cy-hH, W, H };
    SDL_FRect P2_rect = (SDL_FRect){ width -15 -W, cy-hH, W, H };
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

    float a = 0;
    int coin = randomI(1, 10);
    if( coin > 5 ) a = randomF( -QUARTER_PI, QUARTER_PI );
    else a = randomF( 3*QUARTER_PI, 5*QUARTER_PI );
    // ball velocity magnitude
    float vel = 4;
    // ball velocity 2D
    float vbx = vel * SDL_cos(a);
    float vby = vel * SDL_sin(a);

    //control variables. they will store the state of each control key (up or down for the given player)
    int p1u = 0, p1d = 0, p2u = 0, p2d = 0;
    
    //our desired frame period
    int frame_period = SDL_roundf( 1000 / 60.0 );

    puts("<<Entering Loop>>");
    while ( loop ) { //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> L O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
        
        SDL_Event event;
        while( SDL_PollEvent(&event) ){
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    loop = 0;
                    break;
                case SDL_EVENT_KEY_DOWN:
                         if( event.key.key == 'w' )       p1u = 1;
                    else if( event.key.key == 's' )       p1d = 1;
                    else if( event.key.key == SDLK_UP )   p2u = 1;
                    else if( event.key.key == SDLK_DOWN ) p2d = 1; 
                    break;
                case SDL_EVENT_KEY_UP:
                         if( event.key.key == 'w' )       p1u = 0;
                    else if( event.key.key == 's' )       p1d = 0;
                    else if( event.key.key == SDLK_UP )   p2u = 0;
                    else if( event.key.key == SDLK_DOWN ) p2d = 0;
                    break;
            }
        }

        // move ball forward
        bx += vbx;
        by += vby;

        //effect paddle controls
        if( p1u ) P1_rect.y = SDL_clamp( P1_rect.y - pvel, 0, height - H );
        if( p1d ) P1_rect.y = SDL_clamp( P1_rect.y + pvel, 0, height - H );
        if( p2u ) P2_rect.y = SDL_clamp( P2_rect.y - pvel, 0, height - H );
        if( p2d ) P2_rect.y = SDL_clamp( P2_rect.y + pvel, 0, height - H );

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
            int coin = randomI(1, 10);
            if( coin > 5 ) a = randomF( -QUARTER_PI, QUARTER_PI );
            else a = randomF( 3*QUARTER_PI, 5*QUARTER_PI );

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
        SDL_RenderLine( renderer, cx, 0, cx, height );
        //ball.
        gp_fill_24circle( renderer, bx, by, br );

        //score text
        SDL_SetRenderScale(renderer, 3, 3);
        SDL_RenderDebugTextFormat( renderer, 192/3.0, 6, "%d", p1s );
        SDL_RenderDebugTextFormat( renderer, 392/3.0, 6, "%d", p2s );
        SDL_SetRenderScale(renderer, 1, 1);

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

