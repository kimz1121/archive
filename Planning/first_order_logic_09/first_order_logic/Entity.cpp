//=========<inner class Entity>=========

#include "Entity.hpp"

#include <iostream>
#include <cassert>

using namespace std;

unsigned int Entity::total_entity_num = 0;

deque<unsigned int> Entity::unused_entity_ID_list;
// static 변수들은 실체가 클래스의 인스턴스 내부에 존재하지 않기에 실체를 다른 외부에 선언해 주어야 한다고 한다.
// 클래스 내부에서 사용예고에 대한 선언만 이뤄지고, 외부에서 실체를 별도로 선언해주지 못하면, 부호는 존재하나 그 실체와 연결하지 못하는 컴파일 링크 에러가 발생한다.

Entity::Entity()
{
	if (unused_entity_ID_list.empty() == true)
	{
		entity_ID = total_entity_num;//객체의 삭제와 추가를 반복할 때는 중간중간에 사용되지 않는 ID 들이 생기므로 이들을 별도의 vector 자료형으로 정리해 두었다가 오래된 순으로 사용하면 좋을 듯
	}
	else
	{
		entity_ID = unused_entity_ID_list.front();
		unused_entity_ID_list.pop_front();
	}
	total_entity_num++;
	cout << "Entity   added, entity_ID : " << entity_ID << endl;
}

Entity::Entity(const Entity& obj)
{
	// 복사 생성자를 통해 객체생성 시 술어복사를 가능하게 한다.
	// 복사 생성자라 할지라도, ID 는 복사해오지 않고 고유하게 생성한다. 
	if (unused_entity_ID_list.empty() == true)
	{
		entity_ID = total_entity_num;//객체의 삭제와 추가를 반복할 때는 중간중간에 사용되지 않는 ID 들이 생기므로 이들을 별도의 vector 자료형으로 정리해 두었다가 오래된 순으로 사용하면 좋을 듯
	}
	else
	{
		entity_ID = unused_entity_ID_list.front();
		unused_entity_ID_list.pop_front();
	}

	for (int item : obj.predicates_pos_list)
	{
		predicates_pos_list.push_back(item);
	}
	for (int item : obj.predicates_nag_list)
	{
		predicates_nag_list.push_back(item);
	}

	total_entity_num++;
	cout << "Entity   added, entity_ID : " << entity_ID << endl;
}

Entity::~Entity()
{
	// 고유 ID 관리
	unused_entity_ID_list.push_back(entity_ID);//삭제 작업 후에 반복자를 사용하면 안되므로 미리 값을 복사하고, 그 다음에 반복자가 가리키는 원소를 삭제
	total_entity_num--;

	if (total_entity_num <= 0)
	{
		unused_entity_ID_list.clear();
		deque<unsigned int>().swap(unused_entity_ID_list);
	}

	// 삭제 시 그룹 및 맴버 관리
	do_flush_entity();

	// 삭제 시 크기관리	
	// 해당 경우는 자신의 크기는 어차피 삭제되어 0이 되므로 중요하지 않으면서도, 자신의 크기 만큼의 변화를 전파시켜야 하는 상황
	// 그룹 리스트를 먼저 삭제해 버리면, 크기 변화에 대한 정보를 전달할 수 없으므로, 크기변화 관련 처리를 먼저 수행한 후, 삭제 시 그룹관리를 수행.
	// 크기관리는 하위 엔티티를 먼저 정리하고 수행한다.
	// 하위 엔티티를 제거하기 전에 크기를 차감하면
	// 그룹이 그룹이 포함한 하위개체들의 크기 총합과, 각 개별 원소가 삭제될 때 개체크기를 서로 동일한 속성임에도 중복차감하게 되므로 
	// 크기를 과소 추정하게되는 오류를 유발한다.
	do_propagate_change_size(-1 * (long)entity_size);

	//그룹관리
	do_flush_group();

	cout << "Entity deleted, entity_ID : " << entity_ID << endl;
	//cout << "total_entity_num : " << total_entity_num << endl;
}

//int Entity::get_type()
//{
//	return 0;
//}

unsigned int Entity::get_entity_ID()
{
	return entity_ID;
}


unsigned int Entity::get_group_order()
{
	return group_order;
}


unsigned int Entity::get_cover_order()
{
	return cover_order;
}

unsigned int Entity::get_entity_size()
{
	return entity_size;
}

void Entity::do_add_pred(unsigned int predicate_ID, bool nag_sign)
{
	if (nag_sign == 0)
	{
		predicates_pos_list.push_back(predicate_ID);
		predicates_pos_list_length++;//긍정 술어 리스트 길이 관리
	}
	else
	{
		predicates_nag_list.push_back(predicate_ID);
		predicates_nag_list_length++;//부정 술어 리스트 길이 관리
	}
}

void Entity::do_del_pred(unsigned int predicate_ID, bool nag_sign)
{
	//entity 객체 내부의 술어리스트들을 불러오고, 각 리스트를 탐색하기위한 처음 부분과 끝부분의 반복자를 반환받음.
	list<unsigned int>::iterator iter_pos = predicates_pos_list.begin();
	list<unsigned int>::iterator iter_pos_end = predicates_pos_list.end();
	list<unsigned int>::iterator iter_nag = predicates_nag_list.begin();
	list<unsigned int>::iterator iter_nag_end = predicates_nag_list.end();
	//디버그 유의사항 반복자의 값이 항상 반복자의 순서를 의미하지는 않는다.
	// 마지막 원소를 가리키는 반복자의 경우 값이 약 3452816845 정도 된다. 혹은 쓰레기 값일 수도 있다. 이처럼 값이 항상 위치를 대변하지 않는다.

	if (nag_sign == 0)
	{
		//for (; iter_pos != iter_pos_end; iter_pos++)
		if (iter_pos != iter_pos_end)//리스트가 이미 비어있어 반복자에 디레퍼런싱 접근 불가 에러가 발생하는 경우.
		{
			while (1)
			{
				if ((*iter_pos) == predicate_ID)
				{
					iter_pos = predicates_pos_list.erase(iter_pos);
					//이와 같이 삭제후에 반복자가 가리키는 위치를 erase의 반환으로 최신화 시켜주어야만, 삭제된 반복자에 접근 , 마지막 반복자를 증가연산하는 등의 에러를 방지한다.
					//item.predicates_pos.erase(iter_pos);// 이와 같은 방법으로 하면, 삭제된 반복자에 접근 , 마지막 반복자를 증가연산하는 등의 에러가 발생할 수 있다.
					//또한 마지막원소를 가리키는 반복자는 가리키는 자료형이 없음에도 삭제시킬 수 있다....??
					//서로다른 방식의 삭제를 두번하니 프로세스가 종료될 때 시간이 없청 오래걸리는 버그를 경험함. 
					predicates_pos_list_length--;//긍정 술어 리스트 길이 관리
				}
				else
				{
					iter_pos++;
				}
				if (iter_pos == iter_pos_end)
				{
					break;
				}
			}
		}

	}
	else
	{
		if (iter_nag != iter_nag_end)
		{
			while (1)
			{
				if ((*iter_nag) == predicate_ID)
				{
					iter_nag = predicates_nag_list.erase(iter_nag);
					predicates_nag_list_length--;//부정 술어 리스트 길이 관리
				}
				else
				{
					iter_nag++;
				}
				if (iter_nag == iter_nag_end)
				{
					break;
				}
			}
		}
	}
}

bool Entity::do_add_pred_check_inclusion(unsigned int predicate_ID, bool nag_sign)
{
	bool discriminant = false;
	if (is_included(predicate_ID, nag_sign) == false)// 아직 술어가 포함되지 않은 경우
	{
		do_add_pred(predicate_ID, nag_sign);
		discriminant = true;
	}
	return discriminant;
}

bool Entity::do_del_pred_check_inclusion(unsigned int predicate_ID, bool nag_sign)
{
	bool discriminant = false;
	if (is_included(predicate_ID, nag_sign) == true)// 이미 술어가 포함된 경우만 삭제 작업을 진행
	{
		do_del_pred(predicate_ID, nag_sign);
		discriminant = true;
	}
	return discriminant;
}

void Entity::do_flush_pred()
{
	predicates_pos_list.erase(predicates_pos_list.begin(), predicates_pos_list.end());
	predicates_nag_list.erase(predicates_nag_list.begin(), predicates_nag_list.end());
	predicates_pos_list_length = 0;
	predicates_nag_list_length = 0;
}

bool Entity::is_included(unsigned int predicate_ID, bool nag_sign)
{
	bool included = false;
	if (nag_sign == 0)
	{
		for (int element : predicates_pos_list)
		{
			if (element == predicate_ID)
			{
				included = true;
			}
		}
	}
	else
	{
		for (int element : predicates_nag_list)
		{
			if (element == predicate_ID)
			{
				included = true;
			}
		}
	}
	return included;
}

void Entity::do_print_all_predicates()
{
	cout << "positive_predicates : " << endl;
	for (int element : predicates_pos_list)
	{
		cout << element << endl;
	}
	cout << "negative_predicates : " << endl;
	for (int element : predicates_nag_list)
	{
		cout << element << endl;
	}
}

//술어 접근 함수
unsigned int Entity::get_pred_list_length(bool nag_sign)
{
	unsigned int result = 0;
	if (nag_sign == false)
	{//predicats_pos
		result = predicates_pos_list_length;
	}
	else
	{//predicats_nag
		result = predicates_nag_list_length;
	}
	return result;
}

unsigned int* Entity::get_pred_id_ptr(unsigned int index, bool nag_sign)
{
	list<unsigned int>::iterator iter;
	list<unsigned int>::iterator iter_end;

	unsigned int list_length = 0;
	unsigned int* result = nullptr;
	if (nag_sign == false)
	{//predicats_pos
		iter = predicates_pos_list.begin();
		iter_end = predicates_pos_list.end();
		list_length = predicates_pos_list_length;
	}
	else
	{//predicats_nag
		iter = predicates_nag_list.begin();
		iter_end = predicates_nag_list.end();
		list_length = predicates_nag_list_length;
	}
	if (index >= 0 && index < list_length)
	{
		for(unsigned int i = 0; i < index; i++)
		{ 
			iter++;
		}
		result = &(*iter);//nullptr 을 통해 반환 성공 여부를 판별할 수 있지만, 세부적인 주소 및 참조 변환과정이 추가되므로 효율이 떨어지는 단점이 있음.
	}
	return result;
}

//주변 접근 함수
unsigned int Entity::get_group_entity_list_length()
{
	return group_entity_list_length;
}

unsigned int Entity::get_member_entity_list_length()
{
	return member_entity_list_length;
}

Entity* Entity::get_group_entity_ptr(unsigned int index)
{
	list<Entity*>::iterator iter = group_entity_list.begin();
	list<Entity*>::iterator end = group_entity_list.end();

	Entity* result = nullptr;
	if (index >= 0 && index < group_entity_list_length)
	{
		for (unsigned int i = 0; i < index; i++)
		{
			iter++;
		}
		result = (*iter);
	}
	return result;
}

Entity* Entity::get_member_entity_ptr(unsigned int index)
{
	list<Entity*>::iterator iter = member_entity_list.begin();
	list<Entity*>::iterator iter_end = member_entity_list.end();

	Entity* result = nullptr;
	if (index >= 0 && index < member_entity_list_length)
	{
		for (unsigned int i = 0; i < index; i++)
		{
			iter++;
		}
		result = (*iter);
	}
	return result;
}

void Entity::do_add_group(Entity* group_ptr)
{
	// 원소에 그룹 추가
	list<Entity*>::iterator iter = group_entity_list.begin();
	list<Entity*>::iterator iter_end = group_entity_list.end();

	while (1)// 뚜껑 차수가 큰 원소가 맨 앞의 원소가 되도록, 크기를 비교하여 원소를 사이에 추가함.
	{
		if (iter == iter_end)
		{
			group_entity_list.insert(iter, group_ptr);// 리스트가 비었거나 가장 뒤쪽에 추가하는 경우
			break;
		}
		if ((*group_ptr).get_cover_order() >= (**iter).get_cover_order())// 자신이 삽입될 위치의 바로 뒤 원소의 그룹차수와 자신의 뚜껑 차수를 비교
		{
			group_entity_list.insert(iter, group_ptr);// 그룹 차수가 큰 원소가 맨 앞의 원소가 되도록, 크기를 비교하여 원소를 사이에 추가함.
			//insert 함수는 인수로 전달되는 반복자의 바로 앞에 원소를 추가하는 함수이다.
			// 모든 원소보다 뒤쪽인 마지막에 원소를 삽입하고 싶다면, 마지막 원소보다 한자리 뒤를 가리키는 list::end() 를 통해 얻은 반복자를 인수에 대입하면 된다.
			break;
		}
		iter++;
	}
	group_entity_list_length++;// 그룹 리스트 길이 관리

	// 그룹에 원소 추가
	(*group_ptr).do_add_entity_check_inclusion(this);
	// 상호 추가 상호 삭제를 원칙으로 하므로 
	// check_inclusion 함수를 사용하지 않으면 이미 원소를 포함시키는 작업이 완료되었음에도 
	// 재귀적인 함수호출로 인한, 무한 반복이 이어질 수 있다.

	// 뚜껑 차수 계산
	unsigned int order_update = do_predict_cover_order_connection(group_ptr);// 호출한 entity(group)가 인수로 받은 entity(member)를 포함하게 된다면, 호출한 entity(group)의 차수는 얼마가 될 것인가를 예측하는 함수.
	// 그룹 차수 전파 및 변경량 반영 처리
	do_propagate_cover_order_changing(order_update);
	// 자신의 차수가 변함을 상위 그룹으로 전달하는 역할.
}

void Entity::do_del_group(Entity* group_ptr)
{	
	// 뚜껑 차수 계산
	unsigned int order_update = do_predict_cover_order_disconnection(group_ptr);// 호출한 entity(group)가 인수로 받은 entity(member)를 포함하게 된다면, 호출한 entity(group)의 차수는 얼마가 될 것인가를 예측하는 함수.
	// 그룹 차수 전파 및 변경량 반영 처리
	do_propagate_cover_order_changing(order_update);
	// 자신의 차수가 변함을 상위 그룹으로 전달하는 역할.
	
	// 원소에서 그룹 삭제
	list<Entity*>::iterator iter = group_entity_list.begin();
	list<Entity*>::iterator iter_end = group_entity_list.end();

	if (iter != iter_end)
	{
		while (1)
		{
			// Entity는 추상 클래스로써 하위 클래스에 포함된 자료형인 group_ID 개념이 존재하지 않는다.
			// pure virtual 함수로 선언하고 하위 객체에서 group을 구현해야하나....
			//if ((**iter).get_entity_ID() == (*group_ptr).get_entity_ID())
			if (*iter == group_ptr)// 이와 같이 포인터 간의 직접 비교를 하면, 상위 객체에서 group_ID에 접근하지 않으면서도 서로 같은 그룹을 가리키는지 비교할 수 있다.
			{
				iter = group_entity_list.erase(iter);// 개체 삭제와 동시에 반복자 이동.
				// 그룹 자체를 삭제하는 것이 아닌, 포함된 그룹의 소속만 삭제하는 것이므로, 포인터가 가리키는 객체의 매모리를 반납하지는 않는다.
				// 그룹에 자신이외에 다른 개체들도 함께 포함되어 있을 수 있기 때문
				group_entity_list_length--;// 그룹 리스트 길이 관리
			}
			else
			{
				iter++;//삭제하지 않을 시에는 반복자만 이동.
			}
			if (iter == iter_end)// 이동한 반복자가 마지막 반복자일 경우에는 반복문 종료
			{
				break;
			}
		}
	}

	// 그룹에서 원소 삭제 : 상호적으로 삭제가 이루어 져야지만, 삭제 과정에서 유실되는 메모리나, 포인터가 발생하지 않는다
	(*group_ptr).do_del_entity_check_inclusion(this);
	// 상호 추가 상호 삭제를 원칙으로 하므로 
	// check_inclusion 함수를 사용하지 않으면 이미 원소를 포함시키는 작업이 완료되었음에도 
	// 재귀적인 함수호출로 인한, 무한 반복이 이어질 수 있다.
}

bool Entity::do_add_group_check_inclusion(Entity* group_ptr)
{
	bool discriminant = false;
	if (do_check_group_affiliation(group_ptr) == false)// 그룹 리스트에 추가하고자하는 그룹이 아직 없음
	{
		do_add_group(group_ptr);
		discriminant = true;
	}
	return discriminant;
}

bool Entity::do_del_group_check_inclusion(Entity* group_ptr)
{
	bool discriminant = false;
	if (do_check_group_affiliation(group_ptr) == true)// 그룹 리스트에 추가하고자하는 그룹이 이미 있음
	{
		do_del_group(group_ptr);
		discriminant = true;
	}
	return discriminant;
}

void Entity::do_flush_group()
{
	//for (auto item : group_entity_list)
	//{
	//	do_del_group_check_inclusion(item);
	//}
	// 치명적인 버그를 발견, 반복문 동작중에 group_entity_list의 원소를 삭제하게 되므로써, 반복자의 링크가 절단되고, 다음 반복자에 접근하지 못하는 상황이 발생함.
	// 따라서 반복자의 이동을 직접 관리하는 while 문을 사용하여 버그 발생을 방지 할 것임
	list<Entity*>::iterator iter = group_entity_list.begin();
	list<Entity*>::iterator iter_end = group_entity_list.end();
	Entity* item_ptr = nullptr;

		while (1)
		{
			if (iter == iter_end)
			{
				break;
			}
			item_ptr = (*iter);//삭제할 포인터를 미리 담아둠. 실제로 삭제하는 것은 반복자를 이동시키고 난 후에 실행하여, 반복자가 계속 생존할 수 있도록 함.
			iter++;// 삭제된 반복자...?
			do_del_group_check_inclusion(item_ptr);
		}
}

bool Entity::do_check_group_affiliation(Entity* group_ptr)
{
	bool discriminant = false;
	for (auto item : group_entity_list)
	{
		//if ((*item).get_entity_ID() == (*group_ptr).get_entity_ID())
		if (item == group_ptr)// 포인터 간의 직접 비교를 하면, 상위 객체에서 group_ID에 접근하지 않으면서도 서로 같은 그룹을 가리키는지 비교할 수 있다.
		{
			discriminant = true;
			break;
		}
	}
	return discriminant;
}

bool Entity::do_check_group_affiliation_entity_ID(unsigned int entity_ID)// 포인터 비교가 아닌 entity_ID 로만 객체를 검색하고자할 때 사용하는 함수
{
	bool discriminant = false;
	for (auto item : group_entity_list)
	{
		if ((*item).get_entity_ID() == entity_ID)
		{
			discriminant = true;
			break;
		}
	}
	return discriminant;
}


void Entity::do_add_entity(Entity* entity_ptr)
{
	// 그룹에 원소 추가
	list<Entity*>::iterator iter = member_entity_list.begin();
	list<Entity*>::iterator iter_end = member_entity_list.end();

	while (1)// 그룹 차수가 큰 원소가 맨 앞의 원소가 되도록, 크기를 비교하여 원소를 사이에 추가함.
	{
		if (iter == iter_end)
		{
			member_entity_list.insert(iter, entity_ptr);// 리스트가 비었거나 가장 뒤쪽에 추가하는 경우
			break;
		}
		if ((*entity_ptr).get_group_order() >= (**iter).get_group_order())// 자신이 삽입될 위치의 바로 뒤 원소의 그룹차수와 자신의 그룹차수를 비교
		{
			member_entity_list.insert(iter, entity_ptr);// 그룹 차수가 큰 원소가 맨 앞의 원소가 되도록, 크기를 비교하여 원소를 사이에 추가함.
			//insert 함수는 인수로 전달되는 반복자의 바로 앞에 원소를 추가하는 함수이다.
			// 모든 원소보다 뒤쪽인 마지막에 원소를 삽입하고 싶다면, 마지막 원소보다 한자리 뒤를 가리키는 list::end() 를 통해 얻은 반복자를 인수에 대입하면 된다.
			break;
		}
		iter++;
	}
	member_entity_list_length++;// 맴버 리스트 길이 관리

	// 원소에 그룹 추가
	(*entity_ptr).do_add_group_check_inclusion(this);// 원소에 그룹포인터를 추가시킨다. 
	// 상호 추가 상호 삭제를 원칙으로 하므로 
	// check_inclusion 함수를 사용하지 않으면 이미 원소를 포함시키는 작업이 완료되었음에도 
	// 재귀적인 함수호출로 인한, 무한 반복이 이어질 수 있다.

	// 그룹 크기 계산
	long size_of_change = (*entity_ptr).get_entity_size();// 추가한 객체의 크기 만큼 객체의 크기 증가.
	do_change_size(size_of_change);
	// 상위 그룹 크기 변화 요청
	do_propagate_change_size(size_of_change);

	// 그룹 차수 계산
	unsigned int order_update = do_predict_group_order_connection(entity_ptr);// 호출한 entity(group)가 인수로 받은 entity(member)를 포함하게 된다면, 호출한 entity(group)의 차수는 얼마가 될 것인가를 예측하는 함수.
	// 그룹 차수 전파 및 변경량 반영 처리
	do_propagate_group_order_changing(order_update);
	// 자신의 차수가 변함을 상위 그룹으로 전달하는 역할.
}

void Entity::do_del_entity(Entity* entity_ptr)
{
	// 그룹 차수 계산
	unsigned int order_update = do_predict_group_order_disconnection(entity_ptr);// 호출한 entity(group)가 인수로 받은 entity(member)를 분리하게 된다면, 호출한 entity(group)의 차수는 얼마가 될 것인가를 예측하는 함수.
	// 그룹 차수 전파 및 변경량 반영 처리
	do_propagate_group_order_changing(order_update);
	// 자신의 차수가 변함을 상위 그룹으로 전달하는 역할.
	
	// 그룹에서 원소 삭제
	list<Entity*>::iterator iter = member_entity_list.begin();
	list<Entity*>::iterator iter_end = member_entity_list.end();

	if (iter != iter_end)// 리스트가 비어있지 않은 경우
	{
		while (1)
		{
			if (*iter == entity_ptr)// 삭제할 원소의 포인터를 찾은 경우
			{
				iter = member_entity_list.erase(iter);
				// 삭제 후 다음 원소를 가리킴. (중복인 경우 다음에 존재하는 같은 포인터 또한 삭제)
				// 원천적으로 중복을 허용하지 않으나, 중복인 것을 만에하나 발견하더라도 삭제하도록 함.
				member_entity_list_length--;// 맴버 리스트 길이 관리
			}
			else
			{
				iter++;
			}
			if (iter == iter_end)
			{
				break;
			}
		}
	}

	// 원소에서 그룹 삭제
	(*entity_ptr).do_del_group_check_inclusion(this);// 원소에서 그룹포인터를 삭제시킨다.
	// 상호 추가 상호 삭제를 원칙으로 하므로 
	// check_inclusion 함수를 사용하지 않으면 이미 원소를 포함시키는 작업이 완료되었음에도 
	// 재귀적인 함수호출로 인한, 무한 반복이 이어질 수 있다.

	// 그룹 크기 계산
	long size_of_change = -1*(long)(*entity_ptr).get_entity_size();// 추가한 객체의 크기 만큼 객체의 크기 증가.
	do_change_size(size_of_change);
	// 상위 그룹 크기 변화 요청
	do_propagate_change_size(size_of_change);
}

bool Entity::do_add_entity_check_inclusion(Entity* entity_ptr)
{
	bool discriminant = false;
	if (do_check_entity_inclusion(entity_ptr) == false)
	{
		do_add_entity(entity_ptr);
		discriminant = true;
	}
	return discriminant;
}

bool Entity::do_del_entity_check_inclusion(Entity* entity_ptr)
{
	bool discriminant = false;
	if (do_check_entity_inclusion(entity_ptr) == true)
	{
		do_del_entity(entity_ptr);
		discriminant = true;
	}
	return discriminant;
}

void Entity::do_flush_entity()
{
	list<Entity*>::iterator iter = member_entity_list.begin();
	list<Entity*>::iterator iter_end = member_entity_list.end();
	Entity* item_ptr = nullptr;

	while (1)
	{
		if (iter == iter_end)
		{
			break;
		}
		item_ptr = (*iter);//삭제할 포인터를 미리 담아둠. 실제로 삭제하는 것은 반복자를 이동시키고 난 후에 실행하여, 반복자가 계속 생존할 수 있도록 함.
		iter++;// 삭제된 반복자...?
		do_del_entity_check_inclusion(item_ptr);
	}

}

bool Entity::do_check_entity_inclusion(Entity* entity_ptr)
{
	bool discriminant = false;
	for (auto item : member_entity_list)
	{
		if (item == entity_ptr)
		{
			discriminant = true;
			break;
		}
	}
	return discriminant;
}

bool Entity::do_check_entity_inclusion_entity_ID(unsigned int entity_ID)
{
	bool discriminant = false;
	for (auto item : member_entity_list)
	{
		if ((*item).get_entity_ID() == entity_ID)
		{
			discriminant = true;
			break;
		}
	}
	return discriminant;
}

void Entity::do_change_size(long difference)// 한번에 unsigned int 의 최대표현크기 만큼의 값을 바꾸는 경우는 없다고 가정하고, 변화 크기 인수를 int 타입으로 선언
{
	// 자신의 크기를 바꾸는 역할이 아닌, 크기 변화를 전파하는 역할.
	if (difference <= 1000 && difference >= -1000)//entity_size의 크기가 unsigned int 이므로 대입전에 크기를 초과하지 않을지 점검.
	{
		entity_size += difference;
	}
	else
	{
		cout << "value out of range of unsigned int" << endl;
		assert(0);
	}
}

void Entity::do_propagate_change_size(long difference)// 한번에 unsigned int 의 최대표현크기 만큼의 값을 바꾸는 경우는 없다고 가정하고, 변화 크기 인수를 int 타입으로 선언
{
	// 자신의 크기를 바꾸는 역할은 함께 포함하지 않는다.
	// 자신의 크기를 바꾸는 역할이 아닌, 크기 변화를 전파하는 역할.
	for (auto item : group_entity_list)
	{
		(*item).do_change_size(difference);
		(*item).do_propagate_change_size(difference);
	}
}

unsigned int Entity::do_predict_group_order_connection(Entity* entity_ptr)// 접합한 경우
{
	// 어떠한 그룹에서 자신의 맴버 엔티티가 연결되었을 때 그룹인 자신의 차수가 어떻게 바뀌어야 하는지 예측하는 함수.
	unsigned int predicted_order = 0;
	unsigned int connected_entity_order = (*entity_ptr).get_group_order();
	if (group_order <= connected_entity_order)// 차수 갱신의 상황
	{
		predicted_order = connected_entity_order + 1;
	}
	else
	{
		predicted_order = group_order;
	}
	return predicted_order;
}

unsigned int Entity::do_predict_group_order_disconnection(Entity* entity_ptr)// 분리한 경우
{
	// 포함 여부 확인 절차를 거쳐야 하나...? 포함되지 않은 것을 분리할 수는 없으니 고민이 된다.
	// 포함 여부는 이 함수가 작동되기 이전에 확인되어야 한다. 
	// 포함 여부를 확인하고 싶다면 check_inclusion 함수를 별도로 만드는 것이 타당해 보인다. 0803-06:08
	// 어떠한 그룹에서 자신의 맴버 엔티티가 분리되었을 때 그룹인 자신의 차수가 어떻게 바뀌어야 하는지 예측하는 함수.
	unsigned int predicted_order = 0;
	list<Entity*>::iterator iter = member_entity_list.begin();
	Entity* member_entity_ptr = nullptr;
	if (member_entity_list.empty() == false)
	{
		member_entity_ptr = (*iter);// 가장 첫 번 째 원소를 고른다
		if (member_entity_ptr == entity_ptr)//자신이 최고차수 이며, 정렬 순서중 첫 번 째 원소인 경우
		{
			iter++;// 두번째 원소 선택
			if (iter != member_entity_list.end())// 리스트 내의 맴버가 1개 뿐인 경우가 있으므로, 두 번 째 반복자가 참조 불가능한 마지막 반복자일 가능성을 점검한다.
			{
				predicted_order = (**iter).get_group_order() + 1;// 해당 경우의 수가 두번째 원소에 의한 최고차수 유지 경우와, 두번째 요소로의 최고차수 감소의 경우를 모두 내포하여, 분기가 간단해짐.
			}
			else// 리스트의 요소가 자신 뿐인 경우
			{// 분리의 경우가 아닌 감소 변화의 경우라면, 최고차수가 변화한 후의 자신이 되는 경우도 비교해야한다.
				predicted_order = 0;// 분리의 경우
			}
		}
		else//분리되는 entity의 차수가 최고차수가 아닌경우에는 차수를 그대로 유지한다. 자신을 제외한 다른 원소가 최고차수를 가질 것이기 때문.
		{
			predicted_order = group_order;// 만일 차수가 0 인 오브젝트를 포함한다면, 그룹의 차수는 최소 1 이상이다. 
		}
	}
	else
	{
		cout << "그룹에서 더 이상 원소를 제거할 수 없습니다. list was empty. can not delete element anymore" << endl;
		assert(0);
	}
	return predicted_order;
}

unsigned int Entity::do_predict_group_order_changing(Entity* outdated_member_ptr, unsigned int order_update)//변화한 경우
{
	// 어떠한 그룹에서 자신의 맴버 엔티티에서 그룹 차수가 변경될 시 자신의 차수가 어떻게 바뀌어야 하는지 예측하는 함수.
	// 맴버가 추가 제거 소멸한 경우(차수전파에 의한 변화가 아닌 직접 연산에 의한 변화)는  원소 자신의 포인터와 변하고 난 후의 차수를 entity_ptr, order_update에 대입하면 된다.
	// 기본적으로 do_predict_group_order 중에서 연결, 분리의 경우를 참고하여 제작한 함수이다.

	unsigned int predicted_order = 0;
	unsigned int next_highest_member_order = 0;// 아직 정해지지 않음..
	unsigned int outdated_member_order = (*outdated_member_ptr).get_group_order();
	list<Entity*>::iterator member_entity_iter;// 초기화를 굳이 하지 않는다. 초기화 함수를 부르는 만큼의 불필요한 컴퓨팅 자원이 소모되기 때문
	Entity* member_entity_ptr = nullptr;
	if(outdated_member_order == order_update)// 맴버의 그룹차수에 변화가 없는 경우. 전파를 중단해야하는 경우를 판단하기 위해 필요한 분기
	{
		predicted_order = group_order;// 맴버의 그룹차수에 변화가 없다면, 자신의 그룹차수에도 변화가 없다.
	}
	else
	{
		if (outdated_member_order < order_update)
		{
			if (group_order <= order_update)
			{
				predicted_order = order_update + 1;
			}
			else
			{
				predicted_order = group_order;
			}
		}
		else 
		// == 등호의 경우의 수는 제외되었으므로
		// (*entity_ptr).get_group_order() > order_update 조건에 해당하는 분기.
		{
			member_entity_iter = member_entity_list.begin();
			if (member_entity_list.empty() == false)
			{
				member_entity_ptr = (*member_entity_iter);// 가장 첫 번 째 원소를 고른다
				// 정렬에 의해 최소차수 맴버가 가장 앞에 배치된 맴버라는 정보가 암묵적으로 포함되어 있어, 
				// 최고차수의 값을 알아낸 후 직접 비교할 필요가 없어졌고, 그에 따라 분기의 수를 줄여준다.
				if (member_entity_ptr == outdated_member_ptr)// 정렬 순서중 첫 번 째 원소인 경우 자신이 최고차수라는 정보를 암묵적으로 포함하고 있음
				{
					member_entity_iter++;// 두번째 원소 선택
					if (member_entity_iter != member_entity_list.end())// 리스트 내의 맴버가 1개 뿐인 경우가 있으므로, 두 번 째 반복자가 참조 불가능한 마지막 반복자일 가능성을 점검한다.
					{
						next_highest_member_order = (**member_entity_iter).get_group_order();// 두번째 원소의 차수 정보를 알고 있어야 한다. 두 번 째 원소는 최고차수의 후보이기 때문
						if (next_highest_member_order <= order_update)
						{
							predicted_order = order_update + 1;// 최고차수 감소 경우 중 두 번 째로 큰 최고차수로도 차수가 유지되지 않는 경우의 수
						}
						else
						{
							predicted_order = next_highest_member_order + 1;// 해당 경우의 수가 두번째 원소에 의한 최고차수 유지 경우와, 두번째 요소로의 최고차수 감소의 경우를 모두 내포하여, 분기가 간단해짐.
						}
					}
					else// 리스트의 요소가 자신 뿐인 경우
					{// 분리의 경우가 아닌 감소 변화의 경우라면, 최고차수가 변화한 후의 자신이 되는 경우도 비교해야한다.
						predicted_order = order_update + 1;
					}
				}
				else
				//차수감소하는 경우 중 entity의 정렬순서가 맨 앞이 아닌 경우에는 차수를 그대로 유지한다. 
				// 자신이 아닌 다른 원소가 최고차수를 유지하는 중이기 때문이다. 최고차수를 유지해주는 원소의 차수가 바뀌지 않는한, 그룹차수는 변하지 않기 때문
				{
					predicted_order = group_order;
				}
			}
			else
			{
				cout << "그룹에서 더 이상 원소를 제거할 수 없습니다. list was empty. can not delete element anymore" << endl;
				assert(0);
			}
		}
	}
	return predicted_order;
}

void Entity::do_handle_group_order_list_sort(Entity* outdated_member_ptr, unsigned int order_update)// 그룹이 자신의 맴버변화를 감지한 후 자신의 그룹 차수를 계산하기 위한 함수
{
	// 연결과 분리의 경우는 
	// add, del 함수가 맴버리스트를 잘 정렬해 주지만, 
	// 변화, changing 의 경우에는 처리시에 자신이 직접 리스트를 차수 기준 내림차순으로 정렬해 주어야 한다.

	list<Entity*>::iterator iter = member_entity_list.begin();
	list<Entity*>::iterator iter_end = member_entity_list.end();
	
	// 리스트에서 기존의 포인터 삭제
	if (iter != iter_end)// 리스트가 비어있지 않은 경우
	{
		while (1)
		{
			if (*iter == outdated_member_ptr)// 삭제할 원소의 포인터를 찾은 경우
			{
				iter = member_entity_list.erase(iter);
				// 삭제 후 다음 원소를 가리킴. (중복인 경우 다음에 존재하는 같은 포인터 또한 삭제)
				// 원천적으로 중복을 허용하지 않으나, 중복인 것을 만에하나 발견하더라도 삭제하도록 함.
			}
			else
			{
				iter++;
			}
			if (iter == iter_end)
			{
				break;
			}
		}
	}

	// 리스트에 포인터를 다시 삽입
	iter = member_entity_list.begin();//반복자 초기화
	while (1)// 그룹 차수가 큰 원소가 맨 앞의 원소가 되도록, 크기를 비교하여 원소를 사이에 추가함.
	{
		if (iter == iter_end)
		{
			member_entity_list.insert(iter, outdated_member_ptr);// 리스트가 비었거나 가장 뒤쪽에 추가하는 경우
			break;
		}
		if (order_update >= (**iter).get_group_order())// 자신이 삽입될 위치의 바로 뒤 원소의 그룹차수와 자신의 그룹차수를 비교
		{
			member_entity_list.insert(iter, outdated_member_ptr);// 그룹 차수가 큰 원소가 맨 앞의 원소가 되도록, 크기를 비교하여 원소를 사이에 추가함.
			//insert 함수는 인수로 전달되는 반복자의 바로 앞에 원소를 추가하는 함수이다.
			// 모든 원소보다 뒤쪽인 마지막에 원소를 삽입하고 싶다면, 마지막 원소보다 한자리 뒤를 가리키는 list::end() 를 통해 얻은 반복자를 인수에 대입하면 된다.
			break;
		}
		iter++;
	}
}

void Entity::do_propagate_group_order_changing(unsigned int order_update_of_self)// 자신과 자신의 변화가 입력. 자신은 객체를 호출한 본인 그 자체로도 전달됨.(this)
{
	// 자신의 크기를 바꾸는 역할과 자신의 변화를 상위로 전달하는 역할
	// 하위 멤버의 변화를 감지하는 역할이 아니다. 주동사동 관계를 명확히 하자. 여기서의 행동의 주체는 자신(하위 맴버)이다.
	unsigned int order_update_of_group = 0;
	if (group_order != order_update_of_self)// 그룹 차수 변화가 없다면 차수 변화 전파을 중단한다는 의미. 차수 변화가 있을 때만 차수 변화 전파를 수행한다.
	{
		// 만일 차수 변화가 있더라도 최상위 그룹이라면 
		// group_entity_list가 빈 리스트이므로 전파를 중단함.
		for (auto item : group_entity_list)
		{
			order_update_of_group = (*item).do_predict_group_order_changing(this, order_update_of_self);// 상위 그룹의 관점에서, 바뀔 맴버, 바뀔 차수
			(*item).do_propagate_group_order_changing(order_update_of_group);//상위 그룹이 호출하는 자신의 변화 전파 함수.
			// 전파 완료 후, 상위 그룹의 맴버인 자신의 차수가 변했으므로, 상위 그룹의 맴버 리스트를 자신의 변화한 차수를 반영하여, 차수 내림차순 정렬 수행
			(*item).do_handle_group_order_list_sort(this, order_update_of_self);// 상위 그룹의 관점에서, 바뀔 맴버, 바뀔 차수
		}
		// 모든 작업 후 자신의 차수 변화를 반영한다.
		// 궁극적으로 차수값을 바꾸어 주는 역할도 do_propagate_group_order_changing 함수에서 수행한다.
		group_order = order_update_of_self;// 리스트에 직접 접근하지 않아도, 리스트 자체가 주소를 가리키는 포인터 리스트 이므로, 자신의 값을 바꾸면 리스트에서 접근한 자신의 값도 바뀐다.
	}
}

unsigned int Entity::do_predict_group_order_connection_check_inclusion(Entity* entity_ptr)
{
	unsigned int result = 0;
	if (do_check_entity_inclusion(entity_ptr) == false)// 중복 요소가 없을 때 실행
	{
		result = do_predict_group_order_connection(entity_ptr);
	}
	else
	{
		cout << "목록에 요소가 이미 존재합니다. The element is already exist in the list" << endl;
		cout << "중복 시행은 무시합니다." << endl;
	}
	return result;
}

unsigned int Entity::do_predict_group_order_disconnection_check_inclusion(Entity* entity_ptr)
{
	unsigned int result = 0;
	if (do_check_entity_inclusion(entity_ptr) == true)// 찾는 요소가 있을 때 실행
	{
		result = do_predict_group_order_disconnection(entity_ptr);
	}
	else
	{
		cout << "찾는 요소가 없습니다. There's no element you're looking for." << endl;
		assert(0);
	}
	return result;
}

unsigned int Entity::do_predict_group_order_changing_check_inclusion(Entity* outdated_member_ptr, unsigned int order_update)
{
	unsigned int result = 0;
	if (do_check_entity_inclusion(outdated_member_ptr) == true)// 찾는 요소가 있을 때 실행
	{
		result = do_predict_group_order_changing(outdated_member_ptr, order_update);
	}
	else
	{
		cout << "찾는 요소가 없습니다. There's no element you're looking for." << endl;
		assert(0);
	}
	return result;
}

bool Entity::do_handle_group_order_list_sort_check_inclusion(Entity* outdated_member_ptr, unsigned int order_update)
{
	bool discriminant = false;
	if (do_check_entity_inclusion(outdated_member_ptr) == true)// 찾는 요소가 있을 때 실행
	{
		do_handle_group_order_list_sort(outdated_member_ptr, order_update);
		discriminant = true;
	}
	else
	{
		cout << "찾는 요소가 없습니다. There's no element you're looking for." << endl;
		assert(0);
	}
	return discriminant;
}

void Entity::do_propagate_group_order_changing_check_inclusion(unsigned int order_update_of_self)// 자신과 자신의 변화가 입력. 자신은 객체를 호출한 본인 그 자체로도 전달됨.(this)
{
	// 자신의 크기를 바꾸는 역할과 자신의 변화를 상위로 전달하는 역할
	// 하위 멤버의 변화를 감지하는 역할이 아니다. 주동사동 관계를 명확히 하자. 여기서의 행동의 주체는 자신(하위 맴버)이다.
	unsigned int order_update_of_group = 0;
	if (group_order != order_update_of_self)// 그룹 차수 변화가 없다면 차수 변화 전파을 중단한다는 의미. 차수 변화가 있을 때만 차수 변화 전파를 수행한다.
	{
		// 만일 차수 변화가 있더라도 최상위 그룹이라면 
		// group_entity_list가 빈 리스트이므로 전파를 중단함.
		for (auto item : group_entity_list)
		{
			order_update_of_group = (*item).do_predict_group_order_changing_check_inclusion(this, order_update_of_self);// 상위 그룹의 관점에서, 바뀔 맴버, 바뀔 차수
			(*item).do_propagate_group_order_changing_check_inclusion(order_update_of_group);//상위 그룹이 호출하는 자신의 변화 전파 함수.
			// 전파 완료 후, 상위 그룹의 맴버인 자신의 차수가 변했으므로, 상위 그룹의 맴버 리스트를 자신의 변화한 차수를 반영하여, 차수 내림차순 정렬 수행
			(*item).do_handle_group_order_list_sort_check_inclusion(this, order_update_of_self);// 상위 그룹의 관점에서, 바뀔 맴버, 바뀔 차수
		}
		// 모든 작업 후 자신의 차수 변화를 반영한다.
		// 궁극적으로 차수값을 바꾸어 주는 역할도 do_propagate_group_order_changing 함수에서 수행한다.
		group_order = order_update_of_self;// 리스트에 직접 접근하지 않아도, 리스트 자체가 주소를 가리키는 포인터 리스트 이므로, 자신의 값을 바꾸면 리스트에서 접근한 자신의 값도 바뀐다.
	}
}

unsigned int Entity::do_predict_cover_order_connection(Entity* entity_ptr)// 접합한 경우
{
	// 어떠한 맴버에서 자신에게 그룹 엔티티가 연결되었을 때 맴버인 자신의 차수가 어떻게 바뀌어야 하는지 예측하는 함수.
	unsigned int predicted_order = 0;
	unsigned int connected_entity_order = (*entity_ptr).get_cover_order();
	if (cover_order <= connected_entity_order)// 차수 갱신의 상황
	{
		predicted_order = connected_entity_order + 1;
	}
	else
	{
		predicted_order = cover_order;
	}
	return predicted_order;
}

unsigned int Entity::do_predict_cover_order_disconnection(Entity* entity_ptr)// 분리한 경우
{
	// 어떠한 맴버에서 자신의 그룹 엔티티가 분리(뚜껑이 벗겨짐)되었을 때 맴버인 자신의 차수가 어떻게 바뀌어야 하는지 예측하는 함수.
	unsigned int predicted_order = 0;
	list<Entity*>::iterator iter = group_entity_list.begin();
	Entity* group_entity_ptr = nullptr;
	if (group_entity_list.empty() == false)
	{
		group_entity_ptr = (*iter);// 가장 첫 번 째 원소를 고른다
		if (group_entity_ptr == entity_ptr)//자신이 최고차수 이며, 정렬 순서중 첫 번 째 원소인 경우
		{
			iter++;// 두번째 원소 선택
			if (iter != group_entity_list.end())// 리스트 내의 맴버가 1개 뿐인 경우가 있으므로, 두 번 째 반복자가 참조 불가능한 마지막 반복자일 가능성을 점검한다.
			{
				predicted_order = (**iter).get_cover_order() + 1;// 해당 경우의 수가 두번째 원소에 의한 최고차수 유지 경우와, 두번째 요소로의 최고차수 감소의 경우를 모두 내포하여, 분기가 간단해짐.
			}
			else// 리스트의 요소가 자신 뿐인 경우
			{// 분리의 경우가 아닌 감소 변화의 경우라면, 최고차수가 변화한 후의 자신이 되는 경우도 비교해야한다.
				predicted_order = 0;// 분리의 경우
			}
		}
		else//분리되는 entity의 차수가 최고차수가 아닌경우에는 차수를 그대로 유지한다. 자신을 제외한 다른 원소가 최고차수를 가질 것이기 때문.
		{
			predicted_order = cover_order;// 만일 차수가 0 인 오브젝트를 포함한다면, 뚜껑의 차수는 최소 1 이상이다. 
		}
	}
	else// 자신이 이미 최고 높이의 뚜껑인 경우, 더 이상 뚜껑을 벗겨낼 수 없는 경우.
	{
		cout << "맴버에서 더 이상 그룹을 제거할 수 없습니다. list was empty. can not delete group anymore" << endl;
		assert(0);
	}
	return predicted_order;
}

unsigned int Entity::do_predict_cover_order_changing(Entity* outdated_member_ptr, unsigned int order_update)
{
	// 어떠한 그룹에서 자신의 맴버 엔티티에서 그룹 차수가 변경될 시 자신의 차수가 어떻게 바뀌어야 하는지 예측하는 함수.
	// 맴버가 추가 제거 소멸한 경우(차수전파에 의한 변화가 아닌 직접 연산에 의한 변화)는  원소 자신의 포인터와 변하고 난 후의 차수를 entity_ptr, order_update에 대입하면 된다.
	// 기본적으로 do_predict_group_order 중에서 연결, 분리의 경우를 참고하여 제작한 함수이다.

	unsigned int predicted_order = 0;
	unsigned int next_highest_member_order = 0;// 아직 정해지지 않음..
	unsigned int outdated_member_order = (*outdated_member_ptr).get_cover_order();
	list<Entity*>::iterator member_entity_iter;// 초기화를 굳이 하지 않는다. 초기화 함수를 부르는 만큼의 불필요한 컴퓨팅 자원이 소모되기 때문
	Entity* member_entity_ptr = nullptr;
	if (outdated_member_order == order_update)// 맴버의 그룹차수에 변화가 없는 경우. 전파를 중단해야하는 경우를 판단하기 위해 필요한 분기
	{
		predicted_order = cover_order;// 맴버의 그룹차수에 변화가 없다면, 자신의 그룹차수에도 변화가 없다.
	}
	else
	{
		if (outdated_member_order < order_update)
		{
			if (cover_order <= order_update)
			{
				predicted_order = order_update + 1;
			}
			else
			{
				predicted_order = cover_order;
			}
		}
		else
		// == 등호의 경우의 수는 제외되었으므로
		// (*entity_ptr).get_group_order() > order_update 조건에 해당하는 분기.
		{
			member_entity_iter = group_entity_list.begin();
			if (group_entity_list.empty() == false)
			{
				member_entity_ptr = (*member_entity_iter);// 가장 첫 번 째 원소를 고른다
				// 정렬에 의해 최소차수 맴버가 가장 앞에 배치된 맴버라는 정보가 암묵적으로 포함되어 있어, 
				// 최고차수의 값을 알아낸 후 직접 비교할 필요가 없어졌고, 그에 따라 분기의 수를 줄여준다.
				if (member_entity_ptr == outdated_member_ptr)// 정렬 순서중 첫 번 째 원소인 경우 자신이 최고차수라는 정보를 암묵적으로 포함하고 있음
				{
					member_entity_iter++;// 두번째 원소 선택
					if (member_entity_iter != group_entity_list.end())// 리스트 내의 맴버가 1개 뿐인 경우가 있으므로, 두 번 째 반복자가 참조 불가능한 마지막 반복자일 가능성을 점검한다.
					{
						next_highest_member_order = (**member_entity_iter).get_cover_order();// 두번째 원소의 차수 정보를 알고 있어야 한다. 두 번 째 원소는 최고차수의 후보이기 때문
						if (next_highest_member_order <= order_update)
						{
							predicted_order = order_update + 1;// 최고차수 감소 경우 중 두 번 째로 큰 최고차수로도 차수가 유지되지 않는 경우의 수
						}
						else
						{
							predicted_order = next_highest_member_order + 1;// 해당 경우의 수가 두번째 원소에 의한 최고차수 유지 경우와, 두번째 요소로의 최고차수 감소의 경우를 모두 내포하여, 분기가 간단해짐.
						}
					}
					else// 리스트의 요소가 자신 뿐인 경우
					{// 분리의 경우가 아닌 감소 변화의 경우라면, 최고차수가 변화한 후의 자신이 되는 경우도 비교해야한다.
						predicted_order = order_update + 1;
					}
				}
				else
				//차수감소하는 경우 중 entity의 정렬순서가 맨 앞이 아닌 경우에는 차수를 그대로 유지한다. 
				// 자신이 아닌 다른 원소가 최고차수를 유지하는 중이기 때문이다. 최고차수를 유지해주는 원소의 차수가 바뀌지 않는한, 그룹차수는 변하지 않기 때문
				{
					predicted_order = cover_order;
				}
			}
			else
			{
				cout << "그룹에서 더 이상 원소를 제거할 수 없습니다. list was empty. can not delete element anymore" << endl;
				assert(0);
			}
		}
	}
	return predicted_order;
}

void Entity::do_handle_cover_order_list_sort(Entity* outdated_member_ptr, unsigned int order_update)
{
	// 연결과 분리의 경우는 
	// add, del 함수가 맴버리스트를 잘 정렬해 주지만, 
	// 변화, changing 의 경우에는 처리시에 자신이 직접 리스트를 차수 기준 내림차순으로 정렬해 주어야 한다.

	list<Entity*>::iterator iter = group_entity_list.begin();
	list<Entity*>::iterator iter_end = group_entity_list.end();

	// 리스트에서 기존의 포인터 삭제
	if (iter != iter_end)// 리스트가 비어있지 않은 경우
	{
		while (1)
		{
			if (*iter == outdated_member_ptr)// 삭제할 원소의 포인터를 찾은 경우
			{
				iter = group_entity_list.erase(iter);
				// 삭제 후 다음 원소를 가리킴. (중복인 경우 다음에 존재하는 같은 포인터 또한 삭제)
				// 원천적으로 중복을 허용하지 않으나, 중복인 것을 만에하나 발견하더라도 삭제하도록 함.
			}
			else
			{
				iter++;
			}
			if (iter == iter_end)
			{
				break;
			}
		}
	}

	// 리스트에 포인터를 다시 삽입
	iter = group_entity_list.begin();//반복자 초기화
	while (1)// 그룹 차수가 큰 원소가 맨 앞의 원소가 되도록, 크기를 비교하여 원소를 사이에 추가함.
	{
		if (iter == iter_end)
		{
			group_entity_list.insert(iter, outdated_member_ptr);// 리스트가 비었거나 가장 뒤쪽에 추가하는 경우
			break;
		}
		if (order_update >= (**iter).get_cover_order())// 자신이 삽입될 위치의 바로 뒤 원소의 그룹차수와 자신의 그룹차수를 비교
		{
			group_entity_list.insert(iter, outdated_member_ptr);// 그룹 차수가 큰 원소가 맨 앞의 원소가 되도록, 크기를 비교하여 원소를 사이에 추가함.
			//insert 함수는 인수로 전달되는 반복자의 바로 앞에 원소를 추가하는 함수이다.
			// 모든 원소보다 뒤쪽인 마지막에 원소를 삽입하고 싶다면, 마지막 원소보다 한자리 뒤를 가리키는 list::end() 를 통해 얻은 반복자를 인수에 대입하면 된다.
			break;
		}
		iter++;
	}
}

void Entity::do_propagate_cover_order_changing(unsigned int order_update_of_self)
{
	// 자신의 크기를 바꾸는 역할과 자신의 변화를 상위로 전달하는 역할
	// 하위 멤버의 변화를 감지하는 역할이 아니다. 주동사동 관계를 명확히 하자. 여기서의 행동의 주체는 자신(하위 맴버)이다.
	unsigned int order_update_of_cover = 0;
	if (cover_order != order_update_of_self)// 그룹 차수 변화가 없다면 차수 변화 전파을 중단한다는 의미. 차수 변화가 있을 때만 차수 변화 전파를 수행한다.
	{
		// 만일 차수 변화가 있더라도 최상위 그룹이라면 
		// member_entity_list가 빈 리스트이므로 전파를 중단함.
		for (auto item : member_entity_list)
		{
			order_update_of_cover = (*item).do_predict_cover_order_changing(this, order_update_of_self);// 상위 그룹의 관점에서, 바뀔 맴버, 바뀔 차수
			(*item).do_propagate_cover_order_changing(order_update_of_cover);//상위 그룹이 호출하는 자신의 변화 전파 함수.
			// 전파 완료 후, 상위 그룹의 맴버인 자신의 차수가 변했으므로, 상위 그룹의 맴버 리스트를 자신의 변화한 차수를 반영하여, 차수 내림차순 정렬 수행
			(*item).do_handle_cover_order_list_sort(this, order_update_of_self);// 상위 그룹의 관점에서, 바뀔 맴버, 바뀔 차수
		}
		// 모든 작업 후 자신의 차수 변화를 반영한다.
		// 궁극적으로 차수값을 바꾸어 주는 역할도 do_propagate_group_order_changing 함수에서 수행한다.
		cover_order = order_update_of_self;// 리스트에 직접 접근하지 않아도, 리스트 자체가 주소를 가리키는 포인터 리스트 이므로, 자신의 값을 바꾸면 리스트에서 접근한 자신의 값도 바뀐다.
	}
}

unsigned int Entity::do_predict_cover_order_connection_check_inclusion(Entity* entity_ptr)
{
	unsigned int result = 0;
	if (do_check_group_affiliation(entity_ptr) == false)// 중복 요소가 없을 때 실행
	{
		result = do_predict_cover_order_connection(entity_ptr);
	}
	else
	{
		cout << "목록에 요소가 이미 존재합니다. The element is already exist in the list" << endl;
		cout << "중복 시행은 무시합니다." << endl;
	}
	return result;
}

unsigned int Entity::do_predict_cover_order_disconnection_check_inclusion(Entity* entity_ptr)
{
	unsigned int result = 0;
	if (do_check_group_affiliation(entity_ptr) == true)// 찾는 요소가 있을 때 실행
	{
		result = do_predict_cover_order_disconnection(entity_ptr);
	}
	else
	{
		cout << "찾는 요소가 없습니다. There's no element you're looking for." << endl;
		assert(0);
	}
	return result;
}

unsigned int Entity::do_predict_cover_order_changing_check_inclusion(Entity* outdated_member_ptr, unsigned int order_update)
{
	unsigned int result = 0;
	if (do_check_group_affiliation(outdated_member_ptr) == true)// 찾는 요소가 있을 때 실행
	{
		result = do_predict_cover_order_changing(outdated_member_ptr, order_update);
	}
	else
	{
		cout << "찾는 요소가 없습니다. There's no element you're looking for." << endl;
		assert(0);
	}
	return result;
}

bool Entity::do_handle_cover_order_list_sort_check_inclusion(Entity* outdated_member_ptr, unsigned int order_update)
{
	bool discriminant = false;
	if (do_check_group_affiliation(outdated_member_ptr) == true)// 찾는 요소가 있을 때 실행
	{
		do_handle_cover_order_list_sort(outdated_member_ptr, order_update);
		discriminant = true;
	}
	else
	{
		cout << "찾는 요소가 없습니다. There's no element you're looking for." << endl;
		assert(0);
	}
	return discriminant;
}

void Entity::do_propagate_cover_order_changing_check_inclusion(unsigned int order_update_of_self)
{
	// 자신의 크기를 바꾸는 역할과 자신의 변화를 상위로 전달하는 역할
	// 하위 멤버의 변화를 감지하는 역할이 아니다. 주동사동 관계를 명확히 하자. 여기서의 행동의 주체는 자신(하위 맴버)이다.
	unsigned int order_update_of_cover = 0;
	if (cover_order != order_update_of_self)// 그룹 차수 변화가 없다면 차수 변화 전파을 중단한다는 의미. 차수 변화가 있을 때만 차수 변화 전파를 수행한다.
	{
		// 만일 차수 변화가 있더라도 최상위 그룹이라면 
		// member_entity_list가 빈 리스트이므로 전파를 중단함.
		for (auto item : member_entity_list)
		{
			order_update_of_cover = (*item).do_predict_cover_order_changing_check_inclusion(this, order_update_of_self);// 상위 그룹의 관점에서, 바뀔 맴버, 바뀔 차수
			(*item).do_propagate_cover_order_changing_check_inclusion(order_update_of_cover);//상위 그룹이 호출하는 자신의 변화 전파 함수.
			// 전파 완료 후, 상위 그룹의 맴버인 자신의 차수가 변했으므로, 상위 그룹의 맴버 리스트를 자신의 변화한 차수를 반영하여, 차수 내림차순 정렬 수행
			(*item).do_handle_cover_order_list_sort_check_inclusion(this, order_update_of_self);// 상위 그룹의 관점에서, 바뀔 맴버, 바뀔 차수
		}
		// 모든 작업 후 자신의 차수 변화를 반영한다.
		// 궁극적으로 차수값을 바꾸어 주는 역할도 do_propagate_group_order_changing 함수에서 수행한다.
		cover_order = order_update_of_self;// 리스트에 직접 접근하지 않아도, 리스트 자체가 주소를 가리키는 포인터 리스트 이므로, 자신의 값을 바꾸면 리스트에서 접근한 자신의 값도 바뀐다.
	}
}

//=========<inner class Entity>=========