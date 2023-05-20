%{
filename : Full_Car_Class_try_13
date 2021.07.06 11:06
athorizer : sangwoo michael kim
language : matlab R2019b
%}
classdef Full_Car_Class_try_13
    properties
        m_s_ =1200;
        Iyy_ = 950;
        Ixx_ = 400;
        m_wr1_ = 60;
        m_wl1_ = 60;
        m_wr2_ = 60;
        m_wl2_ = 60;
        k_wr1_ = 30000;
        k_wl1_ = 30000;
        k_wr2_ = 30000;
        k_wl2_ = 30000;
        
        k_sr1_ = 25000;
        k_sl1_ = 25000;
        k_sr2_ = 25000;
        k_sl2_ = 25000;
        c_sr1_ = 1000;
        c_sl1_ = 1000;
        c_sr2_ = 1000;
        c_sl2_ = 1000;
        a_ = 1.5;
        b_ = 1.5;
        c_ = 1;
        d_ = 1;
        
        
        type = 0;
        
        velocity_kmh = 0
        velocity_ms = 0
        
        direction = 0
        t_width = 0
        
        length_X = 3.7
        height_Y = 0.1
        
        
        
        t_range = [0 50]; % 시뮬레이션 시간 범위
        initial = [0 0 0 0 0 0 0 0 0 0 0 0 0 0]; % 변수 초기값
        
        result_t = 0;
        result_x = 0;
        result_theta = 0;
        result_phi = 0;
        
        time_value = 0;
    end
    methods
        function obj = set_machine_properties(obj, k, c)
            obj.k_sr1_ = k;
            obj.k_sl1_ = k;
            obj.k_sr2_ = k;
            obj.k_sl2_ = k;
            
            obj.c_sr1_ = c;
            obj.c_sl1_ = c;
            obj.c_sr2_ = c;
            obj.c_sl2_ = c;
        end
        function dz = Full_Car(obj, t, A)
            % Y = half sign signal Part ? if 문 포함 %
            [Z_rr1, Z_rl1, Z_rr2, Z_rl2] = obj.input_displacement(t);
            %             Z_rr1 = 0.001;
            %             Z_rl1 = 0;
            %             Z_rr2 = 0.001;
            %             Z_rl2 = 0;
            dz = zeros(14, 1);
            
            m_s = obj.m_s_;
            Iyy = obj.Iyy_;
            Ixx = obj.Ixx_;
            m_wr1 = obj.m_wr1_;
            m_wl1 = obj.m_wl1_;
            m_wr2 = obj.m_wr2_;
            m_wl2 = obj.m_wl2_;
            k_wr1 = obj.k_wr1_;
            k_wl1 = obj.k_wl1_;
            k_wr2 = obj.k_wr2_;
            k_wl2 = obj.k_wl2_;
            
            k_sr1 = obj.k_sr1_;
            k_sl1 = obj.k_sl1_;
            k_sr2 = obj.k_sr2_;
            k_sl2 = obj.k_sl2_;
            c_sr1 =obj.c_sr1_;
            c_sl1 = obj.c_sl1_;
            c_sr2 =obj.c_sr2_;
            c_sl2 = obj.c_sl2_;
            a = obj.a_;
            b = obj.b_;
            c = obj.c_;
            d = obj.d_;
            
            Z_cg  = A(1);
            pitch = A(2);
            roll   = A(3);
            Z_wr1 = A(4);
            Z_wl1 = A(5);
            Z_wr2 = A(6);
            Z_wl2 = A(7);
            
            dz(1) = A(8);%1계 도함수 부분
            dz(2) = A(9);
            dz(3) = A(10);
            dz(4) = A(11);
            dz(5) = A(12);
            dz(6) = A(13);
            dz(7) = A(14);
            
            %2계 도함수 부분
            dz(8) = (1/m_s).*...
                (...
                -(k_sr1 + k_sl1 + k_sr2 + k_sl2).*Z_cg -(c_sr1 + c_sl1 + c_sr2 + c_sl2).*A(8)...
                +(k_sr1.*a + k_sl1.*a - k_sr2.*b - k_sl2.*b).*pitch + (c_sr1.*a + c_sl1.*a - c_sr2.*b - c_sl2.*b).*A(9)...
                +(k_sr1.*c - k_sl1.*d - k_sr2.*c + k_sl2.*d).*roll + (c_sr1.*c - c_sl1.*d + c_sr2.*c - c_sl2.*d).*A(10)...%부호 2개 실수
                +k_sr1.*Z_wr1 + k_sl1.*Z_wl1 + k_sr2.*Z_wr2 +  k_sl2.*Z_wl2...
                +c_sr1.*A(11) + c_sl1.*A(12) + c_sr2.*A(13) + c_sl2.*A(14)...
                );
            
            dz(9) = (1/Iyy).*...
                (...
                (k_sr1.*a + k_sl1.*a - k_sr2.*b - k_sl2.*b).*Z_cg + (c_sr1.*a + c_sl1.*a - c_sr2.*b - c_sl2.*b).*A(8)...
                -(k_sr1.*a.^2 + k_sl1.*a.^2 + k_sr2.*b.^2 + k_sl2.*b.^2).*pitch - (c_sr1.*a.^2 + c_sl1.*a.^2 + c_sr2.*b.^2 + c_sl2.*b.^2).*A(9)...
                +(-k_sr1.*a.*c + k_sl1.*a.*d + k_sr2.*b.*c- k_sl2.*b.*d).*roll + (-c_sr1.*a.*c + c_sl1.*a.*d + c_sr2.*b.*c - c_sl2.*b.*d).*A(10)...
                -k_sr1.*a.*Z_wr1 - k_sl1.*a.*Z_wl1 + k_sr2.*b.*Z_wr2 +  k_sl2.*b.*Z_wl2...%부호 실수
                -c_sr1.*a.*A(11) - c_sl1.*a.*A(12) + c_sr2.*b.*A(13) + c_sl2.*b.*A(14)...%부호 실수
                );
            
            dz(10) = (1/Ixx).*...
                (...
                (k_sr1.*c - k_sl1.*d + k_sr2.*c - k_sl2.*d).*Z_cg + (c_sr1.*c - c_sl1.*d + c_sr2.*c - c_sl2.*d).*A(8)...
                +(-k_sr1.*a.*c + k_sl1.*a.*d + k_sr2.*b.*c- k_sl2.*b.*d).*pitch + (-c_sr1.*a.*c + c_sl1.*a.*d + c_sr2.*b.*c - c_sl2.*b.*d).*A(9)...
                -(k_sr1.*c.^2 + k_sl1.*d.^2 + k_sr2.*c.^2 + k_sl2.*d.^2).*roll - (c_sr1.*c.^2 + c_sl1.*d.^2 + c_sr2.*c.^2 + c_sl2.*d.^2).*A(10)...
                -k_sr1.*c.*Z_wr1 + k_sl1.*d.*Z_wl1 - k_sr2.*c.*Z_wr2 +  k_sl2.*d.*Z_wl2...
                -c_sr1.*c.*A(11) + c_sl1.*d.*A(12) - c_sr2.*c.*A(13) + c_sl2.*d.*A(14)...
                );
            
            
            dz(11) = (1/m_wr1).*...
                (...
                k_sr1.*Z_cg + c_sr1.*A(8)...
                +(-k_sr1.*a).*pitch + (-c_sr1.*a).*A(9)...
                +(-k_sr1.*c).*roll + (-c_sr1.*c).*A(10)...
                +(-k_sr1 - k_wr1)*Z_wr1 + (-c_sr1).*A(11) + k_wr1.*Z_rr1...
                );
            
            dz(12) = (1/m_wl1).*...
                (...
                k_sl1.*Z_cg + c_sl1.*A(8)...
                +(-k_sl1.*a).*pitch + (-c_sl1.*a).*A(9)...
                +(k_sl1.*d).*roll + (c_sl1.*d).*A(10)...
                +(-k_sl1 - k_wl1)*Z_wl1 + (-c_sl1).*A(12) + k_wl1.*Z_rl1...
                );
            
            dz(13) = (1/m_wr2).*...
                (...
                k_sr2.*Z_cg + c_sr2.*A(8)...
                +(k_sr2.*b).*pitch + (c_sr2.*b).*A(9)...
                +(-k_sr2.*c).*roll + (-c_sr2.*c).*A(10)...
                +(-k_sr2 - k_wr2)*Z_wr2 + (-c_sr2).*A(13) + k_wr2.*Z_rr2...
                );
            
            dz(14) = (1/m_wl2).*...
                (...
                k_sl2.*Z_cg + c_sl2.*A(8)...
                +(k_sl2.*b).*pitch + (c_sl2.*b).*A(9)...
                +(k_sl2.*d).*roll + (c_sl2.*d).*A(10)...
                +(-k_sl2 - k_wl2)*Z_wl2 + (-c_sl2).*A(14) + k_wl2.*Z_rl2...
                );
        end
        function obj = set_input_type(obj, type)
            obj.type = type;
        end
        function [y1, y2, y3, y4] = input_displacement(obj, t)
            switch obj.type
                case 0%step_time_based
                    [y1, y2, y3, y4]  = obj.step_time_based(t);
                case 1%step_speed_based
                    
                case 2%half_sine_speed_based
                    %                     disp("check1");
                    [y1, y2, y3, y4]  = obj.half_sine_speed_based(t);
                case 3
                    [y1, y2, y3, y4]  = obj.free_input(t);
            end
        end
        function obj = set_step_time_based(obj, dir, t_w, h)
            obj.direction = dir;
            obj.t_width = t_w;
            obj.height_Y = h;
        end
        function [y1, y2, y3, y4] = step_time_based(obj, t)
            if t < 1
                y1 = 0;
                y2 = 0;
                y3 = 0;
                y4 = 0;
                %                 disp(obj.t_width);
            elseif  1<= t && t<obj.t_width+1
                switch obj.direction
                    case 0
                        y1 = obj.height_Y;
                        y2 = obj.height_Y;
                        y3 = obj.height_Y;
                        y4 = obj.height_Y;
                    case 1
                        y1 = -obj.height_Y;
                        y2 = -obj.height_Y;
                        y3 = obj.height_Y;
                        y4 = obj.height_Y;
                    case 2
                        y1 = -obj.height_Y;
                        y2 = obj.height_Y;
                        y3 = -obj.height_Y;
                        y4 = obj.height_Y;
                    case 3
                        y1 = -obj.height_Y;
                        y2 = obj.height_Y;
                        y3 = obj.height_Y;
                        y4 = -obj.height_Y;
                end
            elseif obj.t_width+1<=t
                y1 = -obj.height_Y;
                y2 = obj.height_Y;
                y3 = obj.height_Y;
                y4 = -obj.height_Y;
            end
        end
        function obj = set_speed_km_h(obj, velocity)
            obj.velocity_kmh = velocity;
            obj.velocity_ms = velocity*(1000/3600);
        end
        function [y1, y2, y3, y4] = half_sine_speed_based(obj, t)
            %             disp("check2");
            
            %             disp(t_gap);
            t_bump = (obj.length_X)/(obj.velocity_ms);
            t_gap = (obj.a_ + obj.b_)/(obj.velocity_ms);
            %             disp(t);
            y1 = 0;
            y2 = 0;
            y3 = 0;
            y4 = 0;
            if 1<= t && t < t_bump+1
                y1 = obj.height_Y.*sin(pi.*(t - 1)./t_bump);
                y2 = obj.height_Y.*sin(pi.*(t - 1)./t_bump);
                %                 disp("check3");
            end
            if t_gap+1<= t && t < t_gap+t_bump+1
                y3 = obj.height_Y.*sin(pi.*(t - (t_gap+1))./t_bump);
                y4 = obj.height_Y.*sin(pi.*(t - (t_gap+1))./t_bump);
            end
        end
        function obj = calc_time_value(obj)
            obj.time_value = (obj.a_ + obj.b_)/(obj.velocity_ms);
        end
        function [y1, y2, y3, y4] = free_input(obj, t)
            if 1<=t && t<3
                Zrr1=0.25*sin(2*pi*t); Zrl1=0.25*cos(2*pi*t); Zrr2=0.25*sin(2*pi*t); Zrl2=0.25*cos(2*pi*t);
            elseif 3<=t && t<5
                Zrr1=0.25*cos(2*pi*t); Zrl1=0.25*sin(2*pi*t); Zrr2=0.25*cos(2*pi*t); Zrl2=0.25*sin(2*pi*t);
            elseif 5<=t && t<7
                Zrr1=0.25*sin(2*pi*t); Zrl1=0.25*sin(2*pi*t); Zrr2=0.25*sin(2*pi*t); Zrl2=0.25*sin(2*pi*t);
            elseif 7<=t && t<9
                Zrr1=0.25*cos(2*pi*t); Zrl1=0.25*cos(2*pi*t); Zrr2=0.25*cos(2*pi*t); Zrl2=0.25*cos(2*pi*t);
            elseif 9<=t
                Zrr1=0; Zrl1=0; Zrr2=0; Zrl2=0;
            elseif t<1
                Zrr1=0; Zrl1=0; Zrr2=0; Zrl2=0;
            end
            y1 = Zrr1;
            y2 = Zrl1;
            y3 = Zrr2;
            y4 = Zrl2;   
        end
        
        
        
        function obj = solve_ode(obj)
            [t, x] = ode45(@obj.Full_Car, obj.t_range, obj.initial); % ODE45 function
            obj.result_t = t;
            obj.result_x = x(:,1);%바퀴가 아닌 차량만의 변위 만을 확인하고자 함
            obj.result_theta = x(:,2);
            obj.result_phi = x(:,3);
            
            %위 같은 처리를 하지 않았을 때 x가 벡터의 집합으로 처리되어 최대 최소 값을 구하면 각각원 소별로 4개의 최대 최소값이 발생하였다.
        end
        
        function [max_x, min_x] = maxmin_value(obj)
            max_x = max(obj.result_x);
            min_x = min(obj.result_x);
        end
        
        function plot_result_x(obj)
            plot(obj.result_t, obj.result_x);
        end
        function plot_result_theta(obj)
            plot(obj.result_t, obj.result_theta);
        end
        function plot_result_phi(obj)
            plot(obj.result_t, obj.result_phi);
        end
        
        %         function set_properties_2(obj, c2_input, velocity)
        %             c2 = c2_input;
        %             velocity_kmh = velocity;
        %             velocity_ms = velocity*(1000/3600);
        %         end
        function obj = set_properties(obj, velocity)
            obj.velocity_kmh = velocity;
            obj.velocity_ms = velocity*(1000/3600);
        end
        function plot_unit(obj)
            obj = obj.set_input_type(0);
            obj = obj.set_step_time_based(2, 2, 0.1);
            tiledlayout(3,1)
            
            obj = obj.solve_ode();
            
            nexttile
            obj.plot_result_x();
            xlabel("time(s)")
            ylabel("Z(m)")
            
            nexttile
            obj.plot_result_theta();
            xlabel("time(s)")
            ylabel("pitch(rad)")
            
            nexttile
            obj.plot_result_phi();
            xlabel("time(s)")
            ylabel("roll(rad)")
            
        end
        function plot_all_half_sine(obj)
            v = [10, 25, 40];
            
            a = size(v);
            tiledlayout(a(2),3)
            obj = obj.set_input_type(2);
            for i = 1:a(2)%매트랩은 배열의 첫째항의 인덱스가 1이다.
                nexttile
                obj = obj.set_speed_km_h(v(i));
                obj = obj.solve_ode();
                
                obj.plot_result_x();
                xlabel("time(s)")
                ylabel("Z(m)")
                
                nexttile
                
                obj.plot_result_theta();
                xlabel("time(s)")
                ylabel("pitch(rad)")
                
                nexttile
                
                obj.plot_result_phi();
                xlabel("time(s)")
                ylabel("roll(rad)")
            end
        end
        function plot_all_step(obj)
            v = [0, 1, 2];
            
            a = size(v);
            tiledlayout(a(2),3)
            obj = obj.set_input_type(0);
            for i = 1:a(2)%매트랩은 배열의 첫째항의 인덱스가 1이다.
                nexttile
                obj = obj.set_step_time_based(v(i), 2, 0.1);
                obj = obj.solve_ode();
                
                obj.plot_result_x();
                xlabel("time(s)")
                ylabel("Z(m)")
                
                nexttile
                
                obj.plot_result_theta();
                xlabel("time(s)")
                ylabel("pitch(rad)")
                
                nexttile
                
                obj.plot_result_phi();
                xlabel("time(s)")
                ylabel("roll(rad)")
            end
        end
        function [k_t, c_t, R_max] = plot_all_k_c(obj)
            R_max = zeros(4, 4);
            
            k = linspace(25000, 55000, 21);
            c = linspace(1000, 4000, 21);
            a = size(k);
            b = size(c);
            %             tiledlayout(a(2),b(2))
            obj = obj.set_input_type(2);
            obj = obj.set_speed_km_h(40);
            
            for i = 1:a(2)
                for j = 1:b(2)
                    obj = obj.set_machine_properties(k(i), c(j));
                    %                     disp(k(i));
                    %                     disp(c(j));
                    obj = obj.solve_ode();
                    obj.result_x;
                    R_max(i, j) = max(obj.result_x);
                end
            end
            mesh(k, c, R_max);
            
            k_t = transpose(k);
            c_t = transpose(c);
            
        end
    end
end