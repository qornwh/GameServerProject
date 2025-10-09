# Hunters Online

## 소개

- 기간 : 2023.12 ~ 2024.05
- 인원 : 2명 (서버 담당(c++), 클라이언트(Unity) 네트워크 코드 담당)
- 개발환경 : Windows
- 라이브러리 : Google::protobuf, Boost::Json
- 해당 프로젝트는 탑뷰방식의 MMORPG게임입니다.
- [github server 링크](https://github.com/qornwh/GameServerProject)
- [github client 링크](https://github.com/Theta08/RpgProject)
  <br>
  [![Video Label](http://img.youtube.com/vi/bkWg1QWX7Vs/0.jpg)](https://youtu.be/bkWg1QWX7Vs)

## 상세 구현

### 1. 서버 동작(IOCP)

서버의 동작은 다음처럼 구성했습니다.

- `초기에 한번 서버소켓을 초기화`(bind, iocp handler에 등록, listen), 비동기로 일정 수만큼 overlapped를 accept합니다.
  - GQCS를 함수를 사용해서 응답 처리된 overlapped에서 처리되는 타입에 맞게 connect, read, write 처리됩니다.
    - `accept`일 때 해당 처리 및 비동기로 read를 등록
    - `read`일 때 패킷을 파싱후 비동기로 read를 등록
    - `write`는 패킷을 송신
  - 해당 작업들은 멀티스레드 내에서 작동되므로 shared_mutex를 이용해서 lock을 사용했습니다.
    - read, write될때 `raii패턴처럼 객체가 할당될 때 lock이 되고 해제될 때 lock이 해제`되도록 구성했습니다.

### 2. Google::protobuf 이용한 패킷 처리

- 이전에는 `따로 라이브러리를 사용하지 않고 패킷을 구현했지만, 문제`가 있었습니다.

  - 하나의 패킷의 구성정보의 `순서도 중요`하고 클라이언트와 서버에서의 `패킷 구성 코드를 따로 맞춰야 하는 문제`가 있었습니다.
  - 결국은 `패킷을 정의한 개수가 늘어나면서` 추가하지 않은 자료형, struct내에서 또 struct, array 등을 구성하고 코드를 작성하면서 `에러율`이 높아졌습니다.

- 대체방법으로 `Google::protobuf`를 사용한 장점
  - .proto파일을 기반으로 `패킷의 코드 생성`
  - 호환된다면 `다른 언어`라도 proto파일 기반으로 generate가 가능함으로써 `직접 코드를 작성할 필요가 없음`

### 3. DB 처리

- DB처리 주요 구현
  1. DB는 쿼리 요청할 때마다 `커넥션을 하면 비용`이 많이 들기 때문에 `풀링`으로 처리했습니다.
  2. DB에서 사용하는 `자료형` int, bool, char등 bind, param함수를 `래핑`해서 사용하기 쉽게 구성했습니다.
  3. 로그인, 아이템등 각 역할의 `~~~DB로 클래스`로 만들었고 `raii`패턴으로 DB커넥션풀에서 DB커넥션을 가져오는 방식으로 구현했습니다.

### 4. 클라이언트 네트워크 처리

- 네트워크 통신 부분은 C#에서 지원하는 `비동기 관련(ConnectAsync, BeginReceive, BeginSend)을` 사용했습니다.
- `클라이언트 네트워크 동작구조`는 다음처럼 구성했습니다.
  1. 서버와 연결이 되면 `BeginReceive미리 콜백을 등록`한다.
  2. BeginReceive의 콜백함수에서 받아온 `패킷을 queue에 push`한다.
  3. `게임 프레임이 업데이트`될 때 queue에서 모든 패킷을 pop해서 처리한다.

---

## git blog 문서 주소

- [**git blog**](https://qornwh.github.io/bjsBlog.github.io/posts/OnlineGameportfolio-0/)

## 플레이 시연 영상

[![Video Label](http://img.youtube.com/vi/UeYY-_UfRoY/0.jpg)](https://youtu.be/UeYY-_UfRoY?si=uCtZRGLmAZObMDmo)
[![Video Label](http://img.youtube.com/vi/bkWg1QWX7Vs/0.jpg)](https://youtu.be/bkWg1QWX7Vs)

## 테스트 시연 영상

[![Video Label](http://img.youtube.com/vi/WXoosvnCQw4/0.jpg)](https://youtu.be/WXoosvnCQw4)
[![Video Label](http://img.youtube.com/vi/k3-c4AiTmxs/0.jpg)](https://youtu.be/k3-c4AiTmxs)
[![Video Label](http://img.youtube.com/vi/6-NsowB52Xw/0.jpg)](https://youtu.be/6-NsowB52Xw)
