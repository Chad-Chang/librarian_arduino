// #include "CEncoder.h"
// #include "FakeSign.h"

// CEncoder* CEncoder::instances[3] = {NULL, NULL, NULL};  // DC Motor instances

// CEncoder CMotor_L;
// CEncoder CMotor_R;
// CEncoder CMotor_B;

// void setup() {
//   /* Serial, pin setting */
//   Serial.begin(9600);
//   setPinMode();

//   /* IMU setting */
//   // init_imu();
//   setInitAngle();

//   /* DC Motor pin setting */
//   CMotor_L.begin(ENC_L_2, ENC_L_4, 1);   // int pin1, int pin2, motor_num
//   CMotor_R.begin(ENC_R_7, ENC_R_8, 0);
//   CMotor_B.begin(ENC_B_45, 72, 2);
  
//   /* Ball screw position initialize */

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
//   }

//   // 처음 시작할 때 볼 스크류가 한쪽으로 쭉 이동해서 멈출거임. 그 코드(자동실행). 최대 거리는 0~148mm까지 이동 가능
// }


// void loop() {

//   String user_input;
   
//   while(Serial.available())
//   {
//     user_input = Serial.readStringUntil('\n'); // Get Input from Serial Monitor
//     if(user_input =="0"){
//       CMotor_B.input_B = 0;
//     }
//     else if(user_input =="1"){
//       // CMotor_B.input_B = 1000;
//       CMotor_B.input_B = 27;
//     }
//     else if(user_input =="2"){
//       // CMotor_B.input_B = 2000;
//       CMotor_B.input_B = 54;
//     }
//     else if(user_input =="3"){
//       // CMotor_B.input_B = 3000;
//       CMotor_B.input_B = 81;
//     }
//     else if(user_input =="4"){
//       // CMotor_B.input_B = 4000;
//       CMotor_B.input_B = 108;
//     }
//     else if(user_input =="5"){
//       // CMotor_B.input_B = 5500;
//       CMotor_B.input_B = 148;
//     }
//     else if(user_input =="6"){
//       // CMotor_B.input_B = 5500;
//       CMotor_B.input_B = 160;
//     }
//     else{
//       Serial.println("Wrong Number");
//       CMotor_B.input_B = CMotor_B.pulse_count;
//     }
//   }

//   // CMotor_B.MoveEncoder(CMotor_B.input_B);
//   CMotor_B.MoveEncoder_mm(CMotor_B.input_B); // CMotor_B.input_B 이거 대신 너가 원하는 숫자나 변수 넣으면 그 mm로 움직일거임. (절대좌표) 범위 0~148
//   // 이 아래는 볼 스크류 모터 움직이는 부분
//   digitalWrite(motorDirB, CMotor_B.dir);
//   analogWrite(motorPwmB, CMotor_B.pwm);

//   // CMotor_B.mm_position == 현재 mm값(원점 기준) 받아오는 변수

//   // 필요없는거 (출력부)
//   Serial.print(CMotor_B.pulse_count); 
//   Serial.print('\t');
//   // Serial.println(CMotor_B.input_B);
//   Serial.print(CMotor_B.mm_position);
//     Serial.print('\t');
//   Serial.println(CMotor_B.arrival);
// }
