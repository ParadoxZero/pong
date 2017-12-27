//
// Created by Sidhin Thomas on 27/12/17.
//

#ifndef PONG_CIRCLE_H
#define PONG_CIRCLE_H

#include <SDL2/SDL.h>

void drawCircle(SDL_Renderer *renderer, SDL_Point centre, int radius);
void drawFilledCircle(SDL_Renderer *renderer, SDL_Point centre, int radius);

#endif //PONG_CIRCLE_H
