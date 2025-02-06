# Chatting Server TCP 통신 프로젝트 
![image](https://github.com/user-attachments/assets/0168e4ba-8c18-43f1-9481-97143adfd2df)


# Chatting server 구현
>**개발기간** : 2025-02-03 ~ 2025-02-07<br>

## 📬 배포 주소
> **배포 서버 버전** :[![Static Badge](https://img.shields.io/badge/server-zip-blue)](https://github.com/Dowill-hob/client_cpp/releases/tag/v0.01) <br>
> **배포 클라이언트 버전** :[![Static Badge](https://img.shields.io/badge/client-zip-orange)](https://github.com/Dowill-hob/client_cpp/archive/refs/tags/v0.01.zip)
## 👨‍🎓 개발자

>**김준식** : 서버 구현 및 클라이언트 exe 제작

## 🖥️ 프로젝트 소개
간단한 1대1 실시간 채팅 서버를 구현하는 프로젝트입니다.

#### 프로젝트 구동법<br>
파일 여는법 zip해제후 파일을 연다 --> x64폴더 --> DEBUG --> exe 파일 실행

* server 파일의 exe파일을 가장 먼저 구동 시킵니다.
* 그 후 클라이언트 exe를 구동 시키면 작동 준비는 끝입니다.

##### 1. 기능 회원 가입
* 클라이언트가 서버와 통신이 성공하면 메뉴창을 출력합니다. <br>
![image](https://github.com/user-attachments/assets/ce9ac791-d5c5-405e-9536-a8d9ed1ffe9f)


* 1 을 입력하여 회원 가입 커맨드를 서버로 보냅니다. <br>
* 원하는 username(표시되는 이름) 과 Password를 순차적으로 입력하여 서버에 저장합니다<br>
![image](https://github.com/user-attachments/assets/8c790034-1a7e-42d6-b4d4-c0e9f2262650)

* 회원가입이 성공하여 Registration successful이라는 문구를 출력합니다.<br>
* 또 중복되는 username이 있다면 There is a duplicate username.를 출력합니다.<br>
![image](https://github.com/user-attachments/assets/f650af27-863e-4700-8f36-d191233ddf62)

##### 2. 로그인
* 2 를 입력하면 이제 회원 가입으로 저장했던 정보로 로그인을 하실 수 있습니다. <br>
![image](https://github.com/user-attachments/assets/30e9fff9-f654-45f6-bb15-4feb4af5f4c5)

* 로그인에 성공하면 서버창에 DEBUG내용을 출력하고 클라이언트 콘솔창에는 이제 성공여부를 출력합니다.
* 성공일 경우 그림과 같이 채팅기능이 활성화 되어 채팅입력이 가능해지고 입력한 채팅은 자신이외에 클라이언트와 서버에 전달됩니다. 
![image](https://github.com/user-attachments/assets/65141394-a21b-42df-b331-0390d927054e)
##### 3. 회원 탈퇴
* 회원 탈퇴를 원할 경우 두가지 방법이 있습니다.
* 첫 번째는 이제 서버파일에서 삭제하는 법
* 두 번째는 이제 직접 클라이언트 내에서 회원 탈퇴를 하는 방법입니다.
* 방식은 회원가입과 같습니다.
* 3 을 입력하여 회원 탈퇴 커맨드를 서버로 보냅니다.
* 이제 회원가입을 했던 username을 입력해주시면 회원탈퇴가 됩니다.<br>

  ![image](https://github.com/user-attachments/assets/821cb822-4784-49b4-9898-c2f82ed4d1b2)

  
##### 4. 채팅로그
* 이 기능은 특정 유저에 채팅을 확인하고 싶을 때 사용되어집니다.
* 채팅 중에 이 기능을 원할시에는 exit를 입력하여 logout을 진행하신 후 4번을 선택해주시면 됩니다.
* 그리고 검색하실 username을 입력해주시면 됩니다.<br>
![image](https://github.com/user-attachments/assets/66c75421-75d0-425b-b879-b2d8af3ddf8f)

##### 5. 종료
* 시스템에 모든내용을 저장후 goodbye선언과 함께 시스템이 종료됩니다.
* 창이 안 닫히도록 하였기에 확인하시고 직접 창을 닫아주셔야 합니다.<br>

![image](https://github.com/user-attachments/assets/ba7591dc-86d1-4f61-ba0f-e87497ec4e87)
