/*
file name: binary_file_editor.cpp
created: 2021.01.18
modified: 2021.01.25
author: kimz1121, ekg1229 
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
#include <stdlib.h>
// #include <windows.h>

using namespace std;

struct file_header // 비트맵 파일 헤더
{
	uint32_t 	bfType;			// BMP 파일 매직 넘버
	uint32_t	bfSize;			// 바이트 단위로 전체 파일 크기
	uint32_t 	bfReserved1;	// 예약된 변수
	uint32_t 	bfReserved2;	// 예약된 변수
	uint32_t	bfOffBits;		// 비트맵 데이터의 시작 위치
};

struct info_header // 비트맵 정보 헤더
{
	uint32_t 	biSize;				// 현재 비트맵 정보 헤더의 크기
	uint32_t 	biWidth;			// 픽셀단위로 영상의 폭
	uint32_t 	biHeight;			// 영상의 높이
	uint32_t 	biplanes;			// 비트 플레인 수 (항상 1)
	uint32_t 	biBitCount;			// 픽셀당 비트수 (컬러, 흑백 구별) 
	uint32_t 	biCompression;		// 압축 유무
	uint32_t 	biSizeImage;		// 영상의 크기 (바이트 단위)
	uint32_t 	biXPelsPerMeter;	// 가로 해상도
	uint32_t 	biYPelsPerMeter;	// 세로 해상도
	uint32_t 	biClrUsed;			// 실제 사용 색상 수
	uint32_t 	biClrImportant;		// 중요한 색상 인덱스
};

struct layer
{
	/*
		마스크 사용 규칙 정리

		1.
		마스크는 최종용 1개와 과 레이어 용 래이어 마다 2개씩 가지며,
		레이어 인스턴스 복제에따라서 레이어용 마스크의 수는 가변적이다.

		2.
		마스크는 2차원 배열로 구현한다.
		즉, 실체는 더블 포인터와 같다.
	*/

	/*

		레이어 구조체 설명

		1. 레이어 구조체의 역할
		-필요한 데이터를 생성 시와 소멸 시 자동으로 할당하고 해제한다.
		-레이어 범위에서의 그리기 영역선택을 한다.
		-그리기를 했던 영역으로 써의 자취를 통해 그려진 형상을 저장한다.
		-그리기 한 형상의 색을 저장한다. 
		-레이어를 최종 저장본에 적용할지 말지 레이커 켜고 끄기를 통해 선택할 수 있다.
		
		-없는 색은 아직 구현하지 않았으나 색도 형상도 없는 것으로 구현하는 방법이 있다.

		2. 레이어 구조체의 구조
		
		변수
		
		#2개의 마스크
		-레이어 영역 마스크
		>dptr_mask_area[x][y]
		-레이어 형상 마스크
		>dptr_mask_shape[x][y]
		
		#2개의 정보
		-색상 정보
		>tptr_data_coulor[x][y][RGB]
		-끄고 켜기 정보
		>data_on_off

		#3개의 전달 맴버 변수
		>member_width
		>member_height
		>member_byte_for_pixel

		함수
		-생성자
		-소멸자
		-적재
		-저장
		-초기화
		-영역 선택
		-1 픽셀단위 쓰기(색 및 형상 편집)
		-1 픽셀단위 지우기
		

		3. 레이어 구조체의 사용
		-레이어 구조체는 기본적으로 다른 클래스에 맴버로써 복제하여 사용한다.
		-레이어에 버퍼의 데이터를 적재하여 사용한다. 사용 후 적재 명령어로 버퍼로 덮어쓰기하여 데이터를 출력한다. 
		-편집은 1픽셀단위로만 가능함으로 쓰기 함수를 무수히 여러번 호출하여 사용한다. 그러나 동시에 여러개를 호출하는 것은 오버플로를 일으킬 수 있음으로 지양한다.
		-적재시에는 항상 사이즈가 충분한 알맞은 버퍼를 사용한다. 

		4. 작성자의 규칙

		dptr 접두사로 시작하는 변수는 더블 포인터 이다.
		tptr 접두사로 시작하는 변수는 트리플 포인터 이다.
		member 접두사로 시작하는 변수는 editor 객체로 부터 전달 받지만 객체가 종속되지 않고도 고유하게 동작하도록 중복하여 선언한 변수이다.

		-일단 위처럼 계획하구 있어요! 다른 의견 있으면 말씀해 주세요!

	*/

	bool** dptr_mask_area = nullptr;//마스크 행렬이 부여한느 방법이 있고 영역이 항상 사각형이라면 좌표로 부여하는 방법도 있다....차후 브러시를 이용한 영역선택과 같은 고급 기능을 위해 행렬로 부여하자.
	bool** dptr_mask_shape = nullptr;
	uint8_t*** tptr_data_coulor = nullptr;
	bool data_on_off = -1;

	int member_width = 0;
	int member_height = 0;
	int member_byte_for_pixel = 0;

	layer(int width, int height, int byte_for_pixel)
	{
		layer::member_width = width;
		layer::member_height = height;
		layer::member_byte_for_pixel = byte_for_pixel;

		dptr_mask_area = new bool*[width];
		dptr_mask_shape = new bool*[width];
		tptr_data_coulor = new uint8_t**[width];

		for(int i = 0; i<width; i++)
		{
			dptr_mask_area[i] = new bool[height];
			dptr_mask_shape[i] = new bool[height];
			tptr_data_coulor[i] = new uint8_t*[height];

			for(int j = 0; j<height; j++)
			{
				tptr_data_coulor[i][j] = new uint8_t[byte_for_pixel];
			}
		}
		clear();
	}

	~layer()
	{
		for(int i = 0; i<member_height; i++)
		{
			for(int j = 0; j<member_byte_for_pixel; j++)
			{
				delete tptr_data_coulor[i][j];
			}
			delete dptr_mask_area[i];
			delete dptr_mask_shape[i];
			delete tptr_data_coulor[i];
		}
		delete dptr_mask_area;
		delete dptr_mask_shape;
		delete tptr_data_coulor;

		bool** dptr_mask_area = nullptr;
		bool** dptr_mask_shape = nullptr;
		uint8_t*** tptr_data_coulor = nullptr;

	}

	void area(int pos_1_x, int pos_1_y, int pos_2_x, int pos_2_y)
	{
		if(pos_1_x>pos_2_x)
		{
			pos_1_x = pos_1_x^pos_2_x;
			pos_2_x = pos_1_x^pos_2_x;
			pos_1_x = pos_1_x^pos_2_x;

		}

		if(pos_1_y>pos_2_y)
		{
			pos_1_y = pos_1_y^pos_2_y;
			pos_2_y = pos_1_y^pos_2_y;
			pos_1_y = pos_1_y^pos_2_y;
		}
		//xor swap

		if(pos_1_x < 0)
		{
			pos_1_x = 0;
		}
		if(pos_1_y < 0)
		{
			pos_1_y = 0;
		}
		if(pos_2_x >= member_width)
		{
			pos_2_x = member_width-1;
		}
		if(pos_2_y >= member_height)
		{
			pos_2_y = member_height-1;
		}

		for(int i = pos_1_x; i <= pos_2_x; i++)
		{
			for(int j = pos_1_y; j <= pos_2_y; j++)
			{
				dptr_mask_area[i][j] = 1;
			}
		}
	}

	void area_set_total(bool Boolean)
	{
		for(int i = 0; i < member_width; i++)
		{
			for(int j = 0; j < member_height; j++)
			{
				dptr_mask_area[i][j] = Boolean;
			}
		}
	}

	void clear()
	{
		for(int i = 0; i < member_width; i++)
		{
			for(int j = 0; j < member_height; j++)
			{
				dptr_mask_area[i][j] = 0;
				dptr_mask_shape[i][j] = 0;
				tptr_data_coulor[i][j][0] = 0;
				tptr_data_coulor[i][j][1] = 0;
				tptr_data_coulor[i][j][2] = 0;
			}
		}
	}

	void pixel(int pos_x, int pos_y, bool draw, uint8_t R, uint8_t G, uint8_t B)
	{
		if(draw)//쓰기 상태
		{
			cout<<"pixel_test_1"<<endl;
			dptr_mask_shape[pos_x][pos_y] = 1;
			cout<<"pixel_test_2"<<endl;
			tptr_data_coulor[pos_x][pos_y][0] = B;
			cout<<"pixel_test_3"<<endl;
			tptr_data_coulor[pos_x][pos_y][1] = G;
			cout<<"pixel_test_4"<<endl;
			tptr_data_coulor[pos_x][pos_y][2] = R;
			cout<<"pixel_test_5"<<endl;
		}
		else//지우기 상태
		{
			dptr_mask_shape[pos_x][pos_y] = 0;
			tptr_data_coulor[pos_x][pos_y][0] = 0;
			tptr_data_coulor[pos_x][pos_y][1] = 0;
			tptr_data_coulor[pos_x][pos_y][2] = 0;
		}
	}
	
	void load()
	{

	}

	void store(char* refined_data_buff, int buff_width, int buff_height)
	{
		for (int y = 0; y < member_height; y++)
		{
			for (int x = 0; x < member_width; x++)
			{
				if(dptr_mask_shape[x][y] && dptr_mask_area[x][y])
				{
					*(refined_data_buff+(member_byte_for_pixel*x)+(member_byte_for_pixel*buff_width*(member_height-(y+1)))+0) = tptr_data_coulor[x][y][0];
					*(refined_data_buff+(member_byte_for_pixel*x)+(member_byte_for_pixel*buff_width*(member_height-(y+1)))+member_byte_for_pixel/3) = tptr_data_coulor[x][y][1];
					*(refined_data_buff+(member_byte_for_pixel*x)+(member_byte_for_pixel*buff_width*(member_height-(y+1)))+(member_byte_for_pixel/3)*2) = tptr_data_coulor[x][y][2];
				}
			}//이 부분의 대입에서 x와 y의 좌표값을 반전하여 대입하면 레이어 단위로 반전 시키는 것이 가능함.
		}
	}
};

class bmp_editor
{
	public:
	ifstream f_in_image;
	ofstream f_out_image;

	char file_head[5] = { 2, 4, 2, 2, 4 };	// 단 하나만 존재하면 되는 전역 변수로는 어떻게 선언할까?
	char info_head[11] = { 4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4 };

	int size_of_file = 0;
	const int size_of_header = 54;
	int pure_size_of_data = 0;

	char* rare_buff = NULL;

	char* head_buff = NULL;
	char* package_data_buff = NULL;
	char* reverse_data_buff = NULL;

	char* edit_head_buff = NULL;

	char* edit_refined_data_buff = NULL;	

	
	char* data_start_ptr = NULL;

	// (*buff_ptr) = temp_buff;
	// buff_ptr++;

	bool** dptr_mask_area_editor_final = nullptr;


	public:

	struct layer* layer_arr[5] = {nullptr, };// 차후 링크드 리스트로 자료의 양을 동적으로 조절하면 좋을 듯

	struct file_header bitmap_FILE_HEADER = {0, 0, 0, 0, 0};
	struct info_header bitmap_INFO_HEADER = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	bmp_editor(const char *f_in_name)
	{
		open_file(f_in_name, 'I');
		size_of_file = get_file_size();
		
		rare_buff = new char[size_of_file];
		cout << sizeof(*rare_buff) << endl;
		// cout<<sizeof(rare_buff)<<endl;

		f_in_image.read(rare_buff, size_of_file);

		// edit_head_buff = new char(size_of_header);//실수 박제
		

		head_buff = new char[size_of_header];

		read_header_bmp();
		
		edit_head_buff = new char[size_of_header];

		edit_head_load();

		package_data_buff = new char[bitmap_INFO_HEADER.biSizeImage];
		reverse_data_buff = new char[bitmap_INFO_HEADER.biSizeImage];

		read_data_bmp();

		pure_size_of_data = bitmap_INFO_HEADER.biWidth*bitmap_INFO_HEADER.biHeight*(bitmap_INFO_HEADER.biBitCount/8);

		edit_refined_data_buff = new char[pure_size_of_data];
		cout <<pure_size_of_data<< endl;

		edit_data_load();

		//edit_data_buff = new char(size_of_file-size_of_header);

		dptr_mask_area_editor_final = new bool*[bitmap_INFO_HEADER.biWidth];

		for(int i = 0; i<bitmap_INFO_HEADER.biWidth; i++)
		{
			dptr_mask_area_editor_final[i] = new bool[bitmap_INFO_HEADER.biHeight];
		}

		layer_arr[0] = new layer(bitmap_INFO_HEADER.biWidth, bitmap_INFO_HEADER.biHeight, bitmap_INFO_HEADER.biBitCount/8);
		layer_arr[1] = new layer(bitmap_INFO_HEADER.biWidth, bitmap_INFO_HEADER.biHeight, bitmap_INFO_HEADER.biBitCount/8);
		layer_arr[2] = new layer(bitmap_INFO_HEADER.biWidth, bitmap_INFO_HEADER.biHeight, bitmap_INFO_HEADER.biBitCount/8);
		layer_arr[3] = new layer(bitmap_INFO_HEADER.biWidth, bitmap_INFO_HEADER.biHeight, bitmap_INFO_HEADER.biBitCount/8);
		layer_arr[4] = new layer(bitmap_INFO_HEADER.biWidth, bitmap_INFO_HEADER.biHeight, bitmap_INFO_HEADER.biBitCount/8);
		// layer_arr[5] = new layer(bitmap_INFO_HEADER.biWidth, bitmap_INFO_HEADER.biHeight, bitmap_INFO_HEADER.biBitCount/8);
		// layer_arr[6] = new layer(bitmap_INFO_HEADER.biWidth, bitmap_INFO_HEADER.biHeight, bitmap_INFO_HEADER.biBitCount/8);
		// layer_arr[7] = new layer(bitmap_INFO_HEADER.biWidth, bitmap_INFO_HEADER.biHeight, bitmap_INFO_HEADER.biBitCount/8);
		// layer_arr[8] = new layer(bitmap_INFO_HEADER.biWidth, bitmap_INFO_HEADER.biHeight, bitmap_INFO_HEADER.biBitCount/8);
		// layer_arr[9] = new layer(bitmap_INFO_HEADER.biWidth, bitmap_INFO_HEADER.biHeight, bitmap_INFO_HEADER.biBitCount/8);
	}

	void area_editor(int pos_1_x, int pos_1_y, int pos_2_x, int pos_2_y)
	{
		if(pos_1_x>pos_2_x)
		{
			pos_1_x = pos_1_x^pos_2_x;
			pos_2_x = pos_1_x^pos_2_x;
			pos_1_x = pos_1_x^pos_2_x;
		}

		if(pos_1_y>pos_2_y)
		{
			pos_1_y = pos_1_y^pos_2_y;
			pos_2_y = pos_1_y^pos_2_y;
			pos_1_y = pos_1_y^pos_2_y;
		}//xor swap

		if(pos_1_x < 0)
		{
			pos_1_x = 0;
		}
		if(pos_1_y < 0)
		{
			pos_1_y = 0;
		}
		if(pos_2_x >= bitmap_INFO_HEADER.biWidth)
		{
			pos_2_x = bitmap_INFO_HEADER.biWidth-1;
		}
		if(pos_2_y >= bitmap_INFO_HEADER.biHeight)
		{
			pos_2_y = bitmap_INFO_HEADER.biHeight-1;
		}

		for(int i = pos_1_x; i <= pos_2_x; i++)
		{
			for(int j = pos_1_y; j <= pos_2_y; j++)
			{
				dptr_mask_area_editor_final[i][j] = 1;
			}
		}

	}

	void area_set_total_editor(bool Boolean)
	{
		for(int i = 0; i < bitmap_INFO_HEADER.biWidth; i++)
		{
			for(int j = 0; j < bitmap_INFO_HEADER.biHeight; j++)
			{
				dptr_mask_area_editor_final[i][j] = Boolean;
			}
		}
	}

	void clear_editor()
	{
		for(int i = 0; i < bitmap_INFO_HEADER.biWidth; i++)
		{
			for(int j = 0; j < bitmap_INFO_HEADER.biHeight; j++)
			{
				dptr_mask_area_editor_final[i][j] = 0;
			}
		}
	}

	void test_draw()
	{
		(*layer_arr[0]).clear();
		(*layer_arr[0]).area(0, 0, 19, 19);// 영역선택 전체

		(*layer_arr[1]).clear();
		(*layer_arr[1]).area_set_total(1);// 영역선택 전체
	
		(*layer_arr[1]).pixel(10, 10, 1, 0xff, 0xff, 0);
		(*layer_arr[1]).pixel(10, 11, 1, 0xff, 0xff, 0);
		(*layer_arr[1]).pixel(11, 10, 1, 0xff, 0xff, 0);
		(*layer_arr[1]).pixel(11, 11, 1, 0xff, 0xff, 0);
		(*layer_arr[1]).pixel(0, 0, 1, 0xff, 0xff, 0);
		(*layer_arr[1]).pixel(0, 1, 1, 0xff, 0xff, 0);
		(*layer_arr[1]).pixel(1, 0, 1, 0xff, 0xff, 0);
		(*layer_arr[1]).pixel(19, 19, 1, 0xff, 0xff, 0);
		(*layer_arr[1]).store(edit_refined_data_buff, 20, 20);
	}

	void open_file(const char *f_name, char I_O)
	{
		if(I_O == 'I' || I_O == 'i')
		{
			f_in_image.open(f_name, ios_base::binary);
		}
		if(I_O == 'O' || I_O == 'o')
		{
			f_out_image.open(f_name, ios_base::binary);
		}
	}

	// "sample_iw_4_4_4_size_bmp.bmp"

	int get_file_size()
	{
		f_in_image.seekg(0, ios::end);	//오프셋을 0 으로 하여 파일의 끝 지점에 그대로 파일 포인터를 옮긴다.
		size_of_file = f_in_image.tellg();
		cout << size_of_file << endl;
		f_in_image.seekg(0, ios::beg);	//오프셋을 0 으로 하여 파일의 처음 지점에 그대로 파일 포인터를 옮긴다.
		return size_of_file;
	}

	int get_data_size()
	{
		// f_in_image.seekg(18, ios::end);

		// unsigned int temp_buff = 0;

		// int n = 0;

		// f_in_image.read((char*)&temp_buff, 1);

		return 0;
	}


	void read_header_bmp()
	{
		char* buff_ptr = NULL;
		buff_ptr = rare_buff;
		f_in_image.seekg(0, ios::beg);
		f_in_image.read(buff_ptr, 54);
		

		// f_in_image.seekg(0, ios::beg);
		// f_in_image.read(head_buff, 54);
		
		cout<<(void*)head_buff<<endl;

		cout<<endl;
		cout<<endl;
		memmove(head_buff, buff_ptr, 54);//왜 에러 일까남... 디버그 돌리고 싶당...ㅠㅠ//편집하다보니 new 키워드 보다 함수의 실행 시점이 늦어진 것이 문제였음.
		
	}

	void read_data_bmp()
	{	
		char* buff_ptr = NULL;
		//byte_for_pixel 한 픽셀을 차지하는 바이트 수 현재 RGB 3색 각 1바이트 씩 총 3바이트
		buff_ptr = rare_buff;
		f_in_image.seekg(54, ios::beg);
		f_in_image.read(buff_ptr+54, bitmap_INFO_HEADER.biSizeImage);

		int height = bitmap_INFO_HEADER.biHeight;
		int width = bitmap_INFO_HEADER.biWidth;
		int byte_for_pixel = (bitmap_INFO_HEADER.biBitCount/8);

		int length_of_row = width*byte_for_pixel/4 + (width*byte_for_pixel%4 != 0);
		int length_of_col = height;

		int i = 0;

		// f_in_image.seekg(54, ios::beg);
		// f_in_image.read(package_data_buff, bitmap_INFO_HEADER.biSizeImage);
		cout<<"1"<<endl;
		memmove(package_data_buff, buff_ptr+54, bitmap_INFO_HEADER.biSizeImage);//디버그로 포인터가 가리키는 주소 값을 보려면 러떻게 하여야 하남...??
		cout<<"2"<<endl;
		memset(reverse_data_buff, 0, bitmap_INFO_HEADER.biSizeImage);
		cout<<"3"<<endl;
		cout<<(void*)reverse_data_buff<<endl;
		while(i < length_of_col)
		{
			cout<<"i : "<<i<<endl;
			cout<<(void*)reverse_data_buff+((length_of_col-i-1)*4*length_of_row)<<endl;

			memmove(reverse_data_buff+((length_of_col-i-1)*4*length_of_row), package_data_buff+(i*4*length_of_row), 4*length_of_row);
			i = i+1;
		}
		cout<<"4"<<endl;

		// f_in_image.read((char*) &temp_buff, editor.bitmap_INFO_HEADER.biWidth*editor.bitmap_INFO_HEADER.biHeight*byte_for_pixel);
	}

	void edit_head_load()//헤더를 수동으로 읽어 구조체가 차지하는 메모리를 낭비하지 않는 방법도 나중에 시도해 보자
	{
		char* buff_ptr = NULL;
		buff_ptr = head_buff;

		memmove(edit_head_buff, buff_ptr, 54);

		buff_ptr = edit_head_buff;

		void* temp_F_header_struct = &bitmap_FILE_HEADER;
		void* temp_I_header_struct = &bitmap_INFO_HEADER;

		uint32_t bit_temp_1 = 0;	
		uint32_t bit_temp_2 = 0;	

		int n = 1;
		int m = 0;


		cout<<(void*)&bitmap_FILE_HEADER<<endl;
		cout<<(void*)&(bitmap_FILE_HEADER.bfType)<<endl;
		cout<<(void*)&(bitmap_FILE_HEADER.bfSize)<<endl;
		cout<<(void*)&(bitmap_FILE_HEADER.bfReserved1)<<endl;

		cout<<temp_F_header_struct<<endl; 

		bit_temp_1 |= (int)(*buff_ptr);
		
		cout.fill('0');
		cout.width(8);
		cout<<bit_temp_1<<endl;

		bit_temp_1 = bit_temp_1<<8;
		buff_ptr = buff_ptr+1;
		bit_temp_1 |= (int)(*buff_ptr);
		
		cout.fill('0');
		cout.width(8);
		cout<<bit_temp_1<<endl;
		
		buff_ptr = buff_ptr+1;

		(*(uint16_t*)temp_F_header_struct) = bit_temp_1;
		

		temp_F_header_struct = temp_F_header_struct + 4;
		buff_ptr = head_buff+1;
		while (n < 5)
		{
			bit_temp_1 = NULL;
			m = 0;
			buff_ptr = buff_ptr+file_head[n];
			while (m < file_head[n])
			{
				// (*temp_F_header_struct) = (*buff_ptr);
				// bit_temp_1 = bit_temp_1<<8;
				bit_temp_1<<=8;
				bit_temp_2 = (*buff_ptr);
				bit_temp_2 &= 0x000000ff;
				bit_temp_1 |= bit_temp_2;
				
				// (*(temp_F_header_struct)) |= 0xaa;

				cout<<hex;
				cout.fill('0');
				cout.width(8);
				cout<<(int)(*buff_ptr)<<endl;
				cout<<bit_temp_2<<endl;
				cout<<bit_temp_1<<"/"<<endl;

				// temp_F_header_struct = temp_F_header_struct + 1;
				buff_ptr = buff_ptr-1;
				m = m + 1;
			}
			// temp_F_header_struct = temp_F_header_struct+file_head[n];
			(*(uint32_t*)temp_F_header_struct) = (uint32_t)bit_temp_1;
			temp_F_header_struct = temp_F_header_struct+4;
			cout<<(void*)temp_F_header_struct<<endl;

			buff_ptr = buff_ptr+file_head[n];
			n = n + 1;
		}

		cout << endl;

		buff_ptr = rare_buff+13;
		n = 0;
		while (n < 11)
		{
			bit_temp_1 = 0;
			m = 0;
			buff_ptr = buff_ptr+info_head[n];
			while (m < info_head[n])
			{
				// (*temp_I_header_struct) = (*buff_ptr);
				// (*(char*)temp_I_header_struct) |= 0xaa;
				bit_temp_1<<=8;
				bit_temp_2 = (*buff_ptr);
				bit_temp_2 &= 0x000000ff;
				bit_temp_1 |= bit_temp_2;
				

				// cout<<dec;
				cout<<hex;
				cout.fill('0');
				cout.width(8);
				cout<<(int)(*buff_ptr)<<endl;
				cout<<bit_temp_2<<endl;
				cout<<bit_temp_1<<"/"<<endl;

				// temp_I_header_struct = temp_I_header_struct + 1;
				buff_ptr = buff_ptr-1;
				m = m + 1;
			}
			// temp_I_header_struct = temp_I_header_struct+info_head[n];
			(*(uint32_t*)temp_I_header_struct) = (uint32_t)bit_temp_1;
			
			cout<<(*(uint32_t*)temp_I_header_struct)<<endl;
			cout<<(void*)temp_I_header_struct<<endl;
			temp_I_header_struct = temp_I_header_struct+4;
			buff_ptr = buff_ptr+info_head[n];
			n = n + 1;
		}
	}

	void edit_data_load()
	{
		int height = bitmap_INFO_HEADER.biHeight;
		int width = bitmap_INFO_HEADER.biWidth;
		int byte_for_pixel = (bitmap_INFO_HEADER.biBitCount/8);

		int length_of_row = width*byte_for_pixel/4 + (width*byte_for_pixel%4 != 0);
		int length_of_col = height;

		int i = 0;

		memset(edit_refined_data_buff, 0, pure_size_of_data);
		cout<<endl;
		while(i < length_of_col)
		{
			memmove(edit_refined_data_buff+(i*byte_for_pixel*width), reverse_data_buff+(i*4*length_of_row), byte_for_pixel*width);
			
			cout<<(void*)(edit_refined_data_buff+(i*byte_for_pixel*width))<<endl<<endl;
			cout<<(edit_refined_data_buff+(i*byte_for_pixel*width))<<endl<<endl;
			cout<<(void*)(reverse_data_buff+(i*4*length_of_row))<<"::"<<endl<<endl;
			cout<<(reverse_data_buff+(i*4*length_of_row))<<"::"<<endl<<endl;
			i = i+1;
		}
	}

	void edit_head_store()
	{
		memmove(head_buff, edit_head_buff, 54);
	}

	void edit_data_store()
	{
		int height = bitmap_INFO_HEADER.biHeight;
		int width = bitmap_INFO_HEADER.biWidth;
		int byte_for_pixel = (bitmap_INFO_HEADER.biBitCount/8);

		int length_of_row = width*byte_for_pixel/4 + (width*byte_for_pixel%4 != 0);
		int length_of_col = height;

		int i = 0;
		// cout<<endl<<endl<<(void*)reverse_data_buff<<endl<<endl;
		// cout<<sizeof(reverse_data_buff)<<endl<<endl;
		// cout<<bitmap_INFO_HEADER.biSizeImage<<endl;
		memset(reverse_data_buff, 0, bitmap_INFO_HEADER.biSizeImage);
	
		for(int x = 0; x < width; x++)
		{
			for(int y = 0; y < height; y++) 
				if(dptr_mask_area_editor_final[x][y])
				{
					memmove(reverse_data_buff+(byte_for_pixel*x)+(4*length_of_row*(height-(y+1))), edit_refined_data_buff+(byte_for_pixel*x)+(byte_for_pixel*width*(height-(y+1))), byte_for_pixel);//*********************************************** 이곳에 최종 마스크가 영향을 주도록 수정하여야 함.
				}
		}

		memset(package_data_buff, 0, bitmap_INFO_HEADER.biSizeImage);

		i = 0;// 상수 초기화가 되지 않은 경우 복사가 되지 않아 화면이 이상해 지는 버그 발생
		while(i < length_of_col)
		{
			// memmove(package_data_buff+(i*4*length_of_row), reverse_data_buff+((length_of_col-i-1)*4*length_of_row), 4*length_of_row);
			memmove(package_data_buff+((length_of_col-i-1)*4*length_of_row), reverse_data_buff+(i*4*length_of_row), 4*length_of_row);
			i = i+1;
		}

		//아래 코드를 통해 뒤집어진 그림을 복사 할 수 있고 중간의 reverse_data_buff, edit_refined_data_buff 등의 중간 전달 값이 정상적으로 전달되고 있음을 알 수 있다.
		//// while(i < length_of_col)
		//// {
		//// 	// memmove(package_data_buff+(i*4*length_of_row), reverse_data_buff+((length_of_col-i-1)*4*length_of_row), 4*length_of_row);
		//// 	memmove(package_data_buff+((length_of_col-i-1)*4*length_of_row), reverse_data_buff+(i*4*length_of_row), 4*length_of_row);
		//// 	i = i+1;
		//// }
		// memmove(package_data_buff, reverse_data_buff, bitmap_INFO_HEADER.biSizeImage);
	}
	
	void reconstitution_data_bmp()
	{	
		char* buff_ptr = NULL;
		buff_ptr = rare_buff;
	
		memmove(buff_ptr, head_buff, 54);//디버그로 포인터가 가리키는 주소 값을 보려면 러떻게 하여야 하남...??
		
		memmove(buff_ptr+54, package_data_buff, bitmap_INFO_HEADER.biSizeImage);//디버그로 포인터가 가리키는 주소 값을 보려면 러떻게 하여야 하남...??

		// f_in_image.read((char*) &temp_buff, editor.bitmap_INFO_HEADER.biWidth*editor.bitmap_INFO_HEADER.biHeight*byte_for_pixel);
	}

	void write_data_bmp(const char* f_out_name)//"sample_binary_OUTPUT_2.bmp"
	{
		open_file(f_out_name, 'O');
		f_out_image.seekp(0, ios::beg);
		f_out_image.write(rare_buff, bitmap_FILE_HEADER.bfSize);
		// f_out_image.seekp(54, ios::beg);
		// f_out_image.write(reverse_data_buff, bitmap_INFO_HEADER.biSizeImage);
		// f_out_image.seekp(54, ios::beg);
		// f_out_image.write(package_data_buff, bitmap_INFO_HEADER.biSizeImage);
		f_out_image.close();
	}

	void save_as(const char* f_out_name)
	{
		edit_head_store();
		edit_data_store();
		reconstitution_data_bmp();
		write_data_bmp(f_out_name);
	}



	void print_header_bmp()
	{
		f_in_image.seekg(0, ios::beg);

		int n = 0;
		int m = 0;
		unsigned int temp_buff = 0;

		

		while (n < 5)
		{
			m = 0;
			while (m < file_head[n])
			{
				f_in_image.read((char*) &temp_buff, 1);
				cout << hex;
				cout.fill('0');
				cout.width(2);
				cout << temp_buff << " ";
				m = m + 1;
			}

			cout << endl;
			n = n + 1;
		}

		cout << endl;

		n = 0;
		while (n < 11)
		{
			m = 0;
			while (m < info_head[n])
			{
				f_in_image.read((char*) &temp_buff, 1);
				cout << hex;
				cout.fill('0');
				cout.width(2);
				cout << temp_buff << " ";
				m = m + 1;
			}

			cout << endl;
			n = n + 1;
		}
		
	}

	void print_data_bmp()
	{
		f_in_image.seekg(54, ios::beg);
		unsigned int temp_buff = 0;
		int height = bitmap_INFO_HEADER.biHeight;
		int width = bitmap_INFO_HEADER.biWidth;
		int byte_for_pixel = (bitmap_INFO_HEADER.biBitCount/8);

		int length_of_row = width*byte_for_pixel/4 + (width*byte_for_pixel%4 != 0);
		int length_of_col = height;

		cout<<length_of_row<<endl;
		cout<<length_of_col<<endl;

		int i = 0;
		int j = 0;
		int k = 0;
		i = 0;
		while (i < length_of_col)
		{
			cout<<endl;

			j = 0;
			while (j < length_of_row)
			{
				k = 0;
				while (k < 4)//패키지 단위 4바이트씩 읽기
				{
					f_in_image.read((char*) &temp_buff, 1);
					cout.fill('0');
					cout.width(2);
					cout << hex;
					cout << (int)temp_buff << " ";
					k = k + 1;
				}
				j = j + 1;
			}
			
			i = i + 1;
		}
		f_in_image.read((char*) &temp_buff, 1);
		cout.fill('0');
		cout.width(2);
		cout << hex;
		cout << (int)temp_buff << " ";

		f_in_image.read((char*) &temp_buff, 1);
		cout.fill('0');
		cout.width(2);
		cout << hex;
		cout << (int)temp_buff << " ";
	}



	~bmp_editor()
	{
		f_in_image.close();
		delete rare_buff;

		delete head_buff;
		delete edit_head_buff;

		delete package_data_buff;
		delete reverse_data_buff;//munmap_chunk(): invalid pointer
		delete edit_refined_data_buff;//free(): invalid pointer

		for(int i = 0; i< bitmap_INFO_HEADER.biHeight; i++)
		{
			delete dptr_mask_area_editor_final[i];
		}
		delete dptr_mask_area_editor_final;

		bool** dptr_mask_area_editor_final = nullptr;
	}
};

int main()
{
	bmp_editor editor("sample_iw_8_20_20_size_bmp.bmp");

	editor.print_header_bmp();
	editor.print_data_bmp();

	editor.area_set_total_editor(1);
	editor.test_draw();

	editor.save_as("sample_binary_OUTPUT_1.bmp");


	cout << endl;
	cout << endl;
	cout<<hex;
	cout.width(8);
	

	cout <<editor.bitmap_FILE_HEADER.bfOffBits<< endl;
	// cout <<(const char*)&(editor.bitmap_FILE_HEADER.bfType)<< endl;
	cout <<(int)(editor.bitmap_FILE_HEADER.bfType)<< endl;
	cout << dec<<endl;
	cout <<editor.bitmap_FILE_HEADER.bfSize<< endl;
	cout <<(editor.bitmap_INFO_HEADER.biWidth)<< endl;
	cout <<(editor.bitmap_INFO_HEADER.biHeight)<< endl;
	cout <<(editor.bitmap_INFO_HEADER.biBitCount)<< endl;
	cout <<(editor.bitmap_INFO_HEADER.biSizeImage)<< endl;
	cout <<(editor.size_of_file)<< endl;
	

	cout<<(void*)&(editor.bitmap_INFO_HEADER.biWidth)<<endl;
	cout<<(void*)&(editor.bitmap_INFO_HEADER.biHeight)<<endl;

	return 0;
}