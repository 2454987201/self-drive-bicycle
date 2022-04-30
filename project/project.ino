#include <MPU6050_light.h> //MPU6050库，能计算出三个方向角
#include "Wire.h" //包含IIC通信库
#include <Servo.h> //包含舵机库
#include "ArduPID.h"  //包含PID调参库
ArduPID myController;

//mpu
MPU6050 mpu(Wire);  //定义MPU6050对象mpu
unsigned long timer = 0;

//servo
Servo mg90;   //定义舵机对象mg90
int servo_pin = 13;  //定义舵机PWM信号引脚

//pid
double Setpoint, Input, Output; //设置PID的期望值，输入，输出
double p=4.71, i=54.64, d=0;    //设置PID的调节参数

void setup() {
  Serial.begin(9600);  //串口初始化
  Wire.begin();  //加入总线进行IIC通信
  
  //舵机初始化
  mg90.attach ( servo_pin );  //舵机初始化
  
  //mpu6050初始化
  byte status = mpu.begin();  //MPU6050初始化
  Serial.print(F("MPU6050的状态: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  Serial.println(F("正在计算偏移量, 不要移动MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("已完成!\n");
  
  //PID初始化
  Setpoint = 0; //设置期望值
  myController.begin(&Input, &Output, &Setpoint, p, i, d);
  myController.reverse() ;
  //myController.setOutputLimits(-90, 90);
  myController.setBias(180.0 / 2.0);
  myController.setWindUpLimits(-1, 1);
  myController.start();

  Input = mpu.getAngleX();  //设置输入为mpu6050的Y角
  //myPID.SetMode(AUTOMATIC); //设置PID模式
}

void loop() {
  mpu.update();
  if((millis()-timer)>20){ // print data every 10ms
  Input = mpu.getAngleX();
  myController.compute();
  Serial.println(Output);
  mg90.write(Output);
  Serial.print("INTPUT : ");
  Serial.print(mpu.getAngleX());
  Serial.print("  OUTPUT : ");
  Serial.println(Output);
  timer = millis();  
  }
}
