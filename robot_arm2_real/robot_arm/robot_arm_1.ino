// #include "CRobotArm.h"
// #include "CServo.h"
// #include <iostream>
// #include <sstream>

//  //4번 통신해주면 로봇 움직임.
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
// bool sent = false;
// int Last_signal = 0; // 유석이 마지막 패킷 True
// int servo_ang_max = 120;
// int servo_ang_min = 100; // 커질수록 오므라듦
// bool grabed = false;

// CRobotArm robot;
// CServo cservo;

// void setup(){
//   Serial1.begin(115200);
//   Serial1.setTimeout(0.01);
//   Serial.begin(9600);
//   robot.robot_init();
//   jnt_ang[0] = 0; // 초기 status =0 - 대기

//   cservo.attach(3);
//   cservo.setAngleLimit(0, servo_ang_max);
//   cservo.setInitAngle(servo_ang_min, true);  // init angle, inc(+:1, -:0)  // 120 : 오므림
//   cservo.setGrabPeriod(3000);  // set between 0~65535, recommended : 1000
//   cservo.attachFSR(A1); 
//   }


// void loop(){

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
//   if(!grabed) cservo.gripperClose();
//   else cservo.gripperOpen();
//   // cservo.gripperOpen();
//   // cservo.printFSR();  // 압력센서값 출력
  
//   if(robot_op){ // 로봇팔 구동중
//     if(robot.status == end_rob){ //끝났을때
//       robot.robot_run(packet, jnt_ang);
//       Serial.println("status end");
//       robot_op = false;
//       }
//     else if(robot.status == grabb){ //잡는 상태일 때
//       robot.robot_run(packet, jnt_ang);
//       grabed = true;
//       // cservo.gripperClose();
//       cservo.gripperOpen(); // close
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
    

// //gripper check based on serial
//   // switch(gripper){
//   //       case 1:
//   //         cservo.gripperClose();
//   //         break;
//   //       case 2: 
//   //         cservo.gripperOpen();
//   //         break;
//   //       case 3:
//   //         cservo.grabReset();
//   //         break;
//   //       case 4:
//   //         cservo.target = 0;
//   //         break;
//   //       case 5:
//   //         cservo.target = 20;
//   //         break;
//   //       default:
//   //         break;
//   //   }
//   }


// void serialEvent1(){
//   static char serialInBuffer[32];
//   static int data_cnt = 0, buff_cnt = 0; 
//   if(Serial1.available() > 0) {
//     char byteIn = Serial1.read();
//     if(byteIn=='['&& !st_matlab){
//       st_matlab = true;
//       }
//     else if(st_matlab&& byteIn==','){
//       serialInBuffer[buff_cnt] = '\0';
//       data[data_cnt++]=atof(serialInBuffer);
//       buff_cnt = 0;
//     }
//     else if(byteIn == ']'){
//       serialInBuffer[buff_cnt] = '\0';
//       data[data_cnt] = atof(serialInBuffer);
//       buff_cnt = 0; data_cnt = 0;
//       st_matlab = false;

//       jnt_ang[0] = data[0]; // status
//       jnt_ang[1] = data[1]+268.7;
//       jnt_ang[2] = data[2]+178.43;
//       jnt_ang[3] = data[2]+180.11;
//       // jnt_ang[4] = data[3]+183.87;
//       // jnt_ang[5] = data[4]+177.82;
//       // jnt_ang[6] = data[5]+270.99;
//       // jnt_ang[7] = data[6]+197.67;
//       jnt_ang[4] = data[3]+181.37;
//       jnt_ang[5] = data[4]+178.3;
//       jnt_ang[6] = data[5]+270;
//       jnt_ang[7] = data[6]+197.67;
      
//       packet = true;
//     }
//     else{
//       serialInBuffer[buff_cnt++] = byteIn;
//     }
//   }
// }


// uart 각도만 통신 : 이전 버전
// void serialEvent1(){
//   static char serialInBuffer[32];
//   static int data_cnt = 0, buff_cnt = 0; 
//   if(Serial1.available() > 0) {
//     char byteIn = Serial1.read();
//     if(byteIn=='['&& !st_matlab){
//       st_matlab = true;
//       }
//     else if(st_matlab && byteIn==','){
//       serialInBuffer[buff_cnt] = '\0';
//       data[data_cnt++] = atof(serialInBuffer);
//       buff_cnt = 0;
//       Serial1.println("second");
//     }
//     else if(byteIn == ']'){
//       serialInBuffer[buff_cnt] = '\0';
//       data[data_cnt] = atof(serialInBuffer);
//       buff_cnt = 0; data_cnt = 0;
//       st_matlab = false;

//       jnt_ang[0] = data[0]; // status
//       jnt_ang[1] = data[1]+268.7;
//       jnt_ang[2] = data[2]+178.43;
//       jnt_ang[3] = data[2]+180.11;
//       // jnt_ang[4] = data[3]+183.87;
//       // jnt_ang[5] = data[4]+177.82;
//       // jnt_ang[6] = data[5]+270.99;
//       // jnt_ang[7] = data[6]+197.67;
//       jnt_ang[4] = data[3]+181.37;
//       jnt_ang[5] = data[4]+178.3;
//       jnt_ang[6] = data[5]+270;
//       jnt_ang[7] = data[6]+197.67;
      
//       packet = true;
//     }
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
//     int ball_screw = temp.toInt();
//     // Serial.println(ball_screw);
// ///////////////////여기까지///////////////////////
//     // if(!robot_op){
//       if(ball_screw == 1){ // 3입력 후 실행
//         robot_op = true;
//         Serial1.println("/4/");
//         Serial.println("Operating");
//         }
      
//       else if(ball_screw == 2){ // 원상 복귀
//         Serial1.println("/3/");
//         Serial.println("return");
//       }
//       else if(ball_screw == 3){
//         Serial1.println("/10/");
//         Serial.println("grip");
//       }
//       else if(ball_screw == 4){
//         Serial1.println("/s/");
//         Serial.println("keep_going");
//       }
//       // else if(ball_screw == ){
//       //   Serial1.println("/s/");
//       //   Serial.println("keep_going");
//       // }
//     }
// }