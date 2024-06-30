#pragma once
#ifndef BRUTE_FORCE

#define ARRAY_MAX 256
class Brute_force
{
public:
	Brute_force();
	~Brute_force();

	void set_len(int _len);
	void set_mask();
	void do_generate_jump_code();
	int get_count_jump_code(int _count_jump_code);
	void get_jump_code(int _count_jump_code, int& _char_start, int& _char_end);
	void do_count(int* _char_array, int* _section_array, int _depth, int _len_max, int& _overflow);
	void get_next_char(int& _char, int& _count, int& _carry);
	void run();

private:
	const char* preposition = "ka";//ka****
	int section_array[ARRAY_MAX] = { 0, };
	int char_array[ARRAY_MAX] = { 0, };
	int jump_code_start[ARRAY_MAX] = { 0, };
	int jump_code_end[ARRAY_MAX] = { 0, };
	int len_section = 0;
	int len_number = 0;
	//int count_jump_code = 0;

};

#endif // !BRUTE_FORCE
