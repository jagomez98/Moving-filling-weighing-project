// Peripheral Configuration Code (do not edit)
//---CONFIG_BEGIN---
#pragma GCC push_options
#pragma GCC optimize ("Os")
#include <core.h> // Required by cpu
#include <cpu.h>
#pragma GCC pop_options
// Peripheral Constructors
CPU &cpu = Cpu;
void peripheral_setup () {
}
void peripheral_loop() {
}
//---CONFIG_END---

boolean sensor0,sensor1,sensor2,sensor3,stopButton;//sensor and stop button declaration
boolean lift=false;//lift flag
boolean bucketsIn=false;//buckets in flag
boolean countFlag=false;//flag used to count buckets
boolean discharge=false;//discharge flag
boolean fillFlag=false;//filling flag
boolean countFlagS3=false;//flag used to count buckets leaving the weighing area
int count1=0;//counter for buckets entering weighing area
int count2=0;//counter for buckets leaving the weighing area
volatile byte state = LOW;//state of the conveyor
void setup () {
peripheral_setup();
// TODO: put your setup code here, to run once:
pinMode(0,INPUT);//sensor0
pinMode(1,INPUT);//sensor1
pinMode(8,INPUT);//sensor2
pinMode(3,INPUT);//sensor3
//pinMode(8, INPUT);//stopButton
pinMode(11,OUTPUT); //conveyor belt
pinMode(4, OUTPUT);//cylinder0 stopper
pinMode(5,OUTPUT);//cylinder1
pinMode(6,OUTPUT);//cylinder2
pinMode(7, OUTPUT);//cylinder3
pinMode(9,OUTPUT);//Lift
pinMode(10,OUTPUT);//Fill
attachInterrupt(digitalPinToInterrupt(2), stopInterruption, CHANGE);//interruption for stop button
}
void loop() {
peripheral_loop();
// TODO: put your main code here, to run repeatedly:
//reading of the sensors
sensor0=digitalRead(0);
sensor1=digitalRead(1);
sensor2=digitalRead(8);
sensor3=digitalRead(3);
if(sensor0)countFlag=false;
if (!sensor0 && countFlag==false){//count of buckets in
   count1++;
   countFlag=true;
   if(count1==3){//if count is equal to three, activate bucketsIn flag
      digitalWrite(4,HIGH);
      count1=0;
      bucketsIn=true;
      }
}
if(!sensor1 && !sensor2 && !sensor3&& discharge==false &&bucketsIn){//check for sensor1 sensor2 and sensor3
   digitalWrite(5,HIGH);
   lift=true;//activate lift flag
   
}else if(!sensor2 && !sensor3 &&discharge==false){ //check for sensor 2 and 3
   digitalWrite(6,HIGH);
}else if(!sensor3 && discharge==false){//check for sensor3
   digitalWrite(7,HIGH);
}
if (lift){
   digitalWrite(9,HIGH);
   delay(1500);
   digitalWrite(5,LOW);
   digitalWrite(6,LOW);
   digitalWrite(7,LOW);
   fillFlag=true;//activate filling flag
}
if (fillFlag){
digitalWrite(10,HIGH);
delay(5000);
 lift=false;
digitalWrite(10,LOW);
digitalWrite(9,LOW);
digitalWrite(4,LOW);
fillFlag=false;
discharge=true;
bucketsIn=false;
}
if(sensor3)countFlagS3=false;
if(!sensor3 && countFlagS3==false){//count buckets out
   count2++;
   countFlagS3=true;
   if(count2==3){
      count2=0;
      discharge=false;
      delay(800);
   }
}
}

void stopInterruption(){//ISR for interruption
   state=!state;
   digitalWrite(11,state);
   digitalWrite(10,LOW);
   fillFlag=false;
}