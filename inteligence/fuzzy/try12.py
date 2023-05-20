# -*- coding: utf-8 -*-
"""
Created on Tue Jun  1 20:27:06 2021

@author: kimz1121@naver.com

environment : jupyter spyder 4

참고 자료 https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=jerrypoiu&logNo=221178162267

"""

import numpy as np
import matplotlib.pyplot as plt



class Fuzzy_function:
    def set_data(self, N, W, F, L):
        self.Fuzzy_N = N
        self.Width = W
        self.range_first = F
        self.range_last = L
        
    def Fuzzy_func_TA(self, input_data):
        if(input_data > self.range_last):
            input_data = self.range_last
            
        if(input_data < self.range_first):
            input_data = self.range_first
        
        if(input_data == self.Fuzzy_N):
            result_value = 1
            
        elif((self.Fuzzy_N - 0.5*self.Width)< input_data < self.Fuzzy_N):
            result_value = 1-(2/self.Width)*(self.Fuzzy_N - input_data)
        
        elif(self.Fuzzy_N < input_data < (self.Fuzzy_N + 0.5*self.Width)):
            result_value = 1-(2/self.Width)*(-1)*(self.Fuzzy_N - input_data)
            
        else:
            result_value = 0
        
        if(result_value < 0):
            result_value = 0
        
        if(result_value > 1):
            result_value = 1
        
        return result_value


class throtle_contorll_Fuzzy:
    def __init__(self):
        self.F_speed_target = 0
        self.F_speed_current = 0
        self.F_slope = 0
        self.OUTPUT_rule = 0


    def MAX_3(self, input_0, input_1, input_2):
        
        if(input_0 > input_1):
            if(input_0 > input_2):
                value = input_0
            else:
                value = input_2
        else:
            if(input_1 > input_2):
                value = input_1
            else:
                value = input_2
        return value
    
    def MIN_3(self, input_0, input_1, input_2):
        
        if(input_0 < input_1):
            if(input_0 < input_2):
                value = input_0
            else:
                value = input_2
        else:
            if(input_1 < input_2):
                value = input_1
            else:
                value = input_2
        return value

    def Fuzzy_generator(self):
        result = np.zeros((6, 101))
        range_array = np.arange(101)
        INPUT_array = [0, 20, 40, 60, 80, 100]
        A = Fuzzy_function()
        for i in range(6):
            A.set_data(INPUT_array[i], 40, 0, 100)
            for j in range_array:
                result[i, j] = A.Fuzzy_func_TA(j)
        return result
    
    def generate_member(self):
        self.F_speed_target = self.Fuzzy_generator()
        self.F_speed_current = self.Fuzzy_generator()
        self.F_slope = self.Fuzzy_generator()
        plt.plot(self.F_speed_target[0,:])
        plt.plot(self.F_speed_target[1,:])
        plt.plot(self.F_speed_target[2,:])
        plt.plot(self.F_speed_target[3,:])
        plt.plot(self.F_speed_target[4,:])
        plt.plot(self.F_speed_target[5,:])
        plt.show()
        
        plt.plot(self.F_speed_current[0,:])
        plt.plot(self.F_speed_current[1,:])
        plt.plot(self.F_speed_current[2,:])
        plt.plot(self.F_speed_current[3,:])
        plt.plot(self.F_speed_current[4,:])
        plt.plot(self.F_speed_current[5,:])
        plt.show()
        
        plt.plot(self.F_slope[0,:])
        plt.plot(self.F_slope[1,:])
        plt.plot(self.F_slope[2,:])
        plt.plot(self.F_slope[3,:])
        plt.plot(self.F_slope[4,:])
        plt.plot(self.F_slope[5,:])
        plt.show()
    
    def generate_output_mat(self):
        data = np.zeros((6, 6, 6))
        for i in range(6):
            for j in range(6):
                for k in range(6):
                    if(j <= i):
                        value = 5*i + 10*j + 5*k
                    else:
                        value = 15*i + -5*(j-i) + 5*k
                    if(value < 0):
                        value = 0
                    data[i, j, k] = value
        print(data)
        self.OUTPUT_rule = data
        return data         
    
    def plot_3D(self):
        data = self.OUTPUT_rule
        x = np.linspace(0, data.shape[1]-1, data.shape[0])
        x_lable = np.tile(x, (data.shape[2], 1))
        y_lable = x_lable.transpose()
    
        for i in range(data.shape[0]):
            print(i)
            fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
            ax.scatter(x_lable, y_lable, data[i, :, :], c='green')
            ax.set_zlim(0, 100)
            ax.invert_xaxis()

            ax.view_init(30,-25) 

            plt.tight_layout()
            plt.show()

    def rule_series(self, INPUT_speed_target, INPUT_speed_current, INPUT_slope_current):
        denominator = 0
        numerator = 0
        for i in range(self.OUTPUT_rule.shape[0]):
            for j in range(self.OUTPUT_rule.shape[1]):
                for k in range(self.OUTPUT_rule.shape[2]):
                    Factor_Min =self.MIN_3(self.F_speed_target[i, INPUT_speed_target], self.F_speed_current[j, INPUT_speed_current], self.F_slope[k, INPUT_slope_current])
                    
                    denominator = denominator + Factor_Min
                    numerator = numerator + Factor_Min*self.OUTPUT_rule[i, j, k]
        result_value = numerator/denominator
        return result_value
    
    def analyzer(self):
        a = 11
        b = 21
        c = 21
        
        d = int(100/(a-1))
        e = int(100/(b-1))
        f = int(100/(c-1))
        
        data = np.zeros((a, b, c))
        for i in range(a):
            for j in range(b):
                for k in range(c):
                    data[i, j, k] = self.rule_series(d*i, e*j, f*k)
                    
            x = np.linspace(0, data.shape[1]-1, data.shape[1])
            x_lable = np.tile(x, (data.shape[2], 1))
            y_lable = x_lable.transpose()
            fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
            ax.scatter(x_lable, y_lable, data[i, :, :], c='green')
            ax.set_zlim(0, 100)
            ax.invert_xaxis()

            ax.view_init(30,-25) 

            plt.tight_layout()
            plt.show()
            


B = throtle_contorll_Fuzzy()
B.generate_member()#퍼지 멤버 집합을 생성함
B.generate_output_mat()#출력 규칙을 생성함
B.plot_3D()
B.analyzer()
