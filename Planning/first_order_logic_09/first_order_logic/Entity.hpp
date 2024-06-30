#pragma once

#ifndef ENTITY
#define ENTITY

#include <deque>
#include <list>

using namespace std;

class Entity
{
private:
	static unsigned int total_entity_num;// entity의 수는 자연수이므로 unsigned int 를 사용하는 것이 효과적
	static deque<unsigned int> unused_entity_ID_list;// static 변수로 지정하여 전체 Entity 객체 ID 중 사용되지 않는 것을 관리함.

	unsigned int entity_ID = 0;//ID 는 1 부터 시작하도록 한다. ID 0은 아직 할당되지 않은 상태로 하여, 잘못된  entity 가 존재하는지 판단할 때 이용?

protected:// 그룹 관리에 대한 자원은 파생 클래스에서 조작가능하여야 하므로 protected 접근지정자로 선언함.
	unsigned int entity_size = 0;
	// 빈 그룹의 경우에는 크기가 0
	// 하나의 오브젝트는 크기가 1
	// 잘못 선언되거나, 포함된 맴버의 삭제로인해 맴버를 가지지 않게 되어
	//  더 이상 효용이 없어진 그룹을 파악하는 데 효과적인 장치

	unsigned int group_order = 0;
	// 그룹 차수, 자신 아래로 존재하는 개체중 가장 낮은 차수를 가진 개체와의 차수 차이.
	// 0 이라면 자신은 하위그룹을 갖지 않는 상태임을 의미.
	unsigned int cover_order = 0;
	// 뚜껑 차수, 자신부터 위로 존재하는 가장 높은 차수를 가진 그룹과의 차수 차이.
	// 0 이라면 자신위로는 그룹이 존재하지 않음을 의미.

	list<Entity*> group_entity_list;// 자신이 포함된 그룹의 리스트
	list<Entity*> member_entity_list;// 자신이 포함한 맴버의 리스트

	unsigned int group_entity_list_length = 0;
	unsigned int member_entity_list_length = 0;

	list<unsigned int> predicates_pos_list;//긍정리스트
	list<unsigned int> predicates_nag_list;//부정리스트

	unsigned int predicates_pos_list_length = 0;
	unsigned int predicates_nag_list_length = 0;

public:
	Entity();
	Entity(const Entity& obj);
	virtual ~Entity();

	// 객체 정보 함수
	unsigned int get_entity_ID();
	virtual int get_type() = 0;//pure virtual
	//1 : object, 2 : group
	unsigned int get_group_order();
	unsigned int get_cover_order();
	//virtual void set_group_order(unsigned int order_input) = 0;
	// 순수 가상함수로 구현하지 않은 객체에서는 사용할 수 없음. 
	// object는 항상 0차로 고정해야하고, group은 별도로 차수 관리를 해야하니 오버라이딩을 가상함수로써 명시함.
	unsigned int get_entity_size();

	//술어 조작 함수
	void do_add_pred(unsigned int predicate_ID, bool nag_sign);
	void do_del_pred(unsigned int predicate_ID, bool nag_sign);
	bool do_add_pred_check_inclusion(unsigned int predicate_ID, bool nag_sign);
	bool do_del_pred_check_inclusion(unsigned int predicate_ID, bool nag_sign);
	void do_flush_pred();

	//술어 검색 함수
	bool is_included(unsigned int predicate_ID, bool nag_sign);

	//술어 출력 함수
	void do_print_all_predicates();
	
	//술어 접근 함수
	unsigned int get_pred_list_length(bool nag_sign);
	unsigned int* get_pred_id_ptr(unsigned int index, bool nag_sign);

	//주변 접근 함수
	unsigned int get_group_entity_list_length();
	unsigned int get_member_entity_list_length();
	Entity* get_group_entity_ptr(unsigned int index);
	Entity* get_member_entity_ptr(unsigned int index);

	//엔티티->그룹 관리 함수
	void do_add_group(Entity* group_ptr);
	void do_del_group(Entity* group_ptr);
	bool do_add_group_check_inclusion(Entity* group_ptr);
	bool do_del_group_check_inclusion(Entity* group_ptr);
	void do_flush_group();

	bool do_check_group_affiliation(Entity* group_ptr);
	bool do_check_group_affiliation_entity_ID(unsigned int entity_ID);//group의 entity_ID

protected:
	//그룹->엔티티 관리 함수
	virtual void do_add_entity(Entity* entity_ptr);
	virtual void do_del_entity(Entity* entity_ptr);
	virtual bool do_add_entity_check_inclusion(Entity* entity_ptr);
	virtual bool do_del_entity_check_inclusion(Entity* entity_ptr);
	virtual void do_flush_entity();

	virtual bool do_check_entity_inclusion(Entity* entity_ptr);
	virtual bool do_check_entity_inclusion_entity_ID(unsigned int entity_ID);

	// 사용시에는 Base:: 스코프 지정 연산자를 이용하여 private 맴버에 접근하도록 만들기
	// object 객체에서는 상속 받지 않았으면 하는 함수들이기 때문.

public:
	// 크기 관리 함수
	virtual void do_change_size(long difference);//object의 경우는 크기가 항상 1이므로 오버로딩하여 활용할시 크기가 변하지 않도록, 별도로 함수를 구현할 것. 
	void do_propagate_change_size(long difference);// 뚜껑차수에도 의존할 수는 있으나, 구현의 의존성을 최대한 줄이기 위하여 독립적인 방식으로 구현

	// 그룹 차수 관리 함수
	unsigned int do_predict_group_order_connection(Entity* entity_ptr);
	unsigned int do_predict_group_order_disconnection(Entity* entity_ptr);
	unsigned int do_predict_group_order_changing(Entity* outdated_member_ptr, unsigned int order_update);
	void do_handle_group_order_list_sort(Entity* outdated_member_ptr, unsigned int order_update);
	void do_propagate_group_order_changing(unsigned int order_update);

	unsigned int do_predict_group_order_connection_check_inclusion(Entity* entity_ptr);
	unsigned int do_predict_group_order_disconnection_check_inclusion(Entity* entity_ptr);
	unsigned int do_predict_group_order_changing_check_inclusion(Entity* outdated_member_ptr, unsigned int order_update);
	bool do_handle_group_order_list_sort_check_inclusion(Entity* outdated_member_ptr, unsigned int order_update);
	void do_propagate_group_order_changing_check_inclusion(unsigned int order_update);

	// 뚜껑 차수 관리 함수
	unsigned int do_predict_cover_order_connection(Entity* entity_ptr);
	unsigned int do_predict_cover_order_disconnection(Entity* entity_ptr);
	unsigned int do_predict_cover_order_changing(Entity* outdated_member_ptr, unsigned int order_update);
	void do_handle_cover_order_list_sort(Entity* outdated_member_ptr, unsigned int order_update);
	void do_propagate_cover_order_changing(unsigned int order_update);

	unsigned int do_predict_cover_order_connection_check_inclusion(Entity* entity_ptr);
	unsigned int do_predict_cover_order_disconnection_check_inclusion(Entity* entity_ptr);
	unsigned int do_predict_cover_order_changing_check_inclusion(Entity* outdated_member_ptr, unsigned int order_update);
	bool do_handle_cover_order_list_sort_check_inclusion(Entity* outdated_member_ptr, unsigned int order_update);
	void do_propagate_cover_order_changing_check_inclusion(unsigned int order_update);

};

#endif