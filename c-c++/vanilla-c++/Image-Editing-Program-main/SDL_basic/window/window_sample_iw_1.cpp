#include "SDL2/SDL.h"

int main()
{

    bool done = false;
    SDL_Event event;
    SDL_Surface *screen;

    SDL_Init(SDL_INIT_VIDEO);

    screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF);

    while(!done)
    {

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));

        while(SDL_PollEvent(&event))
        {

            switch(event.type)
            {

                case SDL_QUIT:

                    done = 1;

                break;

            }

        }

        SDL_Flip(screen);

    }

    SDL_Quit();

    return 0;

}


// 출처: https://bearsopenstory.tistory.com/entry/SDL-컬러키와-알파값을-설정하여 [곰들의 열린 이야기]