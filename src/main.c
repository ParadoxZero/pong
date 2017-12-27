#include <stdio.h>
#include <SDL2/SDL.h>

#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640
#define BOX_PADDING 10
#define PONG_HEIGHT 10
#define PONG_WIDTH 60

const int BOX_X = BOX_PADDING;
const int BOX_Y = BOX_PADDING;
const int BOX_W = WINDOW_WIDTH - 2 * BOX_PADDING;
const int BOX_H = WINDOW_HEIGHT - 2 * BOX_PADDING;
const SDL_Rect BOX = {BOX_X,BOX_Y,BOX_W,BOX_H};

struct Ball {
    int x,y ;
    int Vx, Vy;
};

typedef SDL_Rect Pong;
typedef struct Ball Ball;

void errorLog(FILE *output, char *message){
    fprintf(output,"%s : %s",message,SDL_GetError());
}

SDL_Texture* loadTexture(char *file_path, SDL_Renderer *ren){
    SDL_Surface *bmp = SDL_LoadBMP(file_path);
    SDL_Texture *tex = NULL;
    if(bmp != NULL){
        tex = SDL_CreateTextureFromSurface(ren,bmp);
        SDL_FreeSurface(bmp);
        if(tex == NULL){
            errorLog(stderr,"Cannot load texture");
        }
    }
    else{
        errorLog(stderr,"Cannot load the Asset");
    }
    return tex;

}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    SDL_QueryTexture(tex,NULL,NULL,&rect.w,&rect.h);
    SDL_RenderCopy(ren,tex,NULL,&rect);
}

void drawStage(SDL_Renderer *renderer, Pong player1, Pong player2, Ball ball){
    SDL_RenderFillRect(renderer, &BOX);
    SDL_RenderFillRect(renderer, &player1);
    SDL_RenderFillRect(renderer, &player2);
}
int main() {
    if(SDL_Init(SDL_INIT_VIDEO)!= 0){
        fprintf(stderr,"Error Initializing Graphics");
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow("Pong",100,100,WINDOW_HEIGHT,WINDOW_WIDTH,SDL_WINDOW_SHOWN);
    if (win == NULL){
        fprintf(stderr,"Cannot create window: %s", SDL_GetError());
        SDL_Quit();
        return 2;
    }
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(ren == NULL){
        SDL_DestroyWindow(win);
        errorLog(stderr,"Cannot create renderer");
        SDL_Quit();
        return 2;
    }

    SDL_Surface *screen = SDL_CreateRGBSurface(0,WINDOW_WIDTH,WINDOW_HEIGHT,32,0,0,0,0);
    if(screen == NULL){
        SDL_DestroyWindow(win);
        SDL_Quit();
    }


    return 0;
}