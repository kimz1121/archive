#include <iostream>
//using namespace std;

int main()
{
int num1 = 1;

int num2 = 4;

 

double result1 = num1 / num2;

double result2 = (double) num1 / num2;

double result3 = double (num1) / num2;

 

std::cout << "result1에 저장된 값은 " << result1 << "입니다." << std::endl;

std::cout << "result2에 저장된 값은 " << result2 << "입니다." << std::endl;

std::cout << "result3에 저장된 값은 " << result3 << "입니다.";
	return 0;
}
