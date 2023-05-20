%{
filename : Quarter_Car_Class_try_4
date 2021.06.05 22:23
athorizer : sangwoo michael kim
language : matlab R2019b
%}
classdef Quarter_Car_Class_try_4
    properties
        m1 = 49.8;
        m2 = 466.5;
        
        k1 = 135000;
        k2 = 5700;
        
        c1 = 1400;
        c2 = 0;%it is going to be input by propertie set method
        
        velocity_kmh = 0
        velocity_ms = 0
        length_L = 5.2
        height_Y = 0.1
        
        t_range = [0 15]; % 시뮬레이션 시간 범위
        initial = [0 0 0 0]; % 변수 초기값
        
        result_t = 0;
        result_x = 0; 
    end
    methods
        function dx = Quarter_Car(obj, t, x)
            % Y = half sign signal Part ? if 문 포함 %   
            [y, dy] = obj.input_displacement(t);
            dx = zeros(4, 1);
            dx(1) = x(2);
            dx(3) = x(4);
            dx(2) =(obj.k1*y+obj.c1*dy-(obj.k1+obj.k2).*x(1)-(obj.c1+obj.c2).*x(2)+obj.k2.*x(3)+obj.c2.*x(4))/obj.m1;
            dx(4) =(-obj.c2.*x(4)+obj.c2.*x(2)-obj.k2.*x(3)+obj.k2.*x(1))/obj.m2;
        end
        function [y, dy] = input_displacement(obj, t)
            T = obj.length_L/obj.velocity_ms;
            w = pi*obj.velocity_ms/obj.length_L;
            y = 0;
            dy = 0;
            if(0 <= t)
                if(t <= T)
                    y = obj.height_Y*sin(w*t);
                    dy = w*obj.height_Y*cos(w*t);
                end
            end
        end
        function obj = solve_ode(obj)
            [t, x] = ode45(@obj.Quarter_Car, obj.t_range, obj.initial); % ODE45 function
            obj.result_t = t;
            obj.result_x = x(:,3);%바퀴가 아닌 차량만의 변위 만을 확인하고자 함
            %위 같은 처리를 하지 않았을 때 x가 벡터의 집합으로 처리되어 최대 최소 값을 구하면 각각원 소별로 4개의 최대 최소값이 발생하였다. 
        end
        
        function [maximun, minimum] = maxmin_value(obj)
            maximun = max(obj.result_x);
            minimum = min(obj.result_x);
        end
        
        function plot_result(obj)
            plot(obj.result_t, obj.result_x);
        end
%         function set_properties_2(obj, c2_input, velocity)
%             c2 = c2_input;
%             velocity_kmh = velocity;
%             velocity_ms = velocity*(1000/3600); 
%         end
        function obj = set_properties(obj, c2_input, velocity)
            obj.c2 = c2_input;
            obj.velocity_kmh = velocity;
            obj.velocity_ms = velocity*(1000/3600); 
        end
        function get_result_and_plot(obj)
            obj = obj.solve_ode();
            obj.plot_result();
        end
        function plot_unit(obj)
            obj = obj.set_properties(1000, 5);
            hold on
            obj.get_result_and_plot();
            obj = obj.set_properties(1000, 10);
            obj.get_result_and_plot();
            obj = obj.set_properties(1000, 15);
            obj.get_result_and_plot();
            obj = obj.set_properties(1000, 20);
            obj.get_result_and_plot();
            obj = obj.set_properties(1000, 25);
            obj.get_result_and_plot();
            hold off
            
        end
        function plot_all(obj)
            c = [1000, 5000, 10000, 15000];
            v = [5, 10, 15, 20, 25];
            maximum = zeros(size(4, 5));
            minimum = zeros(size(4, 5));
            tiledlayout(3,2)
            a = size(c);
            b = size(v);
            for i = 1:a(2)%매트랩은 배열의 첫째항의 인덱스가 1이다.
                nexttile
                hold on
                for j = 1:b(2)
                    obj = obj.set_properties(c(i), v(j));
                    obj = obj.solve_ode();
                    [maximum(i, j), minimum(i, j)] = obj.maxmin_value();%solve_ode() 함수를 수정하므로써 대입 시 차원 불일치 문제를 수정함. 
                    obj.plot_result();
                    
                end
                hold off
                xlabel("time(s)")
                ylabel("displacement(m)")
                switch i
                    case 1
                    	lgd = legend("v = 5", "v = 10", "v = 15", "v = 20", "v = 25");
                        title("responce1");
                        title(lgd, "c = 1000");
                    case 2
                        lgd = legend("v = 5", "v = 10", "v = 15", "v = 20", "v = 25");
                        title("responce2");
                        title(lgd, "c = 5000");
                    case 3
                        lgd = legend("v = 5", "v = 10", "v = 15", "v = 20", "v = 25");
                        title("responce3");
                        title(lgd, "c = 10000");
                    case 4
                        lgd = legend("v = 5", "v = 10", "v = 15", "v = 20", "v = 25");
                        title("responce4");
                        title(lgd, "c = 15000");
                end
            end
            %최대 최소 변위 출력
            
            nexttile
            hold on
            for  i = 1:a(2)
                plot(v, maximum(i, :))
            end
            hold off
            legend("c2 = 1000", "c2 = 5000", "c2 = 10000", "c2 = 15000");
            xlabel("velocity(km/h)")
            ylabel("displacement(m)")
            title("maximum displacement")
            nexttile
            hold on
            for i = 1:a(2)
                plot(v, minimum(i, :))
            end
            hold off
            legend("c2 = 1000", "c2 = 5000", "c2 = 10000", "c2 = 15000");
            xlabel("velocity(km/h)")
            ylabel("displacement(m)")
            title("maximum displacement")
        end
    end
end