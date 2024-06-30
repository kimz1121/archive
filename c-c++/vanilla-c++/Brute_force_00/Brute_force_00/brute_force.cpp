#include "brute_force.hpp"

#include "iostream"
using namespace std;

Brute_force::Brute_force()
{
	len_number = 4;//ARRAY_MAX 보다 값이 작아야 한다.
	len_section = 1;
	jump_code_start[0] = 97;// a 의 아스키 코드 
	jump_code_end[0] = 122;// z 의 아스키 코드
	//len_section = 2;
	//jump_code_start[0] = 65;// a 의 아스키 코드 
	//jump_code_end[0] = 90;// z 의 아스키 코드
	//jump_code_start[1] = 97;// a 의 아스키 코드 
	//jump_code_end[1] = 122;// z 의 아스키 코드
}

Brute_force::~Brute_force()
{

}

void Brute_force::set_len(int _len)
{

}

void Brute_force::set_mask()
{

}

void Brute_force::do_generate_jump_code()
{

}

int Brute_force::get_count_jump_code(int _section_count)
{
	if (_section_count < len_section - 1)
	{
		_section_count++;//class member variable
	}
	else
	{
		_section_count = -1;
	}
	return _section_count;
}

void Brute_force::get_jump_code(int _count_jump_code, int& _char_start, int& _char_end)
{
	_char_start = jump_code_start[_count_jump_code];
	_char_end = jump_code_end[_count_jump_code];
}

void Brute_force::do_count(int* _char_array, int* _section_array, int _depth, int _len_max, int& _overflow)
{
	if (_depth < _len_max)
	{
		int carry = 0;
		get_next_char(_char_array[_depth], _section_array[_depth], carry);
		if (carry > 0)
		{
			do_count(_char_array, _section_array, _depth + 1, _len_max, _overflow);
		}
	}
	else
	{// 종료 조건
		_overflow = 1;
	}
}

void Brute_force::get_next_char(int& _char, int& _section_count, int& _carry)
{
	_carry = 0;// 자리올림은 항상 0으로 초기화
	int point_start = 0;
	int point_end = 0;
	get_jump_code(_section_count, point_start, point_end);
	if (_char == point_end) // 단일 연속의 끝에 도착
	{
		_section_count = get_count_jump_code(_section_count);// 다음 단일 연속으로 이동
		// 상태가 다음 단일 연속이 존재하지 않는 주기의 끝 지점일 경우에는 _section_count == -1 이다
		if (_section_count < 0)// 주기의 끝에 도착
		{
			_section_count = 0;// 주기 초기화
			_carry = 1;
		}
		get_jump_code(_section_count, point_start, point_end);// 다음 포인트로 점프하기.
		_char = point_start;// 문자 초기화
	}
	else
	{
		_char++;
	}
}
//배열을 참조나 포인터로 전달하고, 배열의 길이를 인수로 전달하면, 
// 깊이 우선 탐색이나 너비 우선 탐색으로 만들 수 있을 듯
void Brute_force::run()
{
	int _overflow = 0;
	int carry = 0;

	//첫 문자로 모든 자리를 업데이트
	int charactor_start = 0;
	int temp = 0;
	get_jump_code(0, charactor_start, temp);// 첫 문자 상태 초기화 jump_code 설정에 따라서 첫자리를 공백으로 할지 문자로 할지 자유롭게 정할 수 있다.
	for (int i = 0; i < len_number; i++)// 찾고자 하는 문자의 자리수도 자유롭게 정할 수 있다.
	{
		char_array[i] = charactor_start;
	}

	int i = 0;
	while (1)
	{
		//cout << (char)char_array[1] << (char)char_array[0] << endl;
		cout << preposition << (char)char_array[3] << (char)char_array[2] << (char)char_array[1] << (char)char_array[0] << endl;
		do_count(char_array, section_array, 0, 4, _overflow);
		
		i++;
		
		if (_overflow > 0 or i > 456976)
		//if (_overflow > 0 or i > 10)
		{
			cout << "탐색 완료" << endl;
			//cout << _overflow << endl;
			//cout << i << endl;
			break;
		}
	}
}

