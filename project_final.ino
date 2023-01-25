#include <Stepper.h>
#include <Servo.h>
#include <NewPing.h>
#include<SoftwareSerial.h>


int Tx=0;
int Rx=1;

#define TRIGGER_PIN 12
#define ECHO_PIN 13
#define MAX_D 20 


SoftwareSerial HC_06(Tx,Rx);

String comandValue;
String cmdb;

Servo myServo;

int curr_x,curr_y;
////////////////////////// output position
/////////////////////////////////////////
char *drug[] = {"Etorix", "Acamol", "Disprospan",  "Diclofen", "Trophin", "Lorias"};
int posxo[6]={10,4250,8750,10,4250,8750};
int posyo[6]={7125,7125,7125,1700,1660,1700};
int posxi[6]={10,4250,8750,10,4250,8750};
int posyi[6]={9875,9875,9875,4500,4500,4500};
int degree[6]={88,88,65,88,88,60}; 
int countdrug[6]={2,3,1,1,0,1};

const int steppinx1 = 5;
const int dirpinx1 = 6;

const int steppiny = 8;
const int dirpiny = 9;

const int linerx=2;
const int linery=3;

//const int steppiny = 2;
//const int dirpiny = 3;


const int spr = 2500;

int flag=0;
int pre=0;
int dis=0;
    String trimm;

 

Stepper mystepx(spr, dirpinx1, steppinx1);
Stepper mystepy(spr, dirpiny, steppiny);
//Stepper mystepz(spr, dirpiny, steppiny);

NewPing sonar(TRIGGER_PIN,ECHO_PIN,MAX_D); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  HC_06.begin(9600);
   
  mystepx.setSpeed(50);
  myServo.attach(10);
  
  pinMode(steppinx1, OUTPUT);
  pinMode(dirpinx1, OUTPUT);
  pinMode(linerx,OUTPUT);
  pinMode(linery,OUTPUT);

  
  // mystep.step(-800);
  // movey(1600);


  mystepy.setSpeed(50);
  pinMode(steppiny, OUTPUT);
  pinMode(dirpiny, OUTPUT);
//
//  mystepz.setSpeed(50);
//  pinMode(steppinz, OUTPUT);
//  pinMode(dirpinz, OUTPUT);

  Serial.println("start..");
  Serial.println("Operation Option");
  Serial.println("----------------------------------");
  Serial.println("1.Input process.    Enter Input.");
  Serial.println("2.Output process. Enter Output.");
  Serial.println("----------------------------------");
}
int deg;
void loop() {

  
  
  
  
  //Serial.println("what is the drug you want ?");
  while (Serial.available() == 0) {}
  comandValue = Serial.readString();
  cmdb=Serial.read();
  Serial.println(comandValue);
  Serial.println(cmdb);
  
 
  myServo.write(20);
 comandValue.trim();
 cmdb.trim();
 
 //***********************************************************************************************************************OUTPUT OPERATION
 if(comandValue.equals("output")|| cmdb.equals("output"))
 {
  Serial.print("*****List Of Drug******\n");  
  for(int x=0;x<6;x++)
  {
    Serial.println(drug[x]);
    Serial.println(countdrug[x]);
  }
 Serial.print("**************************\n");
  
  flag=1;

   deg=150;
  //
  //
  if((deg-pre)>=0){
    for(int i=pre; i<deg; i++){
     // Serial.println(i);
      myServo.write(i);
    delay(20);
      }
      pre=deg;
    
    }else{
      
      for(int i= pre; i>=deg;i--){
      myServo.write(i);
    //55
    delay(20);
        
        }
        pre=deg;
      }
  
  Serial.println("Search for Drug");
  while (Serial.available() == 0) {}
  comandValue = Serial.readString();
  Serial.println(comandValue);
  comandValue.trim();
  
  cmdb=Serial.read();
  Serial.println(cmdb);
  comandValue.trim();
  
  
  // Serial.println(comandValue.length());
  // check what is the crosspunding position for the input drug
  Serial.println("Machine in prgress..cnc");

  
 for(int j=0;j<6;j++)
 {
   if(comandValue.equalsIgnoreCase(drug[j])||cmdb.equalsIgnoreCase(drug[j])) {
    Serial.println("position");
    Serial.println(j+1);
    if(j+1==1)//sensor panadol
    {

      dis=sonar.ping_cm();
      Serial.println(dis);
      if(dis==15)
      {
      Serial.print("the Shelf is Empty , please fill The shelf");
      flag=0;
      }
      else if(1<=countdrug[j]&&countdrug[j]<=3){
         countdrug[j]--; 
    curr_x=posxo[j];
    curr_y=posyo[j];
    mystepx.step(posxo[j]);
    delay(200);
    mystepy.step(posyo[j]);
    deg=degree[j];

    flag=1;
    }
    else{
      Serial.print("The Shelf is Empty");
      flag=0;
    }

     
    }
    else if(1<=countdrug[j]&&countdrug[j]<=3)
    {
     countdrug[j]--; 
    curr_x=posxo[j];
    curr_y=posyo[j];
    mystepx.step(posxo[j]);
    delay(200);
    mystepy.step(posyo[j]);
    deg=degree[j];
    flag=1;
    }
      else{
      Serial.print("The Shelf is Empty");
      flag=0;
    }

   
   break;
  }
  else{
    Serial.println("Invalid drug");
    flag=0;
    }
  
 }

 }
 
 //***********************************************************************************************************************INPUT OPERATION
 else if(comandValue.equals("input")||cmdb.equals("input")){

  flag=2;
      
    Serial.println("Search for Drug");
  while (Serial.available() == 0) {}
  comandValue = Serial.readString();
  Serial.println(comandValue);
  comandValue.trim();

  
  // Serial.println(comandValue.length());
  // check what is the crosspunding position for the input drug
  Serial.println("Machine in prgress..cnc");

 for(int j=0;j<6;j++)
 {
   if (comandValue.equalsIgnoreCase(drug[j])||cmdb.equalsIgnoreCase(drug[j])) {
    Serial.println("position");
    Serial.println(j+1);
        if(j+1==1)//sensor
    {
       dis=sonar.ping_cm();
      Serial.println(dis);
      
      if(dis==7)
      {
      Serial.print("the Shelf is Full"); 
      flag=0;
      }
      else if(countdrug[j]<3){
        countdrug[j]++;
        
    curr_x=posxi[j];
    curr_y=posyi[j];

    
        deg=150;
     if((deg-pre)>=0){
    for(int i=pre; i<deg; i++){
      //Serial.println(i);
      myServo.write(i);
    delay(20);
      }
      pre=deg;
    
    }else{
      
      for(int i= pre; i>=deg;i--){
      myServo.write(i);
    //55
    delay(20);
        
        }
        pre=deg;
      } 
//********************************************************************
     Serial.print("put the medcine");
     Serial.print(pre);
     delay(5000);
      deg=degree[j];
      if((deg-pre)>=0){
    for(int i=pre; i<deg; i++){
      //Serial.println(i);
      myServo.write(i);
    delay(20);
      }
      pre=deg;
    
    }else{
      
      for(int i= pre; i>=deg;i--){
      myServo.write(i);
    //55
    delay(20);
        
        }
        pre=deg;
      } 
    mystepx.step(posxi[j]);
    delay(200);
    mystepy.step(posyi[j]);
    flag=2;
    }
    else{
            Serial.print("The Shelf is full");
      flag=0;
    }
    }
    else if(countdrug[j]<3){
         countdrug[j]++;
      
          curr_x=posxi[j];
    curr_y=posyi[j];

    
        deg=150;
     if((deg-pre)>=0){
    for(int i=pre; i<deg; i++){
      //Serial.println(i);
      myServo.write(i);
    delay(20);
      }
      pre=deg;
    
    }else{
      
      for(int i= pre; i>=deg;i--){
      myServo.write(i);
    //55
    delay(20);
        
        }
        pre=deg;
      } 
//********************************************************************
     Serial.print("put the medcine");
     Serial.print(pre);
     delay(5000);
      deg=degree[j];
      if((deg-pre)>=0){
    for(int i=pre; i<deg; i++){
      //Serial.println(i);
      myServo.write(i);
    delay(20);
      }
      pre=deg;
    
    }else{
      
      for(int i= pre; i>=deg;i--){
      myServo.write(i);
    //55
    delay(20);
        
        }
        pre=deg;
      } 
    mystepx.step(posxi[j]);
    delay(200);
    mystepy.step(posyi[j]);
    flag=2;
      
    }
    else{
      Serial.print("The Shelf is Full");
      flag=0;
    }
    
   break;
  }
  else{
    Serial.println("Invalid drug");
    flag=0;
    }
  
 }
 
 }
 else if(comandValue.equals("linearb")){
  Serial.println("S.L.C");
digitalWrite(linerx,LOW);
digitalWrite(linery,HIGH);
delay(16500);
digitalWrite(linerx,LOW);
digitalWrite(linery,LOW);// linear
Serial.println("E.L.C");
 Serial.println("in arm closing!");
 
 }

 else{
  Serial.print("Invalid operation");
  flag=0;

  
 }
 
//Serial.println("S.L.C");
//digitalWrite(linerx,LOW);
//digitalWrite(linery,HIGH);
//delay(16500);
//digitalWrite(linerx,LOW);
//digitalWrite(linery,LOW);// linear
//Serial.println("E.L.C");
// Serial.println("in arm closing!");

Serial.println(flag);

if(flag==1){//OUTPUT
  flag=0;
  //************************************************************************************************** linear open
Serial.println("S.L.O");
digitalWrite(linerx,HIGH);
digitalWrite(linery,LOW);
delay(17500);
digitalWrite(linerx,LOW);
digitalWrite(linery,LOW);// linear
Serial.println("E.L.O");

delay(2000);
//***************************************************************************************** SERVO

  
  
   
  //
  //
  Serial.println(deg);
  if((deg-pre)>=0){
    for(int i=pre; i<deg; i++){
     // Serial.println(i);
      myServo.write(i);
    delay(20);
      }
      pre=deg;
    
    }else{
      
      for(int i= pre; i>=deg;i--){
      myServo.write(i);
    //55
    delay(20);
        
        }
        pre=deg;
      }
  
// ***************************************************************************************************linear close
Serial.println("S.L.C");
digitalWrite(linerx,LOW);
digitalWrite(linery,HIGH);
delay(17500);
digitalWrite(linerx,LOW);
digitalWrite(linery,LOW);// linear
Serial.println("E.L.C");
 Serial.println("in arm closing!");


}
else if(flag==2){//INPUT
  flag=0;

  Serial.println("S.L.O");
digitalWrite(linerx,HIGH);
digitalWrite(linery,LOW);
delay(17800);
digitalWrite(linerx,LOW);
digitalWrite(linery,LOW);// linear
Serial.println("E.L.O");

delay(5000);
//***************************************************************************************** SERVO

  
  
   
  //
  //
  deg=150;
  Serial.println(deg);
  if((deg-pre)>=0){
    for(int i=pre; i<deg; i++){
      //Serial.println(i);
      myServo.write(i);
    delay(20);
      }
      pre=deg;
    
    }else{
      
      for(int i= pre; i>=deg;i--){
      myServo.write(i);
    //55
    delay(20);
        
        }
        pre=deg;
      }
  
// ***************************************************************************************************linear close
Serial.println("S.L.C");
digitalWrite(linerx,LOW);
digitalWrite(linery,HIGH);
delay(17800);
digitalWrite(linerx,LOW);
digitalWrite(linery,LOW);// linear
Serial.println("E.L.C");
 Serial.println("in arm closing!");


  }
  ////////////////////////////////////
  delay(5000);
  // linear close
  
//go back to the origin 
//mystepx.step(-curr_x);
mystepy.step(-curr_y);
delay(200);
//mystepy.step(-curr_y);
mystepx.step(-curr_x);
curr_x=0;
curr_y=0;

delay(1000);

   deg=150;
  if((deg-pre)>=0){
    for(int i=pre; i<deg; i++){
      //Serial.println(i);
      myServo.write(i);
    delay(20);
      }
      pre=deg;
    
    }else{
      
      for(int i= pre; i>=deg;i--){
      myServo.write(i);
    //55
    delay(20);
        
        }
        pre=deg;
      }

  if(flag==1)
  {
  Serial.print("plese catch the medcine from cart");
  }
  
  Serial.println("Operation Finished.");
  Serial.println(".........................................");
  Serial.println("Choose a new Operation..Input/Output");


//  for(int x=0;x<6;x++)
//  {
//
//    Serial.println(countdrug[x]);
//    
//   }
  delay(5000);

   

    
  
   deg=40;
    if((deg-pre)>=0){
    for(int i=pre; i<deg; i++){
     // Serial.println(i);
      myServo.write(i);
    delay(20);
      }
      pre=deg;
    
    }else{
      
      for(int i= pre; i>=deg;i--){
      myServo.write(i);
    //55
    delay(20);
        
        }
        pre=deg;
      }

  
  

  // put your main code here, to run repeatedly:


}
