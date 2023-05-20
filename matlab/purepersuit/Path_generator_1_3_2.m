%{
date: 0425
time: 07:02
version 1.0.0
auther : kimz1121@naver.com
%}

%1 - cos()
% 유턴,


classdef Path_generator_1_3_2
    properties



        margin_legth = 0%m 단위
        Radius_of_curvature = 0
        curve_angle = 0%rad 단위

        gap_btween_point = 0.001

        start_point = [0; 0]%x, y
        start_angle = 0
        start_direction_vector = [1; 0]%x, y = (1, 0)
        path = []

        %1-cos 부분

        %구간의 길이
        %코사인 반복 횟수
        %총 각변화량 = 코사인 반복 횟수 * 2pi
        %길이당 각 변화량 = 2*pi*n / L
        %L 인 지점에서 cos(X * n * 2pi/(L))

        %X = L 일 때
        %n * 2pi

        %유턴 부분,

        %U 턴 진입 전 여유길이
        %곡률 반경
        % 회전 각도는 무조건 pi



    end
    methods
        function curvature = do_convertRadiusToCurvature(~, Radius_of_curvature_in)
            curvature = 1/Radius_of_curvature_in;%m 단위 연산
        end
        function obj = set_properties(obj, margin_legth_in, Radius_of_curvature_in, angle_in, start_angle_in)
            obj.margin_legth = margin_legth_in;
            obj.Radius_of_curvature = Radius_of_curvature_in;
            obj.curve_angle = angle_in;
            obj.start_angle = start_angle_in;
            obj.start_direction_vector = [cos(start_angle_in); sin(start_angle_in)];
        end
        function obj = do_genratePath(obj)
            % 이 함수에서 해해 하는 일
            % 객체 속성에 의해 저으이 된 경로를 점 단위 경로로 반환하는 것이 역할
            % 조건 1 : 점단위 경로는 길이당 점간 간격이 일정한 것이 목표 (점간 간격의 크기는 1 point / mm 이다.)
            % 조건 2 : 경로는
            % 진입 부분 : 직선 주행 (마진길이 적용 약 1m 로 설정 예정)
            % 주행 부분 : 곡선 주행 (곡률, 총 회전각도)
            % 탈출 부분 : 직선 주행 (마진길이 적용 약 1m 로 설정 예정)
            % 의 각 3가지 부분으로 나누어 경로를 설정함.
            % 조건 3 추가로 만들어진 경로를 csv 파일로 변환할 수 있으면 좋겠음.

            % 총 4개의 점을 잡을 것
            % 경로의 정밀도는 1 point/mm 조건을 지키기위해 mm 단위에서 정수로 절삭

            % 직선 구간에서 필요한 점의 개수 구하기
            legth = 1;
            legth = round(legth, 3);%m 단위계에서 1mm 유효 숫자까지 반올림.
            line_linspace = 0: obj.gap_btween_point :legth;
            direction_vector = [1; 0];
            near_point = obj.start_point;
            line_in_path = (direction_vector .* line_linspace) + near_point;
            obj.path = line_in_path;

            if(obj.curve_angle ~= 0)

                % 각도와 곡률에 의해 정해지는 길이 구하기
                % 각도와 곡률에 의해 정해지는 주행길이
                % 주행길이는 회전반경 * 회전 각도 이다.
                % 다만 이계산의 경우 완벽한 직진 주행에서 회전 반경이 무한대가 나오게 되어 표현이 어려운 부분이 있다.
                % 따라서 주어진 경로가 곡선인지 아닌지 구분할 필요도 있다...??
                obj.Radius_of_curvature = 0;
                curve_length = obj.Radius_of_curvature * obj.curve_angle;

                curve_length_round = round(curve_length, 3);

                angle_modify = obj.curve_angle * (curve_length_round / curve_length);
                disp(angle_modify)
                % 1 point / mm 를 유지하기 위해
                % 반올림으로 잘린 이동 거리의 비율만큼 계산하는 각의 크기도 변형함.

                angle_linspace = linspace(0, angle_modify , abs(round(curve_length_round*1000)));
                angle_linspace_apply_initial_angle = angle_linspace + obj.start_angle;
                angle_iter = angle_linspace_apply_initial_angle;
                disp(angle_linspace)
                disp(angle_iter)

                %곡선 경로 생성
                %             obj.path;

                dx = obj.Radius_of_curvature * (abs(obj.curve_angle)/obj.curve_angle) * sin(angle_linspace(2:end));
                dy = obj.Radius_of_curvature * (-1) * (abs(obj.curve_angle)/obj.curve_angle) * (1-cos(angle_linspace(2:end)));
                %
                %(obj.curve_angle/obj.curve_angle) angle의 부호를 판별해 주는 역할(단, angle ~= 0)

                path_radius(1, :) = dx + line_in_path(1, end);
                path_radius(2, :) = dy + line_in_path(2, end);
                obj.path = [obj.path, path_radius];
            end
            angle = obj.curve_angle - obj.start_angle;
            direction_vector = round([cos(-angle); sin(-angle)], 1.^(-10));
            near_point = obj.path(:, end);
            %             near_point = [0; 0];
            line_out_path = (direction_vector * line_linspace(2:end)) + near_point;

            obj.path = [obj.path, line_out_path];


            %1-cos() 구간
            %길이
            %Y
            num = 10;
            L = 2;
            h = 0.05;
            line_linspace = 0: obj.gap_btween_point :L;
            Y = -line_linspace + obj.path(2, end);
            X = h*(1-cos(line_linspace * (num * 2 * pi / L))) + obj.path(1, end);

            obj.path = [obj.path, [X; Y]];


            %유턴 구간
            L = 1;
            line_linspace = 0: obj.gap_btween_point :L;
            Y = -line_linspace + obj.path(2, end);
            X = obj.path(1, end)*ones(size(line_linspace));

            obj.path = [obj.path, [X; Y]];

            obj.curve_angle = pi;
            obj.Radius_of_curvature = 0.1;
            curve_length = obj.Radius_of_curvature * obj.curve_angle;

            curve_length_round = round(curve_length, 3);

            angle_modify = obj.curve_angle * (curve_length_round / curve_length);
            disp(angle_modify)
%             % 1 point / mm 를 유지하기 위해
%             % 반올림으로 잘린 이동 거리의 비율만큼 계산하는 각의 크기도 변형함.
% 
            angle_linspace = linspace(0, angle_modify , abs(round(curve_length_round*1000)));
            angle_linspace_apply_initial_angle = -angle_linspace - pi/2;
            angle_iter = angle_linspace_apply_initial_angle;
            disp(angle_linspace)
            disp(angle_iter)
% 
%             %곡선 경로 생성
%             %             obj.path;

            dx = obj.Radius_of_curvature * -sign(obj.curve_angle) * (1-sin(abs(angle_iter(2:end))));
            dy = obj.Radius_of_curvature * -sign(obj.curve_angle) * (1-cos(angle_iter(2:end)));
%             %
%             %(obj.curve_angle/obj.curve_angle) angle의 부호를 판별해 주는 역할(단, angle ~= 0)
% 
%             path_radius(1, :) = [dx + line_in_path(1, end)];
%             path_radius(2, :) = [dy + line_in_path(2, end)];
            obj.path = [obj.path, [[dx + obj.path(1, end)]; [dy + obj.path(2, end)]]];

            L = 1;
            line_linspace = 0: obj.gap_btween_point :L;
            Y = line_linspace + obj.path(2, end);
            X = obj.path(1, end)*ones(size(line_linspace));

            obj.path = [obj.path, [X; Y]];
            

        end
        function path_retrun = get_path(obj)
            obj = obj.do_genratePath();
            path_retrun = obj.path;
        end
    end
end