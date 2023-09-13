#pragma once

#ifndef ENV_REPRESENTATION //inclusion guard
#define ENV_REPRESENTATION

/*
* 
* 중요한 기능들 목록
* 
* 단어 부호화 장치(인코딩 장치 입력된 단어들이 같지 않으면, 새로운 수로 같으면 기존의 대응되는 수로 변환)
* 
* 입력, 출력 시스템
* 
* 자료 저장 장치 (자료구조를 통해 환경에 대한 1차 논리 설명을 효과적으로 저장하고 필요할 때 사용함.)
* 
*/

//일단 문자열 그대로 저장해봅시당.

#include "Entity.hpp"
#include "Object.hpp"
#include "Group.hpp"

#include <list>
#include <deque>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

class Env_representation
{
public:
	static unsigned int num_of_predicates;
	unordered_map<string, unsigned int> str2int;
	unordered_map<unsigned int, string> int2str;
	// 차후 객체 동적리스트 혹은 객체포인터 배열로 구현, 검색과정에서는 random access 가 가능한 vector 라도 내부의 내용을 따져보는 시간이 필요하므로 검색에서의 이점이 적다. 
	// 따라서 추가와 삭제혹은 메모리 효율성을 위한 장점이라도 얻기위하여 리스트 혹은 포인터 배열로 구현한다.
	// map의 반복자 타입은 pair<int, int> 로 한다.

	list<Entity*> entity_list;
	list<Object*> object_list;
	list<Group*> group_list;

	~Env_representation();

	// 술어 관리 함수
	//void do_add_word(char* char_str);
	void do_add_word(string str);
	bool do_add_word_check_inclusion(string str);

	string* get_pred_int_to_string_ptr(int pred_id);
	unsigned int* get_pred_string_to_int_ptr(string pred);

	// 개체 정보 함수
	void do_show_entity_info(unsigned int entity_ID);

	// 개체 관리 함수
	unsigned int do_add_object();//return entity_ID
	unsigned int do_add_group();//return entity_ID
	void do_del_object(unsigned int object_ID);
	void do_del_group(unsigned int group_ID);
	void do_del_entity(unsigned int entity_ID);

	void do_flush_entity_list();

	// 개체 선택 함수
	Object* get_object_ptr(unsigned int object_ID);
	Group* get_group_ptr(unsigned int group_ID);
	Entity* get_entity_ptr(unsigned int entity_ID);

	//주변 접근 함수
	unsigned int get_group_list_length(unsigned int entity_ID);
	unsigned int get_group_entity_id(unsigned int entity_ID, unsigned int index);
	unsigned int get_group_group_id(unsigned int entity_ID, unsigned int index);
	unsigned int get_member_list_length(unsigned int entity_ID);
	unsigned int get_member_entity_id(unsigned int entity_ID, unsigned int index);

	// 그룹 관리 함수
	bool do_insert_entity_to_group(unsigned int group_ID, unsigned int entity_ID);
	bool do_erase_entity_from_group(unsigned int group_ID, unsigned int entity_ID);
	bool do_insert_object_to_group(unsigned int group_ID, unsigned int object_ID);
	bool do_erase_object_from_group(unsigned int group_ID, unsigned int object_ID);
	bool do_flush_entity_connection(unsigned int entity_ID);
	bool do_flush_group_connection(unsigned int entity_ID);

	// 술어 관리 함수
	void do_access_entity_add_pred_id(unsigned int entity_ID, unsigned int predicate_ID, bool nag_sign);
	void do_access_entity_del_pred_id(unsigned int entity_ID, unsigned int predicate_ID, bool nag_sign);
	void do_access_entity_add_pred_id_check_inclusion(unsigned int entity_ID, unsigned int predicate_ID, bool nag_sign);
	void do_access_entity_del_pred_id_check_inclusion(unsigned int entity_ID, unsigned int predicate_ID, bool nag_sign);
	void do_access_entity_add_pred_str_check_inclusion(unsigned int entity_ID, string predicate_str, bool nag_sign);
	void do_access_entity_del_pred_str_check_inclusion(unsigned int entity_ID, string predicate_str, bool nag_sign);
	void do_access_entity_flush_pred(unsigned int entity_ID);

	bool do_access_entity_is_included(unsigned int entity_ID, unsigned int predicate_ID, bool nag_sign);
	bool do_access_entity_is_included(unsigned int entity_ID, string predicate_str, bool nag_sign);
	unsigned int get_access_entity_pred_length(unsigned int entity_ID, bool nag_sign);
	unsigned int* get_access_entity_pred_id_ptr(unsigned int entity_ID, unsigned int index, bool nag_sign);

	void do_access_entity_print_all_pred(unsigned int entity_ID);
	
	// 개체 검색 함수
	unsigned int do_search_entity_includes_pred(unsigned int predicate_ID, bool nag_sign);
	unsigned int do_search_entity_includes_pred(string predicate_str, bool nag_sign);

	// 정보 획득 함수
	void do_print_all_entity();
	void do_print_all_entity_pred();
	void do_print_info_with_indent(Entity* entity_ptr, unsigned int indent);
	void do_print_tree_entity(Entity* entity_ptr);
	void do_print_tree_entity_recursion(Entity* entity_ptr, unsigned int depth);

};

#endif