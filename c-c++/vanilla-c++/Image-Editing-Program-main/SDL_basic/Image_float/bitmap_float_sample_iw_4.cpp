
#include <SDL2/SDL.h>

#include <iostream>
using namespace std;

int main()
{

    SDL_Window* first_window = nullptr;
	SDL_Surface* window_surface = nullptr;
	SDL_Surface* image_surface = nullptr;

	bool isRunning = 1;
	SDL_Event ev;

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
			window_surface = SDL_GetWindowSurface(first_window);
			image_surface = SDL_LoadBMP("SDL_basic/Image_float/executable/sample_iw_8_20_20_size_bmp.bmp");
			// SDL_LoadBMP_RW(SDL_RWFromFile("sample_iw_8_20_20_size_bmp.bmp", "rb"), 1)


			if(image_surface == NULL)
			{
				cout<<"Image Load Failed"<<endl<<SDL_GetError()<<endl;
			}
			else
			{
				//Important function that show Image in program
				SDL_BlitSurface(image_surface, NULL, window_surface, NULL);
				SDL_UpdateWindowSurface(first_window);
				while(isRunning)
				{
					// cout<<SDL_PollEvent(&ev)<<endl;
					while(SDL_PollEvent(&ev) != 0)
					{
						// cout<<"/";
						// cout<<SDL_PollEvent(&ev)<<endl;//프로그램이 닫힘 버튼이 눌렸을때만 잠시 1이 되었다가 함수 호출 이후 2번쨰 호출시에는 0이 되어 있음.
						if(ev.type == SDL_QUIT)
						{
							isRunning = 0;
						}
					}
					// cout<<SDL_PollEvent(&ev)<<endl;
					// cout<<ev.type<<endl;
				}
			}
		}
    }
    
	//First window is allocated by dynamic mathod by, SDL_CreateWindow() function, therefore it have to be free after use
	SDL_FreeSurface(image_surface);
	SDL_DestroyWindow(first_window);
	image_surface = nullptr;
	window_surface = nullptr;

	first_window = nullptr;

	SDL_Quit();
    return 0;

}

//this code is referenced on 
//https://www.youtube.com/watch?v=Y4CHORvYLVI&list=PLHJE4y54mpC5_eEz9gCqIkNpU-n_2eyNt&index=2


//되긴 하는데 이미지 소스파일경로가 파일이 존재하는 디렉터리보다 상위 폴더의 파일이 읽힌다는 점이 의문임...
//image_surface = SDL_LoadBMP("sample_iw_8_20_20_size_bmp.bmp");

//image_surface = SDL_LoadBMP("sample_iw_8_20_20_size_bmp.bmp");


// image_surface = SDL_LoadBMP("SDL_basic/Image_float/executable/sample_iw_8_20_20_size_bmp.bmp");

//경로를 이와 같이 최상위 폴더로 부터의 경로를 상세히 적어주니 오류가 해결이 되었음.

