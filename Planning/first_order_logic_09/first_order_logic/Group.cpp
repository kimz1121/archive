#include "Group.hpp"

#include <iostream>

using namespace std;

unsigned int Group::total_group_num = 0;// entity의 수는 자연수이므로 unsigned int 를 사용하는 것이 효과적
deque<unsigned int> Group::unused_group_ID_list;// static 변수로 지정하여 전체 Entity 객체 ID 중 사용되지 않는 것을 관리함.

Group::Group()
{
	entity_size = 0;// 생성된 group의 크기는 0; 실체가 존재하지 않는 담기를 위한 용기라는 의미
	if (unused_group_ID_list.empty() == true)
	{
		group_ID = total_group_num;//객체의 삭제와 추가를 반복할 때는 중간중간에 사용되지 않는 ID 들이 생기므로 이들을 별도의 vector 자료형으로 정리해 두었다가 오래된 순으로 사용하면 좋을 듯
	}
	else
	{
		group_ID = unused_group_ID_list.front();
		unused_group_ID_list.pop_front();
	}
	total_group_num++;
	cout << "Group    added, group_ID  : " << group_ID << endl;
}

Group::Group(const Group& obj)
	: Entity(obj)
{
	entity_size = 0;// 생성된 group의 크기는 0; 실체가 존재하지 않는 담기를 위한 용기라는 의미
	// 복사 생성자라 할지라도, ID 는 복사해오지 않고 고유하게 생성한다. 
	if (unused_group_ID_list.empty() == true)
	{
		group_ID = total_group_num;//객체의 삭제와 추가를 반복할 때는 중간중간에 사용되지 않는 ID 들이 생기므로 이들을 별도의 vector 자료형으로 정리해 두었다가 오래된 순으로 사용하면 좋을 듯
	}
	else
	{
		group_ID = unused_group_ID_list.front();
		unused_group_ID_list.pop_front();
	}

	total_group_num++;
	cout << "Group    added, group_ID  : " << group_ID << endl;
}

Group::~Group()
{
	// 그룹의 구성원이었던 개체들에게 해당 그룹이 삭제되었다는 의미로 
	// 구성원들 각각의 affiliated_group_list 에서 자신의 포인터를 삭제해야함.
	// -> 이 부분은 ~Entity에서 자동으로 해결

	// 그룹의 고유 ID 관리만 잘하면 된다.
	unused_group_ID_list.push_back(group_ID);//삭제 작업 후에 반복자를 사용하면 안되므로 미리 값을 복사하고, 그 다음에 반복자가 가리키는 원소를 삭제
	total_group_num--;

	if (total_group_num <= 0)
	{
		unused_group_ID_list.clear();
		deque<unsigned int>().swap(unused_group_ID_list);
	}

	cout << "Group  deleted, group_ID  : " << group_ID << endl;
	//cout << "total_group_num : " << total_group_num << endl;
}

int Group::get_type()
{
	return 2;//1 : object, 2 : group
}

unsigned int Group::get_group_ID()
{
	return group_ID;
}

void Group::do_add_entity(Entity* entity_ptr)
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

void Group::do_del_entity(Entity* entity_ptr)
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
	long size_of_change = -1 * (long)(*entity_ptr).get_entity_size();// 추가한 객체의 크기 만큼 객체의 크기 증가.
	do_change_size(size_of_change);
	// 상위 그룹 크기 변화 요청
	do_propagate_change_size(size_of_change);
}

bool Group::do_add_entity_check_inclusion(Entity* entity_ptr)
{
	bool discriminant = false;
	if (do_check_entity_inclusion(entity_ptr) == false)
	{
		do_add_entity(entity_ptr);
		discriminant = true;
	}
	return discriminant;
}

bool Group::do_del_entity_check_inclusion(Entity* entity_ptr)
{
	bool discriminant = false;
	if (do_check_entity_inclusion(entity_ptr) == true)
	{
		do_del_entity(entity_ptr);
		discriminant = true;
	}
	return discriminant;//discriminant
}

void Group::do_flush_entity()
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

bool Group::do_check_entity_inclusion(Entity* entity_ptr)
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