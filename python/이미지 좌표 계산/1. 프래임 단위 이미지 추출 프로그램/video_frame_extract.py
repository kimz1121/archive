"""
참고 자료 https://thinking-developer.tistory.com/61


"""

import cv2
import os

def extract_image(filepath, time_gap_sec):
    print(cv2.__version__)

    video = cv2.VideoCapture(filepath) #'' 사이에 사용할 비디오 파일의 경로 및 이름을 넣어주도록 함

    if not video.isOpened():
        print("Could not Open :", filepath)
        exit(0)

    #불러온 비디오 파일의 정보 출력
    length = int(video.get(cv2.CAP_PROP_FRAME_COUNT))
    width = int(video.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(video.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = video.get(cv2.CAP_PROP_FPS)

    print("length :", length)
    print("width :", width)
    print("height :", height)
    print("fps :", fps)

    #프레임을 저장할 디렉토리를 생성
    try:
        if not os.path.exists(filepath[:-4]):
            os.makedirs(filepath[:-4])
    except OSError:
        print ('Error: Creating directory. ' +  filepath[:-4])

    count = 0

    frame_gap = time_gap_sec*fps
    while(video.isOpened()):
        ret, image = video.read()
        if(int(video.get(1)) % (frame_gap) == 0): #앞서 불러온 fps 값을 사용하여 time_gap_sec마다 추출
            cv2.imwrite(filepath[:-4] + "/frame{0:>03}.jpg".format(count), image)
            print('Saved frame number :', str(int(video.get(1))))
            count += 1
            
    video.release()

if __name__ == "__main__":
    video_file_path = './예시영상_20220801_094645827.mp4'# 비디오 자료의 파일 위치
    extract_image(video_file_path, time_gap_sec=2.5)# 2.5 초마다 한장씩 이미지 추출