// first_order_logic_env_representation.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

using namespace std;

#include "Env_representer.hpp"

int main()
{
    Env_representation system_0;
    
    cout << "-------create entity-------" << endl;
    unsigned int entity_id_temp;
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "빵", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "빵", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "치즈", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "양파", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "오이", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "버터", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "소금", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "후추", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "도마", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "그릇", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "칼", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "버터칼", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "숟가락", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "주걱", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "로봇", 0);
    //system_0.do_print_all_entity_pred();

    cout << "-------create group-------" << endl;
    entity_id_temp = system_0.do_add_group();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "재료", 0);
    entity_id_temp = system_0.do_add_group();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "도구", 0);
    entity_id_temp = system_0.do_add_group();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "주방", 0);
    //system_0.do_print_all_entity_pred();
      
    system_0.do_insert_entity_to_group(0, 0);
    system_0.do_insert_entity_to_group(0, 1);
    system_0.do_insert_entity_to_group(0, 2);
    system_0.do_insert_entity_to_group(0, 3);
    system_0.do_insert_entity_to_group(0, 4);
    system_0.do_insert_entity_to_group(0, 5);
    system_0.do_insert_entity_to_group(0, 6);
    system_0.do_insert_entity_to_group(0, 7);

    system_0.do_insert_entity_to_group(1, 8);
    system_0.do_insert_entity_to_group(1, 9);
    system_0.do_insert_entity_to_group(1, 10);
    system_0.do_insert_entity_to_group(1, 11);
    system_0.do_insert_entity_to_group(1, 12);
    system_0.do_insert_entity_to_group(1, 13);

    system_0.do_insert_entity_to_group(2, 14);
    system_0.do_insert_entity_to_group(2, 15);
    system_0.do_insert_entity_to_group(2, 16);
    //system_0.do_print_all_entity_pred();

    system_0.do_print_tree_entity(system_0.get_group_ptr(0));
    system_0.do_print_tree_entity(system_0.get_group_ptr(1));
    system_0.do_print_tree_entity(system_0.get_group_ptr(2));

    cout << "-------cook-------" << endl;

    entity_id_temp = system_0.do_add_group();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "재료 자르기 그룹", 0);

    entity_id_temp = system_0.do_add_group();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "칼을 잡은 로봇 그룹", 0);

    entity_id_temp = system_0.do_add_group();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "재료가 올라간 도마 그룹", 0);

    system_0.do_insert_entity_to_group(5, 8);//도마
    system_0.do_insert_entity_to_group(5, 2);//치즈

    system_0.do_access_entity_add_pred_str_check_inclusion(2, "도마에 올라감", 0);
    system_0.do_access_entity_add_pred_str_check_inclusion(8, "재료가 올려짐(Occupied)", 0);

    system_0.do_insert_entity_to_group(4, 14);//로봇
    system_0.do_insert_entity_to_group(4, 10);//칼

    system_0.do_access_entity_add_pred_str_check_inclusion(14, "칼을 사용중", 0);
    system_0.do_access_entity_add_pred_str_check_inclusion(10, "로봇이 사용중(Occupied)", 0);

    system_0.do_insert_entity_to_group(3, 19);//칼을 잡은 로봇
    system_0.do_insert_entity_to_group(3, 20);//재료가 올라간 도마 

    system_0.do_access_entity_add_pred_str_check_inclusion(19, "재료가 올라간 도마를 사용중", 0);
    system_0.do_access_entity_add_pred_str_check_inclusion(20, "로봇이 사용중(Occupied)", 0);

    system_0.do_print_tree_entity(system_0.get_group_ptr(3));

    system_0.do_access_entity_del_pred_str_check_inclusion(2, "도마에 올라감", 0);
    system_0.do_access_entity_del_pred_str_check_inclusion(8, "재료가 올려짐(Occupied)", 0);
    system_0.do_access_entity_del_pred_str_check_inclusion(14, "칼을 사용중", 0);
    system_0.do_access_entity_del_pred_str_check_inclusion(10, "로봇이 사용중(Occupied)", 0);
    system_0.do_access_entity_del_pred_str_check_inclusion(19, "재료가 올라간 도마를 사용중", 0);
    system_0.do_access_entity_del_pred_str_check_inclusion(20, "로봇이 사용중(Occupied)", 0);

    system_0.do_del_group(3);
    system_0.do_del_group(4);
    system_0.do_del_group(5);



    cout << "-------cook-------" << endl;
    system_0.do_del_object(2);
    system_0.do_del_object(3);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "자른 치즈", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "다진 양파", 0);

    entity_id_temp = system_0.do_add_group();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "재료 섞기 그룹", 0);
    entity_id_temp = system_0.do_add_group();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "주걱을 잡은 로봇 그룹", 0);

    system_0.do_insert_entity_to_group(4, 14);//로봇
    system_0.do_insert_entity_to_group(4, 13);//주걱
    system_0.do_access_entity_add_pred_str_check_inclusion(14, "주걱을 사용중", 0);
    system_0.do_access_entity_add_pred_str_check_inclusion(13, "로봇이 사용중(Occupied)", 0);

    entity_id_temp = system_0.do_add_group();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "재료가 담긴 그릇 그룹", 0);

    system_0.do_insert_entity_to_group(5, 9);//그릇
    system_0.do_insert_entity_to_group(5, 18);//자른 치즈
    system_0.do_insert_entity_to_group(5, 19);//다진 양파
    system_0.do_insert_entity_to_group(5, 6);//소금
    system_0.do_insert_entity_to_group(5, 7);//후추

    system_0.do_access_entity_add_pred_str_check_inclusion(9, "재료가 4가지 담김", 0);
    system_0.do_access_entity_add_pred_str_check_inclusion(18, "그릇 담김", 0);
    system_0.do_access_entity_add_pred_str_check_inclusion(19, "그릇 담김", 0);
    system_0.do_access_entity_add_pred_str_check_inclusion(6, "그릇 담김", 0);
    system_0.do_access_entity_add_pred_str_check_inclusion(7, "그릇 담김", 0);

    system_0.do_access_entity_add_pred_str_check_inclusion(2, "재료가 담긴 그릇을 사용중", 0);
    system_0.do_access_entity_add_pred_str_check_inclusion(3, "로봇이 사용중(Occupied)", 0);

    system_0.do_insert_entity_to_group(3, 2);
    system_0.do_insert_entity_to_group(3, 3);

    system_0.do_print_tree_entity(system_0.get_group_ptr(3));

    system_0.do_access_entity_del_pred_str_check_inclusion(14, "주걱을 사용중", 0);
    system_0.do_access_entity_del_pred_str_check_inclusion(13, "로봇이 사용중(Occupied)", 0);

    system_0.do_del_entity(18);
    system_0.do_del_entity(19);
    system_0.do_del_entity(20);
    system_0.do_del_entity(7);
    system_0.do_del_entity(6);
    system_0.do_del_entity(2);
    system_0.do_del_entity(3);


    cout << "-------cook-------" << endl;
    
    system_0.do_flush_entity_list();

    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "빵", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "빵", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "소스", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "마요네즈", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "자른 오이", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "빵", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "빵", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "소스", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "마요네즈", 0);
    entity_id_temp = system_0.do_add_object();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "자른 오이", 0);
    entity_id_temp = system_0.do_add_group();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "반으로 자른 샌드위치", 0);
    entity_id_temp = system_0.do_add_group();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "반으로 자른 샌드위치", 0);
    entity_id_temp = system_0.do_add_group();
    system_0.do_access_entity_add_pred_str_check_inclusion(entity_id_temp, "반으로 자른 샌드위치 묶음", 0);

    system_0.do_insert_entity_to_group(0, 0);
    system_0.do_insert_entity_to_group(0, 1);
    system_0.do_insert_entity_to_group(0, 2);
    system_0.do_insert_entity_to_group(0, 3);
    system_0.do_insert_entity_to_group(0, 4);
    system_0.do_insert_entity_to_group(1, 0);
    system_0.do_insert_entity_to_group(1, 1);
    system_0.do_insert_entity_to_group(1, 2);
    system_0.do_insert_entity_to_group(1, 3);
    system_0.do_insert_entity_to_group(1, 4);
    system_0.do_insert_entity_to_group(2, 10);
    system_0.do_insert_entity_to_group(2, 11);

    system_0.do_print_tree_entity(system_0.get_group_ptr(2));
    cout << "-------print_tree_entity-------" << endl;

    cout << "-------program_end-------" << endl;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
