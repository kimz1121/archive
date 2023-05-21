#include <iostream>
#include <fstream>

using namespace std;



char f_head[5] = {2, 4, 2, 2, 4};
char dib_head[11] = {4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4};

int size_of_file = 0;

char* buff = NULL;
char* buff_ptr = NULL;

char* edit_buff = NULL;

			// (*buff_ptr) = temp_buff;
			// cout<<hex;
			// cout<<(int)(*buff_ptr)<<" ";
			// buff_ptr++;

void print_header_bmp(ifstream& f_in)
{
	int n = 0;
	int m = 0;
	unsigned int temp_buff = 0;
	while(n < 5)
	{
		m = 0;
		while(m < f_head[n])
		{
			f_in.read((char*)&temp_buff, 1);
			cout<<hex;
			cout.fill('0');
			cout.width(2);
			cout<<temp_buff<<" ";
			m = m+1;
		}
		cout<<endl;
		n = n+1;
	}

	cout<<endl;

	n = 0;
	while(n < 11)
	{
		m = 0;
		while(m < dib_head[n])
		{
			f_in.read((char*)&temp_buff, 1);
			cout<<hex;
			cout.fill('0');
			cout.width(2);
			cout<<temp_buff<<" ";
			m = m+1;
		}
		cout<<endl;
		n = n+1;
	}
}

void print_data_bmp(ifstream& f_in, int height, int width, int byte_for_pixel)
{
	int i = 0;
	int j = 0;
	int k = 0;
	unsigned int temp_buff = 0;
	
	i = 0;
	while(i < height)
	{
		j = 0;
		while(j < width)
		{
			k = 0;
			while(k < byte_for_pixel)
			{
					f_in.read((char*)&temp_buff, 1);
					cout.fill('0');
					cout.width(2);
					cout<<hex;
					cout<<temp_buff<<" ";
					k = k+1;
			}
			j = j+1;
		}
		i = i+1;
	}
}



int main()
{
	

	ifstream f_in_image;
	f_in_image.open("sample_iw_4_4_4_size_bmp.bmp", ios_base::binary);
	f_in_image.seekg(0, ios::end);//오프셋을 0 으로 하여 파일의 끝 지점에 그대로 파일 포인터를 옮긴다.
	size_of_file = f_in_image.tellg();
	cout<<size_of_file<<endl;
	f_in_image.seekg(0, ios::beg);//오프셋을 0 으로 하여 파일의 처음 지점에 그대로 파일 포인터를 옮긴다.






	buff = new char(size_of_file);
	buff_ptr = buff;

	print_header_bmp(f_in_image);

	print_data_bmp(f_in_image, 4, 4, 3);


	f_in_image.close();//한번에 두개 이상의 피일이 열려 있으면 에러가 나남??
	
	ofstream f_out_image("sample_iw_4_4_4_size_bmp_copy.bmp", ios_base::binary);
	
	
	
	
	f_out_image.seekp(0, ios::beg);
	f_out_image.write(buff, size_of_file);
	f_out_image.close();
	f_in_image.close();
}