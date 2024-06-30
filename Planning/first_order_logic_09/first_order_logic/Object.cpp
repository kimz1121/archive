#include "Object.hpp"

#include <iostream>

using namespace std;

unsigned int Object::total_object_num = 0;// entity의 수는 자연수이므로 unsigned int 를 사용하는 것이 효과적
deque<unsigned int> Object::unused_object_ID_list;// static 변수로 지정하여 전체 Entity 객체 ID 중 사용되지 않는 것을 관리함.

Object::Object()
{
	entity_size = 1;//생성된 object의 크기는 1
	if (unused_object_ID_list.empty() == true)
	{
		object_ID = total_object_num;//객체의 삭제와 추가를 반복할 때는 중간중간에 사용되지 않는 ID 들이 생기므로 이들을 별도의 vector 자료형으로 정리해 두었다가 오래된 순으로 사용하면 좋을 듯
	}
	else
	{
		object_ID = unused_object_ID_list.front();
		unused_object_ID_list.pop_front();
	}
	total_object_num++;
	cout << "Object   added, object_ID : " << object_ID << endl;
}

Object::Object(const Object& obj)
	: Entity(obj)
{
	entity_size = 1;//생성된 object의 크기는 1
	// 복사 생성자라 할지라도, ID 는 복사해오지 않고 고유하게 생성한다. 
	if (unused_object_ID_list.empty() == true)
	{
		object_ID = total_object_num;//객체의 삭제와 추가를 반복할 때는 중간중간에 사용되지 않는 ID 들이 생기므로 이들을 별도의 vector 자료형으로 정리해 두었다가 오래된 순으로 사용하면 좋을 듯
	}
	else
	{
		object_ID = unused_object_ID_list.front();
		unused_object_ID_list.pop_front();
	}

	//for (int item : obj.predicates_pos)
	//{
	//	predicates_pos.push_back(item);
	//}
	//for (int item : obj.predicates_nag)
	//{
	//	predicates_nag.push_back(item);
	//}

	total_object_num++;
	cout << "Object   added, object_ID : " << object_ID << endl;
}

Object::~Object()
{
	unused_object_ID_list.push_back(object_ID);//삭제 작업 후에 반복자를 사용하면 안되므로 미리 값을 복사하고, 그 다음에 반복자가 가리키는 원소를 삭제
	total_object_num--;
	
	if (total_object_num <= 0)
	{
		unused_object_ID_list.clear();
		deque<unsigned int>().swap(unused_object_ID_list);
	}

	cout << "Object deleted, object_ID : " << object_ID << endl;
	//cout << "total_object_num : " << total_object_num << endl;
}

int Object::get_type()
{
	return 1;//1 : object, 2 : group
}

unsigned int Object::get_object_ID()
{
	return object_ID;
}

//void Object::set_group_order(unsigned int order_input)
//{
//	group_order = order_input;
//}