# 2024_VCpp_Task
2024년 2학년 2학기 VC++과목 기말 대체 과제<br>

## 게임 정보
뱀파이어 서바이벌이나 탕탕특공대를 모티브로한 생존 게임<br>

## 게임 설명
게임을 시작하면 플레이어 주변으로 사거리가 정해진 8개의 총알이 발사된다.<br> 데미지 증가(노란색 오브젝트)와 공격속도 증가(하늘색 오브젝트) 아이템을 통해 플레이어를 성장시킨다.<br><br>
적과 플레이어가 닿게되면 플레이어의 체력이 1씩 줄어들고 일정 시간 무적 판정이 된다. <br>체력이0이 되면 게임이 종료된다.<br> 체력 회복(초록색 오브젝트) 아이템으로 체력을 1회복 시킬수 있다.<br><br>
난이도는 시간에 따라 증가하게 된다. 이에 맞춰 아이템을 통해 플레이어를 성장시켜 생존하는 게임이다.

## 게임 기능 설명
1. WM_ACTIVE, WM_TIMER를 이용하여 키보드 폴링 확인 후 동시 입력 제어<br>
2. 삼각 함수를 이용하여 총알 발사 방향 설정<br>
3. random 라이브러리를 이용한 하드웨어 난수 발생 -> 이를 통해 적과 아이템 스폰 제어<br>
4. chrono 라이브러리를 이용하여 게임 시간 측정<br>

## 게임 화면
게임 초기 화면<br>
![start](https://github.com/user-attachments/assets/e476d599-2876-4ede-b3cf-f974db25112d)
<br><br>
카운트 다운<br>
![countdown](https://github.com/user-attachments/assets/16987402-1007-4c9c-9165-c6c287908617)
<br><br>
플레이 화면<br>
![play](https://github.com/user-attachments/assets/c91fdcd7-6c26-4f90-9df7-00b3b19ccc96)
<br><br>
게임 오버 화면<br>
![gameover](https://github.com/user-attachments/assets/728b7ea6-9751-4bea-a3e9-d000cfe53a00)

## 참고자료
| 레퍼런스 |
| :---: |
| <a href="https://m.blog.naver.com/dorergiverny/223067218069">난수 발생</a> |  
| <a href="https://blockdmask.tistory.com/446">삼각 함수</a> |
| <a href="https://blockdmask.tistory.com/236">static_cast</a> |
| <a href="https://two-parks.tistory.com/32">시간 측정</a> |

## 파일 설명
| 파일명 | 설명 |
| :---: | :---: |
| 202407034_KHG.h | 전역 변수 및 함수 선언 |
| 202407034_KHG.cpp | 코드 구현 및 함수 정의 |
| Resource.h | 사용자 정의 상수 정의 |
| Struct.h | 구조체 및 열거형 정의 |
