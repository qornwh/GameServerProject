# IOCP, boost을 이용한 온라인 게임 서버 구현
 -- IOCP, Boost 모드 2개로 동작됩니다

## git blog 문서 주소
- [**git blog**](https://qornwh.github.io/bjsBlog.github.io/posts/OnlineGameportfolio-0/)

## 플레이 시연 영상

---

[![Video Label](http://img.youtube.com/vi/UeYY-_UfRoY/0.jpg)](https://youtu.be/UeYY-_UfRoY?si=uCtZRGLmAZObMDmo)
[![Video Label](http://img.youtube.com/vi/bkWg1QWX7Vs/0.jpg)](https://youtu.be/bkWg1QWX7Vs)

## 테스트 시연 영상

---

[![Video Label](http://img.youtube.com/vi/WXoosvnCQw4/0.jpg)](https://youtu.be/WXoosvnCQw4)
[![Video Label](http://img.youtube.com/vi/k3-c4AiTmxs/0.jpg)](https://youtu.be/k3-c4AiTmxs)
[![Video Label](http://img.youtube.com/vi/6-NsowB52Xw/0.jpg)](https://youtu.be/6-NsowB52Xw)

## Source Code

- [**Server**](https://github.com/qornwh/GameServerProject)
- [**Client**](https://github.com/Theta08/RpgProject)

## 사용법

1. vcpkg 로 google protobuf 설치 필수
2. CoreLib 빌드 먼저 후 Library 폴더에 lib 파일들 생성 확인 후
3. GameServer로 빌드

### DEBUG

1. Rider에서 빌드 및 디버그 실행

### RELEASE

1. 릴리즈된 파일 실행시 에러뜨면 덤프 만들어짐
2. 사용법은 다음과 같음.
3. rider의 Debug Core Dump.. 실행
4. Core dump path에 GameServer.dmp, Exe path에 GameServer.exe 셋팅 후 OK
5. 에러가 뜬 부분에 디버깅이 걸린다

![3번](https://github.com/qornwh/GameServerProject/blob/main/dump1.png)

_rider의 Debug Core Dump.. 실행_

![4번](https://github.com/qornwh/GameServerProject/blob/main/dump2.png)

_Core dump path에 GameServer.dmp, Exe path에 GameServer.exe 셋팅 후 OK_

![5번](https://github.com/qornwh/GameServerProject/blob/main/dump3.png)

_에러가 뜬 부분에 디버깅이 걸린다_
