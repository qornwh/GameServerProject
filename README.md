# boost을 이용한 온라인 게임 서버 구현

## 포트폴리오 문서 주소
- [**포트폴리오**](https://qornwh.github.io/bjsBlog.github.io/posts/OnlineGameportfolio-0/)

## 플레이 시연 영상

---

<iframe width="600" height="400" src="https://www.youtube.com/embed/UeYY-_UfRoY" title="rpg 궁수" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>
<iframe width="600" height="400" src="https://www.youtube.com/embed/bkWg1QWX7Vs" title="rpg 전사" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

## 테스트 시연 영상

---

<iframe width="800" height="515" src="https://www.youtube.com/embed/WXoosvnCQw4" frameborder="0" allowfullscreen></iframe>
<iframe width="800" height="515" src="https://www.youtube.com/embed/k3-c4AiTmxs" frameborder="0" allowfullscreen></iframe>
<iframe width="800" height="515" src="https://www.youtube.com/embed/6-NsowB52Xw" frameborder="0" allowfullscreen></iframe>

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
