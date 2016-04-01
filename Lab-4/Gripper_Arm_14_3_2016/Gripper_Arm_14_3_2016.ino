#include <Servo.h>

String inputString = "25";         // a string to hold incoming data

int flag = 0;

int servoPin8 = 8;
int servoPin9 = 9;
int servoPin11 = 11;
int servoPin12 = 12;

Servo servo8;
Servo servo9;
Servo servo11;
Servo servo12;
int theta = 90,theta8,theta9,theta11,Delay=500; // Servo position in degrees
float r=25;

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
  
}

void loop() {
  // print the string when a newline arrives:
    Serial.println(inputString);

  if(flag==0){
    flag = 1;
    initialize();
    theta8=theta;
    theta9 = (r-16.6)*50/9.4 +120;
    theta11 = (r-16.6)*90/9.4 ;
    moveto(theta8, theta9, theta11);
    returnback();
    
  }
}

void initialize(){
  servo8.write(0); //0-180
  delay(Delay);
  servo9.write(90);//180-0
  delay(Delay);
  servo11.write(90);//0-180
  delay(Delay);
  servo12.write(100);  //70-100
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
  servo11.write(90);//0-180
  delay(Delay);
  servo8.write(0); //0-180
  delay(Delay);
  servo9.write(170);//180-0
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
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
  }
}
