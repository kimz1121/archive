%{
filename : main_solver_4
date 2021.06.05 22:05
athorizer : sangwoo michael kim
language : matlab R2019b

��ü���� ���α׷��� ����� �̿��Ͽ��� 
��ü �ڵ带 ��ɺ��� ���� ���ȭ �� �� 
Quarter_Car_Class�� �������� ���� �ڵ带 �����Ͽ���.

%}

clc
clear

fprintf("calculating...\n");
Quarter_Car_solver_object = Quarter_Car_Class_try_4();
QCS_obj = Quarter_Car_solver_object;
QCS_obj.plot_all();
fprintf("done!\n");