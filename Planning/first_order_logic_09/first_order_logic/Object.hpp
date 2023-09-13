#pragma once

#ifndef OBJECT
#define OBJECT

#include "Entity.hpp"

#include <deque>
#include <list>

using namespace std;

class Object : public Entity
{
private:
	static unsigned int total_object_num;// entity의 수는 자연수이므로 unsigned int 를 사용하는 것이 효과적
	static deque<unsigned int> unused_object_ID_list;// static 변수로 지정하여 전체 Entity 객체 ID 중 사용되지 않는 것을 관리함.

	unsigned int object_ID = 0;//ID 는 1 부터 시작하도록 한다. ID 0은 아직 할당되지 않은 상태로 하여, 잘못된  entity 가 존재하는지 판단할 때 이용

public:
	Object();
	Object(const Object& obj);
	~Object();

	//객체 정보 함수
	int get_type();
	unsigned int get_object_ID();
	//void set_group_order(unsigned int order_input);// entity 에서는 pure virtual 함수로 오버라이딩 해주어야만 object의 인스턴스를 만들 수 있음.

	////술어 조작 함수
	//void do_add_pred(unsigned int predicate_ID, bool nag_sign);
	//void do_del_pred(unsigned int predicate_ID, bool nag_sign);
	//bool do_add_pred_check_inclusion(unsigned int predicate_ID, bool nag_sign);
	//bool do_del_pred_check_inclusion(unsigned int predicate_ID, bool nag_sign);
	//void do_flush_pred();

	////술어 검색 함수
	//bool is_included(unsigned int predicate_ID, bool nag_sign);

	////술어 출력 함수
	//void do_print_all_predicates();
};

#endif