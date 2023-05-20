# -*- coding: utf-8 -*-
"""
Created on Wed May 12 00:40:47 2021

@author: khk

environment : jupyter spyder 4

"""

"""
기차의 출력 세기

구간별 출력 세기 1 단위로 -5~5까 총 11단계 제어 단위 Km/s^2

구간 1~100

1~20 가속 21~80 유지 81~100 감속



크로모즘 시간별 방법
크로모즘 구간별 방법 - 일반적

evaluation
시뮬레이션 결과에 따라 점수를 매긴다.

점수  = 도착점 근점 점수 1000배 가중치 도달 정확도 점수 100배 가중 도착 시간 점수 10배 가중치
1차 일단 도달하는 객들이 살아남기 적합한 조건
2차 1차 세대 중에서 정확히 도달하는 객체들이 살아남기 적합한 조건
3차 빠르게 도달하는 객체들이 살아남기 적합한 조건

중요한 부분을 단계적으로 조절

도착 개체가 생겨나면 생존에 압도적으로 유리해 짐을 적용

달성 우선 순위에 따라 점수에 가중치를 부여하는 방법을 사용함

1) 시뮬레이터
2) 결과 평가 함수 -(결과의 형식 정하기)

"""


import numpy as np
import math
import matplotlib.pyplot as plt
import random



#fitted_individual

def elvaluater(target, position, end_velocity, time_elapsed):
    max_position = np.max(position)
    return (target - abs(target-max_position)) - 10*math.sqrt(end_velocity**2) + 100*(400-time_elapsed)
    

def simulater(population):
    acceleration = population
    
    
    
    target = 10000
    
    dt = 1
    
    position_0 = 0
    velocity_0 = 0
    
    position_data = np.zeros((acceleration.shape[0],400))
    velocity_data = np.zeros((acceleration.shape[0],1))
    time_elapsed_data = np.zeros((acceleration.shape[0],1))
    
    
    section = 0#구간 은 항 상 정수여야 하므로 다른 변수를 만들어 관리 하도록 함
    
    n = 0
    
    time_elapsed = 0
    while (n < acceleration.shape[0]):
        position = position_0
        velocity = velocity_0
        j = 0
        while j < 400:
            velocity = velocity + acceleration[n,section]*dt
            
            #최대 속력 제한 조건
            if velocity > 300:
                velocity = 300
            #후진 제한
            if velocity < -300:
                velocity = -300
                
                
            position = position + velocity*dt #+ 0.5*acceleration[n,section]*dt*dt
            
            position_data[n, j] = position
            
            section = math.floor(0.01*position)
            
            #크로모즘 초과 방지 조건
            if section > 119:
                section = 119
            if section < 0:
                section = 0
            
            
            
                
            if abs(position - target) < 100 and abs(velocity) < 0.1:
                velocity = 0
                break;#개체별 반복을 끝냄
                
                
            #print(j, time_elapsed, position, velocity, acceleration[n,section], section)
            
            j = j+1
            time_elapsed = dt*j
            
        #print(n, target, np.max(position_data[n, : ]), velocity, time_elapsed)   
        #print(elvaluater(target, position_data[n, : ], velocity, time_elapsed))
        #plt.plot(position_data[n, : ])
        #plt.ylabel('postion')
        #plt.show()    
        
        position_data[n] = position_data[n, : ]
        velocity_data[n] = velocity
        time_elapsed_data[n] = time_elapsed
        n = n+1
    return target, position_data, velocity_data, time_elapsed_data
    
    #가속도 외부에서 주어짐 위치 초기값을 통해 결정 됨
    #속도 가속도를 통해 계산됨
    #위치 속도와 가속도를 통해 계산됨
    
def simulater_with_plt(population):
    target, position_data, velocity_data, time_elapsed_data = simulater(population)
    for n in range(population.shape[0]):
        print(n,  np.max(position_data[n, : ]), elvaluater(target, position_data[n, : ], velocity_data[n], time_elapsed_data[n]))#다음에는 시간에 대한 속도 그래프도 그려보기
        plt.plot(position_data[n, : ])
        plt.ylabel('postion')
        plt.show()  

def GA_get_fitted_index(population):
    value = list(range(population.shape[0]))
    #print(value)
    target , position, velocity, time_elapsed = simulater(population)
    for i in range(population.shape[0]):
        value[i] = elvaluater(target, position[i, : ], velocity[i, 0], time_elapsed[i, 0])
    rank = value[:]#얕은 복사
    rank.sort(reverse = 1)
    #print(rank)
    #print(value)
    
    #print(value.index(rank[0]), value.index(rank[1]))
    
    return value.index(rank[0]), value.index(rank[1])
    
"""
def GA_reproduction_2(population, index1, index2, Number_pop):
    chromosome_1 = population[index1, :]
    chromosome_2 = population[index2, :]
    
    next_generation_pop = np.zeros((population.shape[0], 120))
    
    next_generation_pop[0] = chromosome_1
    next_generation_pop[1] = chromosome_2
    
    if population.shape[0]%2 ==0:
       for i in range(2, population.shape[0]):
            GA_crossover(chromosome_1, chromosome_2, 0.8)
        
"""      
    
def GA_reproduction_1(population, index1, index2, Number_pop):
    chromosome_1 = population[index1, :]
    chromosome_2 = population[index2, :]
    
    next_generation_pop = np.zeros((population.shape[0], 120))
    
    next_generation_pop[0] = chromosome_1[:]
    next_generation_pop[1] = chromosome_2[:]
    
    #print("#1", chromosome_1[:])
    #print("#2",GA_crossover(chromosome_1[:], chromosome_2[:], 0.8))
    for i in range(2, population.shape[0]):
        temp = GA_crossover(chromosome_1[:], chromosome_2[:], 0.8)
        
        #print("#4", temp)
        #print("#5", np.array(temp)[0,:])
        next_generation_pop[i] = np.array(temp)[0,:]
        
    #print("#6", next_generation_pop)
    return next_generation_pop
    
def GA_crossover(chromosome_1, chromosome_2, threshold_crossover):
    cross = False
    
    rand_choice = random.random()
        
    new_chromosome_1 =  [0 for i in range(len(chromosome_1))]
    #new_chromosome_1 = np.zeros((1, 120)) 
    
    #new_chromosome_2 = np.zeros((1, 120)) 
    #print("#2", new_chromosome_1)
    #print("#3", chromosome_1)
    
    for i in range(len(chromosome_1)):
        cross_value = np.random.random((1,120))
        
        if cross_value[0, i] > 0.8:
            if cross == False:
                cross  = True
            else:
                cross = False
        #print(i, new_chromosome_1)
        if  random.random() >= 0.5:
            if cross:
                new_chromosome_1[i] = chromosome_1[i]
                #new_chromosome_2[i] = chromosome_2[i]
            else:
                new_chromosome_1[i] = chromosome_2[i]
                #new_chromosome_2[i] = chromosome_1[i]
        else:
            if cross:
                new_chromosome_1[i] = chromosome_2[i]
                #new_chromosome_2[i] = chromosome_1[i]
            else:
                new_chromosome_1[i] = chromosome_1[i]
                #new_chromosome_2[i] = chromosome_2[i]
        
    return new_chromosome_1, #new_chromosome_2
    
"""
if  rand_choice >= 0.5:
            if cross:
                new_chromosome_1[0, i] = chromosome_1[0, i]
                #new_chromosome_2[0, i] = chromosome_2[0, i]
            else:
                new_chromosome_1[0, i] = chromosome_2[0, i]
                #new_chromosome_2[0, i] = chromosome_1[0, i]
        else:
            if cross:
                new_chromosome_1[0, i] = chromosome_2[0, i]
                #new_chromosome_2[0, i] = chromosome_1[0, i]
            else:
                new_chromosome_1[0, i] = chromosome_1[0, i]
                #new_chromosome_2[0, i] = chromosome_2[0, i]
        
"""


def GA_mutate(next_generation_pop, NPOP_threshold, NCHO_threshold):
    mutate_choice_value = np.random.random(population.shape[0])
    

    
    for i in range(2, population.shape[0]):
        if mutate_choice_value[i] > NPOP_threshold:
            for j in range(population.shape[1]):
                mutate_index_value = np.random.random(population.shape[1])
                if mutate_index_value[j] > NCHO_threshold:
                    next_generation_pop[i, j] = np.random.randint(-2, 3)
                    
    return next_generation_pop
                    
        
def GA_system(population):
    
    
    index1, index2 = GA_get_fitted_index(population)
    next_generation_pop = GA_reproduction_1(population, index1, index2, population.shape[0])
    next_generation_pop_mutate = GA_mutate(next_generation_pop, 0.7, 0.7)
    
    #print("#7", next_generation_pop)
    #print("#8", next_generation_pop_mutate)
    
    return next_generation_pop_mutate
        
def GA_system_iter(population, iteration):
    n = 0
    while(n < iteration):
        population = GA_system(population)
        #simulater(population)
        n = n + 1
    return population
    
population = np.random.randint(-2, 3, size=(40, 120))#인구 랜덤 생성 40개 크로모즘크기 : 100
simulater_with_plt(population)

#population = GA_system_iter(population, 100)
#simulater_with_plt(population)

population = GA_system_iter(population, 100)
#simulater_with_plt(population)

population = GA_system_iter(population, 100)
simulater_with_plt(population)