#include <Servo.h>

String inputString = "25";         // a string to hold incoming data

int flag = 0;

int servoPin8 = 8;
int servoPin9 = 9;
int servoPin11 = 11;
int servoPin12 = 12;
int inChar = 0;

Servo servo8;
Servo servo9;
Servo servo11;
Servo servo12;
int theta = 90,theta8,theta9=120,theta11=0,Delay=2000; // Servo position in degrees
float r=16;

void setup() {
  // initialize serial:
  Serial.begin(9600); 
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  // Initiate Servo Motor
  servo8.attach(servoPin8);
  servo9.attach(servoPin9);
  servo11.attach(servoPin11);
  servo12.attach(servoPin12);
  initialize();
  delay(1000);
}

void loop() {
  
  // print the string when a newline arrives:
    //Serial.println(inputString);
    if(Serial.available()>0 & flag  == 0) {
//      theta = Serial.parseInt();
//      delay(2000);
//      r = Serial.parseInt();
       r = Serial.parseInt();
       theta = r/100;
       r = r-(100*theta);
      flag =1;  
   }
   
   
  if(flag==1 && ((r>16) &&(r<26)) ){   
    flag =2;
    servo9.write(90);//180-0
  delay(Delay);
  servo11.write(90);//0-180
  delay(Delay);
    //flag =1;
   // initialize();
    //delay(1000);
    //servo8.write(theta); //0-180
    //delay(Delay);
    theta8=theta;
    theta9 = 140+((r-16)*40/10);
    
    theta11 = (r-16)*70/10 ;
    
    servo8.write(theta8); //0-180
  delay(Delay);
  servo9.write(theta9);//180-0
  delay(Delay);
  servo11.write(theta11);//0-180
  delay(Delay);
  servo12.write(70);
  delay(Delay);
    //moveto(theta8, theta9, theta11);
   // initialize();
   returnback();
    
  }
}

void initialize(){
  servo8.write(0); //0-180
  delay(Delay);
  servo9.write(180);//180-0
  delay(Delay);
  servo11.write(70);//0-180
  delay(Delay);
  servo12.write(150);  //70-100
  delay(Delay);
    
}

void moveto(int theta8, int theta9,int theta11){
  servo8.write(theta8); //0-180
  delay(Delay);
  servo9.write(theta9);//180-0
  delay(Delay);
  servo11.write(theta11);//0-180
  delay(Delay);
  servo12.write(70);
  delay(Delay);  
}

void returnback(){
  servo9.write(90);//180-0
  delay(Delay);
  servo11.write(70);//0-180
  delay(Delay);
  servo8.write(0); //0-180
  delay(Delay);
  servo9.write(175);//180-0
  delay(1000);
  servo12.write(100);
  delay(Delay);
}
/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
//void serialEvent() {
//  while (Serial.available()) {
//    // get the new byte:
//    inChar = (strToInt)Serial.read() ;
//    //inChar = (int)Serial.parseInt() ;
//    // add it to the inputString:
//   inputString += inChar;
//     delay(15000);
//     Serial.println(inputString);
////    
//    //char theta = (char)Serial.read(); 
//
//    
//  }
//}
