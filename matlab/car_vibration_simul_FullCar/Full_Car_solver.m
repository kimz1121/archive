clc
clear



Full_Car_obj = Full_Car_Class_try_13();
obj = Full_Car_obj;

obj.t_range = [0, 25];
obj = obj.set_input_type(0);
obj = obj.set_step_time_based(2, 5, 0.5);%(type, time_gap, height)
obj = obj.solve_ode();

tiledlayout(1,3)


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







% Full_Car_obj.plot_unit;
%Full_Car_obj.plot_all_step;
%[k, c, Max_displacement] = Full_Car_obj.plot_all_k_c;



