#include <iostream>
//using namespace std;

int main()
{
int num1 = 1;

int num2 = 4;

 

double result1 = num1 / num2;

double result2 = (double) num1 / num2;

double result3 = double (num1) / num2;

 

std::cout << "result1�� ����� ���� " << result1 << "�Դϴ�." << std::endl;

std::cout << "result2�� ����� ���� " << result2 << "�Դϴ�." << std::endl;

std::cout << "result3�� ����� ���� " << result3 << "�Դϴ�.";
	return 0;
}
