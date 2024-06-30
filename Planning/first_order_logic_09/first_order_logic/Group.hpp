#pragma once

#ifndef GROUP
#define GROUP

#include "Entity.hpp"

#include <deque>
#include <list>

using namespace std;

class Group : public Entity
{
private:
	static unsigned int total_group_num;// entity의 수는 자연수이므로 unsigned int 를 사용하는 것이 효과적
	static deque<unsigned int> unused_group_ID_list;// static 변수로 지정하여 전체 Entity 객체 ID 중 사용되지 않는 것을 관리함.

	unsigned int group_ID = 0;//ID 는 1 부터 시작하도록 한다. ID 0은 아직 할당되지 않은 상태로 하여, 잘못된  entity 가 존재하는지 판단할 때 이용
	
public:
	Group();
	Group(const Group& obj);
	// 복사 생성자는 차후 수정이 필요
	// 복사 생성을 하는 경우 술어를 복사할 뿐 아니라, 그룹을 새롭게 만들어 주는 과정을 구현해야하지만,
	// 아직 그룹을 만드는 방법에 대하여 구현하지 못하였기 때문.
	~Group();// 소멸자에서 그룹 자체로 삭제되는 경우 포함된 entity의 affiliated_group_list 에서 자신을 모두 삭제해 주어야한다.

	//객체 정보 함수
	int get_type();
	unsigned int get_group_ID();

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

	// 그룹 조작함수 
	// protected(private도 가능할 듯.) virtual 함수를 public 함수로 오버라이딩 함
	// 이를 통해 group 객체는 외부에서 그룹관리 함수를 사용할 수 있지만
	// object 객체는 외부에서 그룹관리 함수를 사용할 수 없음
	// protected 를 통해 객체 외부 접근을 차단할 수 있으나, Entity를 상속받는 그 이외의 객체들에서의 내부 접근의 여지는 여전히 남겨둠
	void do_add_entity(Entity* entity_ptr);
	void do_del_entity(Entity* entity_ptr);
	bool do_add_entity_check_inclusion(Entity* entity_ptr);
	bool do_del_entity_check_inclusion(Entity* entity_ptr);
	void do_flush_entity();

	bool do_check_entity_inclusion(Entity* entity_ptr);

};

#endif