#include <iostream>
#include <fstream>
using namespace std;

struct file_header
{
	uint16_t bfType;	// 'BM' 이라는 값을 저장함
	uint32_t bfSize;	// 바이트 단위로 전체파일 크기
	uint16_t bfReserved1;	// 예약된 변수
	uint16_t bfReserved2;	// 예약된 변수
	uint32_t bfOffBits;	// 영상데이터 위치까지의 거리
};

struct info_header
{
	uint32_t 	biSize;	// 이 구조체의 크기  
	long 		biWidth;	// 픽셀단위로 영상의 폭
	long 		biHeight;	// 영상의 높이
	uint16_t 	biplanes;	// 비트 플레인 수 (항상 1)
	uint16_t 	biBitCount;	// 픽셀당 비트수 (컬러, 흑백 구별) 
	uint32_t 	biCompression;	// 압축유무
	uint32_t 	biSizeImage;	// 영상의 크기 (바이트 단위)
	long 		biXPelsPerMeter;	// 가로 해상도
	long 		biYPelsPerMeter;	// 세로 해상도
	uint32_t 	biClrUsed;	// 실제 사용 색상 수
	uint32_t 	biClrImportant;	// 중요한 색상 인덱스
};

// https://kevinthegrey.tistory.com/57
// 출처

class binary_edit
{
	public:
	void copy_binary(auto* IN_1, auto* IN_2, int byte_copy)
	{
		int n = 0;
		while(n < byte_copy)
		{
			*((uint8_t*)IN_1+n) = *((uint8_t*)IN_2+n); 
			n = n + 1;
		}
	}
}binary;


class bmp_editor
{
	ifstream f_in_image;

	char file_head[5] = { 2, 4, 2, 2, 4 };	// 단 하나만 존재하면 되는 전역 변수로는 어떻게 선언할까?
	char info_head[11] = { 4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4 };

	int size_of_file = 0;
	const int size_of_header = 54;
	int size_of_data = 0;

	char *buff = NULL;

	char *edit_head_buff = NULL;
	char *edit_data_buff = NULL;

	char *buff_ptr = NULL;
	char *data_start_ptr = NULL;

	// (*buff_ptr) = temp_buff;
	// buff_ptr++;
	public:

	struct file_header bitmap_FILE_HEADER = {0, 0, 0, 0, 0};
	struct info_header bitmap_INFO_HEADER = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	bmp_editor(const char *f_in_name)
	{
		f_in_image.open(f_in_name, ios_base::binary);
		size_of_file = get_file_size();
		buff = new char[size_of_file];
		buff_ptr = buff;
		cout << sizeof(*buff) << endl;
		cout << sizeof(*buff_ptr) << endl;
		// cout<<sizeof(buff)<<endl;

		// edit_head_buff = new char(size_of_header);
		read_header_bmp();

		edit_data_buff = new char[size_of_file - size_of_header];
		//edit_data_buff = new char(size_of_file-size_of_header);

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
		f_in_image.seekg(0, ios::beg);
		f_in_image.read(buff, 54);
		buff_ptr = buff;
		
		char* temp_f_header_struct = (char*)&bitmap_FILE_HEADER;
		char* temp_I_header_struct = (char*)&bitmap_INFO_HEADER;
		
		


		int n = 1;
		int m = 0;
		unsigned int temp_buff = 0;
		
		(*(char*)temp_f_header_struct) = (char)(*buff_ptr);
		temp_f_header_struct = temp_f_header_struct+1;
		buff_ptr = buff_ptr+1;
		(*(char*)temp_f_header_struct) = (char)(*(buff_ptr));

	while (n < 5)
		{
			m = 0;
			buff_ptr = buff_ptr+file_head[n];

			while (m < file_head[n])
			{
				
				(*(char*)temp_f_header_struct) = (char)(*buff_ptr);

				cout<<hex;
				cout.fill('0');
				cout.width(2);
				cout<<(int)(*buff_ptr)<<endl;

				temp_f_header_struct = temp_f_header_struct+1;
				buff_ptr = buff_ptr-1;
				m = m + 1;
			}
			temp_f_header_struct = temp_f_header_struct + file_head[n];
			buff_ptr = buff_ptr+file_head[n];
			n = n + 1;
		}

		
		 ofstream f_out_image("sample_binary_output.bmp", ios_base::binary);

	  f_out_image.seekp(0, ios::beg);
	  f_out_image.write((const char*)&(temp_f_header_struct), 14);
	  f_out_image.close();

		cout << endl;

		n = 0;
		while (n < 11)
		{
			m = 0;
			while (m < info_head[n])
			{
				m = m + 1;
			}
			n = n + 1;
		}
	}

	void read_data_bmp(int height, int width, int byte_for_pixel)
	{
		f_in_image.seekg(size_of_header, ios::beg);
		unsigned int temp_buff = 0;

		buff_ptr = buff + 54;

		int i = 0;
		int j = 0;
		int k = 0;

		i = 0;
		while (i < height)
		{
			j = 0;
			while (j < width)
			{
				k = 0;
				while (k < byte_for_pixel)
				{
					f_in_image.read((char*) &temp_buff, 1);
					(*buff_ptr) = temp_buff;
					buff_ptr++;
					k = k + 1;
				}

				j = j + 1;
			}

			i = i + 1;
		}
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

	void print_data_bmp(int height, int width, int byte_for_pixel)
		{
			f_in_image.seekg(size_of_header, ios::beg);
			unsigned int temp_buff = 0;

			int i = 0;
			int j = 0;
			int k = 0;

			i = 0;
			while (i < height)
			{
				j = 0;
				while (j < width)
				{
					k = 0;
					while (k < byte_for_pixel)
					{
						f_in_image.read((char*) &temp_buff, 1);
						cout.fill('0');
						cout.width(2);
						cout << hex;
						cout << temp_buff << " ";
						k = k + 1;
					}

					j = j + 1;
				}

				i = i + 1;
			}
		}

		~bmp_editor()
		{
			f_in_image.close();

			delete buff;
			delete edit_data_buff;
		}
};

int main()
{
	bmp_editor editor("sample_iw_4_4_4_size_bmp.bmp");

	editor.print_header_bmp();

	editor.print_data_bmp(4, 4, 3);

	cout << endl;
	cout << endl;
	cout<<hex;
	cout <<editor.bitmap_FILE_HEADER.bfSize<< endl;
	cout <<(const char*)&(editor.bitmap_FILE_HEADER.bfType)<< endl;

	//   f_in_image.close();//한번에 두개 이상의 피일이 열려 있으면 에러가 나남??

	//   ofstream f_out_image("sample_binary_output.bmp", ios_base::binary);

	//   f_out_image.seekp(0, ios::beg);
	//   f_out_image.write((const char*)&(editor.bitmap_FILE_HEADER), 54);
	//   f_out_image.close();
	//   f_in_image.close();
	return 0;
}