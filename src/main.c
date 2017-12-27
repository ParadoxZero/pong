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
#define DEBUG

#include <stdio.h>
#include <SDL2/SDL.h>

#include "Circle.h"

#define WINDOW_HEIGHT   480
#define WINDOW_WIDTH    640
#define BOX_PADDING     10
#define PONG_PADDING    10
#define PONG_HEIGHT     10
#define PONG_WIDTH      60
#define BALL_RADIUS     10
#define STEP_SIZE       20


const int BOX_X = BOX_PADDING;
const int BOX_Y = BOX_PADDING;
const int BOX_W = WINDOW_WIDTH - 2 * BOX_PADDING;
const int BOX_H = WINDOW_HEIGHT - 2 * BOX_PADDING;
const SDL_Rect BOX = {BOX_X,BOX_Y,BOX_W,BOX_H};

typedef struct Ball {
    int x,y ;
    int Vx, Vy;
} Ball;

typedef SDL_Rect Pong;
typedef SDL_Rect Collider;

Pong player1 = {WINDOW_WIDTH/2 - PONG_WIDTH, BOX_PADDING+PONG_PADDING, PONG_WIDTH,PONG_HEIGHT};
Pong player2 = {WINDOW_WIDTH/2 - PONG_WIDTH, WINDOW_HEIGHT - BOX_PADDING - PONG_PADDING - PONG_HEIGHT, PONG_WIDTH, PONG_HEIGHT};
Ball ball = {WINDOW_HEIGHT/2, WINDOW_WIDTH/2, 1,1};

Collider ball_collider;
Collider player1_collider, player2_collider; //Initialized in Main
Collider box_collider_left = {BOX_X+5,BOX_Y,10,BOX_H};
Collider box_collider_right = {BOX_X+BOX_W-BOX_PADDING-5,BOX_Y,10,BOX_H};
Collider p1_gameover_collider = {0,0,WINDOW_WIDTH,BOX_PADDING};
Collider p2_gameover_collider = {0,WINDOW_HEIGHT-BOX_PADDING,WINDOW_WIDTH,BOX_PADDING};

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

void drawStage(SDL_Renderer *renderer){
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    SDL_RenderDrawRect(renderer, &BOX);
    SDL_RenderFillRect(renderer, &player1);
    SDL_RenderFillRect(renderer, &player2);
    SDL_Point centre = {ball.x,ball.y};
    drawFilledCircle(renderer, centre, BALL_RADIUS);
#ifdef DEBUG
    SDL_RenderDrawRect(renderer,&box_collider_left);
    SDL_RenderDrawRect(renderer,&box_collider_right);
    SDL_RenderDrawRect(renderer,&player2_collider);
    SDL_RenderDrawRect(renderer,&ball_collider);
    SDL_RenderDrawRect(renderer,&p1_gameover_collider);
    SDL_RenderDrawRect(renderer,&p2_gameover_collider);

#endif
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderPresent(renderer);
}

int checkCollision(const Collider *a, const Collider *b){
    return SDL_HasIntersection(a,b);
}
void handleInput(SDL_Event event){
    SDL_Keycode key = event.key.keysym.sym;
    if(key == SDLK_LEFT){
        if(checkCollision(&player2_collider,&box_collider_left) == SDL_FALSE) {
            player2.x -= STEP_SIZE;
            player2_collider.x = player2.x;
        }
    }
    else if ( key == SDLK_RIGHT){
        if(checkCollision(&player2_collider,&box_collider_right) == SDL_FALSE) {
            player2.x += STEP_SIZE;
            player2_collider.x = player2.x;
        }
    }
}

void gameover(char* mesg){
    printf("Game over %s", mesg);
    SDL_Quit();
    exit(0);
}
int main() {
    player2_collider = player2;
    player1_collider = player1;
    ball_collider.x = ball.x - BALL_RADIUS;
    ball_collider.y = ball.y - BALL_RADIUS;
    ball_collider.w = ball_collider.h = 2 * BALL_RADIUS;
    if(SDL_Init(SDL_INIT_VIDEO)!= 0){
        fprintf(stderr,"Error Initializing Graphics");
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow("Pong",100,100,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
    if (win == NULL){
        fprintf(stderr,"Cannot create window: %s", SDL_GetError());
        SDL_Quit();
        return 2;
    }
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if(ren == NULL){
        SDL_DestroyWindow(win);
        errorLog(stderr,"Cannot create renderer");
        SDL_Quit();
        return 2;
    }
    int loop = 1;
    while(loop)
    {
        ball.x += ball.Vx;
        ball_collider.x += ball.Vx;
        ball.y += ball.Vy;
        ball_collider.y += ball.Vy;

        if(checkCollision(&ball_collider,&player1_collider)==1||checkCollision(&ball_collider,&player2_collider)==1){
            ball.Vy = -ball.Vy;
        }
        if(checkCollision(&ball_collider,&box_collider_right)==1||checkCollision(&ball_collider,&box_collider_left)==1){
            ball.Vx = -ball.Vx;
        }
        if(checkCollision(&ball_collider,&p1_gameover_collider)==1){
            gameover("Player1");
        }
        if(checkCollision(&ball_collider,&p2_gameover_collider)==1){
            gameover("Player2");
        }
        printf("(%d,%d):%d %d\n",ball_collider.x,ball_collider.y,ball_collider.w,ball_collider.h);
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                loop = 0;
            }
            else if(event.type == SDL_KEYDOWN){
                handleInput(event);
            }
        }
        drawStage(ren);


    }

    return 0;
}