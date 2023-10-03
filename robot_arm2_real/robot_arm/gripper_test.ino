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

// CRobotArm robot;
// CServo cservo;

// void setup(){
//   Serial1.begin(115200);
//   Serial1.setTimeout(0.01);
//   Serial.begin(9600);
//   jnt_ang[0] = 0;

//   cservo.attach(5);
//   cservo.setAngleLimit(0, servo_ang_max);
//   cservo.setInitAngle(servo_ang_min, true);  // init angle, inc(+:1, -:0)  // 120 : 오므림
//   cservo.setGrabPeriod(3000);  // set between 0~65535, recommended : 1000
//   cservo.attachFSR(A1); 
//   }


// void loop(){
// /////////////////////////유석이 마지막 자리 패킷/////////////////////////////////
//   cservo.grab();  // 얼마나 벌릴지 cservo.target에 넣어주기 default 0
//   cservo.target = 50;
//   // cservo.checkFSR();
//   // cservo.gripperClose();
//   // cservo.gripperOpen();
  
//   // cservo.printFSR();  // 압력센서값 출력

// //gripper check based on serial
//   switch(gripper){
//         case 1:
//           cservo.gripperClose();
//           break;
//         case 2: 
          
//           cservo.gripperOpen();
//           break;
//         case 3:
//           cservo.grabReset();
//           break;
//         case 4:
//           cservo.target = 0;
//           break;
//         case 5:
//           cservo.target = 20;
//           break;
//         default:
//           break;
//     }
//   }

// void serialEvent1(){
//   if (Serial1.available()) {
//     String temp = Serial1.readStringUntil('\n');
//     Serial1.println(temp);
//     // servo_ang_min = temp.toInt();
//     // Serial1.print("servo ang min=");
//     // Serial1.println(servo_ang_min);
//   }
// }


// // 4을 보내기
// void serialEvent(){
// ////////////// 유석이랑 합칠때 지우기///////////////
//   if (Serial.available()) {
//     String temp = Serial.readStringUntil('\n');
//     int ball_screw = temp.toInt();
//     switch(ball_screw){
//       case 1:
//         Serial.print(gripper);
//         Serial.println("gripper open");
//         gripper =1;
//         break;
//       case 2: 
//         Serial.print(gripper);
//         Serial.println("gripper Close");
//         gripper =2;
//         break;
//       case 3:
//         Serial.print(gripper);
//         Serial.println("gripper Reset");
//         gripper =3;
//         break;
//       case 4:
//         Serial.print(gripper);
//         Serial.println("target 0");
//         gripper =4;
//         break;
//       case 5:
//         Serial.print(gripper);
//         Serial.println("target 20");
//         gripper =5;
//         break;
//       default:
//         gripper = 0;
//     }
//   }
// }