#include "FakeSign.h"   // 기본 헤더파일

void setup() {
  /* CServo setting */
  jnt_ang[0] = 0; // 초기 status =0 - 대기
  cservo.attach(5);
  cservo.setAngleLimit(0, servo_ang_max);
  cservo.setInitAngle(servo_ang_min, true);  // init angle, inc(+:1, -:0)  // 120 : 오므림
  cservo.setGrabPeriod(3000);  // set between 0~65535, recommended : 1000
  cservo.attachFSR(A0); 

  /* RobotArm setting */ 
  robot.robot_init(); // 주행만 할때 꺼야함

  /* pinMode setting */
  setPinMode();

  /* IMU setting */
  init_imu();
  setInitAngle();
  
  /* DC Motor pin setting */
  CMotor_L.begin(ENC_L_2, ENC_L_4, 1);   // int pin1, int pin2, motor_num
  CMotor_R.begin(ENC_R_7, ENC_R_8, 0);
  CMotor_B.begin(ENC_B_45, 72, 2);

  /* Ball screw position initialize */
  init_ballscrew(); // 주행만할때 꺼야함

  /* Serial setting */
  Serial1.begin(115200);
  Serial1.setTimeout(0.1);
  Serial.end();
  Serial.begin(9600);
  init_serial();
}

void loop() {
  /* odom으로 보내는 통신 */
  if(millis() - prev_time >= ODOM_INTERVAL){
    CMotor_L.CurrentVel(millis() - prev_time);
    CMotor_R.CurrentVel(millis() - prev_time);
    
    update_imu();
    update_theta();

    if(heading_angle!=0.){  // 서가 진입 전 IMU로 차체 회전
      rotate_heading_angle();
    }
    
    send_message_to_odom();

    prev_time = millis();
  }

  /* (DC Motor) Encoder pid */
  CMotor_L.EncoderPID();
  CMotor_R.EncoderPID();

  /* (DC Motor) convert pid result to PWM */
  CMotor_L.PIDtoPWM();
  CMotor_R.PIDtoPWM();

  /* (DC Motor) motor output */
  motor_write();

  /* (RobotArm) robot arm main */
  robot_arm();//주행할때 끄셈

  /* check whether robot state done */
  // get_last_signal();  // robot

  /* (Buzzer) lost and couldn't find way */
  // if (sound_on){
  //   sensors.makeMelody(1);
  //   sound_on = sensors.onMelody();
  // }
}

/* Serial Communication Read (Odom -> OpenCR) */
void serialEvent() {
  reset_time = millis();
  // s{linx},{angz}a{sound_logic}b{heading_angle}c{ball_screw}e
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();

    // initialize OpenCR if Nav2 would be restarted
    if (inChar=='S'){  
      rotate_done = "0";
      Last_signal="0";
      count=0;   //임시변수. 테스트용.
      return;
    }

    // add it to the inputString:
    cmd += inChar;

    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      if(cmd.startsWith("s") && cmd.endsWith("e\n")){   //창민이꺼 로봇팔 동작할때는 시작문자를 다른거써서 프로토콜 바꿔사용하기.
        // Serial.println(cmd);
        linx = cmd.substring(1,cmd.indexOf(',')).toFloat();
        
        if(linx < 0.04 and linx > 0.0){  //모터 데드존 고려.
          linx=0.04;
        }
        else if(linx > -0.04 and linx < 0.0){
          linx=-0.04;
        }

        angz = cmd.substring(cmd.indexOf(',')+1,cmd.indexOf('a')).toFloat();
        logic_warn_sound = cmd.substring(cmd.indexOf('a')+1,cmd.indexOf('b')).toInt();
        heading_angle = cmd.substring(cmd.indexOf('b')+1,cmd.indexOf('c')).toFloat();
        ball_screw= cmd.substring(cmd.indexOf('c')+1,cmd.length()-1).toInt();  //이제 추가로 if ball_screw일때 로봇팔 동작하도록 코드짜기.
        // Serial.print(String(int(heading_angle*1000)));
        if (logic_warn_sound==1){
          // sound_on = true;
          sensors.makeMelody(1);
          sensors.onMelody();
          CMotor_R.pulse=0;
          CMotor_L.pulse=0;
          cmd = "";
          return;
        }
        if (heading_angle!=0.){
          if(heading_angle>0.) angz=0.3;
          else angz = -0.3;
          theta = atan2f(ori_w * ori_z, 0.5f - ori_z  * ori_z);  //0.9-> 1.0이 아니라 0.9->0.10이 되어버린다. 아두이노 float->string 변환 안됨. 값이상해짐. 이부분 파이썬으로 옮기기.
          last_theta = theta;
          theta_sum = 0;
        }

        if (ball_screw==1){   //임시변수. 테스트용. 헤딩액션 2번하면 피드백한번. 
          robot_op = true;
          robot.status = bscrew_mode; 
          Serial1.println("/4/");
          // Last_signal="1";
          }
        // if (count==3){
        //   Last_signal="2";
        //   count++;
        // }
        CMotor_L.CMDVELtoTarget(linx, angz);
        CMotor_R.CMDVELtoTarget(linx, angz);
      }
      cmd = "";
    }
  }
}
/* TCP IP communication(uart 각도 + depth 통신) */
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
      // Serial.print("status =");
      // Serial.println(jnt_ang[0]);
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
        // Serial.println(depth);
        // Serial.print("depth =  ");
        // Serial.println(depth);
        // Serial.print("\t");
        // Serial.print("depth_ball =   ");
        // Serial.println(depth_ball);
        }
      }
    else{
      serialInBuffer[buff_cnt++] = byteIn;
    }
  }
}
//안녕