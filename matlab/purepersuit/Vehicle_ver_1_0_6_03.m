%{
date: 0427
time: 23:39
version 1.0.6.3
auther : kimz1121@naver.com
%}

classdef Vehicle_ver_1_0_6_03
    properties
        %       로봇의 현재 위치에 관한 부분
        %       좌표 관련 데이터
        x = 0
        y = 0
        theta = 0

        n_of_log = 1% 배열 첫 요소의 숫자는 1
        log_x = []
        log_y = []
        log_theta = []
        log_min_index = []
        log_min_distance = []
        log_lad = []
        log_position = [];
        %       로봇의 주행성능에 관한 부분
        purepuirsuit_option = 1

        LAD_MAX = 0.5;%m 단위     직선 인식 길이 직선으로 인정하는 길이 기준??-> 좀더 엄밀히 정의할것 사각형 내부에 들어갈 수 있는 최소 회전반경의 크기는 사다리꼴의 총길이와 연관이 많을 것
        LAD_MIN = 0.01;%m 단위    주행가능한 최소곡률 반경 정의
        alpha_keystone = 10;%degree 단위   인식을 시작하는 각도
        near_width = 0.1%m 단위   인식할 최소 곡률 반경

        speed = 1% 속도
        omega = 1% 각속도
        time_gap = 0.001
        %       시스템으로 부터 전달 받은 경로 정보
        path = []

        %       주행 연산시 필요한 정보
        target_index = 0
        target_pass = 0;
        lad = 0

        error_derail_bool = 0

    end
    methods
        %<begin>기본 속성 정의 부분----------------------------------------
        function obj = set_properties_traditional_pp(obj, lad_max_in)
            obj.LAD_MAX = lad_max_in;
        end
        function obj = set_properties_KALAD_pp(obj, lad_max_in, lad_min_in, alpha_in, near_width_in)
            obj.LAD_MAX = lad_max_in;
            obj.LAD_MIN = lad_min_in;
            obj.near_width = near_width_in;
            obj.alpha_keystone = alpha_in;
        end
        function obj = set_purepuirsuit_mode(obj, mode)
            obj.purepuirsuit_option = mode;
        end
        function obj = set_position(obj, x_in, y_in, theta_in)
            obj.x = x_in;
            obj.y = y_in;
            obj.theta = theta_in;
        end
        function obj = set_path(obj, path_in)
            obj.path = path_in;
        end
        %<end>기본 속성 정의 부분----------------------------------------
        %<begin>주행중 자료 계산함수----------------------------------------
        function min_index = do_find_nearst_point_from_path(obj)
            distnace_from_points = obj.do_calc_distanceFromSelf();
            [~, min_index] = min(distnace_from_points);
            %             if(target_pass <= min_index)% 아직 구현을 보류중 어째서인지 이미 원형 주행을
            %             잘함...
            %
            %             end
            %             [min_length, min_index] = min(distnace_from_points);
        end
        function obj = set_target_passed(obj)%가장 가까운 요소중 배열 위치상 가장 가까운 것을 제거 
            obj.target_pass = obj.do_find_nearst_point_from_path();
        end
        function path_length = get_path_length(obj)
            [~, path_length] = size(obj.path);
        end
        function distnace_from_self = do_calc_distanceFromSelf(obj)
            distnace_from_self  = sqrt((obj.path(1,:)-obj.x).^2 + (obj.path(2,:)-obj.y).^2);
        end
        function distnace_from_points = do_calc_distanceFromPoints(obj, x_in, y_in)
            distnace_from_points  = sqrt((obj.path(1,:)-x_in).^2 + (obj.path(2,:)-y_in).^2);
        end
        function slope_inout_Maskbit_retrun = is_keyston_sideSlope_in(obj, dx, dy)
            a = tan(obj.alpha_keystone);
            y_intercept = obj.near_width/2;
            slope_inout_Maskbit_retrun = (dy < ((a.*dx) + y_intercept)) & (dy > ((-a.*dx) - y_intercept));
        end
        function obj = do_find_lad_point_traditonal(obj)
            min_index = obj.do_find_nearst_point_from_path();
            %경로에서 완전히 벗어난 경우 아직 고려 안함.
            %모든 경로에서 원과의 교점을 찾지 못했다면 경로의 마지막을 반환.
            %             if(min_index obj.path.end)
            Distances = obj.do_calc_distanceFromSelf();
            path_test_range = Distances(min_index:end);
            path_inoutMaskbit = path_test_range > obj.LAD_MAX;

            %             disp("--------------")
            path_discriminant_pre = path_inoutMaskbit(1:end-1);%??
            path_discriminant_post = path_inoutMaskbit(2:end);%??

            change_point_mask = path_discriminant_post - path_discriminant_pre;

            cross_point_index = find(path_inoutMaskbit == 1, 1);
            cross_in_point_index = find(change_point_mask == -1, 1);
            cross_out_point_index = find(change_point_mask == +1, 1);
            %             disp(change_point_mask);
            %             if(isempty(cross_in_point_index) == 1)
            %                 disp("+isempty(cross_in_point_index) == 1+")
            %             end
            %             if(isempty(cross_out_point_index) == 1)
            %                 disp("-isempty(cross_out_point_index) == 1-")
            %             end
            if(Distances(min_index) > obj.LAD_MAX)%치명적인 실수,,,, path와 nearst_index의 비교로는 거리 계산을 할 수 없다.
                obj.error_derail_bool = 1;%경로를 벗어남.
                obj.target_index = -1; %측정 반경을 벗어난 경우 경로를 이탈한 경우
                obj.lad = -1;
            else% 경로를 이탈하지 않은 경우
                if(isempty(cross_out_point_index) == 1)
                    %[numRows,numCols] = size(obj.path);
                    [~,numCols] = size(obj.path);
                    obj.target_index = numCols;
                else
                    obj.target_index = (min_index - 1) + cross_point_index;
                    obj.lad = Distances(obj.target_index);
                end
            end
        end
        function obj = do_find_lad_point_KALAD(obj)
            nearst_min_index = obj.do_find_nearst_point_from_path();
            %경로에서 완전히 벗어난 경우 아직 고려 안함.
            %모든 경로에서 원과의 교점을 찾지 못했다면 경로의 마지막을 반환.
            %             if(min_index obj.path.end)
            Distances = obj.do_calc_distanceFromSelf();
            path_test_range = Distances(nearst_min_index:end);
            LAD_MAX_inoutMaskbit = path_test_range > obj.LAD_MAX;% 경로를 구분 하는 조건 문
            LAD_MIN_inoutMaskbit = path_test_range > obj.LAD_MIN;% 경로를 구분 하는 조건 문
            %LAD_MAX와 LAD_MIN 사이에 존재하는 인덱스들을 찾고 그 인덱스들의 좌표가 경계를 벗어나는지 확인
            % 그중 경계를 벗어나는 첫번째 LAD 찾기
            %

            LAD_MAX_discriminant_pre = LAD_MAX_inoutMaskbit(1:end-1);%??
            LAD_MAX_discriminant_post = LAD_MAX_inoutMaskbit(2:end);%??

            LAD_MIN_discriminant_pre = LAD_MIN_inoutMaskbit(1:end-1);%??
            LAD_MIN_discriminant_post = LAD_MIN_inoutMaskbit(2:end);%??

            LAD_MAX_change_point_mask = LAD_MAX_discriminant_post - LAD_MAX_discriminant_pre;
            LAD_MIN_change_point_mask = LAD_MIN_discriminant_post - LAD_MIN_discriminant_pre;

            %             LAD_MAX_cross_point_index = find(path_inoutMaskbit == 1, 1);
            %             LAD_MAX_cross_in_point_index = find(LAD_MAX_change_point_mask == -1, 1);
            LAD_MAX_cross_out_point_index = find(LAD_MAX_change_point_mask == +1, 1);

            %             LAD_MIN_cross_point_index = find(path_inoutMaskbit == 1, 1);
            %             LAD_MIN_cross_in_point_index = find(LAD_MIN_change_point_mask == -1, 1);
            LAD_MIN_cross_out_point_index = find(LAD_MIN_change_point_mask == +1, 1);


            % KALAD 알고리즘
            % LAD_MAX이내 에서 잡히는 점이 없나.?(Distances(nearst_min_index) > obj.LAD_MAX)
            %   ->완전 이탈
            % 잡히는 점이 있나.?
            %   LAD_MIN이내 에서 잡히는 점이 없나.?(Distances(nearst_min_index) > obj.LAD_MIN)
            %           ->nearst_min_index에서 부터 dx가 LAD_MIN 보다 커지는 점을
            %           KALAD로 반환(혹은 end 반환)
            %   있나?
            %       LAD_MIN_cross_out_point_index가 empty 인가??
            %           ->end(=numCols)를 반환
            %       empty 아닌가?
            %           처음 시작할 때부터 사다리꼴 영역에 들어가지 못한 경우?
            %               ->LAD_MIN_cross_out_point_index를 KALAD로 반환 %(LAD_MIN_cross_out_point_index empty 일시 end(=numCols)를 반환)
            %           처음부터 잘 영역에 들어간 경우?
            %               빗변 밖으로 나가는 점이 있나?
            %                   slope_cross_out_point_index를 KALAD로 반환 %(LAD_MIN_cross_out_point_index empty 일시 end(=numCols)를 반환)
            %               없나?
            %                   LAD_MAX_cross_out_point_index가 empty 인가??
            %                        ->end(=numCols)를 반환
            %                   empty 아닌가?
            %                       ->LAD_MAX_cross_out_point_index를 KALAD로 반환
            %


            if(isempty(LAD_MIN_cross_out_point_index) ~= 1)
                if(isempty(LAD_MAX_cross_out_point_index) == 1)
                    path_MIN_TO_MAX = obj.path(:, nearst_min_index + LAD_MIN_cross_out_point_index : end);
                else
                    path_MIN_TO_MAX = obj.path(:, nearst_min_index + LAD_MIN_cross_out_point_index : nearst_min_index + LAD_MAX_cross_out_point_index);
                end
                [dx, dy] = obj.do_convert_dispacementTodxdy(path_MIN_TO_MAX(1, :), path_MIN_TO_MAX(2, :));
                slope_inoutMaskbit = obj.is_keyston_sideSlope_in(dx, dy);

                slope_discriminant_pre = slope_inoutMaskbit(1:end-1);%??
                slope_discriminant_post = slope_inoutMaskbit(2:end);%??

                slope_change_point_mask = slope_discriminant_post - slope_discriminant_pre;
                slope_cross_out_point_index = find(slope_change_point_mask == -1, 1);

                


            end
                
                
                [dx, ~] = obj.do_convert_dispacementTodxdy(obj.path(1, :), obj.path(2, :));
                dx_inoutMaskbit = dx > obj.LAD_MIN;

                dx_discriminant_pre = dx_inoutMaskbit(1:end-1);%??
                dx_discriminant_post = dx_inoutMaskbit(2:end);%??

                dx_change_point_mask = dx_discriminant_post - dx_discriminant_pre;
                dx_cross_out_point_index = find(dx_change_point_mask == 1, 1);

            [~,numCols] = size(obj.path);
            if(Distances(nearst_min_index) > obj.LAD_MAX)%
                obj.error_derail_bool = 1;%경로를 벗어남.
                obj.target_index = -1; %측정 반경을 벗어난 경우 경로를 이탈한 경우
                obj.lad = -1;
                disp("경로 이탈");
            else% 경로를 이탈하지 않은 경우
                if(Distances(nearst_min_index) > obj.LAD_MIN)%LAD_MIN을 이탈한 경우
                    obj.target_index = nearst_min_index + round(obj.LAD_MIN*0);
                else
                    if(isempty(LAD_MIN_cross_out_point_index) == 1)
                        obj.target_index = numCols;%+m LAD_MIN_cross_out_point_index 가 empty 인 시점에는 numCol을 반환하기로 약속 하였으므로 path_MIN_TO_MAX가 계산되지 않아도 된다.

                    else
                        if(slope_inoutMaskbit(end) == 0)
                        end
                        if(slope_inoutMaskbit(1) == 1)%첫 시작 인덱스가 사다리꼴에 포함여부
                            if(isempty(slope_cross_out_point_index) == 1)
                                if(isempty(LAD_MAX_cross_out_point_index) == 1)
                                    obj.target_index = numCols;
                                else
                                    obj.target_index = (nearst_min_index - 1) + LAD_MAX_cross_out_point_index;
                                end
                            else
                                obj.target_index = (nearst_min_index - 1) + (LAD_MIN_cross_out_point_index - 1) + slope_cross_out_point_index;
                            end
                        else
                            obj.target_index = (nearst_min_index - 1) + LAD_MIN_cross_out_point_index;
                        end

                    end
                    %                     obj.target_index = (nearst_min_index - 1) + cross_point_index;
                    %                     obj.lad = Distances(obj.target_index);
                end
            end
        end
        function [dx, dy, dtheta] = do_clac_purepursuit(obj, dx_target, dy_target)
            %퓨어 퍼슛 함수에서 할일 정의
            % 현재 로봇의 좌표 및 자세를 가져옴
            % 앞으로 가야할 위치의 좌표를 찍음
            % 로봇이 주행하는 경로를 직선 혹은 곡선으로서 표현
            % 해당 경로를 따라 단위 시간 동안 이동한 거리만큼 로봇의 위치를 이동시킨다.
            % 해당함수를 반복 호출하여 로봇의 최종 복표 위치까지 주행 시킨다.
            movestep = obj.speed * obj.time_gap;%m 단위로 계산됨. 단, 시간 간격이 짦을 수록 계산 간격이 mm 수준으로 작아짐
            %             distance = 0;
            if((dx_target^2 + dy_target^2) == 0)
                dx = 0;
                dy = 0;
                dtheta = 0;
            else
                gamma = (2 * dy_target)/(dx_target.^2 + dy_target.^2);
                alpha = atan2(dy_target, dx_target);
                if(abs(dy_target) <= 0.000001)
                    %distance = sqrt(dx_target.^2 + dy_target.^2);
                    dx = movestep;
                    dy = 0;
                    dtheta = 0;
                else
                    Radius_of_curvature = (dx_target.^2 + dy_target.^2)/(2 * dy_target);
                    angle = 2*alpha;
                    distance = abs(angle/gamma);
                    angle_step = angle*(movestep/distance);
                    dx = abs(Radius_of_curvature) * sin(abs(angle_step));
                    %                 dy = Radius_of_curvature * (-1) * (abs(angle)/angle) * (1-cos(angle_step));
                    dy = Radius_of_curvature * (-1) * (1-cos(angle_step));
                    dtheta = angle_step;
                    %                 a = [dx, dy, dtheta, Radius_of_curvature]
                end
            end
        end
        function [dx, dy] = do_convert_dispacementTodxdy(obj, x_target, y_target)
            Rot = [cos(-obj.theta) , -sin(-obj.theta); sin(-obj.theta), cos(-obj.theta)];
            displacement = [x_target - obj.x; y_target - obj.y];
            d = Rot * displacement;
            dx = d(1, :);
            dy = d(2, :);
        end
        %<end>주행중 자료 계산함수----------------------------------------
        %<begin>주행 함수----------------------------------------
        function obj = do_drive(obj)
            %             t = 0;
            path_length = obj.get_path_length();
            while(1)
                if(obj.purepuirsuit_option == 0)
                    obj = obj.do_find_lad_point_traditonal();
                else
                    obj = obj.do_find_lad_point_KALAD();
                end
                if(obj.error_derail_bool == 1)% 로봇이 경로를 완전히 이탈한 경우에도 종료
                    break
                end
                target_position = obj.path(:, obj.target_index);

%                 disp("--------")
%                 disp(obj.target_index)
%                 disp("+++++++++")
%                 disp(target_position)

                [dx, dy] = obj.do_convert_dispacementTodxdy(target_position(1, 1), target_position(2, 1));
%                 [dx_local_pp, dy_local_pp, dtheta_pp] = obj.do_clac_purepursuit(dx, dy);
                obj = obj.do_move_step(dx, dy);
                nearst_index = obj.do_find_nearst_point_from_path();
%                 index_distance = abs(path_length - nearst_index);
                distnace_from_points = obj.do_calc_distanceFromSelf();

                obj = obj.do_logging_min(distnace_from_points(nearst_index), nearst_index);%최소거리 로깅
                obj = obj.do_logging_lad(distnace_from_points(obj.target_index));%LAD 로깅
                obj = obj.do_logging_position();
                obj = obj.do_add_n_of_log();
                obj.set_target_passed();
                
%                 if(distnace_from_points(end) <= obj.LAD_MAX)

                if(distnace_from_points(end) <= 0.001)
                    disp("경로 종료")
                    break
                end
            end
        end
        function obj = do_move_step(obj, dx_target, dy_target)
            %
            [dx_local, dy_local, dtheta] = obj.do_clac_purepursuit(dx_target, dy_target);
            %             a = [dx_target, dy_target, dx_local, dy_local, dtheta]
            %             b = [dx_local, dy_local, dtheta]
            obj = obj.do_add_position_local(dx_local, dy_local, dtheta);
        end
        function obj = do_add_position_local(obj, dx_local, dy_local, dtheta_in)
            Rot = [cos(obj.theta), -sin(obj.theta); sin(obj.theta), cos(obj.theta)];
            Global_displacement = Rot * [dx_local; dy_local];
            obj.x = obj.x + Global_displacement(1, 1);
            obj.y = obj.y + Global_displacement(2, 1);
            obj.theta = obj.theta + dtheta_in;
        end
        %<end>주행 함수----------------------------------------
        function obj = get_position(obj)
            obj.x, obj.y, obj.theta
        end
        function obj = do_add_n_of_log(obj)
            obj.n_of_log = obj.n_of_log + 1;%배열에 요소 추가후 번호 상승
        end
        function obj = do_logging_position(obj)
            %             obj.log_position = [obj.log_position, [obj.x; obj.y; obj.theta]];
            obj.log_position(obj.n_of_log, :) = [obj.x; obj.y; obj.theta];
        end
        function obj = do_logging_min(obj, min_legth_in, min_index_in)
            obj.log_min_distance(obj.n_of_log) = min_legth_in;
            obj.log_min_index(obj.n_of_log) = min_index_in;
        end
        function obj = do_logging_lad(obj, lad_in)
            obj.log_lad(obj.n_of_log) = lad_in;
        end
        function log_return = get_log_position(obj)
            log_return = obj.log_position;
        end
        function [log_return_min_distance, log_return_min_index] = get_log_min(obj)
            log_return_min_distance = obj.log_min_distance;
            log_return_min_index = obj.log_min_index;
        end
        function log_return_lad = get_log_lad(obj)
            log_return_lad = obj.log_lad;
        end
        function obj = set_init_log(obj)
            obj.n_of_log = 1;% 배열 첫 요소의 숫자는 1
            obj.log_x = [];
            obj.log_y = [];
            obj.log_theta = [];
            obj.log_min_index = [];
            obj.log_min_distance = [];
            obj.log_lad = [];
            obj.log_position = [];
        end
    end
end
