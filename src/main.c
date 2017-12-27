/*
 *
 *   MIT License
 *
 *   Copyright (c) 2017 Sidhin S Thomas
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

#include <stdio.h>
#include <SDL2/SDL.h>

#include "Circle.h"

#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640
#define BOX_PADDING 10
#define PONG_PADDING 10
#define PONG_HEIGHT 10
#define PONG_WIDTH 60
#define BALL_RADIUS 5


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
    SDL_RenderClear(renderer);
    SDL_RenderFillRect(renderer, &BOX);
    SDL_RenderFillRect(renderer, &player1);
    SDL_RenderFillRect(renderer, &player2);
    SDL_Point centre = {ball.x,ball.y};
    drawFilledCircle(renderer, centre, BALL_RADIUS);
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
    SDL_SetRenderDrawColor(ren,0,0,0,1);
    Pong player1 = {WINDOW_WIDTH/2 - PONG_WIDTH, BOX_PADDING+PONG_PADDING, PONG_WIDTH,PONG_HEIGHT};
    Pong player2 = {WINDOW_WIDTH/2 - PONG_WIDTH, WINDOW_HEIGHT - BOX_PADDING - PONG_PADDING, PONG_WIDTH, PONG_HEIGHT};
    Ball ball = {WINDOW_HEIGHT/2, WINDOW_WIDTH/2, 0,0};
    //drawStage(ren,player1,player2,ball);
    SDL_Delay(10000);

    return 0;
}