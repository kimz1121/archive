from bs4 import BeautifulSoup
import numpy as np
import pandas as pd

import math

def convert_cordinate(x_in, y_in):
    x_out = x_in
    y_out = 1080 - y_in
    return x_out, y_out

def box_centor(box):
    x_min = box[0]
    y_min = box[1]
    x_max = box[2]
    y_max = box[3]

    centor_x = (x_min + x_max)/2
    centor_y = (y_min + y_max)/2
    
    return centor_x, centor_y

def axis_calc(y_p_box, y_m_box, length_between_box_centor):#y 축 
    #length_between_box : mm 단위 
    #1. 2 상자의 중심 좌표
    #2. 상자 중심과 중심 사이의 좌표

    y_p_centor_x, y_p_centor_y = box_centor(y_p_box)
    y_m_centor_x, y_m_centor_y = box_centor(y_m_box)

    origin_x = (y_p_centor_x + y_m_centor_x)/2
    origin_y = (y_p_centor_y + y_m_centor_y)/2

    displacement_x = (y_p_centor_x - y_m_centor_x)
    displacement_y = (y_p_centor_y - y_m_centor_y)

    theta = math.atan2(displacement_y, displacement_x)

    pixel_lenth = math.sqrt((displacement_x**2 + displacement_y**2))

    unit_pixel_length = 1000*(pixel_lenth/length_between_box_centor)

    end_1_x = unit_pixel_length * math.cos(theta - math.pi/2)
    end_1_y = unit_pixel_length * math.sin(theta - math.pi/2)

    return unit_pixel_length, origin_x, origin_y, end_1_x, end_1_y

def relative_position_calc(box, unit_pixel_length, origin_x, origin_y, end_1_x, end_1_y):
    box_centor_x, box_centor_y = box_centor(box)
    
    position_vector_x = box_centor_x - origin_x
    position_vector_y = box_centor_y - origin_y

    position_vector = np.array([[position_vector_x, position_vector_y]])
    
    bias_x = np.array([[end_1_x, end_1_y]])
    rotation_mat = np.array([[0, -1], 
                                [1, 0]])

    # end_2_x = 0 * end_1_x + -1 * end_1_y
    # end_2_y = 1 * end_1_x + 0 * end_1_y

    bias_y = (rotation_mat @ (bias_x.T)).T

    # print(end_2_x, end_2_y)
    # print(position_vector)
    # print(bias_x)
    # print(bias_y)

    #x diretion element
    a = position_vector @ (bias_x.T) / math.hypot(bias_x[0][0], bias_x[0][1])
    abs_dx = a[0][0]/unit_pixel_length

    #x diretion diatnace
    b = position_vector @ (bias_y.T) / math.hypot(bias_y[0][0], bias_y[0][1])
    abs_dy = b[0][0]/unit_pixel_length

    # print(abs_dx)
    # print(abs_dy)
    # print(math.hypot(abs_dx, abs_dy) * 110 / 0.9)
    return abs_dx, abs_dy

def read_absolute_origin_list(file_path):
    df_origin_list = pd.read_csv(file_path)
    print("-----------")
    print(df_origin_list)
    print(df_origin_list.shape)
    abs_origin_set_list = list()
    for i in range(df_origin_list.shape[0]):
        val = df_origin_list.iloc[i][["x", "y"]]
        abs_origin_x = val[0]
        abs_origin_y = val[1]
        print(abs_origin_x, abs_origin_y)
        abs_origin_set = list([abs_origin_x, abs_origin_y])
        abs_origin_set_list.append(abs_origin_set)

    return abs_origin_set_list

def absolute_position_calc(abs_origin_x, abs_origin_y, abs_dx, abs_dy):
    return (abs_origin_x + abs_dx), (abs_origin_y + abs_dy)

def calc_a_object(record_arg, object_name, origin_x_arg, origin_y_arg, length_between_box_centor):
    val = axis_calc(record_arg["y_+_axis_vector"], record_arg["y_-_axis_vector"], length_between_box_centor)
    unit_pixel_length, origin_x, origin_y, end_1_x, end_1_y = val
    dx, dy = relative_position_calc(record_arg[object_name], unit_pixel_length, origin_x, origin_y, end_1_x, end_1_y)
    abs_dx, abs_dy = absolute_position_calc(origin_x_arg, origin_y_arg, dx, dy)

    return abs_dx, abs_dy

def calc_a_iamge(record_arg, name_list, origin_x_arg, origin_y_arg, length_between_box_centor):
    abs_record = list()
    for i in name_list:
        print(i)
        abs_dx, abs_dy = calc_a_object(record_arg, i, origin_x_arg, origin_y_arg, length_between_box_centor)
        abs_record.append(abs_dx)
        abs_record.append(abs_dy)
    return abs_record

def calc_a_video(abs_origin_list, relative_position_list, name_list, length_between_box_centor = 900/2):
    # length_between_box_centor
    # 해당 인수는 Y축 래이블링 박스의 중심과 중심사이의 거리로 
    # Y축을 대칭적으로 래이블링 하였다면, 한 Y 축의 길이와 같다.  
    abs_position_list = list()

    for i in range(len(relative_position_list)):
        relative_position_record = relative_position_list[i]
        origin = abs_origin_list[i]
        origin_x = origin[0]
        origin_y = origin[1]
        
        abs_position_record = calc_a_iamge(relative_position_record, name_list, origin_x, origin_y, length_between_box_centor)
        abs_position_list.append(abs_position_record)
    return abs_position_list

# print(soup)

# print(soup.find("name"))
# print(soup.find_all("name"))

# print(soup.select("object"))

def get_data_list(folder_path, data_len, reverse_option=0):
    data_list = []
    for i in range(data_len):#데이터의 길이 지정
        data = {}
        print(i)
        fp = open(folder_path + "/label/frame{0:>03}.xml".format(i), "r")
        soup = BeautifulSoup(fp, "html.parser")
        objects = soup.select("object")
        size = soup.select_one("size")
        len_x_tag = size.select_one("width")
        len_y_tag = size.select_one("height")
        len_x = int(len_x_tag.get_text())
        len_y = int(len_y_tag.get_text())
        # print(len_x)
        # print(len_y)
        for i in objects: 
            
            name = i.select_one("name").get_text()
            bndbox = i.select_one("bndbox")

            x_min = int(bndbox.select_one("xmin").get_text())
            y_min = int(bndbox.select_one("ymin").get_text())
            x_max = int(bndbox.select_one("xmax").get_text())
            y_max = int(bndbox.select_one("ymax").get_text())
            
            # print(name)
            # print(bndbox)
            # print(x_min)
            # print(y_min)
            # print(x_max)
            # print(y_max)
            if reverse_option == 0:
                data[name] = [x_min, y_min, x_max, y_max]# y 좌표는 원점이 사진의 좌측 아래가 되도록 변환한다. 화면 오른쪽 +x, 회면 위쪽 +y 
            elif reverse_option == 1:#상하 반전
                data[name] = [x_min, len_y - y_min, x_max, len_y - y_max]# y 좌표는 원점이 사진의 좌측 아래가 되도록 변환한다. 화면 오른쪽 +x, 회면 위쪽 +y 
            elif reverse_option == 2:#좌우 반전
                data[name] = [len_x - x_min, y_min, len_x - x_min, y_max]
            
            # data_list.append(data)
            data_dict = dict(sorted(data.items()))
        data_list.append(data_dict)
    return data_list



# main 부분

if __name__ == "__main__":
    #------<데이터 읽어오기>------
    relative_position_list = get_data_list("입력폴더", data_len = 64, reverse_option=1)
    # "레이블링 프로그램을 통한 픽셀 단위 상태좌표 자료"의 입력 경로
    # 예시자료는 영상의 상하가 뒤집혔으므로 reverse_option=1 적용하여 좌표의 상하 관계를 바꾸어 준다.

    abs_origin_list = read_absolute_origin_list('absolute_origin_list.csv')
    # 기준 좌표의 절대좌표를 불러오는 부분

    #------<계산하기>------
    name_list = ["e0", "r1", "r2", "r3", "r4"]
    result = calc_a_video(abs_origin_list, relative_position_list, name_list, 900)
    # print(result)

    #------<결과 출력하기>------
    df = pd.DataFrame(result, columns=["e0_x", "e0_y", "r1_x", "r1_y", "r2_x", "r2_y", "r3_x", "r3_y", "r4_x", "r4_y"])
    df.to_csv("data.csv", index=False)#CSV 파일로 결과를 저장
    # 출력파일의 이름