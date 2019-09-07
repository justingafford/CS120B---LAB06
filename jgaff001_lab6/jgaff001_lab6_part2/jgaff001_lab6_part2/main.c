/*	Partner 1's Name & E-mail: Justin Gafford jgaff001@ucr.edu
*  Partner 2's Name & E-mail: Wang Sitao 934415487@qq.com
*	Lab Section: 022
*	Assignment: Lab #6 Exercise #2
*
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#define button (~PINA & 0x01)

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tmpB = 0x00;

enum states{init,pb0_on,wait1,pb1_on,wait2, pb2_on,wait3}state;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0X00;
}

void TimerISR(){
	TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void lightSSM() {
	switch (state) {
		case init:
			state = pb0_on;
			break;
		case pb0_on:
			if (button) {
				state = wait1;
			}
			else {
				state = pb1_on;
			}
			break;
		case wait1:
			if (button) {
				state = pb0_on;
			}
			else {
				state = wait1;
			}
			break;
		case pb1_on:
			if (button) {
				state = wait2;
			}
			else {
				state = pb2_on;
			}
			break;
		case wait2:
			if (button) {
				state = pb1_on;
			}
			else {
				state = wait2;
			}
			break;
		case pb2_on:
			if (button) {
				state = wait3;
			}
			else {
				state = pb0_on;
			}
			break;
		case wait3:
			if(button) {
				state = pb2_on;
			}
			else {
				state = wait3;
			}
			break;
		default:
			state = init;
			break;
	}
	switch(state) {
		case pb0_on:
			tmpB = 0x01;
			break;
		case wait1:
			tmpB = 0x01;
			break;
		case pb1_on:
			tmpB = 0x02;
			break;
		case wait2:
			tmpB = 0x02;
			break;
		case pb2_on:
			tmpB = 0x04;
			break;
		case wait3:
			tmpB = 0x04;
			break;
		default:
			break;
	}
}

int main()
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF;PORTB = 0x00;
	TimerSet(300);
	TimerOn();
	state = init;
	
	/* Replace with your application code */
	while (1)
	{
		lightSSM();
		PORTB = tmpB;
		while (!TimerFlag);
		TimerFlag = 0;
		
	}
}



