
#include <SDL2/SDL.h>

#include <iostream>
#include <cstring>

using namespace std;

class viewer
{

	SDL_Window* first_window = nullptr;
	SDL_Surface* window_surface = nullptr;
	SDL_Surface* image_surface = nullptr;

	bool isRunning = 0;
	SDL_Event ev;

	public:

	viewer(const char* name_of_window, int width, int height)
	{
		isRunning = 1;

		if(SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			cout<<"SDL Video Initialize failed"<<endl<<SDL_GetError()<<endl;
		}
		else
		{
			first_window = SDL_CreateWindow(name_of_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);	
		}
		if(first_window == nullptr)
		{
			cout<<"window Creation failed"<<SDL_GetError()<<endl;
		}
		else
		{
			window_surface = SDL_GetWindowSurface(first_window);//생성 오류 애러시 다른 함수들에서 에러처리 필요할 가능성 있음.
		}
	}

	void create_d24_Image_surface_in_SDL(int width, int height)
	{
		if(first_window == nullptr)
		{
			cout<<"window Creation failed"<<SDL_GetError()<<endl;
		}
		else
		{
			if(image_surface == nullptr)
			{
				image_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24, 0xff0000, 0x00ff00, 0x0000ff, 0x000000);
				memset((*image_surface).pixels, 0x00, ((*image_surface).format->BytesPerPixel)*width*height);//색상을 런 타이밍 중에 변경하는 것 성공!!
			}
			else
			{
				SDL_FreeSurface(image_surface);
				image_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24, 0xff0000, 0x00ff00, 0x0000ff, 0x000000);
				memset((*image_surface).pixels, 0x00, ((*image_surface).format->BytesPerPixel)*width*height);//색상을 런 타이밍 중에 변경하는 것 성공!!
			}
			if(image_surface == nullptr)
			{
				cout<<"Image Load Failed"<<endl<<SDL_GetError()<<endl;
			}
		}
	}

	void image_deilvery(char* refined_data_deilvery_buff)
	{
		if(image_surface == nullptr)
		{
			cout<<"Image Load Failed"<<endl<<SDL_GetError()<<endl;
		}
		else
		{
			memset((*image_surface).pixels, 0x00, ((*image_surface).format->BytesPerPixel)*((*image_surface).w)*((*image_surface).h));
		}
	}

	void image_safty_load(int width, int height, char* refined_data_deilvery_buff)
	{
		create_d24_Image_surface_in_SDL(width, height);
		image_deilvery(refined_data_deilvery_buff);
	}

	void screen_float()
	{
		if(first_window == nullptr)
		{
			cout<<"window Creation failed"<<SDL_GetError()<<endl;
		}
		else
		{
			if(image_surface == nullptr)
			{
				cout<<"Image Load Failed"<<endl<<SDL_GetError()<<endl;
			}
			else
			{
				//Important function that show Image in program
				SDL_BlitSurface(image_surface, nullptr, window_surface, nullptr);
				cout<<"7"<<endl;
				SDL_UpdateWindowSurface(first_window);
				cout<<"8"<<endl;




				// cout<<sizeof((*image_surface).pixels)<<endl;
				// cout<<(*image_surface).w<<endl;
				// cout<<(*image_surface).h<<endl;
				cout<<(int)((*image_surface).format->BytesPerPixel)<<endl;
				cout<<(int)((*image_surface).format->BitsPerPixel)<<endl;
				cout<<(int)((*image_surface).format->Rmask)<<endl;
				cout<<(int)((*image_surface).format->Gmask)<<endl;
				cout<<(int)((*image_surface).format->Bmask)<<endl;
				cout<<(int)((*image_surface).format->Amask)<<endl;
				cout<<(int)((*image_surface).format->Rloss)<<endl;
				cout<<(int)((*image_surface).format->Gloss)<<endl;
				cout<<(int)((*image_surface).format->Bloss)<<endl;
				cout<<(int)((*image_surface).format->Aloss)<<endl;
			}
		}
	}


	int event_activator()//이벤트나 인터럽트를 사용하는 방법을 알아봐야 겠다.
	{
		int LOOP_state = 1;

		while(SDL_PollEvent(&ev) != 0)//have event been occurred?
		{
			if(ev.type == SDL_QUIT)
			{
				LOOP_state = 0;
				SDL_FreeSurface(image_surface);
				SDL_DestroyWindow(first_window);
				SDL_Quit();//SDL_Quit(); is included
			}
		}
		return LOOP_state;
	}

	// while(isRunning)
	// 			{
	// 				// cout<<SDL_PollEvent(&ev)<<endl;
	// 				while(SDL_PollEvent(&ev) != 0)
	// 				{
	// 					// cout<<"/";
	// 					// cout<<SDL_PollEvent(&ev)<<endl;//프로그램이 닫힘 버튼이 눌렸을때만 잠시 1이 되었다가 함수 호출 이후 2번쨰 호출시에는 0이 되어 있음.
	// 					if(ev.type == SDL_QUIT)
	// 					{
							
	// 					}
	// 				}
	// 				// cout<<SDL_PollEvent(&ev)<<endl;
	// 				// cout<<ev.type<<endl;
	// 			}

	//First window is allocated by dynamic mathod by, SDL_CreateWindow() function, therefore it have to be free after use
	~viewer()
	{
		isRunning = 0;
		// SDL_FreeSurface(image_surface);
		cout<<"9"<<endl;

		// SDL_DestroyWindow(first_window);
		cout<<"10"<<endl;
		image_surface = nullptr;
		window_surface = nullptr;

		first_window = nullptr;

		// SDL_Quit();
	}
};

int main()
{
	viewer V("image_viewer", 1000, 600);
	V.create_d24_Image_surface_in_SDL(20, 20);
	V.screen_float();

	while(V.event_activator() != 0)
	{
		//화면이 켜져 있는 동안 할 작업// 매루프마다 이벤트가 확인됨.
	}

}

//this code is referenced on 
//https://www.youtube.com/watch?v=Y4CHORvYLVI&list=PLHJE4y54mpC5_eEz9gCqIkNpU-n_2eyNt&index=2


//되긴 하는데 이미지 소스파일경로가 파일이 존재하는 디렉터리보다 상위 폴더의 파일이 읽힌다는 점이 의문임...
//image_surface = SDL_LoadBMP("sample_iw_8_20_20_size_bmp.bmp");

//image_surface = SDL_LoadBMP("sample_iw_8_20_20_size_bmp.bmp");


// image_surface = SDL_LoadBMP("SDL_basic/Image_float/executable/sample_iw_8_20_20_size_bmp.bmp");

//경로를 이와 같이 최상위 폴더로 부터의 경로를 상세히 적어주니 오류가 해결이 되었음.

