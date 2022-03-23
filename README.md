# deauth-attack
AP와 Station 간에 통신은 아래와 같은 순서로 이루어집니다.

![image](https://user-images.githubusercontent.com/68851537/159670911-f3710f10-fbe8-4841-b21d-e64aca844e12.png)

Station이 더 이상 AP와 통신을 하지 않을 때(ex. 스마트폰에서 다른 AP를 선택하거나 Wifi 기능을 끄게 되는 경우)는 Deauthentication 패킷이 발생하는데 이를 수신하게 될 경우 AP와 Station 간에 연결이 끊어지게 됩니다. 이를 통해서 wireshark로 Deauthentication 패킷을 캡쳐한 후, 분석을 통해 필요한 값들을 설정해주고, Monitor mode를 지원하는 무선 인터페이스를 이용해 패킷을 전송하는 공격을 생각해볼 수 있고 이를 deauth-attack이라고 합니다.

구현한 프로그램은 무선인터페이스의 인자와 AP, Station의 주소(없을 경우 broadcast)를 인자로 주어 실행시킬 수 있습니다. Deauthentication 프레임에 필요한 최소한의 변수인 `radiotap 헤더의 길이`, `AP mac 주소`, `target mac 주소(Station 혹은 브로드캐스트)`만 설정해서 패킷을 전송하는 형태로 구현하였습니다.

