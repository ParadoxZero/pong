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

#include "Circle.h"

static void plotCirclePoints(SDL_Renderer *renderer, int x, int y, int cx, int cy){
    SDL_Point points[8] = {{x+cx,y+cy},{y+cx,x+cy},{-x+cx,y+cy}, {-y+cx,x+cy},
                           {-x+cx,-y+cy},{-y+cx,-x+cy},{x+cx,-y+cy},{y+cx,-x+cy}};
    SDL_RenderDrawPoints(renderer,points,8);
}

void drawCircle(SDL_Renderer *renderer, SDL_Point centre, int radius){
    int x = radius;
    int y = 0;
    int p = 1 - radius;

    plotCirclePoints(renderer, centre.x, centre.y, x, y);
    while (x > y){
        y++;
        if(p<=0){
            p = p + 2*y + 1;
        }
        else {
            x--;
            p = p - 2*x + 2*y + 1;
        }
        plotCirclePoints(renderer,centre.x,centre.y,x,y);
    }
}

void drawFilledCircle(SDL_Renderer *renderer, SDL_Point centre, int radius){
    for(int i=0;i<radius;++i){
        drawCircle(renderer,centre,i);
    }
}