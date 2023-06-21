// Incluindo algumas bibliotecas padrão.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
// Incluindo o SDL
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_Image.h>

#ifdef WIN32
#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif





//Aqui antes do main(), declaramos alguns valores e funcoes que vamos usar no programa.

typedef int32_t bool;

#define QUARTER_PI  (double) 0.785398163397448309615660845819875721049292349843776455243

int random( int min, int max ){
    return (rand() % (max +1 -min)) + min;
}

double randomD( double min, double max ){
    return ( (max-min) * ( rand() / ((double)RAND_MAX) ) ) + min;
}

void SDL_framerateDelay( int frame_period ){
    //we assume CLOCKS_PER_SEC is 1000, cause it always is...
    static clock_t then = 0;
    clock_t now = clock();
    int delay = frame_period - ( now - then );
    //printf("%d - (%d - %d) = %d\n", frame_period, now, then, delay );
    if( delay > 0 ) SDL_Delay( delay );
    then = clock();
}

int constrain( int a, int min, int max ){
    if( a < min ) return min;
    else if( a > max ) return max;
    else return a;
}

const SDL_Color white = (SDL_Color){ 255, 255, 255, 255 };
const SDL_Color black = (SDL_Color){ 0, 0, 0, 255 };

void render_num_text( SDL_Renderer *R, int num, SDL_Texture **T, SDL_Rect *dst, TTF_Font *font ){
    if( *T != NULL ) SDL_DestroyTexture( *T );
    char str [8];
    sprintf( str, "%d", num );
    SDL_Surface *S = TTF_RenderText_Shaded( font, str, white, black );
    *T = SDL_CreateTextureFromSurface( R, S );
    SDL_QueryTexture( *T, 0, NULL, &(dst->w), &(dst->h) );
    SDL_FreeSurface( S );
}





//Inicio do programa
int main(int argc, char *argv[]){

    //Declarando Janela e Renderizador do SDL
    SDL_Window *window;
    SDL_Renderer *renderer;
    
    //Algumas Variaveis básicas
    int width = 640;
    int height = 480;
    int loop = 1;
    int mouseX, mouseY, pmouseX, pmouseY;
    int cx, cy;
    cx = width / 2;
    cy = height / 2;

    //Inicializando o sistema de video do SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }
    //Inicializando nossa Janela e Renderizador
    if (SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    //inicializando a biblioteca de imagem
    IMG_Init( IMG_INIT_PNG );
    //carregar a imagem:
    SDL_Texture *sprites = IMG_LoadTexture( renderer, "sprites.png");
    SDL_Rect P1_paddle_src = (SDL_Rect){0, 0, 16, 88};
    SDL_Rect P2_paddle_src = (SDL_Rect){48, 0, 16, 88};
    SDL_Rect ball1_src = (SDL_Rect){17, 1, 13, 13};
    SDL_Rect ball2_src = (SDL_Rect){17, 17, 13, 13};

    // Inicializacao do gerador de numeros aleatorios
    srand (time(NULL));

    //Inicializacao do TTF, a lib de renderizacao de texto do SDL.
    if( TTF_Init() < 0 ) puts("TTF nao conseguiu inicializar");
    //Carregando o arquivo da nossa fonte
    TTF_Font *font = TTF_OpenFont( "7segments.ttf", 40 );

    // pontuacao dos dois jogadores
    int p1s = 0;
    int p2s = 0;

    // texturas para o placar
    SDL_Texture *p1s_texture = NULL;
    SDL_Rect p1s_dst = (SDL_Rect){ 0.25*width, 10, 0, 0 };
    render_num_text( renderer, p1s, &p1s_texture, &p1s_dst, font );
    p1s_dst.x -= p1s_dst.w/2;
    SDL_Texture *p2s_texture = NULL;
    SDL_Rect p2s_dst = (SDL_Rect){ 0.75*width, 10, 0, 0 };
    render_num_text( renderer, p2s, &p2s_texture, &p2s_dst, font );
    p2s_dst.x -= p2s_dst.w/2;

    int Z = 2;// escala dos sprites

    // informacoes das raquetes
    int H = Z * 88;//altura
    int hH = H / 2;
    int W = Z * 16;//largura
    int hW = W / 2;
    // retangulos das raquetes. {x, y, w, h}, (x,y) = canto superior esquerdo
    SDL_Rect P1_rect = (SDL_Rect){ 15, cy-hH, W, H };
    SDL_Rect P2_rect = (SDL_Rect){ width -15 -W, cy-hH, W, H };
    // velocidade da raquete
    int pvel = 6;

    // coordenadas da bola
    float bx = cx;
    float by = cy;
    // raio da bola
    float br = Z * 6.5;

    //inicializacao da velocidade da bola
    double a = 0;
    int coin = random(1, 10);
    if( coin > 5 ) a = randomD( -QUARTER_PI, QUARTER_PI );
    else a = randomD( 3*QUARTER_PI, 5*QUARTER_PI );
    // Magnitude da velocidade da bola
    float vel = 6;
    // velocidade da bola em 2D
    float vbx = vel * cos(a);
    float vby = vel * sin(a);

    //variaveis de controle, representam o estado das teclas usadas pelos jogadores
    int p1u = 0, p1d = 0, p2u = 0, p2d = 0;



    //Inicio do loop de animação
    while ( loop ) {
        
        SDL_Event event;
        while( SDL_PollEvent(&event) ){
            switch (event.type) {
                case SDL_QUIT:
                    loop = 0;
                    break;
                
                case SDL_KEYDOWN://tecla apertada
                         if( event.key.keysym.sym == 'w' )       p1u = 1;
                    else if( event.key.keysym.sym == 's' )       p1d = 1;
                    else if( event.key.keysym.sym == SDLK_UP )   p2u = 1;
                    else if( event.key.keysym.sym == SDLK_DOWN ) p2d = 1; 
                    break;
                case SDL_KEYUP://tecla solta
                         if( event.key.keysym.sym == 'w' )       p1u = 0;
                    else if( event.key.keysym.sym == 's' )       p1d = 0;
                    else if( event.key.keysym.sym == SDLK_UP )   p2u = 0;
                    else if( event.key.keysym.sym == SDLK_DOWN ) p2d = 0;
                    break;

                case SDL_MOUSEMOTION:

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

        //controles com o mouse para o Player 2:
        if( abs(mouseY - (P2_rect.y + hH)) > pvel ){
            if( mouseY < P2_rect.y + hH ) p2u = 1;
            else p2u = 0;
            if( mouseY > P2_rect.y + hH ) p2d = 1;
            else p2d = 0;
        }
        else{
            p2u = 0;
            p2d = 0;
        }

        // executar o controle das raquetes
        if( p1u ) P1_rect.y = constrain( P1_rect.y - pvel, 0, height - H );
        if( p1d ) P1_rect.y = constrain( P1_rect.y + pvel, 0, height - H );
        if( p2u ) P2_rect.y = constrain( P2_rect.y - pvel, 0, height - H );
        if( p2d ) P2_rect.y = constrain( P2_rect.y + pvel, 0, height - H );

        // bola da um passo para frente
        bx += vbx;
        by += vby;

        if( by < br ){// colisao com o teto
          vby *= -1;
          by = br;//restituir a posicao evita bugs!
        }
        if( by > height-br ){// colisao com o chao
          vby *= -1;
          by = height-br;
        }

        //colisoes com as raquetes
        if( bx < P1_rect.x + W + br && by > P1_rect.y && by < P1_rect.y + H ){
            vbx *= -1;
            bx = P1_rect.x + W + br;
        }
        else if( bx > P2_rect.x -br && by > P2_rect.y && by < P2_rect.y + H ){
            vbx *= -1;
            bx = P2_rect.x -br;
        }

        //colisoes com as paredes verticais, os "gols"
        bool reset_ball = 0;
        if( bx < 0 ){
          ++p2s;
          render_num_text( renderer, p2s, &p2s_texture, &p2s_dst, font );
          p2s_dst.x = 0.75*width - p2s_dst.w/2;
          reset_ball = 1;
        }
        if( bx > width ){
          ++p1s;
          render_num_text( renderer, p1s, &p1s_texture, &p1s_dst, font );
          p1s_dst.x = 0.25*width - p1s_dst.w/2;
          reset_ball = 1;
        }

        //se houve um gol, resetamos a bola
        if( reset_ball ){
            bx = cx;
            by = cy;

            a = 0;
            int coin = random(1, 10);
            if( coin > 5 ) a = randomD( -QUARTER_PI, QUARTER_PI );
            else a = randomD( 3*QUARTER_PI, 5*QUARTER_PI );

            // ball velocity 2D
            vbx = vel * cos(a);
            vby = vel * sin(a);
        }


        //Limpar o Quadro
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
        SDL_RenderClear( renderer );

        //DESENHAR:

        //Linha de Centro
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine( renderer, cx, 0, cx, height );

        //placar
        SDL_RenderCopy( renderer, p1s_texture, NULL, &p1s_dst );
        SDL_RenderCopy( renderer, p2s_texture, NULL, &p2s_dst );
        
        /*
        //Graficos basicos
        //raquetes
        SDL_RenderFillRect( renderer, &P1_rect );
        SDL_RenderFillRect( renderer, &P2_rect );
        
        //bola
        SDL_RenderFillRect( renderer, &(SDL_Rect){bx-br, by-br, 2*br, 2*br} );
        */

        //Graficos com sprites
        //raquetes
        SDL_RenderCopy( renderer, sprites, &P1_paddle_src, &P1_rect );
        SDL_RenderCopy( renderer, sprites, &P2_paddle_src, &P2_rect );
        //bola
        SDL_RenderCopyF( renderer, sprites, &ball1_src, &(SDL_FRect){bx-br, by-br, 2*br, 2*br} );


        // Apresentar o quadro para o usuário
        SDL_RenderPresent(renderer);

        SDL_framerateDelay( 16 );
    }
    // saimos do loop de animacao, o jogo foi fechado.

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

