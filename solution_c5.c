#ifndef F_CPU
#define F_CPU 4000000UL
#endif
#include <avr/io.h>
int main() {
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0x00;
	PORTB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	unsigned int num1 = PINA;
	unsigned int num2 = PINB;
	unsigned int num = num1 + num2;
	if (num >= 100) {
		PORTC = 0b01111001;
		PORTD = 0b00000000;
		while (1) {
			continue;
		}
	}
	switch(num%10) {
		case 0:
			PORTC = 0b00111111;
			break;
		case 1:
			PORTC = 0b00000110;
			break;
		case 2:
			PORTC = 0b01011011;
			break;
		case 3:
			PORTC = 0b01001111;
			break;
		case 4:
			PORTC = 0b01100110;
			break;
		case 5:
			PORTC = 0b01101101;
			break;
		case 6:
			PORTC = 0b01111101;
			break;
		case 7:
			PORTC = 0b00000111;
			break;
		case 8:
			PORTC = 0b01111111;
			break;
		case 9:
			PORTC = 0b01101111;
			break;
	}
	switch((num/10)%10) {
		case 0:
			PORTD = 0b00111111;
			break;
		case 1:
			PORTD = 0b00000110;
			break;
		case 2:
			PORTD = 0b01011011;
			break;
		case 3:
			PORTD = 0b01001111;
			break;
		case 4:
			PORTD = 0b01100110;
			break;
		case 5:
			PORTD = 0b01101101;
			break;
		case 6:
			PORTD = 0b01111101;
			break;
		case 7:
			PORTD = 0b00000111;
			break;
		case 8:
			PORTD = 0b01111111;
			break;
		case 9:
			PORTD = 0b01101111;
			break;
	}
	while (1) {
		continue;
	}
}
