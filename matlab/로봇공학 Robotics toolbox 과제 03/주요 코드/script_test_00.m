close all
clear
clc

x_0 = [ 0,  90, 90,  0,  0,  0;
    0,  0,  0,  0,  0,  0;
    0,  0,  0,  0,  0,  0
    ];
% -89.9650  -50.7913  -48.8711   97.8688  149.0435 -105.3287
% x_1 = [90, -60, -45, -60, 36, 30;
%     0,  0,  0,  0,  0,  0;
%     0,  0,  0,  0,  0,  0
%     ];

x_1 = [90, -15, 0, -60, 36, 30;
    0,  0,  0,  0,  0,  0;
    0,  0,  0,  0,  0,  0
    ];

t_0 = 0;
t_1 = 100;
t_gap = 1;

x = x_0;

L(1) = Link([0, 0.815, 0.350, pi/2]);
L(2) = Link([0, 0, 0.850, 0]);
L(3) = Link([0, 0, 0.145, pi/2]);
L(4) = Link([0, 0.820, 0, -pi/2]);
L(5) = Link([0, 0, 0, pi/2]);
L(6) = Link([0, 0.170, 0, 0]);

%             L(1) = Link([0, 0.815, 0.350, pi/2]);
%             L(2) = Link([0, 0, 0.850, 0]);
%             L(3) = Link([0, 0, 0.145, -pi/2]);
%             L(4) = Link([0, 0.820, 0, pi/2]);
%             L(5) = Link([0, 0, 0, -pi/2]);
%             L(6) = Link([0, 0.170, 0, 0]);

robot = SerialLink(L);
robot.name = 'KUKA';
robot


SE_0 = robot.fkine(x_0(1, :)*(pi/180))
SE_1 = robot.fkine(x_1(1, :)*(pi/180))

T_0(1:3, 1) = SE_0.n;
T_0(1:3, 2) = SE_0.o;
T_0(1:3, 3) = SE_0.a;
T_0(1:3, 4) = SE_0.t;
T_0(4, 1:4) = [0, 0, 0, 1];

T_1(1:3, 1) = SE_1.n;
T_1(1:3, 2) = SE_1.o;
T_1(1:3, 3) = SE_1.a;
T_1(1:3, 4) = SE_1.t;
T_1(4, 1:4) = [0, 0, 0, 1];

t = (t_0:t_gap:t_1)';
time_domain = (ones(101, 4, 4)).*t;
T_delta = reshape(T_1 - T_0, 1, 4, 4);
T_tensor = (ones(101, 4, 4)).*reshape(T_0, 1, 4, 4) + time_domain.*T_delta;



% <point to point trajectory planning>----------
T_instance = @(n)reshape(T_tensor(n, :, :), 4, 4);

[j, j_d, j_dd] = poly5(x_0*(pi/180), x_1*(pi/180), t_0, t_1, t_gap);

% <point to point trajectory planning>----------
% <plot robot motion>-----------------------


figure(1);
title("robotics tool box");

shape = size(j)
length = shape(1, 1)
T_end = zeros(length, 4, 4);
n=1;
while(n <= length)
    robot.plot(j(n, :), 'jaxes');
    T = robot.fkine(j(n, :));
    T_end(n , :, :) = [T.n, T.o, T.a, T.t; 0, 0, 0, 1]; 
    n = n+1;
end
figure(2);
tiledlayout(3, 1);

title("joint domain");
nexttile;
plot(j);
ylabel("Angle(rad)");
xlabel("Time(s)");
nexttile;
plot(j_d);
ylabel("speed(rad/s)");
xlabel("Time(s)");
nexttile;
plot(j_dd);
ylabel("acceleration(rad/s^2)");
xlabel("Time(s)");


%--- 중간 계산

roll = atan2(T_end(:, 3, 2), T_end(:, 3, 3));
pitch = asin(-T_end(:, 3, 1));
yaw = atan2(T_end(:, 1, 1), T_end(:, 2, 1));

P_x = T_end(:, 1, 4);
P_y = T_end(:, 2, 4);
P_z = T_end(:, 3, 4);

%--- 중간 계산

figure(3);
title("end-effector(terminal) domain");
tiledlayout(1, 2);
nexttile;
plot3(roll, pitch, yaw);
xlabel("Roll(rad)");
ylabel("Pitch(rad)");
zlabel("Yaw(rad)");

nexttile;
plot3(P_x, P_y, P_z);
xlabel("x(m)");
ylabel("y(m)");
zlabel("z(m)");

% <plot robot motion>-----------------------


