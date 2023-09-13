function [x, x_d, x_dd] = poly5(x_0, x_1, t_0, t_1, t_gap)
%poly5 Summary of this function goes here
%   x_0 : 3*6 행렬
%   x_1 : 3*6 행렬
%   1행 : 위치
%   2행 : 속도
%   3행 : 가속도
%   n열 : n 번 조인트의 위치, 속도, 가속도

%5차 다항식 풀이를 위한 상수 정의
%식의 차수가 변화하지 않는한 아래의 상수는 일정.
order = [0 1 2 3 4 5;
    0 0 1 2 3 4;
    0 0 0 1 2 3;
    0 0 0 0 1 2;
    0 0 0 0 0 1;
    0 0 0 0 0 0;
    ];

diff_factor_0 = [
    1 1 1 1 1  1;
    0 1 2 3 4  5;
    0 0 2 6 12 20;
    0 0 0 6 24 60;
    0 0 0 0 24 120;
    0 0 0 0 0  120;
    ];

%방정식 풀이를 위한 중간 유도변수

t_mat_0 = t_0.^order;
t_mat_1 = t_1.^order;

system_0 = diff_factor_0.*t_mat_0;%t_0 일 때 계수 방정식
system_1 = diff_factor_0.*t_mat_1;%t_1 일 때 계수 방정식

%두 계수 방정 식 system_0와 system_1 의 모든 부분이 필요하지 않고,
%가속도 조건 까지만 주어지는 경우 계수 방정식의 2계도함수 항 까지만 상수항이 존재하므로,
%상수항이 정의된 0~2계 도함수의 행들만 방정식 풀이에 이용할 수 있다.
system_combination = [system_0(1:3, :);system_1(1:3, :)]%t_0의 계수 방정식 행렬과 t_1의 계수 방정식 행렬을 조합하는 부분.
const_mat = [x_0;x_1];%주어진 조건을 토대로 얻어진 상수행렬

%연립방정식 행렬풀이 x = a\b;
x_vect = system_combination\const_mat;

%시간벡터
t_input = (t_0:t_gap:t_1)';

x = (t_input.^order(1, :)).*diff_factor_0(1, :)*x_vect;
x_d = (t_input.^order(2, :)).*diff_factor_0(2, :)*x_vect;
x_dd = (t_input.^order(3, :)).*diff_factor_0(3, :)*x_vect;

% hold on
% plot(t_input, x);
% plot(t_input, x_d);
% plot(t_input, x_dd);
% hold off

% 주요 행렬의 형태 예시
% t_0 = 0, t_1 = 5 일 때
% system_combination =
%
%            1           0           0           0           0           0
%            0           1           0           0           0           0
%            0           0           2           0           0           0
%            1           5          25         125         625        3125
%            0           1          10          75         500        3125
%            0           0           2          30         300        2500
end