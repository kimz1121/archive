"""

version : 0.0.40
date : 0906 / 0056
auther : kimz1121@naver.com

language : python

discription :
가천대학교 종합설계 1조 팀의
메카넘 휠에어를 위한 시스템

position_manager와 
Kinematics_mechanum까지 완성된 버전

-> 0.0.10 버전의 코드를 
아두이노 코드와 연동할 수 있도록 변경한 버전 

    -현재 문제점 
        -> progress tugger가 정상 작동하지않음....? 정확히 기억아 나지 않음.
        -> 아두이노와 통신으로 연결되는 부분잉 없다.
        -> 통신된 내용을 받아서 localizer로 전닿할 필요가 있다.
            -통신을 회신하는 방법은 선배님들의 코드를 참고 할것 
        -> 입력되는 단위가 0.1초 단위 정도로 경로의 입력 단위가 커질 필요가 있음
        -> 라즈베리파이의 GPIO와 연관된 부분은 모두 삭제 할것
            -GPIO와 연관된 부분은 모두 Motor 객치와 GPIO_manager 뿐이다.
            -ENGINE의 방식이 크게 달라질 가능성이 있음.

        import serial 
            -> pyserial 라이브러리
            sudo pip3 install pyserial

    0.0.12
        0.0.11 버전이 작동함을 확인 후 
        아두이노 통신과 연결하기위해 소스코드 흐름을 끊을 지점을 찾기 위해 생성한 파일
        __calc_TravelDistance 에서 선회를 처리할 수 없음을 다시 기억함.

    0.0.13
        __calc_TravelDistance 에서 선회를 처리를 일단 간단하게
            x, y, theta 의 변화량을 동시에 주는 것으로 처리함
            -> 해당 방법은 변위가 작을때 좌표의 이동이 선형으로 근사될 수 있음과 
            오차는 증가하겠지만 로컬라이제션 피드백을 통하여 위치를 지속적으로 피드백할 것이기 때문.

    0.0.14
        아두이노와 통신하기 위한 클래스 코드를 추가

    0.0.15
        아두이노 통신용 코드를 invoke하여테스트


    0.0.19
        통신 코드를 완성하였으나 피드백 방식은 아두이노의 느린 응답으로 인해 로봇이 중간중간 정지하는 것을 확인하였음
        그에 따라 
        먼 거리를 이동하는 동안은 피드백을 제한하고 
        단거리 정밀 제어시에만 피드백을 자주 적용하도록 함.

    0.0.20
        Kinematics의 역할을 파이썬 코드 영역으로 다시 편입함.
        -> 주행중 편향의 보정을 위해서는 kinematics 와 localization의 기능이 물리적으로 하나의 코드 스스템에 포함되는 것이 
        실험 및 수정에 유리함. 
        주행 중 편향 보상을 적용함.

        아두이노 코드가 기존의 버전과 더이상 호환되지 않음.
            -> 호환 가능한 버전 : motor_controller_part_0_0_13
    
        -test 버전은 

    0.0.21
        loop time 지연을 해결하기 위해 생성한 파일
        -> 고민해야할 내용, 주행 구간의 세분화를 아두이노에서 해야하는가??, 라즈베리파이에서 해야하는가?
        ->loop time 을 해결 할 수 있는 방식으로 2가지 방법이 제안된다.
            -1.(필수) loop time이 상태에 따라 지연 될 수 있도록 할 것.
                -> 경로 tugger 가 시간 개념으로 움직이는 것이 아닌, 길이 우선으로 작동하도록 함.
            -2. 아두이노에서 경로 세분화를 담당 할 수 있도록 하여, loop time의 크기가 작아질 필요성을 줄일 것. 

        Path_segmentator의
        set_path() 함수의 sallow copy 문제로 인한 경로 분할의 불균일함을 해결함.

    0.0.22
        아두이노 코드에 경로 주행중 중단 기능을 추가함.
        그에 맞추어 주행중 중던신호를 python 차원에서 입력할 수 있도록 기능을 추가함. 

        아두이노 코드가 기존의 버전과 더이상 호환되지 않음.
            -> 호환 가능한 버전 : motor_controller_part_0_0_15

    0.0.23
        send_stop을 이용하기위해서는 
        시스템이 비동기적으로 작동해야만 하는 필요성을 확인하였음.
        get_job_result() 프로세스가 작동하는 send_stop을 포함한 다른 작업을 수 행할 수 없기 때문임.
        따라서 해결 방식은
            -get_job_result() 프로세스 내부에서 중단을 판단하고 명령하거나
            -비동기적 프로그램을 작성하여야 함.
                비동시적 프로그램 작성의 방법은 python 기능을 이용하거나
                ROS를 이용할 수 있음.

        -> 절충안 제안
            Scan용 job 함수를 별도로 작성하는 것이 제안됨.
            일반 get_job_result()함수의 경우 중간에 중단될 필요가 없음에도 중단 여부를 중간중간에 확인하여야 한다...
            특히 중간중간에 확인하기위해 카메라와 AI용 코드를 돌려하야하니 시스템에 부담이 크다.
            따라서 get_job_result()와 get_job_scan_result()를 구분하고자 한다.
            get_job_scan_result()의 경우에만 카메라용 코드와 AI용 코드를 구동하며 AI의 목표물 존재여부를 파악할 수 있도록 하고자 한다.

    0.0.24
        job manager를 작성하여 작업을 묶음단위로 만듦.
        작업중 localizer에 set_position(self, position_in_arg) 함수를 추가함.

    0.0.25
        작업 함수화 및 세분화를 완료한 버전

        AI 판단용 함수는 구조만 작성.

    0.0.26
        Job_manager_0.do_move_free_jobs() 해당 함수가 2번이상 반복하여 실행될 때
        두번 째 수행 부터는 경로의 세분화가 제대로 이루어 지지 않는 점을 확인함.

        tugger에 초기화 기능을 추가하여
        2번째 이후의 수행에서도 세분화가 제대로 이루어 지도록 만들어 문제를 해결함. 

    0.0.29
        카메라 코드와 메카넘 휠 이동코드가 합쳐진 최초의 버전

    0.0.30
        객체 탐지 코드가 반환한 벡터의 방향으로 로봇이 이동하도록 하는 코드 추가


    0.0.33
        
        메카넘 휠 인식후 send_command_stop() 한번만 실행되도록 IF 문을 추가함.
        Job_manager_0.do_move_axis_alert_jobs(Position_0) 내부에서

        data = self.Serial_manager_0.job_get_result_alert()
        해당 함수의 리턴은 data를 제대로 처리하지 못하는 문제가 있었으나
        요소를 개별적으로 할당 시켜주어 문제를 해결함. 
        
        self.steps_displacement_out[0] = data[0]
        self.steps_displacement_out[1] = data[1]
        self.steps_displacement_out[2] = data[2]
        self.steps_displacement_out[3] = data[3]
        find_val = data[4]
        x = data[5]
        y = data[6]

    0.0.34
        0.0.33 버전에서 Job_manager 의 
        def do_move(self) 와 def do_move_alert(self) 함수에 의해
        변수 
        self.steps_displacement_out의 변수를 이용함에 충졸이 발생하는 에로를 해결함

        def do_move(self) 함수는 변수를 튜플로서 반환하여 self.steps_displacement_out 대입하여 사용한 후
        do_move_alert(self) 함수가 self.steps_displacement_out변수의 요소를 변경하려 하자 
        튜플 변수는 요소의 변경이 불가하다는 에러가 발생함.

        해당 문제는 튜플을 
        *self.steps_displacement_out, = return 방식으로 튜플을 리스트가 되도록 언팩하여 문제를 해결함 

    0.0.35
        0.0.34 버전에서 라즈베리파이의 파이썬 스크립트를 통한 
        충전기 리프트 용 아두이노제어 부분을 추가함.  

        +m 라즈베리파이에서 실험하기 전에 /dev/ttyACM0 와 /dev/ttyACM1 으로 USB 연결 포트 설정 변경해 주는 것 잊지 말기 

    0.0.36 
        Job_manager에서 빠뜨린
        경로 보상률 설정을 추가함

    0.0.37
        0.0.36 버전에서 더이상 필요하지 않은 부분을 제거함.
        
    0.0.38 
        주행 부분의 회전 kinematics를 수정함. 

    0.0.39
        Serial manager에 
        Reel 풀어 주기 역할을 첨가하는 중

"""
# import RPi.GPIO as GPIO
import math
import time
import serial
from threading import Lock
import numpy as np

class Position:
    def __init__(self):
        self.x = 0#m 단위
        self.y = 0#m 단위
        self.theta = 0# rad 단위
class Attitude:
    def __init__(self):
        self.phi = 0
class Linear_travelDistance:
    def __init__(self):
        self.x_travelDistance = 0#m 단위
        self.y_travelDistance = 0#m 단위
        self.theta_travelDistance = 0# rad 단위
class Localizer_step:
    def __init__(self):
        self.__position_of_self = Position()

    def set_position(self, position_in_arg):
        self.__position_of_self.x = position_in_arg.x
        self.__position_of_self.y = position_in_arg.y
        self.__position_of_self.theta = position_in_arg.theta

    def get_position(self):
        return self.__position_of_self

    def calc_travledisplacement(self, travle_input):
        #->각도로 받는 것이 아니라 step으로 값을 받을 것 
        delta_x_local = travle_input.x_travelDistance
        delta_y_local = travle_input.y_travelDistance
        delta_theta_local = travle_input.theta_travelDistance

        # print(delta_x_local, delta_y_local)
        x_local = np.array([delta_x_local])
        y_local = np.array([delta_y_local])
        
        XY = np.concatenate([x_local, y_local])
        theta_global = self.__position_of_self.theta 
        rot = np.array([[math.cos(theta_global), -math.sin(theta_global)], [math.sin(theta_global), math.cos(theta_global)]])
        XY_global = rot.dot(XY)
        
        delta_x_global = XY_global[0]
        
        delta_y_global = XY_global[1]
        
        
        delta_theta_global = delta_theta_local
        
        self.__position_of_self.x += delta_x_global
        self.__position_of_self.y += delta_y_global
        self.__position_of_self.theta += delta_theta_global
        
        return self.__position_of_self
class Progress_time_tugger:
    def __init__(self):
        self.__progress_start = 0
        self.__progress_end = 0
        self.__progress_start = 0

        self.__complite = -1#-1 시작 전 -1, 진행 중 0, 완료 1

    def set_progress_now(self, time_out):
        self.__progress_start = time.time()
        self.__progress_end = time.time() + time_out

    def set_progress_future(self, time_start, time_out):
        self.__progress_start = time.time() + time_start
        self.__progress_end = time.time() + time_start + time_out

    def get_progress(self):
        Range = self.__progress_end - self.__progress_start
        temp_now = time.time() -  self.__progress_start

        if(temp_now < 0):
            temp_now = 0
            self.__complite = -1
        elif(Range < temp_now):
            temp_now = Range
            self.__complite = 1
        else:
            self.__complite = 0
        now = temp_now

        return now, Range, self.__complite

    def get_isItComplite(self):
        self.get_progress()
        return self.__complite
class Progress_length_tugger:
    def __init__(self):
        self.__progress_start = 0
        self.__progress_end = 0
        self.__progress_now = 0

        self.__tug_gap = 0.005#m 단위 0.5cm 단위로 경로를 세분화 함.

        self.__complite = -1#-1 시작 전 -1, 진행 중 0, 완료 1

    def do_init_progress(self):
        self.__progress_start = 0
        self.__progress_end = 0
        self.__progress_now = 0
        self.__complite = -1

    def set_progress_now(self, length):
        self.__progress_start = 0
        self.__progress_end = length

    def get_progress(self):
        Range = self.__progress_end - self.__progress_start
        temp_now = self.__progress_start + self.__progress_now

        if(temp_now < 0):
            temp_now = 0
            self.__complite = -1
        elif(Range < temp_now):
            temp_now = Range
            self.__complite = 1
        else:
            self.__complite = 0
        now = temp_now

        return now, Range, self.__complite

    def add_progress(self):
        self.__progress_now += + self.__tug_gap

    def get_isItComplite(self):
        self.get_progress()
        return self.__complite
class path_Query:
    def __init__(self):
        self.path_type = 'l'
        self.path_start = Position()
        self.path_end =  Position()

    def set_path(self, position0, position1):
        self.path_start.x = position0.x
        self.path_start.y = position0.y
        self.path_end.x = position1.x
        self.path_end.y = position1.y

    def get_path_length(self):
        length = math.sqrt((self.path_start.x - self.path_end.x)**2 + (self.path_start.y - self.path_end.y)**2)
        return length
class Path_segmentator:
    def __init__(self):
        self.__position_present = Position()
        self.__position_target = Position()
        self.__internal_division = Position()

    def set_path(self, present, target):#
        self.__position_present.x = present.x
        self.__position_present.y = present.y
        self.__position_present.theta = present.theta
        self.__position_target.x = target.x
        self.__position_target.y = target.y
        self.__position_target.theta = target.theta

        # print("present : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(present.x, present.y, present.theta))
        # print("target : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(target.x, target.y, target.theta))

    def get_segmentation(self, progress, range):
        x_present = self.__position_present.x
        x_target = self.__position_target.x
        y_present = self.__position_present.y
        y_target = self.__position_target.y
        theta_present = self.__position_present.theta
        theta_target = self.__position_target.theta

        if(range > 0):
            if(progress < 0):
                progress = 0#범위를 미달한 값을 입력 받지 않도록 수정
            elif(range < progress):
                #범위를 초과한 값을 입력 받지 않도록 수정
                progress = range

            self.__internal_division.x = x_present + ((x_target - x_present) * (progress / range)) 
            self.__internal_division.y = y_present + ((y_target - y_present) * (progress / range)) 
            self.__internal_division.theta = theta_present + ((theta_target - theta_present) * (progress / range)) 
        else:
            self.__internal_division.x = x_present
            self.__internal_division.y = y_present
            self.__internal_division.theta = theta_present

        return self.__internal_division

class cable_segmentator:
    def __init__(self):
        self.__length_present = 0
        self.__length_target = 0
        self.__internal_division = 0

    def set_path(self, present, target):#
        self.__length_present = present
        self.__length_target = target

    def get_segmentation(self, progress, range):
        length_present = self.__length_present
        length_target = self.__length_target

        if(range > 0):
            if(progress < 0):
                progress = 0#범위를 미달한 값을 입력 받지 않도록 수정
            elif(range < progress):
                #범위를 초과한 값을 입력 받지 않도록 수정
                progress = range
            
            self.__internal_division = length_present + ((length_target - length_present) * (progress / range))  
        else:
            self.__internal_division = length_present

        return self.__internal_division


class Position_manager:
    def __init__(self):
        self.__position_base = Position()
        self.__position_target = Position()
        self.__returnTravelDistance = Linear_travelDistance()

    def set_PositionBase(self, Position_obj_in):
        self.__position_base.x = Position_obj_in.x
        self.__position_base.y = Position_obj_in.y
        self.__position_base.theta = Position_obj_in.theta

    def set_PositionTarget(self, Position_obj_in):
        self.__position_target.x = Position_obj_in.x
        self.__position_target.y = Position_obj_in.y
        self.__position_target.theta = Position_obj_in.theta

    # 계산 원리
    # 변위가 아주 작은 상태에서는 경로의 변화를 항상 선형으로 근사할 수 있다.
    # 따라서 곡선으로 주행하는 구간을 직진 주행과 곡선 주행의 중첩으로 취급할 수 있다.(∵ 중첩의 원리는 선형시스템에서 항상 성립)
    # 따라서 작은 변위에 대해서는 직선 주행과 곡선 중행을 동시에 수행하여 곡선 경로를 표현 할 수 있다.

    #따라서 이번 기구학 클래스는 선형 구산에서 두좌표 사이의 회전과 필요한 직진 주행 거리를 반환한다.

    # 이번 기구학 코드는
    # 주 좌표사이의 이동 거리와 회전각도의 차이를 반환한다.

    # 이동 거리 계산

    # 회전 각도와 주 좌표사이의 변위를 통해 일정한 곡률의 경로를 만든다.

    # 그때의 경로의 회전 반경과 각도를 구하고 서로 곱하여 곡선 구간의 이동거리를 구하고
    # 회전 각은 두 목표 좌표의 각조차를 차하여 구한다.

    def __calc_TravelDistance(self):
        x_displacement = self.__position_target.x - self.__position_base.x
        y_displacement = self.__position_target.y - self.__position_base.y
        theta_displacement = self.__position_target.theta - self.__position_base.theta

        # print(x_displacement, y_displacement) 

        if(abs(theta_displacement) < 0.000001):# 회전 성분 theta 가 없을 때의 이동 거리 반환#회전 성분에 대한 관대 수치를 키워서 회전이 조금 발생하여도 무시하는 것이 좋을 듯.
            #사행 및 병진 주행
            self.__returnTravelDistance.x_travelDistance = x_displacement
            self.__returnTravelDistance.y_travelDistance = y_displacement
            self.__returnTravelDistance.theta_travelDistance = 0

        else:# 회전 성분 theta가 있는 경우들을 열거
            length = math.sqrt(x_displacement**2 + y_displacement**2)
            if(length < 0.000001): #theta가 있고 병진 성분인 x, y 성분이 없는 경우, 제자리 회전 이동거리 정보를 반환
                self.__returnTravelDistance.x_travelDistance = 0
                self.__returnTravelDistance.y_travelDistance = 0
                self.__returnTravelDistance.theta_travelDistance = theta_displacement
            else: #회전 주행 성분과 직진 주행 성분이 같이 존재하는 경우, 곧 선회 주행의 경우
                print("선회 주행 방식은 필수 기동 방식이 아님으로 구현을 차후로 미룸.")
                print("임시 방편으로 제자리 회전으로 대체함.")
                self.__returnTravelDistance.theta_travelDistance = theta_displacement
                pass

            # 선회 주행의 경우 잠시 미뤄두기
            # else:# 회전 주행 성분과 직진 주행 성분이 같이 존재하는 경우, 곧 선회 주행의 경우
            #     # <선회 반경 R과 선회 경로를 따른 이동거리의 길이 구하기>
            #     # 먼저 선회의 회전 각을 구한다.
            #     # theta_displacement # 앞서 구했으으로 생략
            #     # 이동 거리는 절대 값으로써 구해야 하므로 
            #     abs_theta_displacement = abs(theta_displacement)
            #     # 두 좌표 사이를 잇는 직선 거리를 구한다.
            #     # length 로 앞서 구했으므로 생략
            #     # 회전 반경 R 구하기
            #     # 회전 반경은 시작과 목표 좌표 사이 선분을 밑변으로 하고
            #     # 길이가 같은 나머지 두변 사이의 사잇각을 theta로 갖는 이등변 삼각형의
            #     # 이등변 중 하나의 길이이다.
            #     turningRadius = (length/2)/(math.sin(abs_theta_displacement/2))# theta 가 음수인 경우에 대한 예외 처리를 해주기

            #     # 회전 변경의 따른 이동 거리는
            #     # 회전 반경 * 라디안 단위의 회전 각이므로
            #     travelDistance = turningRadius * theta_displacement

            #     # 여기서 선회이동의 정의하기 위해서는 선회의 방향 까지 정의할 수 있어야 한다.
            #     # 선회의 방향은 선회의 뿐만 아니라
            #     # 이동 경로의 시점과 종점 사이를 잇는 벡터의 방향도 중요하다.
    def get_TravelDistance(self):
        self.__calc_TravelDistance()
        return self.__returnTravelDistance

class Kinematics_mechanum:
    def __init__(self):
        self.travel_0 = Linear_travelDistance()#정방향 kinematics의 입력
        self.travel_1 = Linear_travelDistance()#역방향 kinematics의 출력
        self.__wheel_radius = 0.04#m 단위
        self.__radius_of_wheel_array_width = 0.09#m 단위# 회전에 관여하는 휠의 주행방향 성분과 중심사이의 모멘트 팔길이.
        self.__radius_of_wheel_array_heigt = 0.09#m 단위# 회전에 관여하는 휠의 주행방향 성분과 중심사이의 모멘트 팔길이.
        
        self.__lead_angle_of_mechanum_wheel = math.pi/2

        self.__steps_per_rev = 48 * 50 * 2
        
        self.delta_x_local = 0
        self.delta_y_local = 0
        self.delta_yaw_local = 0

        self.__eachWheelTravels = [0, 0, 0, 0]
        self.__wheel_displacement_angles = [0, 0, 0, 0]#FR, FL, RR, RL 순서이다.
        self.__wheel_displacement_steps = [0, 0, 0, 0]#FR, FL, RR, RL 순서이다.

        self.compensation_coefficient_FR = 1
        self.compensation_coefficient_FL = 1
        self.compensation_coefficient_RR = 1
        self.compensation_coefficient_RL = 1

    def set_travel(self, travel_input):
        self.travel_0 = travel_input
    
    def set_wheelSize(self, size):
        self.__wheel_radius = size

    def set_compensation_coefficient(self, coefficient_FR, coefficient_FL, coefficient_RR, coefficient_RL):
        self.compensation_coefficient_FR = coefficient_FR
        self.compensation_coefficient_FL = coefficient_FL
        self.compensation_coefficient_RR = coefficient_RR
        self.compensation_coefficient_RL = coefficient_RL

    def __convert_travelToEachWheelTravel(self):
        x_travel = self.travel_0.x_travelDistance
        y_travel = self.travel_0.y_travelDistance
        theta_travel = self.travel_0.theta_travelDistance

        # 변수 초기화
        FR = 0
        FL = 0
        RR = 0
        RL = 0

        # x 방향 성분
        FR += x_travel
        FL += x_travel
        RR += x_travel
        RL += x_travel

        # y 방향 성분
        FR += -y_travel# = math.tan(__lead_angle_of_mechanum_wheel) ==1 (∵ tan(π/2) == 1)
        FL += +y_travel
        RR += +y_travel
        RL += -y_travel

        # theta 방향 성분(이때 theta는 Radian 단위 이어야 함.)
        FR += +(theta_travel * (self.__radius_of_wheel_array_width + self.__radius_of_wheel_array_heigt))
        FL += -(theta_travel * (self.__radius_of_wheel_array_width + self.__radius_of_wheel_array_heigt))
        RR += +(theta_travel * (self.__radius_of_wheel_array_width + self.__radius_of_wheel_array_heigt))
        RL += -(theta_travel * (self.__radius_of_wheel_array_width + self.__radius_of_wheel_array_heigt))

        #motor num, motor_num, 모터번호, 모터 번호 대응 순서
        self.__eachWheelTravels[0] = FR * self.compensation_coefficient_FR
        self.__eachWheelTravels[1] = FL * self.compensation_coefficient_FL
        self.__eachWheelTravels[2] = RR * self.compensation_coefficient_RR
        self.__eachWheelTravels[3] = RL * self.compensation_coefficient_RL

    def convert_stepsToTravle(self, steps_displacement):
        # print(np.array(steps_displacement).shape)
        steps_fr = steps_displacement[0]
        steps_fl = steps_displacement[1]
        steps_rr = steps_displacement[2]
        steps_rl = steps_displacement[3]
        # print(steps_fr, steps_fl, steps_rr, steps_rl)


        # angle_fr = (steps_fr/(48*50))*2*math.pi
        # angle_fl = (steps_fl/(48*50))*2*math.pi
        # angle_rr = (steps_rr/(48*50))*2*math.pi
        # angle_rl = (steps_rl/(48*50))*2*math.pi

        angle_fr = (steps_fr/(self.__steps_per_rev)) * 2 * math.pi / self.compensation_coefficient_FR
        angle_fl = (steps_fl/(self.__steps_per_rev)) * 2 * math.pi / self.compensation_coefficient_FL
        angle_rr = (steps_rr/(self.__steps_per_rev)) * 2 * math.pi / self.compensation_coefficient_RR
        angle_rl = (steps_rl/(self.__steps_per_rev)) * 2 * math.pi / self.compensation_coefficient_RL

        #print(angle_fr, angle_fl, angle_rr, angle_rl)
        trevel_fr = angle_fr*(self.__wheel_radius)
        trevel_fl = angle_fl*(self.__wheel_radius)
        trevel_rr = angle_rr*(self.__wheel_radius)
        trevel_rl = angle_rl*(self.__wheel_radius)

        delta_x_local = (+ trevel_fr + trevel_fl + trevel_rr + trevel_rl)/4
        #displacemet of x
        delta_y_local = (- trevel_fr + trevel_fl + trevel_rr - trevel_rl)/4
        #displacemet of y
        delta_theta_local = (+ trevel_fr - trevel_fl + trevel_rr - trevel_rl)*math.sin(3*math.pi/4 - math.atan(7.5/9))/math.sqrt(9**2 + 7.5**2)
        #displacemet of theta
        
        self.travel_1.x_travelDistance = delta_x_local
        self.travel_1.y_travelDistance = delta_y_local
        self.travel_1.theta_travelDistance = delta_theta_local


        return self.travel_1

    def __convert_travelToAngles(self):
        self.__wheel_displacement_angles[0] = self.__eachWheelTravels[0] / (self.__wheel_radius)
        self.__wheel_displacement_angles[1] = self.__eachWheelTravels[1] / (self.__wheel_radius)
        self.__wheel_displacement_angles[2] = self.__eachWheelTravels[2] / (self.__wheel_radius)
        self.__wheel_displacement_angles[3] = self.__eachWheelTravels[3] / (self.__wheel_radius)


    def __convert_anglesToSteps(self):
        self.__wheel_displacement_steps[0] = round(self.__wheel_displacement_angles[0] / (2 * math.pi) * self.__steps_per_rev)
        self.__wheel_displacement_steps[1] = round(self.__wheel_displacement_angles[1] / (2 * math.pi) * self.__steps_per_rev)
        self.__wheel_displacement_steps[2] = round(self.__wheel_displacement_angles[2] / (2 * math.pi) * self.__steps_per_rev)
        self.__wheel_displacement_steps[3] = round(self.__wheel_displacement_angles[3] / (2 * math.pi) * self.__steps_per_rev)
    
    # def set_presentAngles(self):
    #     self.__wheel_present_angles = 

    # def set_targetAngles(self, FR, FL, RR, RL):
    #     self.__wheel_target_angles[0] = self.__wheel_present_angles[0] + FR
    #     self.__wheel_target_angles[1] = self.__wheel_present_angles[1] + FL
    #     self.__wheel_target_angles[2] = self.__wheel_present_angles[2] + RR
    #     self.__wheel_target_angles[3] = self.__wheel_present_angles[3] + RL




    def get_stepsDisplacement(self):
        self.__convert_travelToEachWheelTravel()
        self.__convert_travelToAngles()
        self.__convert_anglesToSteps()
        return self.__wheel_displacement_steps

class Serial_manager:
    def __init__(self, port = "/dev/ttyACM0", baudrate = 115200, timeout = 5):
        self.ser = serial.Serial(port = port, baudrate = baudrate, timeout = timeout)
        self.writelock = Lock()
        self.readlock = Lock()

        self.__Liftstate = ""
        self.__Liftresponse = ""

        self.__Wheelstate = ""
        self.__Wheelresponse = ""

        self.__Wheelstep_FR = 0
        self.__Wheelstep_FL = 0
        self.__Wheelstep_RR = 0
        self.__Wheelstep_RL = 0
        
        self.__Wheelstep_FR_cumulated = 0
        self.__Wheelstep_FL_cumulated = 0
        self.__Wheelstep_RR_cumulated = 0
        self.__Wheelstep_RL_cumulated = 0

    def Wheel_send_command_set_steps(self, steps_FR, steps_FL, steps_RR, steps_RL):
        self.ser.reset_input_buffer()
        self.ser.reset_output_buffer()
        command = "w, " + str(steps_FR) + ", " + str(steps_FL) + ", " + str(steps_RR) + ", " + str(steps_RL) + "\n"
        print(command.encode())
        with self.writelock:
            self.ser.write(command.encode())

    def Wheel_send_command_stop(self):
        self.ser.reset_input_buffer()
        self.ser.reset_output_buffer()
        command = "s\n"
        print(command.encode())
        with self.writelock:
            self.ser.write(command.encode())

    def Wheel_get_response(self):
        if self.ser.readable():
            with self.readlock:
                response_raw = self.ser.readline()
            self.__Wheelresponse = response_raw.decode()
            print(self.__Wheelresponse[: len(self.__Wheelresponse)-1])

            return True
        else:
            return False


    def Wheel_wait_for_response(self):
        while(1):
            value = self.Wheel_get_response()
            if(value == True):
                break

    def Wheel_get_state_from_responce(self):
        self.__Wheelstate = self.__Wheelresponse[1]

 
    def Wheel_get_step_in_str(self):

        index_0 = self.__Wheelresponse.find(':', 0)
        index_1 = self.__Wheelresponse.find(',', index_0+1)
        index_2 = self.__Wheelresponse.find(',', index_1+1)
        index_3 = self.__Wheelresponse.find(',', index_2+1)

        str_step_FR = self.__Wheelresponse[index_0+1 : index_1]
        str_step_FL = self.__Wheelresponse[index_1+1 : index_2]
        str_step_RR = self.__Wheelresponse[index_2+1 : index_3]
        str_step_RL = self.__Wheelresponse[index_3+1 : ]

        self.__Wheelstep_FR = int(str_step_FR)
        self.__Wheelstep_FL = int(str_step_FL)
        self.__Wheelstep_RR = int(str_step_RR)
        self.__Wheelstep_RL = int(str_step_RL)

    def Wheel_reset_steps_cumulate(self):
        self.__Wheelstep_FR_cumulated = 0
        self.__Wheelstep_FL_cumulated = 0
        self.__Wheelstep_RR_cumulated = 0
        self.__Wheelstep_RL_cumulated = 0

    def Wheel_cumulate_steps(self):
        self.__Wheelstep_FR_cumulated += self.__Wheelstep_FR
        self.__Wheelstep_FL_cumulated += self.__Wheelstep_FL
        self.__Wheelstep_RR_cumulated += self.__Wheelstep_RR
        self.__Wheelstep_RL_cumulated += self.__Wheelstep_RL

    def Wheel_job_test_result(self):
        rtn_val = -1
        if(self.Wheel_get_response() == True):
            self.Wheel_get_state_from_responce()
            if(self.__Wheelstate == 'f'):
                rtn_val = 1
                self.Wheel_get_step_in_str()
                self.Wheel_cumulate_steps()
            if(self.__Wheelstate == 'd'):
                rtn_val = 2
        else:
            rtn_val = 0
        return rtn_val

    def Wheel_job_get_result_step(self):
        return self.__Wheelstep_FR_cumulated, self.__Wheelstep_FL_cumulated, self.__Wheelstep_RR_cumulated, self.__Wheelstep_RL_cumulated

    def Wheel_job_get_result(self):
        self.Wheel_reset_steps_cumulate()
        while(1):
            if(self.Wheel_get_response() == True):
                self.Wheel_get_state_from_responce()
                if(self.__Wheelstate == 'f'):
                    self.Wheel_get_step_in_str()
                    self.Wheel_cumulate_steps()
                if(self.__Wheelstate == 'd'):
                    break
        return self.__Wheelstep_FR_cumulated, self.__Wheelstep_FL_cumulated, self.__Wheelstep_RR_cumulated, self.__Wheelstep_RL_cumulated



    def Lift_send_command_up(self):
        command = "l,u\n"
        print(command.encode())
        with self.writelock:
            self.ser.write(command.encode())

    def Lift_send_command_stop(self):
        command = "s\n"
        print(command.encode())
        with self.writelock:
            self.ser.write(command.encode())

    def Lift_send_command_down(self):
        command = "l,d\n"
        print(command.encode())
        with self.writelock:
            self.ser.write(command.encode())

    def Lift_get_response(self):
        if self.ser.readable():
            with self.readlock:
                response_raw = self.ser.readline()
            temp = response_raw.decode()
            if(len(temp) > 0):
                self.__Liftresponse = temp
            else:
                self.__Liftresponse = "-timeout\n"

            print("----")
            print(len(self.__Liftresponse))
            print(self.__Liftresponse[: len(self.__Liftresponse)-1])

            return True
        else:
            return False


    def Lift_wait_for_response(self):
        while(1):
            value = self.Lift_get_response()
            if(value == True):
                break

    def Lift_get_state_from_responce(self):
        self.__state = self.__Liftresponse[1]



    def Lift_job_get_result_recive(self):
        # self.reset_steps_cumulate()
        while(1):
            # self.wait_for_response()
            if(self.Lift_get_response() == True):
                self.Lift_get_state_from_responce()
                if(self.__state == 'r'):#s는 standby, r은 recive, d는 done을 의미 한다. 
                    break
            # print(self.__state)
        return self.__state

    def Lift_job_get_result_done(self):
        # self.reset_steps_cumulate()
        while(1):
            # self.wait_for_response()
            if(self.Lift_get_response() == True):
                self.Lift_get_state_from_responce()
                if(self.__state == 'N'):#s는 standby, r은 recive, d는 done을 의미 한다. 
                    break
            # print(self.__state)
        return self.__Liftstate


    def Lift_do_wait_for_sign_weak(self):
        result = self.Lift_job_get_result_done()
        print(result)
        print("콘솔창에 아무키나 입력하면 다음으로 넘어가기")
        input()
        self.Lift_send_command_stop()
        self.Lift_job_get_result_recive()

    def Lift_do_wait_for_sign_strong(self):
        print("콘솔창에 아무키나 입력하면 다음으로 넘어가기")
        input()
        self.Lift_send_command_down()
        time.sleep(5)
        self.Lift_send_command_stop()
        self.Lift_job_get_result_recive()

class Serial_manager_cable:
    def __init__(self, port = "/dev/ttyACM1", baudrate = 115200, timeout = 10):
        self.ser = serial.Serial(port = port, baudrate = baudrate, timeout = timeout)
        self.writelock = Lock()
        self.readlock = Lock()

        self.__state = ""
        self.__response = ""

        self.__step_FR = 0
        self.__step_FL = 0
        self.__step_RR = 0
        self.__step_RL = 0
        self.__step_cable0 = 0
        self.__step_cable1 = 0
        
        self.__step_FR_cumulated = 0
        self.__step_FL_cumulated = 0
        self.__step_RR_cumulated = 0
        self.__step_RL_cumulated = 0
        self.__step_cable0_cumulated = 0
        self.__step_cable1_cumulated = 0

    def send_command_set_steps(self, steps_FR, steps_FL, steps_RR, steps_RL, steps_cable0, steps_cable1):
        self.ser.reset_input_buffer()
        self.ser.reset_output_buffer()
        command = str(steps_FR) + ", " + str(steps_FL) + ", " + str(steps_RR) + ", " + str(steps_RL) + ", " + str(steps_cable0) + ", " + str(steps_cable1) + "\n"
        # command = "100, 0, 0"
        print(command.encode())
        with self.writelock:
            self.ser.write(command.encode())

    def send_command_stop(self):
        self.ser.reset_input_buffer()
        self.ser.reset_output_buffer()
        command = "s\n"
        print(command.encode())
        with self.writelock:
            self.ser.write(command.encode())

    def get_response(self):
        if self.ser.readable():
            with self.readlock:
                response_raw = self.ser.readline()
            self.__response = response_raw.decode()
            # print("----")
            # print(len(self.__response))
            print(self.__response[: len(self.__response)-1])

            return True
        else:
            return False


    def wait_for_response(self):
        while(1):
            value = self.get_response()
            if(value == True):
                break

    def get_state_from_responce(self):
        self.__state = self.__response[1]

 
    def get_step_in_str(self):

        index_0 = self.__response.find(':', 0)
        index_1 = self.__response.find(',', index_0+1)
        index_2 = self.__response.find(',', index_1+1)
        index_3 = self.__response.find(',', index_2+1)
        index_4 = self.__response.find(',', index_3+1)
        
        str_step_FR = self.__response[index_0+1 : index_1]
        str_step_FL = self.__response[index_1+1 : index_2]
        str_step_RR = self.__response[index_2+1 : index_3]
        str_step_RL = self.__response[index_3+1 : index_4]
        str_step_cable0 = self.__response[index_4+1 : ]

        self.__step_FR = int(str_step_FR)
        self.__step_FL = int(str_step_FL)
        self.__step_RR = int(str_step_RR)
        self.__step_RL = int(str_step_RL)
        self.__step_cable0 = int(str_step_cable0)
        self.__step_cable1 = int(0)
        
    def reset_steps_cumulate(self):
        self.__step_FR_cumulated = 0
        self.__step_FL_cumulated = 0
        self.__step_RR_cumulated = 0
        self.__step_RL_cumulated = 0
        self.__step_cable0_cumulated = 0
        self.__step_cable1_cumulated = 0

    def cumulate_steps(self):
        self.__step_FR_cumulated += self.__step_FR
        self.__step_FL_cumulated += self.__step_FL
        self.__step_RR_cumulated += self.__step_RR
        self.__step_RL_cumulated += self.__step_RL
        self.__step_cable0_cumulated += self.__step_cable0
        self.__step_cable1_cumulated += self.__step_cable1

    def job_test_result(self):
        rtn_val = -1
        if(self.get_response() == True):
            self.get_state_from_responce()
            if(self.__state == 'f'):
                rtn_val = 1
                self.get_step_in_str()
                self.cumulate_steps()
            if(self.__state == 'd'):
                rtn_val = 2
        else:
            rtn_val = 0
        return rtn_val
    
    def job_get_result_step(self):
        return self.__step_FR_cumulated, self.__step_FL_cumulated, self.__step_RR_cumulated, self.__step_RL_cumulated, self.__step_cable0_cumulated, self.__step_cable1_cumulated

    def job_get_result(self):
        self.reset_steps_cumulate()
        while(1):
            # self.wait_for_response()
            if(self.get_response() == True):
                self.get_state_from_responce()
                if(self.__state == 'f'):
                    self.get_step_in_str()
                    self.cumulate_steps()
                if(self.__state == 'd'):
                    break
            # print(self.__step_FR_cumulated, self.__step_FL_cumulated, self.__step_RR_cumulated, self.__step_RL_cumulated, self.__step_cable0_cumulated, self.__step_cable1_cumulated)
            # print(self.__state)
        return self.__step_FR_cumulated, self.__step_FL_cumulated, self.__step_RR_cumulated, self.__step_RL_cumulated, self.__step_cable0_cumulated, self.__step_cable1_cumulated

class Job_manager:
    '''
    일의 종류 3가지
        이동            : 한번에 장거리를 이동
        탐색            : s 모양으로 스켄하며 이동 물체 발견시에 정지해야함.
        정밀 접근       : 물체를 발견 하였으면 피드백제어를 통하여 카메라를 물체의 정 중앙에 위치 시킴
        충전 시작       : 카메라 중심위치에 충전 모듈이 가도록 로봇을 정해진 방향으로 조금 이동 시키고 충전 모듈을 상승 시켜야함.
        충전 종료 대기  : 충전이 종료 되었다는 신호를 받을 때까지 대기하기
    '''
    def __init__(self):
        #objs
        #차량의 대수를 늘릴때 차량 단위로 인스턴스화 하여 복제 가능할 부분
        
        #CAR0-head
        self.car0_Position_sample_0 = Position()
        self.car0_Position_sample_1 = Position()
        self.car0_Position_micro_present = Position()
        self.car0_Position_micro_target = Position()
        self.car0_Path_segmentator = Path_segmentator()
        self.car0_localizer = Localizer_step()
        self.car0_Path_query = path_Query()
        self.car0_Kinematics_mechanum = Kinematics_mechanum()

        #CAR1-mid
        self.car1_Position_sample_0 = Position()
        self.car1_Position_sample_1 = Position()
        self.car1_Position_micro_present = Position()
        self.car1_Position_micro_target = Position()
        self.car1_Path_segmentator = Path_segmentator()
        self.car1_localizer = Localizer_step()
        self.car1_Path_query = path_Query()
        self.car1_Kinematics_mechanum = Kinematics_mechanum()

        #cable0
        self.cable0_length_sample_0 = 0
        self.cable0_length_sample_1 = 0
        self.cable0_length_micro_present = 0
        self.cable0_length_micro_target_0 = 0
        self.cable0_segmentator = cable_segmentator()
        #cable1
        self.cable1_length_sample_0 = 0
        self.cable1_length_sample_1 = 0
        self.cable1_length_micro_present = 0
        self.cable1_length_micro_target_0 = 0
        self.cable1_segmentator = cable_segmentator()
        #길이 변화량 계산이 x, y, theta 처럼 여러개가 아니고 length 방향 하나 뿐이므로 path_Query()를 필요로 하지 않음.
        #방향이 하나 뿐이므로 방향에 따른 각 죄표축 방향으로의 길이 변화를 계산해 주는 Localizer가 따로 필요 없음.
        #x, y, theta 를 한번에 계산하여야 하는 것이 아니므로, 세분화 연산도 다르게 사용한다.
        #길이변화가 가장 긴것을 기준으로 length기준-Progress를 설정★ 
        #length to step 계산기 함수 구현하기

        #하나만 사용하여도 괜찮은 객체
        self.Position_manager_0 = Position_manager()
        self.Progress_tugger_0 = Progress_length_tugger()

        #차량의 대수를 늘릴시 복제 불가능한 부분
        self.Serial_manager_0 = Serial_manager()
        self.Serial_manager_1 = Serial_manager_cable()

        #progress manage values
        
        self.complite = -1
        self.now = 0
        self.Range = 0
        #find value
        self.obj_detec_alert = False#앞으로는 필요 없을 변수 AI 탐지 방식에서 SCAN을 하지 않을 것이기 때문, would't be used again
        #travle
        self.car0_travel = Linear_travelDistance()
        self.car1_travel = Linear_travelDistance()
        self.cable0_travel = 0
        self.cable1_travel = 0

        #step value
        self.car0_steps_displacement_in = [0, 0, 0, 0]
        self.car0_steps_displacement_out = [0, 0, 0, 0]
        self.car1_steps_displacement_in = [0, 0, 0, 0]
        self.car1_steps_displacement_out = [0, 0, 0, 0]
        self.cable0_steps_displacement_in = 0
        self.cable0_steps_displacement_out = 0
        self.cable1_steps_displacement_in = 0
        self.cable1_steps_displacement_out = 0

    def set_kinemitics_compensation_coefficient(self, car0_fr, car0_fl, car0_rr, car0_rl, car1_fr, car1_fl, car1_rr, car1_rl):#3
        self.car0_Kinematics_mechanum.set_compensation_coefficient(car0_fr, car0_fl, car0_rr, car0_rl)
        self.car1_Kinematics_mechanum.set_compensation_coefficient(car1_fr, car1_fl, car1_rr, car1_rl)

    def set_start_point(self, car0_Position_in_arg, car1_Position_in_arg, cable0_length_in_arg, cable1_length_in_arg):#1
        self.car0_localizer.set_position(car0_Position_in_arg)
        self.car1_localizer.set_position(car1_Position_in_arg)
        
        self.car0_Position_sample_0.x = car0_Position_in_arg.x
        self.car0_Position_sample_0.y = car0_Position_in_arg.y
        self.car0_Position_sample_0.theta = car0_Position_in_arg.theta

        self.car1_Position_sample_0.x = car1_Position_in_arg.x
        self.car1_Position_sample_0.y = car1_Position_in_arg.y
        self.car1_Position_sample_0.theta = car1_Position_in_arg.theta

        self.cable0_length_sample_0 = cable0_length_in_arg
        self.cable1_length_sample_0 = cable1_length_in_arg
        
    def set_update_start_position(self):#2
        Position_present = self.car0_localizer.get_position()        
        self.car0_Position_micro_present.x = Position_present.x
        self.car0_Position_micro_present.y = Position_present.y
        self.car0_Position_micro_present.theta = Position_present.theta

        Position_present = self.car1_localizer.get_position()
        self.car1_Position_micro_present.x = Position_present.x
        self.car1_Position_micro_present.y = Position_present.y
        self.car1_Position_micro_present.theta = Position_present.theta

        self.cable0_length_micro_present = self.cable0_length_sample_0# 코드의 의미를 micro_present계 변수를 업데이트 해주는 것으로 결정하고 수정함.
        self.cable1_length_micro_present = self.cable1_length_sample_0

    def set_path(self, car0_Position_in_arg, car1_Position_in_arg, cable0_length_in_arg, cable1_length_in_arg):#0
        
        self.car0_Position_sample_1 = car0_Position_in_arg
        self.car1_Position_sample_1 = car1_Position_in_arg
        self.cable0_length_sample_1 = cable0_length_in_arg
        self.cable1_length_sample_1 = cable1_length_in_arg

        self.car0_Path_segmentator.set_path(self.car0_Position_sample_0, car0_Position_in_arg)
        self.car0_Path_query.set_path(self.car0_Position_sample_0, car0_Position_in_arg)
        self.car1_Path_segmentator.set_path(self.car1_Position_sample_0, car1_Position_in_arg)
        self.car1_Path_query.set_path(self.car1_Position_sample_0, car1_Position_in_arg)
        self.cable0_segmentator.set_path(self.cable0_length_sample_0, cable0_length_in_arg)
        self.cable1_segmentator.set_path(self.cable1_length_sample_0, cable1_length_in_arg)

    def set_job_info(self):#진행도 설정 부분
        #경로와 프로세스 등록 및 변수의 상태 지정
        #3개중 가장 길이가 긴것을 진행도 기준으로 설정
        car0_path_length = self.car0_Path_query.get_path_length()
        car1_path_length = self.car1_Path_query.get_path_length()
        cable0_path_length = abs(self.cable0_length_sample_1 - self.cable0_length_sample_0)
        cable1_path_length = abs(self.cable1_length_sample_1 - self.cable1_length_sample_0)
        print(cable1_path_length)
        path_length = max([car0_path_length, car1_path_length, cable0_path_length, cable1_path_length])

        print(path_length)
        self.Progress_tugger_0.do_init_progress()
        self.Progress_tugger_0.set_progress_now(path_length)
        self.now, self.Range, self.complite = self.Progress_tugger_0.get_progress()

        self.obj_detec_alert = False#물체 탐지여부초기화

        self.complite = -1

    def push_progress(self):
        self.Progress_tugger_0.add_progress()
        self.now, self.Range, self.complite = self.Progress_tugger_0.get_progress()
        # print("now : {}, range : {}".format(self.now, self.Range))

    def get_segmentation(self):#6
        Position_micro_target_update = self.car0_Path_segmentator.get_segmentation(self.now, self.Range)
        self.car0_Position_micro_target.x = Position_micro_target_update.x
        self.car0_Position_micro_target.y = Position_micro_target_update.y
        self.car0_Position_micro_target.theta = Position_micro_target_update.theta

        Position_micro_target_update = self.car1_Path_segmentator.get_segmentation(self.now, self.Range)
        self.car1_Position_micro_target.x = Position_micro_target_update.x
        self.car1_Position_micro_target.y = Position_micro_target_update.y
        self.car1_Position_micro_target.theta = Position_micro_target_update.theta

        cable_Position_micro_target_update = self.cable0_segmentator.get_segmentation(self.now, self.Range)
        self.cable0_length_micro_target = cable_Position_micro_target_update

        cable_Position_micro_target_update = self.cable1_segmentator.get_segmentation(self.now, self.Range)
        self.cable1_length_micro_target = cable_Position_micro_target_update

        # print(self.Position_micro_present.x, self.Position_micro_present.y, self.Position_micro_present.theta)
        # print(Position_micro_target.x, Position_micro_target.y, Position_micro_target.theta)
        return Position_micro_target_update

    def get_travel(self):#4
        self.Position_manager_0.set_PositionBase(self.car0_Position_micro_present)
        self.Position_manager_0.set_PositionTarget(self.car0_Position_micro_target)
        travel = self.Position_manager_0.get_TravelDistance()
        self.car0_travel.x_travelDistance = travel.x_travelDistance
        self.car0_travel.y_travelDistance = travel.y_travelDistance
        self.car0_travel.theta_travelDistance = travel.theta_travelDistance

        self.Position_manager_0.set_PositionBase(self.car1_Position_micro_present)
        self.Position_manager_0.set_PositionTarget(self.car1_Position_micro_target)
        travel = self.Position_manager_0.get_TravelDistance()
        self.car1_travel.x_travelDistance = travel.x_travelDistance
        self.car1_travel.y_travelDistance = travel.y_travelDistance
        self.car1_travel.theta_travelDistance = travel.theta_travelDistance
        
        self.cable0_travel = self.cable0_length_micro_target - self.cable0_length_micro_present#self.cable0_length_micro_target을 self.cable0_length_micro_target_0로 잘 못 작성한 오류로 인해 케이블이 작동하지 않았음.
        self.cable1_travel = self.cable1_length_micro_target - self.cable1_length_micro_present

    def get_step(self):#5

        cable0_radius = 0.06#m
        cable1_radius = 0.06#m
        steps_per_rev = 48 * 50 * 2

        self.car0_Kinematics_mechanum.set_travel(self.car0_travel)
        self.car1_Kinematics_mechanum.set_travel(self.car1_travel)

        *self.car0_steps_displacement_in, = self.car0_Kinematics_mechanum.get_stepsDisplacement()
        *self.car1_steps_displacement_in, = self.car1_Kinematics_mechanum.get_stepsDisplacement()

        self.cable0_steps_displacement_in = int((self.cable0_travel / cable0_radius) * steps_per_rev)
        self.cable1_steps_displacement_in = int((self.cable1_travel / cable1_radius) * steps_per_rev)

    def do_move(self):#7
        car0_steps_FR = self.car0_steps_displacement_in[0]
        car0_steps_FL = self.car0_steps_displacement_in[1]
        car0_steps_RR = self.car0_steps_displacement_in[2]
        car0_steps_RL = self.car0_steps_displacement_in[3]

        car1_steps_FR = self.car1_steps_displacement_in[0]
        car1_steps_FL = self.car1_steps_displacement_in[1]
        car1_steps_RR = self.car1_steps_displacement_in[2]
        car1_steps_RL = self.car1_steps_displacement_in[3]

        steps_cable_0 = self.cable0_steps_displacement_in
        steps_cable_1 = self.cable1_steps_displacement_in
        
        self.Serial_manager_0.Wheel_send_command_set_steps(car0_steps_FR, car0_steps_FL, car0_steps_RR, car0_steps_RL)#step 단위 정수만 입력 받을 수 있다.
        self.Serial_manager_1.send_command_set_steps(car1_steps_FR, car1_steps_FL, car1_steps_RR, car1_steps_RL, steps_cable_0, steps_cable_1)#step 단위 정수만 입력 받을 수 있다.
        print("&&&&&")
        print(steps_cable_0, steps_cable_1)
        # Serial_manager_0.send_command_set_steps(steps_FR, 6000, steps_RR, steps_RL)#step 단위 정수만 입력 받을 수 있다.
        self.job_get_result()#여기 내부에 주행 중 동안 실행되는 while 문이 존재, steps_displacement_out을 계산한다.


    def do_move_test(self):#7
        car0_steps_FR = self.car0_steps_displacement_in[0]
        car0_steps_FL = self.car0_steps_displacement_in[1]
        car0_steps_RR = self.car0_steps_displacement_in[2]
        car0_steps_RL = self.car0_steps_displacement_in[3]

        car1_steps_FR = self.car1_steps_displacement_in[0]
        car1_steps_FL = self.car1_steps_displacement_in[1]
        car1_steps_RR = self.car1_steps_displacement_in[2]
        car1_steps_RL = self.car1_steps_displacement_in[3]

        steps_cable_0 = self.cable0_steps_displacement_in
        steps_cable_1 = self.cable0_steps_displacement_in

        self.car0_steps_displacement_out[0] = self.car0_steps_displacement_in[0]
        self.car0_steps_displacement_out[1] = self.car0_steps_displacement_in[1]
        self.car0_steps_displacement_out[2] = self.car0_steps_displacement_in[2]
        self.car0_steps_displacement_out[3] = self.car0_steps_displacement_in[3]
        
        self.car1_steps_displacement_out[0] = self.car1_steps_displacement_in[0]
        self.car1_steps_displacement_out[1] = self.car1_steps_displacement_in[1]
        self.car1_steps_displacement_out[2] = self.car1_steps_displacement_in[2]
        self.car1_steps_displacement_out[3] = self.car1_steps_displacement_in[3]

        self.cable0_steps_displacement_out = self.cable0_steps_displacement_in
        self.cable1_steps_displacement_out = self.cable1_steps_displacement_in

    def do_move_alert(self):
        car0_steps_FR = self.car0_steps_displacement_in[0]
        car0_steps_FL = self.car0_steps_displacement_in[1]
        car0_steps_RR = self.car0_steps_displacement_in[2]
        car0_steps_RL = self.car0_steps_displacement_in[3]

        car1_steps_FR = self.car1_steps_displacement_in[0]
        car1_steps_FL = self.car1_steps_displacement_in[1]
        car1_steps_RR = self.car1_steps_displacement_in[2]
        car1_steps_RL = self.car1_steps_displacement_in[3]

        steps_cable_0 = self.cable0_steps_displacement_in
        steps_cable_1 = self.cable0_steps_displacement_in

        self.Serial_manager_0.Wheel_send_command_set_steps(car0_steps_FR, car0_steps_FL, car0_steps_RR, car0_steps_RL)#step 단위 정수만 입력 받을 수 있다.
        self.Serial_manager_1.send_command_set_steps(car1_steps_FR, car1_steps_FL, car1_steps_RR, car1_steps_RL, steps_cable_0, steps_cable_1)#step 단위 정수만 입력 받을 수 있다.

        # Serial_manager_0.send_command_set_steps(steps_FR, 6000, steps_RR, steps_RL)#step 단위 정수만 입력 받을 수 있다.
        self.job_get_result()#여기 내부에 주행 중 동안 실행되는 while 문이 존재, steps_displacement_out을 계산한다.

        # self.steps_displacement_out[0] = data0[0]
        # self.steps_displacement_out[1] = data0[1]
        # self.steps_displacement_out[2] = data0[2]
        # self.steps_displacement_out[3] = data0[3]
        # find_val = data[4]
        # x = data[5]
        # y = data[6]

        # return find_val, x, y


    def do_localization(self):
        car0_travle_displacement = self.car0_Kinematics_mechanum.convert_stepsToTravle(self.car0_steps_displacement_out)
        car1_travle_displacement = self.car1_Kinematics_mechanum.convert_stepsToTravle(self.car1_steps_displacement_out)
        
        self.car0_localizer.calc_travledisplacement(car0_travle_displacement)
        Position_micro_present_update = self.car0_localizer.get_position()
        self.car0_Position_micro_present.x = Position_micro_present_update.x
        self.car0_Position_micro_present.y = Position_micro_present_update.y
        self.car0_Position_micro_present.theta = Position_micro_present_update.theta

        self.car1_localizer.calc_travledisplacement(car1_travle_displacement)
        Position_micro_present_update = self.car1_localizer.get_position()
        self.car1_Position_micro_present.x = Position_micro_present_update.x
        self.car1_Position_micro_present.y = Position_micro_present_update.y
        self.car1_Position_micro_present.theta = Position_micro_present_update.theta
        
        cable0_radius = 0.06#m
        cable1_radius = 0.06#m
        steps_per_rev = 48 * 50 * 2

        self.cable0_length_micro_present += (self.cable0_steps_displacement_out / steps_per_rev) * cable0_radius
        self.cable1_length_micro_present += (self.cable1_steps_displacement_out / steps_per_rev) * cable1_radius

    def find_alert(self):
        if(self.obj_detec_alert == True):
            self.complite = 1

    def set_update_present_postion(self):
        self.car0_Position_sample_0.x = self.car0_Position_micro_present.x
        self.car0_Position_sample_0.y = self.car0_Position_micro_present.y
        self.car0_Position_sample_0.theta = self.car0_Position_micro_present.theta

        self.car1_Position_sample_0.x = self.car1_Position_micro_present.x
        self.car1_Position_sample_0.y = self.car1_Position_micro_present.y
        self.car1_Position_sample_0.theta = self.car1_Position_micro_present.theta
        
        self.cable0_length_sample_0 = self.cable0_length_micro_present
        self.cable1_length_sample_0 = self.cable1_length_micro_present

    def send_stop(self):
        self.Serial_manager_0.Wheel_send_command_stop()

    def job_get_result(self):
        self.Serial_manager_0.Wheel_reset_steps_cumulate()
        self.Serial_manager_1.reset_steps_cumulate()
        val_0 = 0
        val_1 = 0
        while(1):
            if(val_0 != 2):
                val_0 = self.Serial_manager_0.Wheel_job_test_result()
            if(val_1 != 2):
                val_1 = self.Serial_manager_1.job_test_result()

            if (val_0 == 2 and val_1 == 2):
                #self.complite = 1
                break

        *self.car0_steps_displacement_out, = self.Serial_manager_0.Wheel_job_get_result_step()
        
        *self.car1_steps_displacement_out, self.cable0_steps_displacement_out, self.cable1_steps_displacement_out = self.Serial_manager_1.job_get_result_step()
        print("$$$$$")
        print(self.car0_steps_displacement_out)
        print(self.car1_steps_displacement_out, self.cable0_steps_displacement_out, self.cable1_steps_displacement_out)


    def do_move_free_jobs(self, car0_Position_in_arg, car1_Position_in_arg, cable0_length_in_arg, cable1_length_in_arg):
        self.set_update_start_position()#micro_present 업데이트
        self.set_path(car0_Position_in_arg, car1_Position_in_arg, cable0_length_in_arg, cable1_length_in_arg)
        self.set_job_info()#/

        self.get_segmentation()#세분화 함수
        self.get_travel()#세분화 사이의 거리차 계산 함수
        self.get_step()#계산된 거리 스텝값 불러오기 함수

        print("*#Ready#*")
        print("pr : {:>0.5f}, {:>0.5f}".format(self.now, self.Range))
        print("P0 : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_sample_0.x, self.car1_Position_sample_0.y, self.car1_Position_sample_0.theta))
        print("P1 : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_sample_1.x, self.car1_Position_sample_1.y, self.car1_Position_sample_1.theta))
        print("tr : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_travel.x_travelDistance, self.car1_travel.y_travelDistance, self.car1_travel.theta_travelDistance))
        print("Pp : {:>0.5f}, {:>0.5f}, {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_micro_present.x, self.car1_Position_micro_present.y, self.car1_Position_micro_present.theta, self.cable0_length_micro_present, self.cable1_length_micro_present))
        print("Pt : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_micro_target.x, self.car1_Position_micro_target.y, self.car1_Position_micro_target.theta))
        print(self.car1_steps_displacement_in)

        while(self.complite != 1):
            self.push_progress()#진행도 함수, 완료시 self.complite = 1 연산 수행 
            self.get_segmentation()#세분화 함수
            self.get_travel()#세분화 사이의 거리차 계산 함수
            self.get_step()#계산된 거리 스텝값 불러오기 함수

            print("*moving*")
            print("pr : {:>0.5f}, {:>0.5f}".format(self.now, self.Range))
            print("P0 : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_sample_0.x, self.car1_Position_sample_0.y, self.car1_Position_sample_0.theta))
            print("P1 : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_sample_1.x, self.car1_Position_sample_1.y, self.car1_Position_sample_1.theta))
            print("tr : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_travel.x_travelDistance, self.car1_travel.y_travelDistance, self.car1_travel.theta_travelDistance))
            print("Pp : {:>0.5f}, {:>0.5f}, {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_micro_present.x, self.car1_Position_micro_present.y, self.car1_Position_micro_present.theta, self.cable0_length_micro_present, self.cable1_length_micro_present))
            print("Pt : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_micro_target.x, self.car1_Position_micro_target.y, self.car1_Position_micro_target.theta))
            print(self.car1_steps_displacement_in)

            self.do_move()#불러와서 대기된 스텝값 만큼 이동하는 함수
            self.do_localization()#이동한 변위를 누적하는 로컬라이제이션 함수
            self.find_alert()#ai 탐지 함수
        self.set_update_present_postion()#이동후 로컬라이제이션 된 위치로 현재 위치를 재지정하는 함수

        print("*+done+*")
        print("pr : {:>0.5f}, {:>0.5f}".format(self.now, self.Range))
        print("P0 : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_sample_0.x, self.car1_Position_sample_0.y, self.car1_Position_sample_0.theta))
        print("P1 : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_sample_1.x, self.car1_Position_sample_1.y, self.car1_Position_sample_1.theta))
        print("tr : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_travel.x_travelDistance, self.car1_travel.y_travelDistance, self.car1_travel.theta_travelDistance))
        print("Pp : {:>0.5f}, {:>0.5f}, {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_micro_present.x, self.car1_Position_micro_present.y, self.car1_Position_micro_present.theta, self.cable0_length_micro_present, self.cable1_length_micro_present))
        print("Pt : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_micro_target.x, self.car1_Position_micro_target.y, self.car1_Position_micro_target.theta))
        print(self.car1_steps_displacement_in)

        return self.obj_detec_alert#물체를 탐지하고 종료되었는지 물체를 탐지하지 못하고 종료 되었는지 알수 있음

    def do_move_free_jobs_test(self, car0_Position_in_arg, car1_Position_in_arg, cable0_length_in_arg, cable1_length_in_arg):
        self.set_update_start_position()#micro_present 업데이트
        self.set_path(car0_Position_in_arg, car1_Position_in_arg, cable0_length_in_arg, cable1_length_in_arg)
        self.set_job_info()#/

        self.get_segmentation()#세분화 함수
        self.get_travel()#세분화 사이의 거리차 계산 함수
        self.get_step()#계산된 거리 스텝값 불러오기 함수

        print("*#Ready#*")
        print("pr : {:>0.5f}, {:>0.5f}".format(self.now, self.Range))
        print("P0 : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_sample_0.x, self.car1_Position_sample_0.y, self.car1_Position_sample_0.theta))
        print("P1 : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_sample_1.x, self.car1_Position_sample_1.y, self.car1_Position_sample_1.theta))
        print("tr : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_travel.x_travelDistance, self.car1_travel.y_travelDistance, self.car1_travel.theta_travelDistance))
        print("Pp : {:>0.5f}, {:>0.5f}, {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_micro_present.x, self.car1_Position_micro_present.y, self.car1_Position_micro_present.theta, self.cable0_length_micro_present, self.cable1_length_micro_present))
        print("Pt : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_micro_target.x, self.car1_Position_micro_target.y, self.car1_Position_micro_target.theta))
        print(self.car1_steps_displacement_in)

        while(self.complite != 1):
            self.push_progress()#진행도 함수, 완료시 self.complite = 1 연산 수행 
            self.get_segmentation()#세분화 함수
            self.get_travel()#세분화 사이의 거리차 계산 함수
            self.get_step()#계산된 거리 스텝값 불러오기 함수

            print("*moving*")
            print("pr : {:>0.5f}, {:>0.5f}".format(self.now, self.Range))
            print("P0 : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_sample_0.x, self.car1_Position_sample_0.y, self.car1_Position_sample_0.theta))
            print("P1 : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_sample_1.x, self.car1_Position_sample_1.y, self.car1_Position_sample_1.theta))
            print("tr : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_travel.x_travelDistance, self.car1_travel.y_travelDistance, self.car1_travel.theta_travelDistance))
            print("Pp : {:>0.5f}, {:>0.5f}, {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_micro_present.x, self.car1_Position_micro_present.y, self.car1_Position_micro_present.theta, self.cable0_length_micro_present, self.cable1_length_micro_present))
            print("Pt : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_micro_target.x, self.car1_Position_micro_target.y, self.car1_Position_micro_target.theta))
            print(self.car1_steps_displacement_in)

            self.do_move_test()#불러와서 대기된 스텝값 만큼 이동하는 함수
            self.do_localization()#이동한 변위를 누적하는 로컬라이제이션 함수
            self.find_alert()#ai 탐지 함수
        self.set_update_present_postion()#이동후 로컬라이제이션 된 위치로 현재 위치를 재지정하는 함수

        print("*+done+*")
        print("pr : {:>0.5f}, {:>0.5f}".format(self.now, self.Range))
        print("P0 : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_sample_0.x, self.car1_Position_sample_0.y, self.car1_Position_sample_0.theta))
        print("P1 : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_sample_1.x, self.car1_Position_sample_1.y, self.car1_Position_sample_1.theta))
        print("tr : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_travel.x_travelDistance, self.car1_travel.y_travelDistance, self.car1_travel.theta_travelDistance))
        print("Pp : {:>0.5f}, {:>0.5f}, {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_micro_present.x, self.car1_Position_micro_present.y, self.car1_Position_micro_present.theta, self.cable0_length_micro_present, self.cable1_length_micro_present))
        print("Pt : {:>0.5f}, {:>0.5f}, {:>0.5f}".format(self.car1_Position_micro_target.x, self.car1_Position_micro_target.y, self.car1_Position_micro_target.theta))
        print(self.car1_steps_displacement_in)

        return self.obj_detec_alert#물체를 탐지하고 종료되었는지 물체를 탐지하지 못하고 종료 되었는지 알수 있음

    def do_move_axis_jobs(self, car0_Position_in_arg, car1_Position_in_arg, cable0_length_in_arg, cable1_length_in_arg):
        self.set_update_start_position()
        self.set_path(car0_Position_in_arg, car1_Position_in_arg, cable0_length_in_arg, cable1_length_in_arg)
        self.set_job_info()
        
        self.get_travel()
        
        self.get_step()
        self.do_move()
        self.complite = 1
        self.do_localization()
        self.set_update_present_postion()

    def do_lift_up(self):
        self.Serial_manager_0.Lift_send_command_up()
        self.Serial_manager_0.Lift_job_get_result_done()

    def do_lift_up_test_weak(self):
        self.Serial_manager_0.Lift_send_command_up()
        self.Serial_manager_0.Lift_do_wait_for_sign_weak()

    def do_lift_up_test_strong(self):
        self.Serial_manager_0.Lift_send_command_up()
        self.Serial_manager_0.Lift_do_wait_for_sign_strong()

if __name__ == '__main__':
    Job_manager_0 = Job_manager()
    Position_0 = Position()
    Position_1 = Position()
    cable0_length = 0
    cable1_length = 0

    time.sleep(3)

#<0>초기 좌표 설정====================

    # 초기 위치를 편의상 모두 0으로 설정함.

    #Position_0 Robot0 (머리 로봇)
    Position_0.x = 0.0
    Position_0.y = 0.0
    Position_0.theta = 0.0
    #Position_0 Robot1 (중간 로봇)
    Position_1.x = 0.0
    Position_1.y = 0.0
    Position_1.theta = 0.0
    #cable0_length 중간로봇 케이블 (머리-중간 사이 케이블 길이)
    cable0_length = 0.0
    #cable0_length 중간로봇 케이블 (중간-집 사이 케이블 길이)
    cable1_length = 0.0#사용하지 않는다면 계속 0으로 사용할 것.

    Job_manager_0.set_start_point(Position_0, Position_1, cable0_length, cable1_length)#각 로봇의 위치를 받고 작동, 이동하는 함수
    # Job_manager_0.do_move_free_jobs(Position_0, Position_1, cable0_length, cable1_length)#각 로봇의 위치를 받고 작동, 이동하는 함수
    # Job_manager_0.do_move_free_jobs_test(Position_0, Position_1, cable0_length, cable1_length)#프린트만 하는 테스트용 함수

#<1>목표 좌표 설정#1====================
    print("========================================")

    #Position_0 Robot0 (머리 로봇)
    Position_0.x = 0.05
    Position_0.y = 0.0
    Position_0.theta = 0.0
    #Position_0 Robot1 (중간 로봇)
    Position_1.x = 0.05
    Position_1.y = 0.0
    Position_1.theta = 0.0
    #cable0_length 중간로봇 케이블 (머리-중간 사이 케이블 길이)
    cable0_length = 0.01    #cable0_length 중간로봇 케이블 (중간-집 사이 케이블 길이)
    cable1_length = 0.0#사용하지 않는다면 계속 0으로 사용할 것.

    Job_manager_0.do_move_free_jobs(Position_0, Position_1, cable0_length, cable1_length)#각 로봇의 위치를 받고 작동, 이동하는 함수
    # Job_manager_0.do_move_free_jobs_test(Position_0, Position_1, cable0_length, cable1_length)#프린트만 하는 테스트용 함수

#<2>목표 좌표 설정#2====================
    print("========================================")

    #Position_0 Robot0 (머리 로봇)
    Position_0.x = 0.0
    Position_0.y = 0.0
    Position_0.theta = 0.0
    #Position_0 Robot1 (중간 로봇)
    Position_1.x = -0.05
    Position_1.y = 0.0
    Position_1.theta = 0.0
    #cable0_length 중간로봇 케이블 (머리-중간 사이 케이블 길이)
    cable0_length = 0.01
    #cable0_length 중간로봇 케이블 (중간-집 사이 케이블 길이)
    cable1_length = 0.0#사용하지 않는다면 계속 0으로 사용할 것.

    Job_manager_0.do_move_free_jobs(Position_0, Position_1, cable0_length, cable1_length)#각 로봇의 위치를 받고 작동, 이동하는 함수
    # Job_manager_0.do_move_free_jobs_test(Position_0, Position_1, cable0_length, cable1_length)#프린트만 하는 테스트용 함수

#<3>리니어 액추에이터 가동#1====================
    print("========================================")

    # Job_manager_0.do_lift_up_test_strong()# 키보드 입력을 하여야 리니어 액추에이터의 상태가 복구됨.

#<4>목표 좌표 설정#3====================
    print("========================================")
    
    #Position_0 Robot0 (머리 로봇)
    Position_0.x = 0.0
    Position_0.y = 0.0
    Position_0.theta = 0.0
    #Position_0 Robot1 (중간 로봇)
    Position_1.x = 0.0
    Position_1.y = 0.0
    Position_1.theta = 0.0
    #cable0_length 중간로봇 케이블 (머리-중간 사이 케이블 길이)
    cable0_length = 0.0
    #cable0_length 중간로봇 케이블 (중간-집 사이 케이블 길이)
    cable1_length = 0.0#사용하지 않는다면 계속 0으로 사용할 것.

    Job_manager_0.do_move_free_jobs(Position_0, Position_1, cable0_length, cable1_length)#각 로봇의 위치를 받고 작동, 이동하는 함수
    # Job_manager_0.do_move_free_jobs_test(Position_0, Position_1, cable0_length, cable1_length)#프린트만 하는 테스트용 함수

    print("========================================")


"""

★USB 포트 설정★
    리눅스 환경 기준
        일반적으로 
            /dev/ttyACM0
            /dev/ttyAMA2
            /dev/ttyUSB0
            꼴
        대표 오류
            dev/ttyACM0
            /dev/ttyAMC0
    
    연결이 불량할 때 마다. USB의 번호가 바뀔 수 있음에 주의
        이때는 USB를 하나씪 꼽아서 확인할 것
        
        확인 명령어 
        리눅스 콘솔창에
            ls /dev
        입력 후 결과 목록중에 
        ttyACM, ttyAMA, ttyUSB 과 유사한 것을 찾으면 아두이노 통신일 것이다.

코드상 각 설정 위치

robot 0 (머리로봇 USB 포트 설정)
    line 627 
        Serial_maneger 생성자 함수 2번째 인수 port에 대입하여 사용

robot 1 (중간로봇 USB 포트 설정)
    line 752
        Serial_manager_cable 생성자 함수 2번째 인수 port에 대입하여 사용

lift (머리로봇 USB 포트 설정)
    line 893
        Serial_manager_lift 생성자 함수 2번째 인수 port에 대입하여 사용

"""