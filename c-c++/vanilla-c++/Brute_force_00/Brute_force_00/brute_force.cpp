#include "brute_force.hpp"

#include "iostream"
using namespace std;

Brute_force::Brute_force()
{
	len_number = 4;//ARRAY_MAX ���� ���� �۾ƾ� �Ѵ�.
	len_section = 1;
	jump_code_start[0] = 97;// a �� �ƽ�Ű �ڵ� 
	jump_code_end[0] = 122;// z �� �ƽ�Ű �ڵ�
	//len_section = 2;
	//jump_code_start[0] = 65;// a �� �ƽ�Ű �ڵ� 
	//jump_code_end[0] = 90;// z �� �ƽ�Ű �ڵ�
	//jump_code_start[1] = 97;// a �� �ƽ�Ű �ڵ� 
	//jump_code_end[1] = 122;// z �� �ƽ�Ű �ڵ�
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
	{// ���� ����
		_overflow = 1;
	}
}

void Brute_force::get_next_char(int& _char, int& _section_count, int& _carry)
{
	_carry = 0;// �ڸ��ø��� �׻� 0���� �ʱ�ȭ
	int point_start = 0;
	int point_end = 0;
	get_jump_code(_section_count, point_start, point_end);
	if (_char == point_end) // ���� ������ ���� ����
	{
		_section_count = get_count_jump_code(_section_count);// ���� ���� �������� �̵�
		// ���°� ���� ���� ������ �������� �ʴ� �ֱ��� �� ������ ��쿡�� _section_count == -1 �̴�
		if (_section_count < 0)// �ֱ��� ���� ����
		{
			_section_count = 0;// �ֱ� �ʱ�ȭ
			_carry = 1;
		}
		get_jump_code(_section_count, point_start, point_end);// ���� ����Ʈ�� �����ϱ�.
		_char = point_start;// ���� �ʱ�ȭ
	}
	else
	{
		_char++;
	}
}
//�迭�� ������ �����ͷ� �����ϰ�, �迭�� ���̸� �μ��� �����ϸ�, 
// ���� �켱 Ž���̳� �ʺ� �켱 Ž������ ���� �� ���� ��
void Brute_force::run()
{
	int _overflow = 0;
	int carry = 0;

	//ù ���ڷ� ��� �ڸ��� ������Ʈ
	int charactor_start = 0;
	int temp = 0;
	get_jump_code(0, charactor_start, temp);// ù ���� ���� �ʱ�ȭ jump_code ������ ���� ù�ڸ��� �������� ���� ���ڷ� ���� �����Ӱ� ���� �� �ִ�.
	for (int i = 0; i < len_number; i++)// ã���� �ϴ� ������ �ڸ����� �����Ӱ� ���� �� �ִ�.
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
			cout << "Ž�� �Ϸ�" << endl;
			//cout << _overflow << endl;
			//cout << i << endl;
			break;
		}
	}
}

