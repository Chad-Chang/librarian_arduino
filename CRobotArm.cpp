#include "CRobotArm.h"

uint32_t CRobotArm::ang2pwm(float x){
  uint32_t pwm = (uint32_t)((x/360)*4095);
  return pwm;
}
uint32_t CRobotArm::ang2pwm_ext(float x){
  if(x>=0 && x<=420){
    uint32_t pwm = (uint32_t)((x/420)*4780);
    return pwm;
  }
  else if(x<0 && -60<=x){
    
    uint32_t pwm = (uint32_t)((x/60)*683);
    return pwm;
  }
}

float CRobotArm::pwm2ang(float x){
  float angle = (float)((x/4095)*360);
  return angle;
}

void CRobotArm::robot_init(){
  bool result = this->init(DEVICE_NAME, BAUDRATE, &log);

  for (int cnt = 0; cnt < 7; cnt++)
  {
    result = this->ping(dxl_id[cnt], &model_number0, &log);
  }

  result = this->torqueOff(dxl_id[0], &log);
  result = this->setExtendedPositionControlMode(dxl_id[0], &log);
  result = this->torqueOn(dxl_id[0], &log);

  result = this->jointMode(dxl_id[1], 60, 0, &log);

  result = this->jointMode(dxl_id[2], 60, 0, &log);

  result = this->jointMode(dxl_id[3], 40, 0, &log);

  result = this->jointMode(dxl_id[4], 40, 0, &log);

  result = this->torque(dxl_id[5],1, &log);

  result = this->torque(dxl_id[6],1, &log);

  for(int i=0; i<7; i++){
    result = this -> writeRegister(dxl_id[i], "Profile_Velocity", 120, &log);  
    result = this -> writeRegister(dxl_id[i], "Position_I_Gain", 400, &log);
    result = this -> writeRegister(dxl_id[i], "Position_D_Gain", 30, &log);
    result = this -> writeRegister(dxl_id[i], "Profile_Acceleration", 20, &log);
  }

  result = this -> addSyncWriteHandler(dxl_id[0], "Goal_Position", &log);

  result = this -> addSyncReadHandler(dxl_id[0], "Present_Position", &log);

  goal_position[0] = this->ang2pwm_ext(180.7);// angle(0);
  goal_position[1] = this->ang2pwm(88.33);//angle(1.892950/pi*180);
  goal_position[2] = this->ang2pwm(89.95);//angle(0.515393/pi*180);
  goal_position[3] = this->ang2pwm(181.37);//angle(0);
  goal_position[4] = this->ang2pwm(179.3);//angle(0.192981/pi*180);
  goal_position[5] = this->ang2pwm(270);//angle(0);
  goal_position[6] = this->ang2pwm(197.67);

  result = this -> syncWrite(handler_index, &goal_position[0], &log);
  delay(1000);
}

void CRobotArm::robot_run(bool& packet_, float *jnt_ang_){
  if(packet_){
      packet_ = false;
      status = jnt_ang_[0];
      if((status!=end_rob)&&(status!=read_fail)&&(status!=BS_back)&&(status!=bscrew_mode)&&(status!=shelve1_f)&&(status!=shelve2_f)&&(status!=100)&&(status!=101)){
        goal_position[0] = this->ang2pwm_ext(jnt_ang_[1]);// angle(0);
        goal_position[1] = this->ang2pwm(jnt_ang_[2]);//angle(1.892950/pi*180);
        goal_position[2] = this->ang2pwm(jnt_ang_[3]);//angle(0.515393/pi*180);
        goal_position[3] = this->ang2pwm(jnt_ang_[4]);//angle(0);
        goal_position[4] = this->ang2pwm(jnt_ang_[5]);//angle(0.192981/pi*180);
        goal_position[5] = this->ang2pwm(jnt_ang_[6]);//angle(0);
        goal_position[6] = this->ang2pwm(jnt_ang_[7]);
        for(int j= 1; j<8;j++){
          goal_position[j-1] = this->ang2pwm(jnt_ang_[j]);
        }
      }
      result = this -> syncWrite(handler_index, &goal_position[0], &log);
    }
}

void CRobotArm::robot_read(){
  bool result = false;
  int32_t present_position[8] = {0, 0, 0, 0, 0, 0, 0};

  result = this->syncRead(handler_index, &log);
    if (result == false)
    {
      Serial.println(log);
      Serial.println("Failed to sync read position");
    }

    result = this->getSyncReadData(handler_index, &present_position[0], &log);
    if (result == false)
    {
      Serial.println(log);
    }
    else
    {
      for(int cnt = 0; cnt<8; cnt++){
        Serial.print("[ID ");
        Serial.print(dxl_id[cnt]);
        Serial.print(" ]");
        Serial.print(" Goal Position : ");
        Serial.print(pwm2ang(goal_position[cnt]));
        Serial.print(" Present Position : ");
        Serial.println(pwm2ang(present_position[cnt]));
      }
    }
}
