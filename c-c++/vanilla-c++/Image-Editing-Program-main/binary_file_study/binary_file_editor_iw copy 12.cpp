#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct file_header
{
	uint32_t 	bfType;	// 'BM' 이라는 값을 저장함
	uint32_t bfSize;	// 바이트 단위로 전체파일 크기
	uint32_t 	bfReserved1;	// 예약된 변수
	uint32_t 	bfReserved2;	// 예약된 변수
	uint32_t bfOffBits;	// 영상데이터 위치까지의 거리
};

struct info_header
{
	uint32_t 	biSize;	// 이 구조체의 크기  
	uint32_t 	biWidth;	// 픽셀단위로 영상의 폭
	uint32_t 	biHeight;	// 영상의 높이
	uint32_t 		biplanes;	// 비트 플레인 수 (항상 1)
	uint32_t 		biBitCount;	// 픽셀당 비트수 (컬러, 흑백 구별) 
	uint32_t 	biCompression;	// 압축유무
	uint32_t 	biSizeImage;	// 영상의 크기 (바이트 단위)
	uint32_t 	biXPelsPerMeter;	// 가로 해상도
	uint32_t 	biYPelsPerMeter;	// 세로 해상도
	uint32_t 	biClrUsed;	// 실제 사용 색상 수
	uint32_t 	biClrImportant;	// 중요한 색상 인덱스
};

// https://kevinthegrey.tistory.com/57 --> 저작권 허락 못받으면 테더 수정하기
// 출처




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
	public:

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

		while(i < length_of_col)
		{
			memmove(reverse_data_buff+(i*4*length_of_row), edit_refined_data_buff+(i*byte_for_pixel*width), byte_for_pixel*width);
			i = i+1;
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

		
	}
};

int main()
{
	bmp_editor editor("sample_iw_8_20_20_size_bmp.bmp");

	editor.print_header_bmp();
	editor.print_data_bmp();


	editor.save_as("sample_binary_OUTPUT_1.bmp");


	cout << endl;
	cout << endl;
	cout<<hex;
	cout.width(8);
	
	cout << "시작함미아아ㅏㅇ아ㅏsadfasdgjfvsdak아앙ㅇㅇ" << endl;

	cout <<editor.bitmap_FILE_HEADER.bfOffBits<< endl;
	// cout <<(const char*)&(editor.bitmap_FILE_HEADER.bfType)<< endl;
	cout <<(int)(editor.bitmap_FILE_HEADER.bfType)<< endl;
	cout << dec<<endl;
	cout <<editor.bitmap_FILE_HEADER.bfSize<< endl;
	cout << "시작함미아아ㅏㅇ아ㅏsadfasdgjfvsdak아앙ㅇㅇ" << endl;
	cout <<(editor.bitmap_INFO_HEADER.biWidth)<< endl;
	cout <<(editor.bitmap_INFO_HEADER.biHeight)<< endl;
	cout <<(editor.bitmap_INFO_HEADER.biBitCount)<< endl;
	cout <<(editor.bitmap_INFO_HEADER.biSizeImage)<< endl;
	cout <<(editor.size_of_file)<< endl;
	

	cout<<(void*)&(editor.bitmap_INFO_HEADER.biWidth)<<endl;
	cout<<(void*)&(editor.bitmap_INFO_HEADER.biHeight)<<endl;

	//   f_in_image.close();//한번에 두개 이상의 피일이 열려 있으면 에러가 나남??

	//   ofstream f_out_image("sample_binary_output.bmp", ios_base::binary);

	//   f_out_image.seekp(0, ios::beg);
	//   f_out_image.write((const char*)&(editor.bitmap_FILE_HEADER), 14);
	//   f_out_image.write((const char*)&(editor.bitmap_INFO_HEADER), 40);
	//   f_out_image.close();
	return 0;
}