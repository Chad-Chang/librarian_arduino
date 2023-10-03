

CServo::attachFSR(핀 번호) : 핀 번호를 FSR 센서 입력 핀으로 설정 <br/>
CServo::checkFSR() : 메인 루프에서 계속 실행 되어야 하는 부분. FSR 센서 값 입력받음 <br/>
CServo::printFSR() : FSR 센서의 입력값을 출력 <br/>
CServo::setAngleLimit(최소 각도, 최대 각도) : 최소 각도와 최대 각도 제한 <br/>
CServo::setInitAngle(시작 각도,증가 부호) : 시작 각도와 각도의 증가 부호를 설정 <br/>
CServo::setGrabPeriod(주기) : 서보모터 그랩 시 "주기"만큼의 시간이 걸리도록 설정 <br/>
CServo::enable_grab() : 그랩 모드를 0으로 바꾸고 그랩 시작 <br/>
CServo::grab(벌릴 각도) : 메인 루프에서 계속 실행 되어야 하는 부분. 모드를 나눔. 아래에 자세히 기술 <br/>
CServo::grabReset() : 만약 물체를 잡고 있을 때 물체를 내려놓음 <br/>

CServo::grab() <br/>
모드 0 : init angle로 이동 (0도로 오므리고 있음) <br/>
모드 1 : init angle에서 90도로 벌리기 <br/>
모드 2 : 90도에서 0도로 오므리기 (못 잡으면 오므린 채로 대기) <br/>
모드 3 : 무언가를 잡음. 이전 각도에서 멈추고 대기 <br/>


로봇팔 코드 <br/>
CRobotArm.cpp : 로봇팔 함수 정의
CRobotArm.h : 로봇팔 헤더 파일
extended_position.ino : 지금 안씀
robot_arm_1.ino : 버전 1, 로봇팔, 그리퍼, 압력센서 통합
robot_arm_2.ino : 버전 2, 로봇팔, 그리퍼, 압력센서, 볼스크류 통합
