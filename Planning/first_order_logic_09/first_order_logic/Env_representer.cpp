#include "Env_representer.hpp"
#include "Pad.hpp"

#include <iostream>
#include <deque>
#include <cassert>

using namespace std;

unsigned int Env_representation::num_of_predicates = 0;

Env_representation::~Env_representation()
{
	do_flush_entity_list();
}

//void Env_representation::do_add_word(char* char_str)
//{
//	string str(char_str);
//	str2int.insert({ str, num_of_predicates });
//	int2str.insert({ num_of_predicates, str });
//	num_of_predicates += 1;
//}

void Env_representation::do_add_word(string str)
{
	str2int.insert({ str, num_of_predicates });
	int2str.insert({ num_of_predicates, str });
	num_of_predicates += 1;
}

bool Env_representation::do_add_word_check_inclusion(string str)
{
	bool discriminant = false;
	if (str2int.count(str) == 0)
	{
		do_add_word(str);
		discriminant = true;
	}
	cout << "debug num_of_predicates : " << num_of_predicates << endl;
	return discriminant;
}

string* Env_representation::get_pred_int_to_string_ptr(int pred_id)
{
	string* result_ptr = nullptr;
	unordered_map<unsigned int, string>::iterator iter = int2str.find(pred_id);
	if (iter != int2str.end())
	{
		result_ptr = &(*iter).second;
	}
	return result_ptr;
}

unsigned int* Env_representation::get_pred_string_to_int_ptr(string pred)
{
	unsigned int* result_ptr = nullptr;
	unordered_map<string, unsigned int>::iterator iter = str2int.find(pred);
	if (iter != str2int.end())
	{
		result_ptr = &(*iter).second;
	}
	return result_ptr;
}

void Env_representation::do_show_entity_info(unsigned int entity_ID)
{
	Entity* entity_ptr = get_entity_ptr(entity_ID);
	cout << "---entity_ID : " << entity_ID << "---" << endl;
	cout << "entity_type : ";
	switch ((*entity_ptr).get_type())
	{
	case 1:
		cout << "1 - object" << endl;
		break;
	case 2:
		cout << "2 - group" << endl;
		break;
	}
	cout << "group order : " << (*entity_ptr).get_group_order() << endl;
	cout << "cover order : " << (*entity_ptr).get_cover_order() << endl;
	cout << "entity size : " << (*entity_ptr).get_entity_size() << endl;
}

unsigned int Env_representation::do_add_object()
{
	Object* Entity_ptr = new(Object);
	// list 에 객체를 전달할때 객체 자체를 넘기며 객체가 복사됨을 방지하기 위해 
	// 객체 자체는 동적할당을 하고 객체의 포인터만 전달함.
	entity_list.push_back(Entity_ptr);
	object_list.push_back(Entity_ptr);
	return (*Entity_ptr).get_entity_ID();
}

unsigned int Env_representation::do_add_group()
{
	Group* Entity_ptr = new(Group);
	// list 에 객체를 전달할때 객체 자체를 넘기며 객체가 복사됨을 방지하기 위해 
	// 객체 자체는 동적할당을 하고 객체의 포인터만 전달함.
	entity_list.push_back(Entity_ptr);
	group_list.push_back(Entity_ptr);
	return (*Entity_ptr).get_entity_ID();
}

void Env_representation::do_del_object(unsigned int object_ID)
{
	list<Object*>::iterator iter = object_list.begin();
	list<Object*>::iterator iter_end = object_list.end();

	for (; iter != iter_end; iter++)
	{
		if ((**iter).get_object_ID() == object_ID)
		{
			do_del_entity((**iter).get_entity_ID());
			break;
		}
	}
}

void Env_representation::do_del_group(unsigned int group_ID)
{
	list<Group*>::iterator iter = group_list.begin();
	list<Group*>::iterator iter_end = group_list.end();

	for (; iter != iter_end; iter++)
	{
		if ((**iter).get_group_ID() == group_ID)
		{
			do_del_entity((**iter).get_entity_ID());
			break;
		}
	}
}

void Env_representation::do_del_entity(unsigned int entity_ID)
{
	list<Entity*>::iterator iter_entity;
	list<Entity*>::iterator iter_end_entity;
	list<Object*>::iterator iter_object;
	list<Object*>::iterator iter_end_object;
	list<Group*>::iterator iter_group;
	list<Group*>::iterator iter_end_group;

	iter_entity = entity_list.begin();
	iter_end_entity = entity_list.end();
	unsigned int ID = 0;
	Entity* entity_ptr = nullptr;
	while (1)
	{
		if ((**iter_entity).get_entity_ID() == entity_ID)
		{
			entity_ptr = *iter_entity;
			switch ((*entity_ptr).get_type())
			{
			case 1:// 다운 캐스팅은 타입이 정확히 대입되지 못하면 에러를 유발 할 수 있으므로 조심해서 사용.
				iter_object = object_list.begin();
				iter_end_object = object_list.end();
				ID = (*((Object*)entity_ptr)).get_object_ID();
				while (1)
				{
					if ((**iter_object).get_object_ID() == ID)
					{
						if (*iter_object != nullptr)
						{
							delete(*iter_object);
							*iter_object = nullptr;
							entity_ptr = nullptr;// 중복 없이 잘 관리된 리스트에서 entity_ptr == iter_object 이기 때문
							if (entity_ptr == *iter_object)
							{
								cout << "entity_ptr == *iter_object" << endl;
							}
						}
						iter_object = object_list.erase(iter_object);
					}
					else
					{
						iter_object++;
					}
					if (iter_object == iter_end_object)
					{
						break;
					}
				}
				break;
			case 2:
				iter_group = group_list.begin();
				iter_end_group = group_list.end();
				ID = (*((Group*)entity_ptr)).get_group_ID();
				while (1)
				{
					if ((**iter_group).get_group_ID() == ID)
					{
						if (*iter_group != nullptr)
						{
							delete(*iter_group);
							*iter_group = nullptr;
							entity_ptr = nullptr;// 중복 없이 잘 관리된 리스트에서 entity_ptr == iter_object 이기 때문
							if (entity_ptr == *iter_group)
							{
								cout << "entity_ptr == *iter_group" << endl;
							}
						}
						iter_group = group_list.erase(iter_group);
					}
					else
					{
						iter_group++;
					}
					if (iter_group == iter_end_group)
					{
						break;
					}
				}
				break;
			}
			if (entity_ptr != nullptr)
			{
				delete(entity_ptr);
				entity_ptr = nullptr;
			}
			iter_entity = entity_list.erase(iter_entity);//ID 및 포인터는 고유하므로 삭제 이후로는 리스트의 뒷부분을 탐색할 필요가 없으므로 알고리즘이 간단하다.]
		}
		else
		{
			iter_entity++;
		}
		if (iter_entity == iter_end_entity)
		{
			break;
		}
	}
}

void Env_representation::do_flush_entity_list()
{	
	//Entity 의 공간을 동적할당하여 사용하므로 객체의 소멸시 동적할당된 공간을 다시 반납해야한다.
	list<Entity*>::iterator iter = entity_list.begin();
	list<Entity*>::iterator iter_end = entity_list.end();

	if (iter != iter_end)
	{
		while (1)
		{
			delete(*iter);
			iter = entity_list.erase(iter);// 삭제 후 다음 객체를 가리키므로 iter++의 작업을 이미 내포하고 있음. // 소멸하는 상황이라면, 리스트는 메모리 유출 없이 스스로 소멸하므로 포인터만 삭제하고, 리스트의 원소까지 삭제할 필요는 없다.
			//iter++;
			if (iter == iter_end)
			{
				break;
			}
		}
	}
}

Object* Env_representation::get_object_ptr(unsigned int object_ID)
{
	list<Object*>::iterator iter = object_list.begin();
	list<Object*>::iterator iter_end = object_list.end();

	Object* item = nullptr;

	for (; iter != iter_end; iter++)
	{
		if ((**iter).get_object_ID() == object_ID)
		{
			item = *iter;
			break;
		}
	}
	if (item == nullptr)
	{
		cout << "there is no entity that matchs the condition" << endl;
		assert(0);
	}
	return item;// 참조 과정 중에서 shallow copy가 일어나서 변수조작에 방해가 되지 않는지 잘 살펴보기
}

Group* Env_representation::get_group_ptr(unsigned int group_ID)
{
	list<Group*>::iterator iter = group_list.begin();
	list<Group*>::iterator iter_end = group_list.end();

	Group* item = nullptr;

	for (; iter != iter_end; iter++)
	{
		if ((**iter).get_group_ID() == group_ID)
		{
			item = *iter;
			break;
		}
	}
	if (item == nullptr)
	{
		cout << "there is no entity that matchs the condition" << endl;
		assert(0);
	}
	return item;// 참조 과정 중에서 shallow copy가 일어나서 변수조작에 방해가 되지 않는지 잘 살펴보기
}

Entity* Env_representation::get_entity_ptr(unsigned int entity_ID)
{
	list<Entity*>::iterator iter = entity_list.begin();
	list<Entity*>::iterator iter_end = entity_list.end();
	
	Entity* item = nullptr;
	
	for (; iter != iter_end; iter++)
	{
		if ((**iter).get_entity_ID() == entity_ID)
		{
			item = *iter;
			break;
		}
	}
	if (item == nullptr)
	{
		cout << "there is no entity that matchs the condition" << endl;
		assert(0);
	}
	return item;// 참조 과정 중에서 shallow copy가 일어나서 변수조작에 방해가 되지 않는지 잘 살펴보기
}

//주변 접근 함수
unsigned int Env_representation::get_group_list_length(unsigned int entity_ID)
{
	unsigned int list_length = 0;
	Entity* entity_ptr = get_entity_ptr(entity_ID);
	if (entity_ptr != nullptr)
	{
		list_length = (*entity_ptr).get_group_entity_list_length();
	}
	else
	{
		cout << "entity_ptr != nullptr" << endl;
		assert(0);
	}
	return list_length;
}

unsigned int Env_representation::get_group_entity_id(unsigned int entity_ID, unsigned int index)
{
	unsigned int entity_id = 0;
	Entity* entity_ptr = get_entity_ptr(entity_ID);
	Entity* group_ptr = (*entity_ptr).get_group_entity_ptr(index);
	if (entity_ptr != nullptr && group_ptr != nullptr)
	{
		entity_id = (*group_ptr).get_entity_ID();
	}
	else
	{
		cout << "entity_ptr != nullptr && member_ptr != nullptr" << endl;
		assert(0);
	}
	return entity_id;
}

unsigned int Env_representation::get_group_group_id(unsigned int entity_ID, unsigned int index)
{
	unsigned int entity_id = 0;
	Entity* entity_ptr = get_entity_ptr(entity_ID);
	Entity* group_ptr = (*entity_ptr).get_group_entity_ptr(index);
	if (entity_ptr != nullptr && group_ptr != nullptr)
	{
		entity_id = (*(Group*)group_ptr).get_group_ID();
	}
	else
	{
		cout << "entity_ptr != nullptr && member_ptr != nullptr" << endl;
		assert(0);
	}
	return entity_id;
}

unsigned int Env_representation::get_member_list_length(unsigned int entity_ID)
{
	unsigned int list_length = 0;
	Entity* entity_ptr = get_entity_ptr(entity_ID);
	if (entity_ptr != nullptr)
	{
		list_length = (*entity_ptr).get_member_entity_list_length();
	}
	else
	{
		cout << "entity_ptr != nullptr" << endl;
		assert(0);
	}
	return list_length;
}

unsigned int Env_representation::get_member_entity_id(unsigned int entity_ID, unsigned int index)
{
	unsigned int entity_id = 0;
	Entity* entity_ptr = get_entity_ptr(entity_ID);
	Entity* member_ptr = (*entity_ptr).get_member_entity_ptr(index);
	if (entity_ptr != nullptr && member_ptr != nullptr)
	{
		entity_id = (*member_ptr).get_entity_ID();
	}
	else
	{
		cout << "entity_ptr != nullptr && member_ptr != nullptr" << endl;
		assert(0);
	}
	return entity_id;
}

bool Env_representation::do_insert_entity_to_group(unsigned int group_ID, unsigned int entity_ID)
{
	Group* group_ptr = get_group_ptr(group_ID);
	Entity* entity_ptr = get_entity_ptr(entity_ID);
	bool discriminant = false;
	if(group_ptr != nullptr && entity_ptr != nullptr)
	{
		discriminant = (*group_ptr).do_add_entity_check_inclusion(entity_ptr);
		cout << "information : group" << endl;
		cout << "group order : " << (*group_ptr).get_group_order() << endl;
		cout << "cover order : " << (*group_ptr).get_cover_order() << endl;
		cout << "entity size : " << (*group_ptr).get_entity_size() << endl;

		cout << "information : member" << endl;
		cout << "group order : " << (*entity_ptr).get_group_order() << endl;
		cout << "cover order : " << (*entity_ptr).get_cover_order() << endl;
		cout << "entity size : " << (*entity_ptr).get_entity_size() << endl;
	}
	return discriminant;
}

bool Env_representation::do_erase_entity_from_group(unsigned int group_ID, unsigned int entity_ID)
{
	Group* group_ptr = get_group_ptr(group_ID);
	Entity* entity_ptr = get_entity_ptr(entity_ID);	
	bool discriminant = false;
	if (group_ptr != nullptr && entity_ptr != nullptr)
	{
		discriminant = (*group_ptr).do_del_entity_check_inclusion(entity_ptr);
		cout << "information : group" << endl;
		cout << "group order : " << (*group_ptr).get_group_order() << endl;
		cout << "cover order : " << (*group_ptr).get_cover_order() << endl;
		cout << "entity size : " << (*group_ptr).get_entity_size() << endl;

		cout << "information : member" << endl;
		cout << "group order : " << (*entity_ptr).get_group_order() << endl;
		cout << "cover order : " << (*entity_ptr).get_cover_order() << endl;
		cout << "entity size : " << (*entity_ptr).get_entity_size() << endl;
	}
	return discriminant;
}

bool Env_representation::do_insert_object_to_group(unsigned int group_ID, unsigned int object_ID)
{
	Group* group_ptr = get_group_ptr(group_ID);
	Object* object_ptr = get_object_ptr(object_ID);
	bool discriminant = false;
	if (group_ptr != nullptr && object_ptr != nullptr)
	{
		discriminant = (*group_ptr).do_add_entity_check_inclusion(object_ptr);
		cout << "information : group" << endl;
		cout << "group order : " << (*group_ptr).get_group_order() << endl;
		cout << "cover order : " << (*group_ptr).get_cover_order() << endl;
		cout << "entity size : " << (*group_ptr).get_entity_size() << endl;

		cout << "information : object" << endl;
		cout << "group order : " << (*object_ptr).get_group_order() << endl;
		cout << "cover order : " << (*object_ptr).get_cover_order() << endl;
		cout << "entity size : " << (*object_ptr).get_entity_size() << endl;
	}
	return discriminant;
}

bool Env_representation::do_erase_object_from_group(unsigned int group_ID, unsigned int object_ID)
{
	Group* group_ptr = get_group_ptr(group_ID);
	Object* object_ptr = get_object_ptr(object_ID);
	bool discriminant = false;
	if (group_ptr != nullptr && object_ptr != nullptr)
	{
		discriminant = (*group_ptr).do_del_entity_check_inclusion(object_ptr);
		cout << "information : group" << endl;
		cout << "group order : " << (*group_ptr).get_group_order() << endl;
		cout << "cover order : " << (*group_ptr).get_cover_order() << endl;
		cout << "entity size : " << (*group_ptr).get_entity_size() << endl;

		cout << "information : object" << endl;
		cout << "group order : " << (*object_ptr).get_group_order() << endl;
		cout << "cover order : " << (*object_ptr).get_cover_order() << endl;
		cout << "entity size : " << (*object_ptr).get_entity_size() << endl;
	}
	return discriminant;
}

bool Env_representation::do_flush_entity_connection(unsigned int entity_ID)
{
	Entity* entity_ptr = get_entity_ptr(entity_ID);
	bool discriminant = false;
	if (entity_ptr != nullptr)
	{
		if ((*entity_ptr).get_type() == 2)
		{
			(*(Group*)entity_ptr).do_flush_entity();
		}
		discriminant = true;
	}
	return discriminant;
}

bool Env_representation::do_flush_group_connection(unsigned int entity_ID)
{
	Entity* entity_ptr = get_entity_ptr(entity_ID);
	bool discriminant = false;
	if (entity_ptr != nullptr)
	{
		(*entity_ptr).do_flush_group();
		discriminant = true;
	}
	return discriminant;
}

void Env_representation::do_access_entity_add_pred_id(unsigned int entity_ID, unsigned int predicate_ID, bool nag_sign)
{
	Entity* item = get_entity_ptr(entity_ID);
	(*item).do_add_pred(predicate_ID, nag_sign);
}

void Env_representation::do_access_entity_del_pred_id(unsigned int entity_ID, unsigned int predicate_ID, bool nag_sign)
{
	Entity* item = get_entity_ptr(entity_ID);
	(*item).do_del_pred(predicate_ID, nag_sign);
}

void Env_representation::do_access_entity_add_pred_id_check_inclusion(unsigned int entity_ID, unsigned int predicate_ID, bool nag_sign)
{
	Entity* item = get_entity_ptr(entity_ID);
	(*item).do_add_pred_check_inclusion(predicate_ID, nag_sign);
}

void Env_representation::do_access_entity_del_pred_id_check_inclusion(unsigned int entity_ID, unsigned int predicate_ID, bool nag_sign)
{
	Entity* item = get_entity_ptr(entity_ID);
	(*item).do_del_pred_check_inclusion(predicate_ID, nag_sign);
}

void Env_representation::do_access_entity_add_pred_str_check_inclusion(unsigned int entity_ID, string predicate_str, bool nag_sign)
{
	unsigned int* predicate_ID_ptr = nullptr;
	do_add_word_check_inclusion(predicate_str);
	Entity* item = get_entity_ptr(entity_ID);
	predicate_ID_ptr = get_pred_string_to_int_ptr(predicate_str);
	if (predicate_ID_ptr != nullptr)
	{
		(*item).do_add_pred_check_inclusion(*predicate_ID_ptr, nag_sign);
	}
}

void Env_representation::do_access_entity_del_pred_str_check_inclusion(unsigned int entity_ID, string predicate_str, bool nag_sign)
{
	unsigned int* predicate_ID_ptr = nullptr;
	Entity* item = get_entity_ptr(entity_ID);
	predicate_ID_ptr = get_pred_string_to_int_ptr(predicate_str);
	if (predicate_ID_ptr != nullptr)
	{
		(*item).do_del_pred_check_inclusion(*predicate_ID_ptr, nag_sign);
	}
}


void Env_representation::do_access_entity_flush_pred(unsigned int entity_ID)
{
	Entity* item = get_entity_ptr(entity_ID);
	(*item).do_flush_pred();
}

bool Env_representation::do_access_entity_is_included(unsigned int entity_ID, unsigned int predicate_ID, bool nag_sign)
{
	Entity* item = get_entity_ptr(entity_ID);
	return (*item).is_included(predicate_ID, nag_sign);
}

bool Env_representation::do_access_entity_is_included(unsigned int entity_ID, string predicate_str, bool nag_sign)
{
	unsigned int* predicate_ID_ptr = get_pred_string_to_int_ptr(predicate_str);
	bool discriminant = false;
	if(predicate_ID_ptr != nullptr)
	{
		Entity* item = get_entity_ptr(entity_ID);
		discriminant = (*item).is_included(*predicate_ID_ptr, nag_sign);
	}
	return discriminant;
}

unsigned int Env_representation::get_access_entity_pred_length(unsigned int entity_ID, bool nag_sign)
{
	Entity* entity_ptr = nullptr;
	int length = 0;
	entity_ptr = get_entity_ptr(entity_ID);
	if (entity_ptr != nullptr)
	{
		length = (*entity_ptr).get_pred_list_length(nag_sign);
	}
	return length;
}

unsigned int* Env_representation::get_access_entity_pred_id_ptr(unsigned int entity_ID, unsigned int index, bool nag_sign)
{
	Entity* entity_ptr = nullptr;
	unsigned int* pred_ID = 0;
	entity_ptr = get_entity_ptr(entity_ID);
	if (entity_ptr != nullptr)
	{
		pred_ID = (*entity_ptr).get_pred_id_ptr(index, nag_sign);
	}
	return pred_ID;
}

void Env_representation::do_access_entity_print_all_pred(unsigned int entity_ID)
{
	Entity* item = get_entity_ptr(entity_ID);
	(*item).do_print_all_predicates();
}

unsigned int Env_representation::do_search_entity_includes_pred(unsigned int predicate_ID, bool nag_sign)
{
	unsigned int entity_id = 0;
	for (auto item : entity_list)
	{
		if ((*item).is_included(predicate_ID, nag_sign) == true)
		{
			entity_id = (*item).get_entity_ID();
			break;
		}
	}
	return entity_id;
}

unsigned int Env_representation::do_search_entity_includes_pred(string predicate_str, bool nag_sign)
{
	unsigned int entity_id = 0;
	unsigned int* predicate_id_ptr = 0;
	predicate_id_ptr = get_pred_string_to_int_ptr(predicate_str);
	if (predicate_id_ptr != nullptr)
	{
		for (auto item : entity_list)
		{
			if ((*item).is_included(*predicate_id_ptr, nag_sign) == true)
			{
				entity_id = (*item).get_entity_ID();
				break;
			}
		}
	}
	return entity_id;
}

void Env_representation::do_print_all_entity()
{
	cout << "=====<list>====" << endl;
	for (auto item : entity_list)
	{
		cout << "entity_ID : " << (*item).get_entity_ID() << endl;
		if ((*item).get_type() == 1)//object
		{
			cout << "type : object" << endl;
		}
		if ((*item).get_type() == 2)//group
		{
			cout << "type : group" << endl;
		}
		(*item).do_print_all_predicates();
		cout << "=========" << endl;
	}
	cout << "=====<list>====" << endl;
}


void Env_representation::do_print_all_entity_pred()
{
	cout << "=====<list>====" << endl;
	for (auto entity_ptr : entity_list)
	{
		do_print_info_with_indent(entity_ptr, 0);
		cout << "=========" << endl;
	}
	cout << "=====<list>====" << endl;
}

void Env_representation::do_print_info_with_indent(Entity* entity_ptr, unsigned int indent)
{	// 보여줄 정보
	// 아이디
	// 타입
	// 타입 별 아이디
	// 차수
	// 크기
	// 술어

	unsigned int pred_pos_list_length = 0;
	unsigned int pred_nag_list_length = 0;
	unsigned int pred_id = 0;
	string* pred_str_ptr = nullptr;

	if (indent >= 1)
	{
		cout << pad(' ' , indent) << "<entity_ID : " << (*entity_ptr).get_entity_ID() << ">" << endl;
	}
	else
	{
		cout << "<entity_ID : " << (*entity_ptr).get_entity_ID() << ">" << endl;
	}
	switch ((*entity_ptr).get_type())
	{
	case 1:
		cout << pad(indent) << "<object_ID : " << (*(Object*)entity_ptr).get_object_ID() << ">" << endl;
		cout << pad(indent) << "entity_type : 1 - object" << endl;
		break;
	case 2:
		cout << pad(indent) << "<group_ID  : " << (*(Group*)entity_ptr).get_group_ID() << ">" << endl;
		cout << pad(indent) << "entity_type : 2 - group" << endl;
		break;
	}
	cout << pad(indent) << "group order : " << (*entity_ptr).get_group_order() << endl;
	cout << pad(indent) << "cover order : " << (*entity_ptr).get_cover_order() << endl;
	cout << pad(indent) << "entity size : " << (*entity_ptr).get_entity_size() << endl;

	pred_pos_list_length = (*entity_ptr).get_pred_list_length(0);// 0 : positive
	pred_nag_list_length = (*entity_ptr).get_pred_list_length(1);// 0 : nagative

	cout << pad(indent) << "positive_predicates" << endl;
	if (pred_pos_list_length > 0)
	{
		for (unsigned int i = 0; i < pred_pos_list_length; i++)
		{
			pred_id = *((*entity_ptr).get_pred_id_ptr(i, 0));
			pred_str_ptr = get_pred_int_to_string_ptr(pred_id);
			if (pred_str_ptr != nullptr)
			{
				cout << pad(indent) << " > " << *pred_str_ptr << endl;
			}
		}
	}
	else
	{
		cout << pad(indent) << " > -Null-" << endl;
	}
	cout << pad(indent) << "negative_predicates" << endl;
	if (pred_nag_list_length > 0)
	{
		for (unsigned int i = 0; i < pred_nag_list_length; i++)
		{
			pred_id = *((*entity_ptr).get_pred_id_ptr(i, 0));
			pred_str_ptr = get_pred_int_to_string_ptr(pred_id);
			if (pred_str_ptr != nullptr)
			{
				cout << pad(indent) << " > " << *pred_str_ptr << endl;
			}
		}
	}
	else
	{
		cout << pad(indent) << " > -Null-" << endl;
	}
}

//void Env_representation::do_show_entity_info(unsigned int entity_ID)
//{
//	Entity* entity_ptr = get_entity_ptr(entity_ID);
//	cout << "---entity_ID : " << entity_ID << "---" << endl;
//	cout << "entity_type : ";
//	switch ((*entity_ptr).get_type())
//	{
//	case 1:
//		cout << "1 - object" << endl;
//		break;
//	case 2:
//		cout << "2 - group" << endl;
//		break;
//	}
//	cout << "group order : " << (*entity_ptr).get_group_order() << endl;
//	cout << "cover order : " << (*entity_ptr).get_cover_order() << endl;
//	cout << "entity size : " << (*entity_ptr).get_entity_size() << endl;
//}

void Env_representation::do_print_tree_entity(Entity* entity_ptr)
{
	do_print_tree_entity_recursion(entity_ptr, 0);
}

void Env_representation::do_print_tree_entity_recursion(Entity* entity_ptr, unsigned int depth)
{
	unsigned int member_list_len = 0;
	unsigned int index = 0;
	unsigned int cover_order = 0;
	Entity* member_entity_ptr = nullptr;
	// 깊이 우선 탐색 방법으로 요소를 나열하면 Tree 로 나타낼 수 있다.
	member_list_len = (*entity_ptr).get_member_entity_list_length();
	cover_order = (*entity_ptr).get_cover_order();
	do_print_info_with_indent(entity_ptr, depth * 3);

	if (member_list_len > 0)
	{
		while (1)
		{// 재귀문을 활용한 깊이 우선 탐색법
			member_entity_ptr = (*entity_ptr).get_member_entity_ptr(index);
			do_print_tree_entity_recursion(member_entity_ptr, depth+1);
			index++;
			if (index >= member_list_len)
			{
				break;
			}
		}
	}
}
