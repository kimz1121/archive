# -*- coding: utf-8 -*-
"""
Created on Tue Apr 13 00:36:19 2021

@author: khk
"""



import numpy as np
import random


def sigmoid(x):
    return 1/(1 + np.exp(-x))

def softmax(a):

    c = np.max(a)

    minus = a - c

    exp_a = np.exp(minus)

    sum_exp_a = np.sum(exp_a)

    y = exp_a / sum_exp_a

    return y

def heavy_side_05(x):
    return x > 0.5    


def init_network(x_node, h_node, y_node):

    
    network = {}#딕셔너리형 자료 키값을 문자열로 하는 형태
    
    
    network['w1'] = np.random.rand(x_node, h_node)
    network['w2'] = np.random.rand(h_node, y_node)
    
    network['dw1'] = np.full((x_node, h_node), 0)
    network['dw2'] = np.full((h_node, y_node), 0)
    
    network['d1'] = np.full((1, h_node), 0)
    network['d2'] = np.full((1, y_node), 0)
    network['z1'] = np.full((1, h_node), 0)
    network['z2'] = np.full((1, y_node), 0)
    return network
#딕셔너리형태로 전체 네트워크를 담았다.
#노드라도 인스턴스로 구현할 필요는 없고 단순히 행렬과 행렬의 연산으로 나타낼 수 있다.

def test_softmax(network, x_in):
    #forwardTrain
    w1, w2 = network['w1'], network['w2'] #w1 입력레이어와 히든레이어 사이 가중치, w2 히든레이어와 출력레이어 사이 가중치 
    #d1, d2 = network['d1'], network['d2']
    
    h_net = x_in.dot(w1)#1-2 레이어의 연산
    h_out = sigmoid(h_net)#1*15
    y_net = h_out.dot(w2)#2-3 레이어 사이의 입력은 이전 레이어의 계산 결과!
    y_out = sigmoid(y_net)#
    return softmax(y_out)
def test_sigmoid(network, x_in):
    #forwardTrain
    w1, w2 = network['w1'], network['w2'] #w1 입력레이어와 히든레이어 사이 가중치, w2 히든레이어와 출력레이어 사이 가중치 
    #d1, d2 = network['d1'], network['d2']
    
    h_net = x_in.dot(w1)#1-2 레이어의 연산
    h_out = sigmoid(h_net)#1*15
    y_net = h_out.dot(w2)#2-3 레이어 사이의 입력은 이전 레이어의 계산 결과!
    y_out = sigmoid(y_net)#
    return y_out

def prediction_softmax(n):
    if(n == 0):
        data = test_softmax(network, x['0_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 0])
    elif(n == 1):   
        data = test_softmax(network, x['1_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 1])
    elif(n == 2):  
        data = test_softmax(network, x['2_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 2])
    elif(n == 3):  
        data = test_softmax(network, x['3_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 3])
    elif(n == 4):   
        data = test_softmax(network, x['4_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 4])
    elif(n == 5):   
        data = test_softmax(network, x['5_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 5])
    elif(n == 6):   
        data = test_softmax(network, x['6_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 6])
    elif(n == 7):   
        data = test_softmax(network, x['7_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 7])
    elif(n == 8):   
        data = test_softmax(network, x['8_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 8])
    elif(n == 9):   
        data = test_softmax(network, x['9_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 9])
        

def prediction_sigmoid(n):
    if(n == 0):
        data = test_sigmoid(network, x['0_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 0])
    elif(n == 1):   
        data = test_sigmoid(network, x['1_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 1])
    elif(n == 2):  
        data = test_sigmoid(network, x['2_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 2])
    elif(n == 3):  
        data = test_sigmoid(network, x['3_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 3])
    elif(n == 4):   
        data = test_sigmoid(network, x['4_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 4])
    elif(n == 5):   
        data = test_sigmoid(network, x['5_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 5])
    elif(n == 6):   
        data = test_sigmoid(network, x['6_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 6])
    elif(n == 7):   
        data = test_sigmoid(network, x['7_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 7])
    elif(n == 8):   
        data = test_sigmoid(network, x['8_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 8])
    elif(n == 9):   
        data = test_sigmoid(network, x['9_i'].reshape(1, 25))
        print(result_max(data))
        print(data[0, 9])
        
def train(network, x_in, x_out, eta):
    #forwardTrain
    w1, w2 = network['w1'], network['w2'] #w1 입력레이어와 히든레이어 사이 가중치, w2 히든레이어와 출력레이어 사이 가중치 
    #d1, d2 = network['d1'], network['d2']
    
    h_net = x_in.dot(w1)#1-2 레이어의 연산
    h_out = sigmoid(h_net)#1*15
    y_net = h_out.dot(w2)#2-3 레이어 사이의 입력은 이전 레이어의 계산 결과!
    y_out = sigmoid(y_net)#
    
#    print("******")
#    print(y1)
    
    d2 = y_out*(1 - y_out)*(x_out - y_out)#1*10 shape
    d1 = h_out*(1 - h_out)*(d2.dot(w2.T))#1*15 shape #요기 계산이 균일한 값이 자꾸 발생하는 것 문제가 없는가??#랜덤 가중치 부여를 통해 해결!!
    #설마 1*1 행렬 상수배 하고 있는 것인가..?
    #network['z1'], network['z2'] = x_in, h_out
    #network['d1'], network['d2'] = d1, d2
    
    
    #backwardTrain
    
    dw1 = eta*np.dot(x_in.T, d1)
    dw2 = eta*np.dot(h_out.T, d2)
    
    network['dw1'], network['dw2'] = dw1, dw2
    
    w1 = w1 + dw1
    w2 = w2 + dw2
    network['w1'], network['w2'] = w1, w2
    return y_out

def epoch(eta):
    #print("-<0>--------------------")
    train(network, x['0_i'].reshape(1, 25), x['0_o'], eta)
    #print("-<1>--------------------")
    train(network, x['1_i'].reshape(1, 25), x['1_o'], eta)
    #print("-<0>--------------------")
    train(network, x['2_i'].reshape(1, 25), x['2_o'], eta)
    #print("-<1>--------------------")
    train(network, x['3_i'].reshape(1, 25), x['3_o'], eta)
    #print("-<0>--------------------")
    train(network, x['4_i'].reshape(1, 25), x['4_o'], eta)
    #print("-<1>--------------------")
    train(network, x['5_i'].reshape(1, 25), x['5_o'], eta)
    #print("-<0>--------------------")
    train(network, x['6_i'].reshape(1, 25), x['6_o'], eta)
    #print("-<1>--------------------")
    train(network, x['7_i'].reshape(1, 25), x['7_o'], eta)
    #print("-<0>--------------------")
    train(network, x['8_i'].reshape(1, 25), x['8_o'], eta)
    #print("-<1>--------------------")
    train(network, x['9_i'].reshape(1, 25), x['9_o'], eta)

#학습 자료 : 0~9 의 숫자
x = {}
x['0_o'] = np.array([[1, 0, 0, 0, 0, 0, 0, 0, 0, 0]])
x['0_i'] = np.array([[0, 1, 1, 1, 0], 
                     [1, 0, 0, 0, 1], 
                     [1, 0, 0, 0, 1], 
                     [1, 0, 0, 0, 1], 
                     [0, 1, 1, 1, 0]])

x['1_o'] = np.array([[0, 1, 0, 0, 0, 0, 0, 0, 0, 0]])
x['1_i'] = np.array([[0, 0, 1, 0, 0], 
                     [0, 0, 1, 0, 0], 
                     [0, 0, 1, 0, 0], 
                     [0, 0, 1, 0, 0], 
                     [0, 0, 1, 0, 0]])

x['2_o'] = np.array([[0, 0, 1, 0, 0, 0, 0, 0, 0, 0]])
x['2_i'] = np.array([[1, 1, 1, 1, 1], 
                     [0, 0, 0, 0, 1], 
                     [1, 1, 1, 1, 1], 
                     [1, 0, 0, 0, 0], 
                     [1, 1, 1, 1, 1]])

x['3_o'] = np.array([[0, 0, 0, 1, 0, 0, 0, 0, 0, 0]])
x['3_i'] = np.array([[1, 1, 1, 1, 1], 
                     [0, 0, 0, 0, 1], 
                     [1, 1, 1, 1, 1], 
                     [0, 0, 0, 0, 1], 
                     [1, 1, 1, 1, 1]])

x['4_o'] = np.array([[0, 0, 0, 0, 1, 0, 0, 0, 0, 0]])
x['4_i'] = np.array([[0, 0, 1, 1, 0], 
                     [0, 1, 0, 1, 0], 
                     [1, 0, 0, 1, 0], 
                     [1, 1, 1, 1, 1], 
                     [0, 0, 0, 1, 0]])

x['5_o'] = np.array([[0, 0, 0, 0, 0, 1, 0, 0, 0, 0]])
x['5_i'] = np.array([[1, 1, 1, 1, 1], 
                     [1, 0, 0, 0, 0], 
                     [1, 1, 1, 1, 1], 
                     [0, 0, 0, 0, 1], 
                     [1, 1, 1, 1, 1]])

x['6_o'] = np.array([[0, 0, 0, 0, 0, 0, 1, 0, 0, 0]])
x['6_i'] = np.array([[1, 1, 1, 1, 1], 
                     [1, 0, 0, 0, 0], 
                     [1, 1, 1, 1, 1], 
                     [1, 0, 0, 0, 1], 
                     [1, 1, 1, 1, 1]])

x['7_o'] = np.array([[0, 0, 0, 0, 0, 0, 0, 1, 0, 0]])
x['7_i'] = np.array([[1, 1, 1, 1, 1], 
                     [0, 0, 0, 1, 0], 
                     [0, 0, 1, 0, 0], 
                     [0, 0, 1, 0, 0], 
                     [0, 0, 1, 0, 0]])

x['8_o'] = np.array([[0, 0, 0, 0, 0, 0, 0, 0, 1, 0]])
x['8_i'] = np.array([[1, 1, 1, 1, 1], 
                     [1, 0, 0, 0, 1], 
                     [1, 1, 1, 1, 1], 
                     [1, 0, 0, 0, 1], 
                     [1, 1, 1, 1, 1]])

x['9_o'] = np.array([[0, 0, 0, 0, 0, 0, 0, 0, 0, 1]])
x['9_i'] = np.array([[1, 1, 1, 1, 1], 
                     [1, 0, 0, 0, 1], 
                     [1, 1, 1, 1, 1], 
                     [0, 0, 0, 0, 1], 
                     [0, 0, 0, 0, 1]])



x["9'_i"]= np.array([[1, 1, 1, 1, 1], 
                     [1, 0, 0, 0, 1], 
                     [1, 1, 1, 1, 1], 
                     [0, 0, 0, 0, 1], 
                     [0, 0, 1, 1, 1]])
 
x["6'_i"]= np.array([[1, 1, 1, 0, 0], 
                     [1, 0, 0, 0, 0], 
                     [1, 1, 1, 0, 0], 
                     [1, 0, 0, 0, 0], 
                     [1, 1, 1, 0, 0]])

network = init_network(25, 15, 10)


'''
epoch(0.1)
epoch(0.1)
epoch(0.1)
'''

def error_calc(network, n, m):
    data = (np.std(network['dw1']) < n)  and (np.std(network['dw2']) < m)
    return data
#
def result_max(array):
    return np.argmax(array)
    
iter_num = 10000000
n = 0
m = 0.0001
epoch(0.1)
while(n < iter_num):
    if(error_calc(network, 0.00001, 0.00001)):
        break
    print("*****")
    print("epoch : %d", n*500)
    
    prediction_sigmoid(0)
    prediction_sigmoid(1)
    prediction_sigmoid(2)
    prediction_sigmoid(3)
    prediction_sigmoid(4)
    prediction_sigmoid(5)
    prediction_sigmoid(6)
    prediction_sigmoid(7)
    prediction_sigmoid(8)
    prediction_sigmoid(9)
    
    '''
    prediction_softmax(0)
    prediction_softmax(1)
    prediction_softmax(2)
    prediction_softmax(3)
    prediction_softmax(4)
    prediction_softmax(5)
    prediction_softmax(6)
    prediction_softmax(7)
    prediction_softmax(8)
    prediction_softmax(9)
    '''
    error_calc
    
    #print(x['0_i'].reshape(1, 25))
 
#    print(network['w1'])
#    print(network['dw2'])
    
    for j in range(500):
        epoch(1)
    
    n = n + 1
