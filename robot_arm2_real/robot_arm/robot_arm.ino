// #include "CRobotArm.h"
// #include "CServo.h"
// #include <iostream>
// #include <sstream>
// #include "CEncoder.h"
// #include "FakeSign.h"

// //4번 통신해주면 로봇 움직임.
// void serialEvent1();
// void serialEvent();
// unsigned long now = 0;
// unsigned long before = 0;
// int gripper = 0;
// float data[8];
// float jnt_ang[9]; // float형태
// bool packet = false;
// bool robot_op = false;
// bool timer_on = false;
// bool grab_begin = false; // 그리핑 처음에 성공하고, 나중에도 성공하는지 확인 
// bool grab_suc= false; // 그리핑 성공상태를 유지하고 있음
// bool st_matlab = false;
// bool st_matlab2 = false;
// bool st_matlab3 = false;
// bool sent = false;
// int Last_signal = 0; // 유석이 마지막 패킷 True
// int servo_ang_max = 120;
// int servo_ang_min = 100; // 커질수록 오므라듦
// bool grabed = false;
// int depth = 0;
// int depth_desire = 260; // 내가 원하는 만큼의 이격거리
// bool wait_s = false;
// bool forward = false;

// CEncoder* CEncoder::instances[3] = {NULL, NULL, NULL};  // DC Motor instances
// CEncoder CMotor_L;
// CEncoder CMotor_R;
// CEncoder CMotor_B;
// CRobotArm robot;
// CServo cservo;

// void setup(){
//   Serial1.begin(115200);
//   Serial1.setTimeout(1);
//   Serial.begin(9600);

//   jnt_ang[0] = 0; // 초기 status =0 - 대기
//   cservo.attach(5);
//   cservo.setAngleLimit(0, servo_ang_max);
//   cservo.setInitAngle(servo_ang_min, true);  // init angle, inc(+:1, -:0)  // 120 : 오므림
//   cservo.setGrabPeriod(3000);  // set between 0~65535, recommended : 1000
//   cservo.attachFSR(A0); 

//   // robot.robot_init();
//   // robot.status = bscrew_mode;
//   setPinMode();

// /* IMU setting */
//   // init_imu();
//   // setInitAngle();

// /* DC Motor pin setting */
//   CMotor_L.begin(ENC_L_2, ENC_L_4, 1);   // int pin1, int pin2, motor_num
//   CMotor_R.begin(ENC_R_7, ENC_R_8, 0);
//   CMotor_B.begin(ENC_B_45, 72, 2);

// /* Ball screw position initialize */
//   if (digitalRead(3)==LOW)
//   {
//     CMotor_B.dir = 0;
//     CMotor_B.initEncoder = 2;
//   }
//   else {
//     while (CMotor_B.initEncoder != 2)
//     {
//       CMotor_B.InitPosition();
//       digitalWrite(motorDirB, CMotor_B.dir);
//       analogWrite(motorPwmB, CMotor_B.pwm);
//     }
//     CMotor_B.dir = 0;
//     }
//   }


// void loop(){
//   String user_input;

// /////////////////////////유석이 마지막 자리 패킷/////////////////////////////////
//   if(robot.status == end_rob){ // end_rob =2 로봇팔 작업 끝났을 때
//     Last_signal = end_rob;
//     // Serial.println("Ylast = end_rob");
//   }
//   else if(robot.status == NRead){ // NRead = 1 못 읽었을때 
//     Last_signal = NRead;
//     // Serial.println("Ylast= NRead");
//   }
//   else{ // 보통의 경우 
//     Last_signal = 0; 
//     // Serial.print("Ylast=");Serial.println(Ylast);
//   }
//   // if(Last_signal){
//   //   //통신
//   // }
// ////////////////////////////////////////////////////////////////////////////
  
//   cservo.grab();  // 얼마나 벌릴지 cservo.target에 넣어주기 default 0
//   cservo.target = 50;
//   // cservo.checkFSR();
//   if(!grabed) cservo.gripperOpen();
//   else cservo.gripperClose();
//   // Serial.print("grap = ");
//   // Serial.println(grabed);
//   // cservo.printFSR();  // 압력센서값 출력

//   if(robot_op){ // 로봇팔 구동중
//     if(robot.status == bscrew_mode){  // 시작신호
//       robot.robot_run(packet,jnt_ang);
//       // if(!forward){
//         if(depth > depth_desire){    
//           int depth_ball = depth - depth_desire;
//           CMotor_B.MoveEncoder_mm(depth_ball); // CMotor_B.input_B 이거 대신 너가 원하는 숫자나 변수 넣으면 그 mm로 움직일거임. (절대좌표) 범위 0~148
//           // 이 아래는 볼 스크류 모터 움직이는 부분
//           digitalWrite(motorDirB, CMotor_B.dir);
//           analogWrite(motorPwmB, CMotor_B.pwm);
//           if(CMotor_B.arrival){Serial1.println("/5/"); robot.status = 5; forward = true}
//         }
//       // }
//     }
//     else if(robot.status == end_rob){ //끝났을때
//       robot.robot_run(packet, jnt_ang);
//       Serial.println("status end");
//       robot_op = false;
//       }
//     else if(robot.status == BS_back){ // ball screw를 돌려 뒤로 빼는 과정
//       robot.robot_run(packet,jnt_ang);
//       CMotor_B.MoveEncoder_mm(30); // CMotor_B.input_B 이거 대신 너가 원하는 숫자나 변수 넣으면 그 mm로 움직일거임. (절대좌표) 범위 0~148
//       digitalWrite(motorDirB, CMotor_B.dir);
//       analogWrite(motorPwmB, CMotor_B.pwm);
//       if(CMotor_B.arrival){Serial1.println("/s/");}
//     }
//     else if(robot.status == grabb){ //잡는 상태일 때
//       robot.robot_run(packet, jnt_ang);
//       grabed = true;
//       // cservo.gripperClose();
//       cservo.gripperClose(); // close
//       // cservo.gripperOpen(); // close
//       Serial.println("grabb");
//       Serial.print("cservo = ");
//       Serial.println(cservo.isGrab);
//       if(cservo.isGrab){ // 압력 센서가 눌렸을 때
//         grab_begin = true;
//         if(!timer_on){ // 시간초 여러번 실행하는 거 방지
//           Serial.println("pressed1");
//           timer_on = true;
//           before = millis();
//           Serial.print("time1 = ");
//           Serial.println(now-before);
//           }
//         else{
//           now = millis();
//           Serial.print("time2 = ");
//           Serial.println(now-before);
//           if(now-before >= 2000){ // 2초 동안 계속 눌렸다고 판단되면 -> 잘 잡음
//             timer_on = false;
//             now = 0;
//             Serial.println("pressed2");
//             Serial1.println("/s/");  // 그리핑 성공하면 통신
//             }
//           }
//         } 
//       }
//     else{ //보통 구동
//         robot.robot_run(packet, jnt_ang);
//       }

//   // 그리핑 실패 통신 보내기
//     if(grab_begin){ 
//       if(!sent){
//         now = millis();
//         Serial.print("time3 = ");
//         Serial.println(now-before);
//         if(now-before > 2030){
//           Serial.println("failed_last");
//           Serial1.println("/f/"); // 그리핑 실패했을때의 통신 
//           // robot.status = fail_grip;
//           timer_on = false;
//           sent = true;
//           }
//         }    
//       }
//     }  
//     // Serial.println(robot.status);
//   }

// // uart 각도 + depth 통신 

// void serialEvent1(){
//   static char serialInBuffer[32];
//   static int data_cnt = 0, buff_cnt = 0; 
  
//   if(Serial1.available() > 0) {
//     char byteIn = Serial1.read();
//     if(byteIn=='['&& !st_matlab){
//       st_matlab = true;
//       }
//     else if(byteIn == 'a' && !st_matlab3 && st_matlab){
//       st_matlab2 = true;
//     }
//     else if(byteIn == 'b' && !st_matlab2 && st_matlab){
//       st_matlab3 = true;
//     }
//     else if(st_matlab2 && byteIn==','){
//       // Serial1.println(serialInBuffer);
//       serialInBuffer[buff_cnt] = '\0';
//       data[data_cnt++] = atof(serialInBuffer);
//       buff_cnt = 0;
//     }
//     else if(st_matlab2 && byteIn == ']'){
//       serialInBuffer[buff_cnt] = '\0';
//       data[data_cnt] = atof(serialInBuffer);
//       buff_cnt = 0; data_cnt = 0;
//       st_matlab = false;
//       st_matlab2 = false;
//       st_matlab3 = false;
//       jnt_ang[0] = data[1]; // status
//       Serial.print("status =");
//       Serial.println(jnt_ang[0]);
//       jnt_ang[1] = data[2]+268.7;
//       jnt_ang[2] = data[3]+178.43;
//       jnt_ang[3] = data[3]+180.11;
//       jnt_ang[4] = data[4]+181.37;
//       jnt_ang[5] = data[5]+178.3;
//       jnt_ang[6] = data[6]+270;
//       jnt_ang[7] = data[7]+197.67;
//       packet = true;
//       // Serial1.print("angle");
//       // Serial1.print("\t");
//       // Serial1.print(jnt_ang[0]);
//       // Serial1.print("\t");
//       // Serial1.print(jnt_ang[1]);
//       // Serial1.print("\t");
//       // Serial1.print(jnt_ang[2]);
//       // Serial1.print("\t");
//       // Serial1.print(jnt_ang[3]);
//       // Serial1.print("\t");
//       // Serial1.print(jnt_ang[4]);
//       // Serial1.print("\t");
//       // Serial1.print(jnt_ang[5]);
//       // Serial1.print("\t");
//       // Serial1.print(jnt_ang[6]);
//       // Serial1.print("\t");
//       // Serial1.println(jnt_ang[7]);
//       // Serial1.print("robot status =  ");
//       // Serial1.println(robot.status);

//       }

//     else if(st_matlab3 && byteIn==','){
//       // Serial1.println(serialInBuffer);
//       serialInBuffer[buff_cnt] = '\0';
//       data[data_cnt++] = atof(serialInBuffer);
//       buff_cnt = 0;
//     }
//     else if(st_matlab3 && byteIn == ']'){
//       serialInBuffer[buff_cnt] = '\0';
//       data[data_cnt] = atof(serialInBuffer);
//       buff_cnt = 0; data_cnt = 0;
//       st_matlab = false;
//       st_matlab2 = false;
//       st_matlab3 = false;
//       depth = data[1];
//       if(depth > depth_desire){
//         int depth_ball = depth-depth_desire;
//         Serial1.println(depth);
//         Serial1.print("depth =  ");
//         Serial1.println(depth);
//         Serial1.print("\t");
//         Serial1.print("depth_ball =   ");
//         Serial1.println(depth_ball);
//         }
//       }
//     else{
//       serialInBuffer[buff_cnt++] = byteIn;
//     }
//   }
// }

// // 4을 보내기
// void serialEvent(){
// ////////////// 유석이랑 합칠때 지우기///////////////
//   if (Serial.available() > 0) {
//     String temp = Serial.readStringUntil('\n');
//     int input0 = temp.toInt();
//     // Serial.println(ball_screw);
// ///////////////////여기까지///////////////////////
//     // if(!robot_op){
//       if(input0 == 1){ // 3입력 후 실행
//         robot_op = true;
//         robot.status=bscrew_mode;
//         Serial1.println("/4/");
//         Serial.println("Operating");
//         }
      
//       else if(input0 == 2){ // 원상 복귀
//         Serial1.println("/3/");
//         Serial.println("return");
//       }
//       else if(input0 == 3){
//         Serial1.println("/12/");
//         Serial.println("grip");
        
//       }
//       else if(input0 == 4){ // 그리핑에 성공했다는 신호
//         Serial1.println("/s/");
//         Serial.println("keep_going");
//       }
//       else if(input0 == 5){
//         Serial.println("reverse");
//         grabed = !grabed;
//       }
//       else if(input0== 6){
//         Serial.print("status");
//         Serial.println(robot.status);
//       }
//       else if(input0== 7){
//         Serial.print("depth");
//         Serial.println(depth);
//       }
//       // else if(ball_screw == ){
//       //   Serial1.println("/s/");
//       //   Serial.println("keep_going");
//       // }
//     }
// }