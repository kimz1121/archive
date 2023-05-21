# Image-Editing-Program(20210111~)
> 2020 JJE++ PROJECT   
> This repository is founded for Image Editing Program team of JJE++ study group.

## 사용법
추후 작성 예정

## 참고 자료
1. openCV_이미지를 영상으로 변환   
https://diyver.tistory.com/50   
https://nowonbun.tistory.com/745

+) openCV의 Mat(matrix)라는 클래스.

2. 이미지 확대, 축소   
https://oneday0012.tistory.com/17

3. 파일 입출력   
https://m.blog.naver.com/PostView.nhn?blogId=tkdldjs35&logNo=221308244184&   proxyReferer=https:%2F%2Fwww.google.com%2F

4. bitmap 구조   
https://kevinthegrey.tistory.com/57
https://dojang.io/mod/page/view.php?id=702
https://msdn.microsoft.com/ko-kr/library/windows/desktop/dd183374(v=vs.85).aspx
https://msdn.microsoft.com/ko-kr/library/windows/desktop/dd183376(v=vs.85).aspx
https://msdn.microsoft.com/ko-kr/library/windows/desktop/dd162939(v=vs.85).aspx

5. 16진수 색상 코드   
http://www.hipenpal.com/tool/html-color-charts-rgb-color-table-in-korean.php

6. 

## 저작권
추후 작성 예정



>>상우가 쓴 내용
## SDL 한정 사용법
0. 개발환경은 리눅스 입니다.

1. necessary_basic_ommand.txt 을 읽고 프로그램을 사용하기 권장합니다.

2. 실행파일은 리눅스환경의 실행 파일이며
SDL_basic/window/executable 와 
SDL_basic/Image_float/executable 위치에 모아 두었습니다.

3. 실행 리눅스 명령어
예제로 사용가능한 실행파일들의 명령어는 아래와 같습니다.
(
    실행이 안 될 때에는 실행권한을 
    ls -l 명령어로 확인해주시고 필요에 따라
    chmod 명령어로 실행권한을 부여해주세요.
)

./SDL_basic/window/executable/first_window_2

./SDL_basic/Image_float/executable/first_bitmap_float_2
./SDL_basic/Image_float/executable/first_bitmap_float_3

4. 코드를 컴파일할 때에는
이름 설정과 라이브러리 포함에 주의하며 컴파일하여야 합니다.
(컴파일이 안되는 경우에는 작성자의 개발환경과 같이 g++ 컴파일러를 추가로 설치하신 후 다시 시도해 보시기 바랍니다.)
(설치 확인 명령어 : g++ --version)
(설치 명령어 : sudo apt install g++)

예제로 사용가능한 컴파일 명령어는 아래와 같습니다.

g++ SDL_basic/window/window_sample_iw_2.cpp -o SDL_basic/window/executable/first_window_2 -lSDL2 -ldl

g++ SDL_basic/Image_float/bitmap_float_sample_iw_2.cpp -o SDL_basic/Image_float/executable/first_bitmap_float_2 -lSDL2 -ldl

이상입니다.