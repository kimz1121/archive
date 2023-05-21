
#include <SDL2/SDL.h>

#include <iostream>
using namespace std;

int main()
{

    SDL_Window* first_window = nullptr;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout<<"SDL Video Initialize failed"<<endl<<SDL_GetError()<<endl;
    }
    else
    {
        first_window = SDL_CreateWindow("First_SDL_window_float", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, SDL_WINDOW_SHOWN);
		if(first_window == NULL)
		{
			cout<<"window Creation failed"<<SDL_GetError()<<endl;
		}
		else
		{
			SDL_UpdateWindowSurface(first_window);
			SDL_Delay(2000);
		}
		
    }
    
	//First window is allocated by dynamic mathod by, SDL_CreateWindow() function, therefore it have to be free after use

	SDL_DestroyWindow(first_window);
	SDL_Quit();
    return 0;

}

//this code is referenced on 
//https://www.youtube.com/watch?v=Y4CHORvYLVI&list=PLHJE4y54mpC5_eEz9gCqIkNpU-n_2eyNt&index=2



// 출처: https://bearsopenstory.tistory.com/entry/SDL-SDL을-이용하여-윈도우-창을-띄워보자?category=113196 [곰들의 열린 이야기]