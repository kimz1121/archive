%{
filename : main_solver_4
date 2021.06.05 22:05
athorizer : sangwoo michael kim
language : matlab R2019b

객체지향 프로그래밍 기법을 이용하여서 
전체 코드를 기능별로 분할 모듈화 한 후 
Quarter_Car_Class로 응집시켜 동작 코드를 정리하였음.

%}

clc
clear

fprintf("calculating...\n");
Quarter_Car_solver_object = Quarter_Car_Class_try_4();
QCS_obj = Quarter_Car_solver_object;
QCS_obj.plot_all();
fprintf("done!\n");