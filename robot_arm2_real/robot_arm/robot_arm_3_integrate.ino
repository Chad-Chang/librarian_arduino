#include "CRobotArm.h"
#include "CServo.h"
#include <iostream>
#include <sstream>
#include "CEncoder.h"
#include "FakeSign.h"

//4번 통신해주면 로봇 움직임.
void serialEvent1();
void serialEvent();
unsigned long now = 0;
unsigned long before = 0;
float data[8];
float jnt_ang[9]; // float형태
bool packet = false;
bool robot_op = false;
bool grab_begin = false; // 그리핑 처음에 성공하고, 나중에도 성공하는지 확인 
bool st_matlab = false;
bool st_matlab2 = false;
bool st_matlab3 = false;
bool sent = false;
int ball_mm = 0;
int ball_screw;

String Last_signal = "0";

// target 작으면 벌어짐 0 : 끝까지 
// init angle 클수록 좁혀짐 : 

int servo_ang_max = 110; // 서보 최대 회전

bool grabed = false; // 통신을 통한 수동 그리핑 테스트
int depth = 0; int depth_desire = 310; // 내가 원하는 만큼의 이격거리
bool forward = false;
bool ball_screw_only = false;
bool servo_only_open = false;



CEncoder* CEncoder::instances[3] = {NULL, NULL, NULL};  // DC Motor instances
CEncoder CMotor_L;
CEncoder CMotor_R;
CEncoder CMotor_B;
CRobotArm robot;
CServo cservo;

void setup(){
  Serial1.begin(115200);
  Serial1.setTimeout(1);
  Serial.begin(9600);
  
  // servo setting
  jnt_ang[0] = 0; // 초기 status =0 - 대기
  cservo.attach(5);
  cservo.setAngleLimit(0, servo_ang_max); 
  cservo.setInitAngle(servo_ang_max, true);  // 오므라드는 각도 조정 init angle, inc(+:1, -:0)  // 120 : 오므림 //0: 
  cservo.setGrabPeriod(100);  // set between 0~65535, recommended : 1000 : 주기임  => 
  cservo.attachFSR(A0); 

  // robot setting
  robot.robot_init();
  setPinMode();

/* IMU setting */
  // init_imu();
  // setInitAngle();

/* DC Motor pin setting */
  CMotor_L.begin(ENC_L_2, ENC_L_4, 1);   // int pin1, int pin2, motor_num
  CMotor_R.begin(ENC_R_7, ENC_R_8, 0);
  CMotor_B.begin(ENC_B_45, 72, 2);

/* Ball screw position initialize */
  if (digitalRead(3)==LOW){
    CMotor_B.dir = 0;
    CMotor_B.initEncoder = 2;
  }
  else {
    while (CMotor_B.initEncoder != 2){
      CMotor_B.InitPosition();
      digitalWrite(motorDirB, CMotor_B.dir);
      analogWrite(motorPwmB, CMotor_B.pwm);
    }
    CMotor_B.dir = 0;
  }
}

void loop(){

  if(ball_screw){ 
    robot_op = true;
    Serial1.println("/4/");
    robot.status = bscrew_mode;
  }
  ball_screw = 0;

////////////ball_screw////////////////////////////////////////////////////////////////
 if (ball_screw_only){
    CMotor_B.MoveEncoder_mm(ball_mm); // ball_mm로 움직임. (절대좌표) 범위 0~148
    digitalWrite(motorDirB, CMotor_B.dir);
    analogWrite(motorPwmB, CMotor_B.pwm);
    if(CMotor_B.arrival) {ball_screw_only = false;}
  }

  if(robot_op){ // 로봇팔 구동중
    Last_signal = "0";
    if(robot.status == bscrew_mode){  // 시작신호
      robot.robot_run(packet,jnt_ang);
      cservo.grab();  // 얼마나 벌릴지 cservo.target에 넣어주기 default 0
      cservo.target = 60;  // 열리는것 각도
      cservo.gripperOpen();
      if(robot.status == bscrew_mode){
        if(!forward){
          if(depth > depth_desire){
            int depth_ball = depth - depth_desire;
            CMotor_B.MoveEncoder_mm(depth_ball); // CMotor_B.input_B 이거 대신 너가 원하는 숫자나 변수 넣으면 그 mm로 움직일거임. (절대좌표) 범위 0~148
            // 이 아래는 볼 스크류 모터 움직이는 부분
            digitalWrite(motorDirB, CMotor_B.dir);
            analogWrite(motorPwmB, CMotor_B.pwm);
            if(CMotor_B.arrival){
              Serial1.println("/5/"); robot.status = 5;}
              }
            // else{
            //   Serial1.println("/5/"); robot.status = 5;}
          }
        else{
          Serial1.println("/5/"); robot.status = 5;
        } 
      }
      }
//====================== 정렬 과정================================
    else if(robot.status == 29){ // move upper side
      robot.robot_run(packet, jnt_ang);
      }
//======================그리퍼 개폐================================
    else if(robot.status == 100){ // 열때
      robot.robot_run(packet, jnt_ang);
      cservo.grab();  // 얼마나 벌릴지 cservo.target에 넣어주기 default 0
      cservo.target = 60;  // 열리는것 각도
      cservo.gripperOpen();
      }
//===============================================================
    else if(robot.status == 101){ // 닫을때 
    robot.robot_run(packet, jnt_ang);
    cservo.grab();  // 얼마나 벌릴지 cservo.target에 넣어주기 default 0
    cservo.target = 60;  // 열리는것 각도
    cservo.gripperClose();
      }
//================================================================

    else if(robot.status == end_rob){ //끝났을때
      robot.robot_run(packet, jnt_ang);
      cservo.gripperOpen();
      Last_signal = String(end_rob);
      robot_op = false;
      forward = false;
      }
    else if(robot.status == BS_back){ // ball screw를 돌려 뒤로 빼는 과정
      robot.robot_run(packet,jnt_ang);
      CMotor_B.MoveEncoder_mm(30); // CMotor_B.input_B 이거 대신 너가 원하는 숫자나 변수 넣으면 그 mm로 움직일거임. (절대좌표) 범위 0~148
      digitalWrite(motorDirB, CMotor_B.dir);
      analogWrite(motorPwmB, CMotor_B.pwm);
      if(CMotor_B.arrival){Serial1.println("/s/");}
      }
    else if(robot.status == grabb){ //잡는 상태일 때
      cservo.grab();  // 얼마나 벌릴지 cservo.target에 넣어주기 default 0
      cservo.target = 50; 
      cservo.gripperClose(); // close
      robot.robot_run(packet, jnt_ang);
      }
    else if(robot.status == read_fail){
      Last_signal = String(read_fail);
      robot.robot_run(packet, jnt_ang);
      robot_op = false;
      // forward = false;
      }
    else{ //보통 구동
      robot.robot_run(packet, jnt_ang);
      }
  }  
}



// uart 각도 + depth 통신 
void serialEvent1(){
  static char serialInBuffer[32];
  static int data_cnt = 0, buff_cnt = 0; 
  if(Serial1.available() > 0) {
    char byteIn = Serial1.read();
    if(byteIn=='['&& !st_matlab){
      st_matlab = true;
    }
    else if(byteIn == 'a' && !st_matlab3 && st_matlab){
      st_matlab2 = true;
    }
    else if(byteIn == 'b' && !st_matlab2 && st_matlab){
      st_matlab3 = true;
    }
    else if(st_matlab2 && byteIn==','){
      serialInBuffer[buff_cnt] = '\0';
      data[data_cnt++] = atof(serialInBuffer);
      buff_cnt = 0;
    }
    else if(st_matlab2 && byteIn == ']'){
      serialInBuffer[buff_cnt] = '\0';
      data[data_cnt] = atof(serialInBuffer);
      buff_cnt = 0; data_cnt = 0;
      st_matlab = false;
      st_matlab2 = false;
      st_matlab3 = false;
      jnt_ang[0] = data[1]; // status
      Serial.print("status =");
      Serial.println(jnt_ang[0]);
      jnt_ang[1] = data[2]+270.7;
      jnt_ang[2] = data[3]+178.43;
      jnt_ang[3] = data[3]+180.11;
      jnt_ang[4] = data[4]+181.37;
      jnt_ang[5] = data[5]+179.3;
      jnt_ang[6] = data[6]+270;
      jnt_ang[7] = data[7]+197.67;
      packet = true;
    }
    else if(st_matlab3 && byteIn==','){
      // Serial1.println(serialInBuffer);
      serialInBuffer[buff_cnt] = '\0';
      data[data_cnt++] = atof(serialInBuffer);
      buff_cnt = 0;
    }
    else if(st_matlab3 && byteIn == ']'){
      serialInBuffer[buff_cnt] = '\0';
      data[data_cnt] = atof(serialInBuffer);
      buff_cnt = 0; data_cnt = 0;
      st_matlab = false;
      st_matlab2 = false;
      st_matlab3 = false;
      depth = data[1];
      if(depth > depth_desire){
        int depth_ball = depth-depth_desire;
        Serial.println(depth);
        Serial.print("depth =  ");
        Serial.println(depth);
        Serial.print("\t");
        Serial.print("depth_ball =   ");
        Serial.println(depth_ball);
        }
      }
    else{
      serialInBuffer[buff_cnt++] = byteIn;
    }
  }
}

void serialEvent(){
////////////// 유석이랑 합칠때 지우기///////////////
  if (Serial.available() > 0) {
    String temp = Serial.readStringUntil('\n');
    int input0 = temp.toInt();
///////////////////여기까지///////////////////////
    if(input0 == 1){ // 3입력 후 실행
      ball_screw = 1;
      // Serial1.println("/4/");
      Serial.println("Operating");
      }
    
    else if(input0 == 2){ // 원상 복귀
      Serial1.println("/3/");
      Serial.println("return");
    }
    else if(input0 == 3){
      Serial1.println("/12/");
      Serial.println("grip");
      
    }
    else if(input0 == 4){ // 그리핑에 성공했다는 신호
      Serial1.println("/s/");
      Serial.println("keep_going");
    }
    else if(input0 == 5){
      Serial.println("reverse");
      grabed = !grabed;
    }
    else if(input0== 6){
      Serial.print("status");
      Serial.println(robot.status);
    }
    else if(input0== 7){
      Serial.print("depth");
      Serial.println(depth);
    }
    else if(input0== 8){
      Serial.print("reset");
      Serial1.println("/r/");
    }
    else if(input0 == 9){
      servo_only_open = true;
      Serial.print("Open");
    }
    else if(input0 == 10){
      servo_only_open = false;
      Serial.print("Close");
    }
    else{
      Serial.print("ball_mm = ");
      Serial.println(input0);
      ball_screw_only = true;
      ball_mm = input0;
    }

  }
}
/*시스템 통합용 검증 시리얼 코드*/
// void serialEvent(){
// ////////////// 유석이랑 합칠때 지우기///////////////
//   if (Serial.available() > 0) {
//     String temp = Serial.readStringUntil('\n');
//     int input0 = temp.toInt();
// ///////////////////여기까지///////////////////////
//     if(input0 == 1){ // 3입력 후 실행
//       ball_screw = 1;
//       Serial.println(ball_screw);
//       Serial1.println("/4/");
//       Serial.println("Operating");
//       }
//     else if(input0==2){
//       robot.status = end_rob;
//       Serial.print("end robot status= ");
//       Serial.println(robot.status); 
//     }
//     else if(input0 ==3){
//       robot.status = read_fail;
//       Serial.print("fail to read and robot status= ");
//       Serial.println(robot.status);
//     }
//     else if(input0 ==4){
//       Serial.print("robot_op = ");
//       Serial.println(robot_op);
//     }
//     else if(input0 == 5){
//       Serial.print("ball_screw = ");
//       Serial.println(ball_screw);
//     }
//     else if(input0 == 6 ){
//       Serial.print("status = ");
//       Serial.println(robot.status);
//       // Serial.println()
//     }
//     else if(input0 == 7){
//       Serial.print("Last_signal = ");
//       Serial.println(Last_signal);          
//     }
//   }
// }