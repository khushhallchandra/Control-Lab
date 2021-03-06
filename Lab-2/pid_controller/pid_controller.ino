#define motorout A0
#define finalout1 6
#define finalout2 7

int  out, feedback, timer, T = 100; 
float error, perr, derr, ierr, kp, kd, ki, setpoint, lasterr = 0,count =0;

void setup() {
	Serial.begin(9600);

    pinMode(motorout,INPUT);
    pinMode(finalout1,OUTPUT);
    pinMode(finalout2,OUTPUT);

    kp=100;
    kd=10;
    ki=0.0001;
     
}

void loop() {
    feedback = analogRead(motorout);

    if(count==0){
        setpoint=2.4 + feedback * 5.0 / 1024;
        if (setpoint >5 )
            setpoint -= 5; 
        count++;
    }
    
    error = setpoint - feedback * 5.0 / 1024;
    perr = kp * error;
    derr = kd * (error - lasterr);
    ierr += ki * error;
    lasterr=error;
    out = perr + derr + ierr;

    timer = millis();
    Serial.print(timer);
    Serial.print("    ");
    Serial.println(out);

    if(out>0) {
        digitalWrite(finalout1 , HIGH);
        digitalWrite(finalout2 , LOW);

        if(out<T) {
            delay(out);
            digitalWrite(finalout1 , LOW);  
            digitalWrite(finalout2 , LOW);
            delay(T-out);
        }
        else
            delay(T);

    }

    else
    if(out <0 ) {
        digitalWrite(finalout1 , LOW);
        digitalWrite(finalout2 , HIGH);

        if(out > -T) {
            delay(-out);
            digitalWrite(finalout1 , LOW);  
            digitalWrite(finalout2 , LOW);
            delay(T+out);
        }
        else
            delay(T);

    }
}
