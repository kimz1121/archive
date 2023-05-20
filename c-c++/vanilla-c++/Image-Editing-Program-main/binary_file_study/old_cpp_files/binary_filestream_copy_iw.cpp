#include <iostream>
#include <fstream>

using namespace std;

char f_head[5] = {2, 4, 2, 2, 4};
char dib_head[11] = {4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4};

int size_of_file = 0;

char* buff = NULL;
char* buff_ptr = NULL;

char* edit_buff = NULL;



int main()
{
	unsigned int outbuff = 0;
	int n = 0;
	int m = 0;

	ifstream f_in_image;
	f_in_image.open("sample_iw_4_4_4_size_bmp.bmp", ios_base::binary);
	f_in_image.seekg(0, ios::end);//오프셋을 0 으로 하여 파일의 끝 지점에 그대로 파일 포인터를 옮긴다.
	size_of_file = f_in_image.tellg();
	cout<<size_of_file<<endl;
	f_in_image.seekg(0, ios::beg);//오프셋을 0 으로 하여 파일의 처음 지점에 그대로 파일 포인터를 옮긴다.

	buff = new char(size_of_file);
	buff_ptr = buff;

	while(n < 5)
	{
		m = 0;
		while(m < f_head[n])
		{
			f_in_image.read((char*)&outbuff, 1);

			(*buff_ptr) = outbuff;
			cout<<hex;
			cout<<(int)(*buff_ptr)<<" ";
			buff_ptr++;
		
			cout<<hex;
			cout.fill('0');
			cout.width(2);
			cout<<(int)outbuff<<" ";
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
			f_in_image.read((char*)&outbuff, 1);
			(*buff_ptr) = outbuff;
			buff_ptr++;
		
			cout<<hex;
			cout.fill('0');
			cout.width(2);
			cout<<(int)outbuff<<" ";
			m = m+1;
		}
		cout<<endl;
		n = n+1;
	}

	n = 0;
	while(n < 16*3)
	{
		f_in_image.read((char*)&outbuff, 1);
		(*buff_ptr) = outbuff;
		buff_ptr++;

		cout.fill('0');
		cout.width(2);
		cout<<hex;
		cout<<outbuff<<" ";
		

		n = n+1;
		
	}
	// cout<<f_in_image.eof();//파일 끝을 일근 것이 이게 맞나??
	
	//-------------------------------------------------------
	// ofstream f_out_image("sample_iw_4_4_4_size_bmp_copy.bmp", ios_base::binary);
	// f_out_image.seekp(0, ios::beg);
	// f_out_image.close();
	// f_in_image.close();
	//-------------------------------------------------------

	f_in_image.close();//한번에 두개 이상의 피일이 열려 있으면 에러가 나남??
	
	ofstream f_out_image("sample_iw_4_4_4_size_bmp_copy.bmp", ios_base::binary);
	f_out_image.seekp(0, ios::beg);
	// f_out_image<<(buff);
	f_out_image.write(buff, size_of_file);
	f_out_image.close();
	f_in_image.close();
}