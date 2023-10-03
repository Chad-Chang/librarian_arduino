// #include "CServo.h"

// CServo cservo;

// void setup() {
//   Serial.begin(9600);
//   cservo.attach(5);
//   cservo.setAngleLimit(0, 120);
//   cservo.setInitAngle(60, true);  // init angle, inc(+:1, -:0)  // 90 : 오므림
//   cservo.setGrabPeriod(1000);  // set between 0~65535, recommended : 1000
//   // cservo.attachFSR(A1); 
// }

// void loop() {
//   String user_input;
//   while(Serial.available())
//   {
//     user_input = Serial.readStringUntil('\n'); // Get Input from Serial Monitor
//     if(user_input =="1"){
//       cservo.gripperOpen();
//     }
//     else if(user_input =="2"){
//       cservo.gripperClose();
//     }
//     else if(user_input =="3"){
//       cservo.grabReset();
//     }
    
    
//     else if(user_input =="4"){
//       cservo.target = 0;
//     }
//     // else if(user_input =="5"){
//     //   cservo.target = 20;
//     // }
//     else if(user_input =="5"){
//       cservo.target = 20;
//     }


//   }
//   // Serial.print("isGrab=");Serial.println(cservo.isGrab);
//   // cservo.grab(0);


//   cservo.grab();

//   // cservo.checkFSR();
//   // cservo.printFSR();

// }
