% close
clear
clc

path_obj = Path_generator_1_3_2();
Curvature = path_obj.do_convertRadiusToCurvature(10);
path_obj = path_obj.set_properties(3, 0.5, 2*pi/4, 0);
% path_obj = path_obj.do_genratePath();
path = path_obj.get_path();
plot(-path(2, :), path(1, :),"b-" , "lineWidth", 1)
ylim([-0.5 1.5])
xlim([-0.5 4.5])
pbaspect([5, 2, 1])

vehicle_obj = Vehicle_ver_1_0_6_03();
vehicle_obj = vehicle_obj.set_purepuirsuit_mode(0);%0 : traditional, 1 : my-method
vehicle_obj = vehicle_obj.set_position(0, 0, 0);
[dx, dy]= vehicle_obj.do_convert_dispacementTodxdy(path(1, :), path(2, :));
vehicle_obj = vehicle_obj.set_path(path);
%%
N = 0;%사용자 정의 시행 횟수
M = 0;%반복 기록 횟수
Parameter = 45;
vehicle_obj = vehicle_obj.set_init_log();
vehicle_obj = vehicle_obj.set_purepuirsuit_mode(0);%0 : traditional, 1 : my-method
vehicle_obj = vehicle_obj.set_position(0, 0, 0);%reset position
vehicle_obj = vehicle_obj.set_properties_KALAD_pp(1, 0.1, 10, 0.1);%set_properties_KALAD_pp(obj, lad_max_in, lad_min_in, alpha_in, near_width_in)
vehicle_obj = vehicle_obj.do_drive();

% val_Fail = vehicle_obj.error_derail_bool;
log_position_1 = vehicle_obj.get_log_position();
% [log_min_distance, log_min_index] = vehicle_obj.get_log_min();
% lateral_error = log_min_distance;
% log_LAD = vehicle_obj.get_log_lad();


vehicle_obj = vehicle_obj.set_init_log();
vehicle_obj = vehicle_obj.set_purepuirsuit_mode(0);%0 : traditional, 1 : my-method
vehicle_obj = vehicle_obj.set_position(0, 0, 0);%reset position
vehicle_obj = vehicle_obj.set_properties_KALAD_pp(0.1, 0.1, 10, 0.1);%set_properties_KALAD_pp(obj, lad_max_in, lad_min_in, alpha_in, near_width_in)
vehicle_obj = vehicle_obj.do_drive();


log_position_2 = vehicle_obj.get_log_position();


vehicle_obj = vehicle_obj.set_init_log();
vehicle_obj = vehicle_obj.set_purepuirsuit_mode(1);%0 : traditional, 1 : my-method
vehicle_obj = vehicle_obj.set_position(0, 0, 0);%reset position
vehicle_obj = vehicle_obj.set_properties_KALAD_pp(1, 0.1, 10, 0.1);%set_properties_KALAD_pp(obj, lad_max_in, lad_min_in, alpha_in, near_width_in)
vehicle_obj = vehicle_obj.do_drive();


log_position_3 = vehicle_obj.get_log_position();

% log_LAD
% lateral_error

Param_str = string(Parameter);

%%
hold on
plot(-log_position_1(:, 2), log_position_1(:, 1), 'r--' ,  "lineWidth", 2)%
% writematrix(log_position_1, Str_file_name);

plot(-log_position_2(:, 2), log_position_2(:, 1), 'g:' ,  "lineWidth", 2)%
% writematrix(log_position_2, Str_file_name);

plot(-log_position_3(:, 2), log_position_3(:, 1), 'm-.' ,  "lineWidth", 2)%
% writematrix(log_position_3, Str_file_name);

hold off