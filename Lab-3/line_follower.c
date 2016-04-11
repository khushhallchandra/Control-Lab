#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void motion_pin_config (void){
	DDRB = DDRB | 0x0F;   //set direction of the PORTB3 to PORTB0 pins as output
	PORTB = PORTB & 0xF0; // set initial value of the PORTB3 to PORTB0 pins to logic 0
	DDRD = DDRD | 0x30;   //Setting PD4 and PD5 pins as output for PWM generation
	PORTD = PORTD | 0x30; //PD4 and PD5 pins are for velocity control using PWM
}

//Function to initialize ports
void port_init(){
	motion_pin_config();
	DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
	PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
}

//Function used for setting motor's direction
void motion_set (unsigned char Direction){
	unsigned char PortBRestore = 0;
	Direction &= 0x0F;                         // removing upper nibbel as it is not needed
	PortBRestore = PORTB;                         // reading the PORTB's original status
	PortBRestore &= 0xF0;                         // setting lower direction nibbel to 0
	PortBRestore |= Direction;         // adding lower nibbel for direction command and restoring the PORTB status
	PORTB = PortBRestore;                         // setting the command to the port
}

void forward (void)         //both wheels forward
{
	motion_set(0x06);
}

void back (void)            //both wheels backward
{
	motion_set(0x09);
}

void left (void)            //Left wheel backward, Right wheel forward
{
	motion_set(0x05);
}

void right (void)           //Left wheel forward, Right wheel backward
{
	motion_set(0x0A);
}

void soft_left (void)       //Left wheel stationary, Right wheel forward
{
	motion_set(0x04);
}

void soft_right (void)      //Left wheel forward, Right wheel is stationary
{
	motion_set(0x02);
}

void soft_left_2 (void)     //Left wheel backward, right wheel stationary
{
	motion_set(0x01);
}

void soft_right_2 (void)    //Left wheel stationary, Right wheel backward
{
	motion_set(0x08);
}

void hard_stop (void)       //hard stop(stop suddenly)
{
	motion_set(0x00);
}

void soft_stop (void)       //soft stop(stops solowly)
{
	motion_set(0x0F);
}

void adc_pin_config (void){
	DDRA = 0x00; //set PORTA direction as input
	PORTA = 0x00; //set PORTA pins floating
}

//Function to Initialize ADC
void adc_init(){
	ADCSRA = 0x00;
	ADMUX = 0x20; //Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86; //ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

//This Function accepts the Channel Number and returns the correspondingAnalog Value
unsigned char ADC_Conversion(unsigned char Ch){
	unsigned char a;
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40; //Set start conversion bit
	while((ADCSRA&0x10)==0); //Wait for ADC conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	//ADCSRB = 0x00;
	return a;
}


void init_devices (void){
	cli(); //Clears the global interrupts
	port_init();
	adc_init();
	timer1_init();
	sei(); //Enables the global interrupts
}

// Timer 5 initialized in PWM mode for velocity control
// Prescale: 64
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:450.000Hz
void timer1_init(){
	TCCR1B = 0x00; //Stop
	TCNT1H = 0xFF; //Counter higher 8-bit value to which OCR1xH value iscompared with
	TCNT1L = 0x01; //Counter lower 8-bit value to which OCR1xH value iscompared with
	OCR1AH = 0x00; //Output compare register high value for Left Motor
	OCR1AL = 0xFF; //Output compare register low value for Left Motor
	OCR1BH = 0x00; //Output compare register high value for Right Motor
	OCR1BL = 0xFF; //Output compare register low value for Right Motor
	TCCR1A = 0xA1; //COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0;
	//For Overriding normal port functionality to OCR1A outputs. WGM11=0,WGM10=1 Along With GM12
	//in TCCR1B for Selecting FAST PWM 8-bit Mode
	TCCR1B = 0x0D; //WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

void velocity (unsigned char left_motor, unsigned char right_motor){
    OCR1AL = left_motor;
    OCR1BL = right_motor;
}

int main(){
	init_devices();
	int delay = 6;
	unsigned char l,c,r;
	int error, prev_error = 0;
	float Kp = 1.6, Kd = 0.13, PID = 0;
	forward();
	while(1){
		_delay_ms(delay);
		l = ADC_Conversion('3');
		c = ADC_Conversion('4');
		r = ADC_Conversion('5');
		
		error = r-l;
		PID = Kp*error+ Kd*(error-prev_error);
		prev_error = error;

		// At the starting of the loop the value of l,c,r
		// becomes low. So to keep the bot on the track 
        // move it little back so that "c" may be on the track
		if(l<70 && c<70 && r<70){
			back();
			velocity(210,210);
			_delay_ms(delay);
		}
		else
		// if the error is large when track is
		// turning left, move the bot to left
		if(PID <-180){
			left();
		}
		// if the error is large when track is 
		// turning right, move the bot to right
		else if(PID >165){
			right();
		}
		else if(PID <-100){
			soft_left();
		}
		else if(PID >63){
			soft_right();
		}
		else{
			forward();
			velocity(255,255);
		}	
	}
}
